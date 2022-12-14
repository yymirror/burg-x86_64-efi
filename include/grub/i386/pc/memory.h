/* memory.h - describe the memory map */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002,2007,2008,2009  Free Software Foundation, Inc.
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

#ifndef GRUB_MEMORY_MACHINE_HEADER
#define GRUB_MEMORY_MACHINE_HEADER	1

#include <grub/symbol.h>
#ifndef ASM_FILE
#include <grub/types.h>
#include <grub/err.h>
#include <grub/memory.h>
#endif

#include <grub/i386/memory.h>

/* The scratch buffer used in real mode code.  */
#define GRUB_MEMORY_MACHINE_SCRATCH_ADDR	0x68000
#define GRUB_MEMORY_MACHINE_SCRATCH_SEG	(GRUB_MEMORY_MACHINE_SCRATCH_ADDR >> 4)
#define GRUB_MEMORY_MACHINE_SCRATCH_SIZE	0x10000

/* The real mode stack.  */
#define GRUB_MEMORY_MACHINE_REAL_STACK	(0x2000 - 0x10)

/* The size of the protect mode stack.  */
#define GRUB_MEMORY_MACHINE_PROT_STACK_SIZE	0x8000

/* The upper memory area (starting at 640 kiB).  */
#define GRUB_MEMORY_MACHINE_UPPER		0xa0000

/* The protected mode stack.  */
#define GRUB_MEMORY_MACHINE_PROT_STACK	\
	(GRUB_MEMORY_MACHINE_SCRATCH_ADDR + GRUB_MEMORY_MACHINE_SCRATCH_SIZE \
	 + GRUB_MEMORY_MACHINE_PROT_STACK_SIZE - 0x10)

/* The memory area where GRUB uses its own purpose. This part is not added
   into free memory for dynamic allocations.  */
#define GRUB_MEMORY_MACHINE_RESERVED_START	\
	GRUB_MEMORY_MACHINE_SCRATCH_ADDR
#define GRUB_MEMORY_MACHINE_RESERVED_END	\
	(GRUB_MEMORY_MACHINE_PROT_STACK + 0x10)

/* The area where GRUB is decompressed at early startup.  */
#define GRUB_MEMORY_MACHINE_DECOMPRESSION_ADDR	0x100000

/* The address of a partition table passed to another boot loader.  */
#define GRUB_MEMORY_MACHINE_PART_TABLE_ADDR	0x7be

/* The address where another boot loader is loaded.  */
#define GRUB_MEMORY_MACHINE_BOOT_LOADER_ADDR	0x7c00

/* The code segment of the protected mode.  */
#define GRUB_MEMORY_MACHINE_PROT_MODE_CSEG	0x8

/* The data segment of the protected mode.  */
#define GRUB_MEMORY_MACHINE_PROT_MODE_DSEG	0x10

/* The code segment of the pseudo real mode.  */
#define GRUB_MEMORY_MACHINE_PSEUDO_REAL_CSEG	0x18

/* The data segment of the pseudo real mode.  */
#define GRUB_MEMORY_MACHINE_PSEUDO_REAL_DSEG	0x20

#define GRUB_MEMORY_MACHINE_BIOS_DATA_AREA_ADDR	0x400

#ifndef ASM_FILE

/* See http://heim.ifi.uio.no/~stanisls/helppc/bios_data_area.html for a
   description of the BIOS Data Area layout.  */
struct grub_machine_bios_data_area
{
  grub_uint8_t unused1[0x17];
  grub_uint8_t keyboard_flag_lower; /* 0x17 */
  grub_uint8_t unused2[0xf0 - 0x18];
};

struct grub_machine_mmap_entry
{
  grub_uint32_t size;
  grub_uint64_t addr;
  grub_uint64_t len;
#define GRUB_MACHINE_MEMORY_AVAILABLE	1
#define GRUB_MACHINE_MEMORY_RESERVED	2
#define GRUB_MACHINE_MEMORY_ACPI	3
#define GRUB_MACHINE_MEMORY_NVS 	4
#define GRUB_MACHINE_MEMORY_MAX_TYPE 	4
  /* This one is special: it's used internally but is never reported
     by firmware. */
#define GRUB_MACHINE_MEMORY_HOLE 	5

  grub_uint32_t type;
} __attribute__((packed));

grub_err_t grub_machine_mmap_iterate (int (*hook) (grub_uint64_t, grub_uint64_t,
						   grub_uint32_t, void *), void *);

grub_uint64_t grub_mmap_get_post64 (void);
grub_uint64_t grub_mmap_get_upper (void);
grub_uint64_t grub_mmap_get_lower (void);

#define GRUB_MMAP_MALLOC_LOW 1

#ifdef GRUB_MACHINE_PCBIOS
grub_err_t grub_machine_mmap_register (grub_uint64_t start, grub_uint64_t size,
				       int type, int handle);
grub_err_t grub_machine_mmap_unregister (int handle);
#else
static inline grub_err_t
grub_machine_mmap_register (grub_uint64_t start __attribute__ ((unused)),
			    grub_uint64_t size __attribute__ ((unused)),
			    int type __attribute__ ((unused)),
			    int handle __attribute__ ((unused)))
{
  return GRUB_ERR_NONE;
}
static inline grub_err_t
grub_machine_mmap_unregister (int handle  __attribute__ ((unused)))
{
  return GRUB_ERR_NONE;
}
#endif

#endif

#endif /* ! GRUB_MEMORY_MACHINE_HEADER */
