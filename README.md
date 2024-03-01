# rbs_kernel_lib

In order to use (on ARM):
1. Download and extract the Linux Kernel that you want to modify
2. In kernel_modifications, find the file called sys_mod.c and add the code it contains to linux/kernel/sys.c
3. In kernel_modifications, find the file called kernel_mod.h and add the code it contains to linux/include/linux/kernel.h
4. In kernel_modifications, find the file called uni_std.h, it contains the new system_calls. These must be added to linux/include/uapi/asm-generic/unistd.h
They must be added before lines:

#undef __NR_syscalls
#define __NR_syscalls 460

When adding, take care to assign free numbers to the newly added system calls and to modify the number of system calls value to last system call number + 1.


5. Compile the kernel, install it and restart the system.
6. Use makefile to compile the example and run it with "sudo"