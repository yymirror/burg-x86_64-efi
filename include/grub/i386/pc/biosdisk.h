/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002,2005,2007,2008  Free Software Foundation, Inc.
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

#ifndef GRUB_BIOSDISK_MACHINE_HEADER
#define GRUB_BIOSDISK_MACHINE_HEADER	1

#include <grub/symbol.h>
#include <grub/types.h>

#define GRUB_BIOSDISK_FLAG_LBA	1
#define GRUB_BIOSDISK_FLAG_CDROM 2

#define GRUB_BIOSDISK_CDTYPE_NO_EMUL	0
#define GRUB_BIOSDISK_CDTYPE_1_2_M	1
#define GRUB_BIOSDISK_CDTYPE_1_44_M	2
#define GRUB_BIOSDISK_CDTYPE_2_88_M	3
#define GRUB_BIOSDISK_CDTYPE_HARDDISK	4

#define GRUB_BIOSDISK_CDTYPE_MASK	0xF

struct grub_biosdisk_data
{
  int drive;
  unsigned long cylinders;
  unsigned long heads;
  unsigned long sectors;
  unsigned long flags;
};

/* Drive Parameters.  */
struct grub_biosdisk_drp
{
  grub_uint16_t size;
  grub_uint16_t flags;
  grub_uint32_t cylinders;
  grub_uint32_t heads;
  grub_uint32_t sectors;
  grub_uint64_t total_sectors;
  grub_uint16_t bytes_per_sector;
  /* ver 2.0 or higher */

  union
  {
    grub_uint32_t EDD_configuration_parameters;

    /* Pointer to the Device Parameter Table Extension (ver 3.0+).  */
    grub_uint32_t dpte_pointer;
  };

  /* ver 3.0 or higher */
  grub_uint16_t signature_dpi;
  grub_uint8_t length_dpi;
  grub_uint8_t reserved[3];
  grub_uint8_t name_of_host_bus[4];
  grub_uint8_t name_of_interface_type[8];
  grub_uint8_t interface_path[8];
  grub_uint8_t device_path[8];
  grub_uint8_t reserved2;
  grub_uint8_t checksum;

  /* XXX: This is necessary, because the BIOS of Thinkpad X20
     writes a garbage to the tail of drive parameters,
     regardless of a size specified in a caller.  */
  grub_uint8_t dummy[16];
} __attribute__ ((packed));

struct grub_biosdisk_cdrp
{
  grub_uint8_t size;
  grub_uint8_t media_type;
  grub_uint8_t drive_no;
  grub_uint8_t controller_no;
  grub_uint32_t image_lba;
  grub_uint16_t device_spec;
  grub_uint16_t cache_seg;
  grub_uint16_t load_seg;
  grub_uint16_t length_sec512;
  grub_uint8_t cylinders;
  grub_uint8_t sectors;
  grub_uint8_t heads;
  grub_uint8_t dummy[16];
} __attribute__ ((packed));

/* Disk Address Packet.  */
struct grub_biosdisk_dap
{
  grub_uint8_t length;
  grub_uint8_t reserved;
  grub_uint16_t blocks;
  grub_uint32_t buffer;
  grub_uint64_t block;
} __attribute__ ((packed));

int grub_biosdisk_rw_int13_extensions (int ah, int drive, void *dap);
int grub_biosdisk_rw_standard (int ah, int drive, int coff, int hoff,
			       int soff, int nsec, int segment);
int grub_biosdisk_check_int13_extensions (int drive);
int grub_biosdisk_get_diskinfo_int13_extensions (int drive,
           void *drp);
int grub_biosdisk_get_cdinfo_int13_extensions (int drive,
           void *cdrp);
int grub_biosdisk_get_diskinfo_standard (int drive,
					 unsigned long *cylinders,
					 unsigned long *heads,
					 unsigned long *sectors);
int grub_biosdisk_get_num_floppies (void);

void grub_biosdisk_init (void);
void grub_biosdisk_fini (void);

#endif /* ! GRUB_BIOSDISK_MACHINE_HEADER */
