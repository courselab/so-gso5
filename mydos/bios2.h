/*
 *    SPDX-FileCopyrightText: 2024 Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
 *    SPDX-FileCopyrightText: 2024 Monaco F. J. <monaco@usp.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
 */

#ifndef BIOS2_H
#define BIOS2_H

int __attribute__((fastcall)) kread(char *);

void __attribute__((fastcall)) udelay(unsigned short);

#endif  /* BIOS2_H  */
