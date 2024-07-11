/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
 */

/* This source file implements the kernel entry function 'kmain' called
   by the bootloader, and the command-line interpreter. Other kernel functions
   were implemented separately in another source file for legibility. */

#include "bios1.h"		/* For kwrite() etc.            */
#include "bios2.h"		/* For kread() etc.             */
#include "kernel.h"		/* Essential kernel functions.  */
#include "kaux.h"		  /* Auxiliary kernel functions.  */

struct fs_header_t
{
	unsigned char signature[FS_SIGLEN];		  /* The file system signature.              */
	unsigned short total_number_of_sectors; /* Number of 512-byte disk blocks.         */
	unsigned short number_of_boot_sectors;	/* Sectors reserved for boot code.         */
	unsigned short number_of_file_entries;	/* Maximum number of files in the disk.    */
	unsigned short max_file_size;			      /* Maximum size of a file in blocks.       */
	unsigned int unused_space;				      /* Remaining space less than max_file_size.*/
} __attribute__((packed)) fs_header;		  /* Disable alignment to preserve offsets.  */

/* Kernel's entry function. */

void kmain(void)
{
  int i, j;
  
  register_syscall_handler();	/* Register syscall handler at int 0x21.*/

  splash();			/* Uncessary spash screen.              */

  shell();			/* Invoke the command-line interpreter. */
  
  halt();			/* On exit, halt.                       */
  
}

/* Tiny Shell (command-line interpreter). */

char buffer[BUFF_SIZE];
int go_on = 1;

void shell()
{
  int i;
  clear();
  kwrite ("GladiaDOS 1.0\n");

  while (go_on)
    {

      /* Read the user input. 
	 Commands are single-word ASCII tokens with no blanks. */
      do
	{
	  kwrite(PROMPT);
	  kread (buffer);
	}
      while (!buffer[0]);

      /* Check for matching built-in commands */
      
      i=0;
      while (cmds[i].funct)
	{
	  if (!strcmp(buffer, cmds[i].name))
	    {
	      cmds[i].funct();
	      break;
	    }
	  i++;
	}

      /* If the user input does not match any built-in command name, just
	 ignore and read the next command. If we were to execute external
	 programs, on the other hand, this is where we would search for a 
	 corresponding file with a matching name in the storage device, 
	 load it and transfer it the execution. Left as exercise. */
      
      if (!cmds[i].funct)
	kwrite ("Command not found\n");
    }
}


/* Array with built-in command names and respective function pointers. 
   Function prototypes are in kernel.h. */

struct cmd_t cmds[] =
  {
    {"help",    f_help},     /* Print a help message.       */
    {"list",    f_list},     /* List files in image.        */
    {"quit",    f_quit},     /* Exit TyDOS.                 */
    {0, 0}
  };


/* Build-in shell command: help. */

void f_help()
{
  kwrite ("Mobilize your system resources and unleash your fury!\n\n");
  kwrite ("   Here are some commands for you:\n");
  kwrite ("      list    (Call for your files\n");
  kwrite ("      exec    (Catapult an example program\n");
  kwrite ("      quit    (Raise your white flag)\n");
}

void f_quit()
{
  kwrite ("You have conceeded. Bye.");
  go_on = 0;
}

/*
  Function to list files in disk.

  1. Loads FS header for coordinates to search files;
  2. Calculate the starting sector of the directory list;
  3. Calculate the number of sectors to read (tyFS uses 512-byte sectors and the size of the directory entry is 32);
  4. Allocate enough space in RAM and loads the content from disk;
  5. Then, for each file entries, prints the file name.
 */
void f_list() {
  
  struct fs_header_t *fs_header = (struct fs_header_t *)0x7c00;

  int initial_sector = fs_header->number_of_boot_sectors;

  int sectors_to_read = fs_header->number_of_file_entries * 32 / 512;

  void *directory = (void *)(initial_sector * 512);

  load_content(initial_sector, sectors_to_read, directory);

  for (int i = 0; i < fs_header->number_of_file_entries; i++) {
    char *file_name = directory + (i * 32);
    if (file_name[0] == '\0') {
      continue;
    }
    kwrite(file_name);
    kwrite("\n");
  }
}

/* Built-in shell command: example.

   Execute an example user program which invokes a syscall.

  */

extern int main();
