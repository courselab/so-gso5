# AT&T version of eg-01.asm
# Alternative solution
.code16			# Set 16-bit mode
.global _start
_start:
movb $0xe, %ah  # set BIOS teletype mode
mov $'H', %al	# Load 'H' ascii code
int $0x10		# Issue BIOS interrupt
mov $'e', %al	# Load 'H' ascii code
int $0x10		# Issue BIOS interrupt
mov $'l', %al	# Load 'H' ascii code
int $0x10		# Issue BIOS interrupt
mov $'l', %al	# Load 'H' ascii code
int $0x10		# Issue BIOS interrupt
mov $'o', %al	# Load 'H' ascii code
int $0x10		# Issue BIOS interrupt
mov $' ', %al	# Load ' ' ascii code
int $0x10		# Issue BIOS interrupt
mov $'W', %al	# Load 'W' ascii code
int $0x10		# Issue BIOS interrupt
mov $'o', %al	# Load 'o' ascii code
int $0x10		# Issue BIOS interrupt
mov $'r', %al	# Load 'r' ascii code
int $0x10		# Issue BIOS interrupt
mov $'l', %al	# Load 'l' ascii code
int $0x10		# Issue BIOS interrupt
mov $'d', %al	# Load 'd' ascii code
int $0x10		# Issue BIOS interrupt
halt:
hlt			    # Halt the machine
jmp halt		# Safeguard
.fill 510 - (. - _start), 1, 0	# Pad with zeros
.word 0xaa55		        # Boot signature
