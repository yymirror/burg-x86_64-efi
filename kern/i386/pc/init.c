/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002,2003,2004,2005,2006,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/kernel.h>
#include <grub/mm.h>
#include <grub/machine/boot.h>
#include <grub/machine/init.h>
#include <grub/machine/memory.h>
#include <grub/machine/console.h>
#include <grub/machine/kernel.h>
#include <grub/types.h>
#include <grub/err.h>
#include <grub/dl.h>
#include <grub/misc.h>
#include <grub/loader.h>
#include <grub/env.h>
#include <grub/cache.h>
#include <grub/time.h>
#include <grub/cpu/tsc.h>

GRUB_EXPORT(grub_os_area_addr);
GRUB_EXPORT(grub_os_area_size);
GRUB_EXPORT(grub_get_mmap_entry);
GRUB_EXPORT(grub_reboot);
GRUB_EXPORT(grub_halt);
GRUB_EXPORT(grub_stop_floppy);

struct mem_region
{
  grub_addr_t addr;
  grub_size_t size;
};

#define MAX_REGIONS	32

static struct mem_region mem_regions[MAX_REGIONS];
static int num_regions;

grub_addr_t grub_os_area_addr;
grub_size_t grub_os_area_size;

static char *
make_install_device (void)
{
  /* XXX: This should be enough.  */
  char dev[100], *ptr = dev;

  if (grub_prefix[0] != '(')
    {
      /* No hardcoded root partition - make it from the boot drive and the
	 partition number encoded at the install time.  */
      if (grub_boot_drive == GRUB_BOOT_MACHINE_PXE_DL)
	{
	  grub_strcpy (dev, "(pxe");
	  ptr += sizeof ("(pxe") - 1;
	}
      else
	{
	  grub_snprintf (dev, sizeof (dev),
			 "(%cd%u", (grub_boot_drive & 0x80) ? 'h' : 'f',
			 grub_boot_drive & 0x7f);
	  ptr += grub_strlen (ptr);

	  if (grub_install_dos_part >= 0)
	    grub_snprintf (ptr, sizeof (dev) - (ptr - dev),
			   ",%u", grub_install_dos_part + 1);
	  ptr += grub_strlen (ptr);

	  if (grub_install_bsd_part >= 0)
	    grub_snprintf (ptr, sizeof (dev) - (ptr - dev), ",%c",
			   grub_install_bsd_part + 'a');
	  ptr += grub_strlen (ptr);
	}

      grub_snprintf (ptr, sizeof (dev) - (ptr - dev), ")%s", grub_prefix);
      grub_strcpy (grub_prefix, dev);
    }

  return grub_prefix;
}

/* Add a memory region.  */
static void
add_mem_region (grub_addr_t addr, grub_size_t size)
{
  if (num_regions == MAX_REGIONS)
    /* Ignore.  */
    return;

  mem_regions[num_regions].addr = addr;
  mem_regions[num_regions].size = size;
  num_regions++;
}

/* Compact memory regions.  */
static void
compact_mem_regions (void)
{
  int i, j;

  /* Sort them.  */
  for (i = 0; i < num_regions - 1; i++)
    for (j = i + 1; j < num_regions; j++)
      if (mem_regions[i].addr > mem_regions[j].addr)
	{
	  struct mem_region tmp = mem_regions[i];
	  mem_regions[i] = mem_regions[j];
	  mem_regions[j] = tmp;
	}

  /* Merge overlaps.  */
  for (i = 0; i < num_regions - 1; i++)
    if (mem_regions[i].addr + mem_regions[i].size >= mem_regions[i + 1].addr)
      {
	j = i + 1;

	if (mem_regions[i].addr + mem_regions[i].size
	    < mem_regions[j].addr + mem_regions[j].size)
	  mem_regions[i].size = (mem_regions[j].addr + mem_regions[j].size
				 - mem_regions[i].addr);

	grub_memmove (mem_regions + j, mem_regions + j + 1,
		      (num_regions - j - 1) * sizeof (struct mem_region));
	i--;
        num_regions--;
      }
}

static int
mmap_hook (grub_uint64_t addr, grub_uint64_t size, grub_uint32_t type,
	   void *closure __attribute__ ((unused)))
{
  /* Avoid the lower memory.  */
  if (addr < 0x100000)
    {
      if (size <= 0x100000 - addr)
	return 0;

      size -= 0x100000 - addr;
      addr = 0x100000;
    }

  /* Ignore >4GB.  */
  if (addr <= 0xFFFFFFFF && type == GRUB_MACHINE_MEMORY_AVAILABLE)
    {
      grub_size_t len;

      len = (grub_size_t) ((addr + size > 0xFFFFFFFF)
			   ? 0xFFFFFFFF - addr
			   : size);
      add_mem_region (addr, len);
    }

  return 0;
}

void
grub_machine_init (void)
{
  int i;
  int grub_lower_mem;

  /* Initialize the console as early as possible.  */
  grub_console_init ();

  grub_lower_mem = grub_get_memsize (0) << 10;

  /* Sanity check.  */
  if (grub_lower_mem < GRUB_MEMORY_MACHINE_RESERVED_END)
    grub_fatal ("too small memory");

#if 0
  /* Turn on Gate A20 to access >1MB.  */
  grub_gate_a20 (1);
#endif

/* FIXME: This prevents loader/i386/linux.c from using low memory.  When our
   heap implements support for requesting a chunk in low memory, this should
   no longer be a problem.  */
#if 0
  /* Add the lower memory into free memory.  */
  if (grub_lower_mem >= GRUB_MEMORY_MACHINE_RESERVED_END)
    add_mem_region (GRUB_MEMORY_MACHINE_RESERVED_END,
		    grub_lower_mem - GRUB_MEMORY_MACHINE_RESERVED_END);
#endif

  grub_machine_mmap_iterate (mmap_hook, 0);

  compact_mem_regions ();

  /* Add the memory regions to free memory, except for the region starting
     from 1MB. This region is partially used for loading OS images.
     For now, 1/4 of this is added to free memory.  */
  for (i = 0; i < num_regions; i++)
    if (mem_regions[i].addr == 0x100000)
      {
	grub_size_t quarter = mem_regions[i].size >> 2;

	grub_os_area_addr = mem_regions[i].addr;
	grub_os_area_size = mem_regions[i].size - quarter;
	grub_mm_init_region ((void *) (grub_os_area_addr + grub_os_area_size),
			     quarter);
      }
    else
      grub_mm_init_region ((void *) mem_regions[i].addr, mem_regions[i].size);

  if (! grub_os_area_addr)
    grub_fatal ("no upper memory");

  grub_tsc_init ();
}

void
grub_machine_set_prefix (void)
{
  /* Initialize the prefix.  */
  grub_env_set ("prefix", make_install_device ());
}

void
grub_machine_fini (void)
{
  grub_console_fini ();
  grub_stop_floppy ();
}

/* Return the end of the core image.  */
grub_addr_t
grub_arch_modules_addr (void)
{
  return GRUB_MEMORY_MACHINE_DECOMPRESSION_ADDR
    + (grub_kernel_image_size - GRUB_KERNEL_MACHINE_RAW_SIZE);
}
