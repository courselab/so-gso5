/*
 *    SPDX-FileCopyrightText: 2024 Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
 *    SPDX-FileCopyrightText: 2024 Monaco F. J. <monaco@usp.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
*  katherincm <93104605+katherincm@users.noreply.github.com>
 */

#ifndef KERNEL_H
#define KERNEL_H

#define FS_SIGLEN 4

/* This is kernel's entry function, which is called by the bootloader
   as soon as it loads the kernel from the this image. */

void kmain(void);		

/* This is the command interpreter, which is invoked by the kernel as
   soon as the boot is complete. 
   
   Our tiny command-line parser is too simple: commands are ASCII single words
   with no command line arguments (no blanks). */


void shell();			/* Command interpreter. */
#define BUFF_SIZE 64		/* Max command length.  */
#define PROMPT "> "		/* Command-line prompt. */


/* Built-in commands. */

void f_help();
void f_exec();
void f_quit();

/* Implemented commands */
void f_list();

extern struct cmd_t
{
  char name[32];
  void (*funct)();
} cmds[];

#endif  /* KERNEL_H  */
