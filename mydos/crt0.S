#    SPDX-FileCopyrightText: 2024 Felipe Tavoni <56005905+FelTavoni@users.noreply.github.com>
#    SPDX-FileCopyrightText: 2024 katherincm <93104605+katherincm@users.noreply.github.com>
#   
#    SPDX-License-Identifier: GPL-3.0-or-later

	.code16gcc      /* Select 16-bit code.                    */
	.global _start	/* This will be the program entry point.  */
	.text

	jmp _start
_start:
  call main
	ret