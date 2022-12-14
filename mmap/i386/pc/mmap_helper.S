/* Mmap management. */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2009  Free Software Foundation, Inc.
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

#include <grub/symbol.h>

#define DS(x) ((x) - LOCAL (segstart))

LOCAL (segstart):
VARIABLE(grub_machine_mmaphook_start)
	.code16
VARIABLE(grub_machine_mmaphook_int12)
	push %ds
	push %cs
	pop %ds
	movw DS (LOCAL (kblow)), %ax
	pop %ds
	iret

VARIABLE(grub_machine_mmaphook_int15)
	pushf
	cmpw $0xe801, %ax
	jz LOCAL (e801)
	cmpw $0xe820, %ax
	jz LOCAL (e820)
	cmpb $0x88, %ah
	jz LOCAL (h88)
	popf
	/* ljmp */
	.byte	0xea
VARIABLE (grub_machine_mmaphook_int15offset)
	.word	0
VARIABLE (grub_machine_mmaphook_int15segment)
	.word	0

LOCAL (e801):
	popf
	push %ds
	push %cs
	pop %ds
	movw DS (LOCAL (kbin16mb)), %ax
	movw DS (LOCAL (m64kbin4gb)), %bx
	movw %ax, %cx
	movw %bx, %dx
	pop %ds
	clc
	iret

LOCAL (h88):
	popf
	push %ds
	push %cs
	pop %ds
	movw DS (LOCAL (kbin16mb)), %ax
	pop %ds
	clc
	iret

LOCAL (e820):
	popf
	push %ds
	push %cs
	pop %ds
	cmpw $20, %cx
	jb LOCAL (errexit)
	cmpw DS (LOCAL (mmap_num)), %bx
	jae LOCAL (errexit)
	cmp $0x534d4150, %edx
	jne LOCAL (errexit)
	push %si
	push %di
	movw $20, %cx
	movw $(DS(LOCAL (mmaphook_mmap))), %si
	mov %bx, %ax
	imul $20, %ax
	add %ax, %si
	rep movsb
	pop %di
	pop %si
	movl $20, %ecx
	inc %bx
	cmpw DS(LOCAL (mmap_num)), %bx
	jb LOCAL (noclean)
	xor %bx, %bx
LOCAL (noclean):
	mov $0x534d4150, %eax
	pop %ds
	clc
	iret
LOCAL (errexit):
	mov $0x534d4150, %eax
	pop %ds
	stc
	xor %bx, %bx
	iret

VARIABLE(grub_machine_mmaphook_mmap_num)
LOCAL (mmap_num):
	.word 0
VARIABLE(grub_machine_mmaphook_kblow)
LOCAL (kblow):
	.word 0
VARIABLE (grub_machine_mmaphook_kbin16mb)
LOCAL (kbin16mb):
	.word 0
VARIABLE (grub_machine_mmaphook_64kbin4gb)
LOCAL (m64kbin4gb):
	.word 0
LOCAL (mmaphook_mmap):
	/* Memory map is placed just after the interrupt handlers. */
VARIABLE(grub_machine_mmaphook_end)
	.byte 0
