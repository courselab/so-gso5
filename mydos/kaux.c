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

#include "bios2.h"		/* For udelay().      */
#include "kaux.h"		/* For ROWS and COLS. */

/* Video RAM as 2D matrix: short vram[row][col]. */

short (*vram)[COLS] = (short (*)[COLS])0xb8000;

char character_color = 0x02;	/* Default fore/background character color.*/

/* Write 'string' starting at the position given by 'row' and 'col'.
   Text is wrapped around both horizontally and vertically. 

   The implementation manipulates the video-RAM rather than BIOS services.
*/

void writexy(unsigned char row, unsigned char col, const char* string)
{
  int k=0;
  
  while (string[k])
    {

      col = col % COLS;
      row = row % ROWS;
      
      vram[row][col] = color_char(string[k]);
      col++;
      k++;
    }
}

/* Clear the entire screen

   The implementation manipulates the video-RAM rather than BIOS services.

 */

void clearxy()
{
  int i,j;

  for (j=0; j<ROWS; j++)
    for (i=0; i<COLS; i++)
      vram[j][i] = color_char(' ');
}

/* A not-that-impressive splash screen that is entirely superfluous. */

extern const char logo[];
void splash(void)
{
  int i,j, k;

  clearxy();

  for (i=0; i<COLS; i++)
    {
      for (j=0; j<ROWS; j+=2)
	{
	  vram[j][i] = color_char(logo[j*COLS+i]);
	  vram[j+1][COLS-i] = color_char(logo[(j+1)*COLS+(COLS-i)]);
	  udelay (1);
	}
    }

  udelay (500);
  clearxy();
}

/* Return 0 is string 's1' and 's2' are equal; return non-zero otherwise.*/

int strcmp(const char *s1, const char *s2)
{
  while (*s1 && *s2 && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*s1 - *s2);
}

void load_content(int block_coordinate, int sectors_to_read, void *target_addres) {

  // Computes the head number for the disk access
  int head_coordinate = (block_coordinate - 1) / 63;

  // Computes the sector number within the cylinder for the disk access.
  int sector_coordinate = (block_coordinate - 1) % 63 + 1;

  // Perform disk I/O operations using BIOS interrupts 
  __asm__ volatile(
      "pusha \n"
      "mov boot_drive, %%dl \n"
      "mov $0x2, %%ah \n"
      "mov %[sectToRead], %%al \n" // Sectors to read
      "mov $0x0, %%ch \n"          // Cylinder coordinate
      "mov %[sectCoord], %%cl \n"  // Sector coordinate
      "mov %[headCoord], %%dh \n"  // Head coordinate
      "mov %[targetAddr], %%bx \n" // Position to load
      "int $0x13 \n"               // Call BIOS int 0x13 to read sectors
      "popa \n" ::
          [headCoord] "g"(head_coordinate),
      [sectCoord] "g"(sector_coordinate),
      [sectToRead] "g"(sectors_to_read),
      [targetAddr] "g"(target_addres));
}
