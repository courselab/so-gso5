#    SPDX-FileCopyrightText: 2024 Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
#   
#    SPDX-License-Identifier: GPL-3.0-or-later
#
#    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
#
#    This file implements the "time" routine, showing current system time
#    Reference: https://en.wikipedia.org/wiki/BIOS_interrupt_call

    .code16
    .global time

    .section .text

time:
    pusha               /*  Save all GP registers.              */
                        /*  BIOS interruption for system time   */
    mov $0x02, %ah
    int $0x1A
                        /*  Result is stored at cx:dx           */
                        /*  For Hours...                        */
    mov %ch, %al        /*  Moving content to AL                */
    shr $4, %al         /*  Shift to get first 4 bytes          */
    call number         /*  Print number in ASCII               */

    mov %ch, %al
    and $0x0F, %al      /*  Apply mask to get last 4 bytes      */
    call number         /*  Print number in ASCII               */

    call semicolon      /*  Print ':' as separator              */

                        /*  For Minutes...                      */
    mov %cl, %al
    shr $4, %al
    call number

    mov %cl, %al
    and $0x0F, %al
    call number

    call semicolon

                        /*  For Seconds...                      */
    mov %dh, %al
    shr $4, %al
    call number

    mov %dh, %al
    and $0x0F, %al
    call number

    call breakline      /*  Breakline for next command          */

	popa                /*  Restore all GP registers.           */
	ret

show:
    mov $0x0E, %ah      /*  Set function to write char in TTY Mode  */
    int $0x10           /*  BIOS interruption to service        */
    ret

number:
    add $'0', %al       /*  Convert to ASCII                    */
    call show
    ret

semicolon:
    mov $':', %al       /*  Set register with ASCII of ':'      */
    call show
    ret

breakline:              /*  Routine to break line               */
    mov $0xD, %al
    call show
    mov $0xA, %al
    call show
    ret
