# Ridwan Hussain & Nicholas Storniolo 
.section .bss
	.comm buf 4096
	#initialize variable to store oneshot userinput 4096 bytes/chars

.section .text
	.globl _start

_start:
	#Calling read system call, file descriptor=0 (stdin), char buffer (store input), and size of input (4096 bytes) 
	mov $0, %rax             # system call 3 =does system call read()
	mov $0, %rdi             # file descriptor
	lea [buf], %rsi          # char* to store the value
	mov $4096, %rdx          # size of the buffer
	syscall                  # tells the kernel to take over
	
	#Call write system call, file descriptor=1 (stdout), char buffer (where output was stored), and size of input (4096 bytes)
	mov $1, %rax             # system call 3 =does system call read()
	mov $1, %rdi             # file descriptor
	lea [buf], %rsi          # char* to store the value
	mov $4096, %rdx          # size of the buffer
	syscall                  # tells the kernel to take over

	#Exiting program
	mov $60, %rax 			 # Calling exit system calls
	mov $0, %rdi             # setting return value to 0
	syscall                  # tells the kernel to take over
