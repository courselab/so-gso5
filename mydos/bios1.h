/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
*  katherincm <93104605+katherincm@users.noreply.github.com>
 */

#ifndef BIOS1_H
#define BIOS1_H

void __attribute__((fastcall)) clear (void);
void __attribute__((fastcall)) kwrite(const char*);
void __attribute__((fastcall)) kwriteln(const char*);
/* void __attribute__((fastcall)) kread(char *); */
void __attribute__((fastcall)) fatal(const char*);
void __attribute__((fastcall)) load_kernel(void);
void __attribute__((fastcall)) set_cursor(char, char);

#endif
