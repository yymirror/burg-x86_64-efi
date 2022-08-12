/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2004,2007  Free Software Foundation, Inc.
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

#ifndef GRUB_LOADER_MACHINE_HEADER
#define GRUB_LOADER_MACHINE_HEADER	1

/* The symbol shared between the normal mode and rescue mode
   loader.  */
void grub_rescue_cmd_linux (int argc, char *argv[]);
void grub_rescue_cmd_initrd (int argc, char *argv[]);

void grub_linux_init (void);
void grub_linux_fini (void);
void grub_linux_normal_init (void);
void grub_linux_normal_fini (void);

#endif /* ! GRUB_LOADER_MACHINE_HEADER */
