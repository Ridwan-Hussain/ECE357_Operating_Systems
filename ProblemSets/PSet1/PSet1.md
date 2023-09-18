PSet 1 - Ridwan Hussain
---

# Question 1
1. reading one char from a file on disk using fgetc - Requires the systemcall, "read".
2. calling a function - It depends on what function you use. Using a simple user-defined function like an incrementer wouldn't require a system call, but fgetc does require a systemcall, as seen above.
3. `struct whack *s = malloc (sizeof *s);` - Requires the systemcalls, "sbrk" and "mmap".
4. getting the current time of day - Requires the systemcall, "gettimeofday".
5. `double e = exp(1.0);` - Can be done in user-mode, no system call needed.

# Question 2
1. `n=write(1,"XYZ",3);` - In snippet 1 represents the file decriptor, "XYZ" is the buffer, and 3 is the count bytes. No errors, prints out "XYZ" and returns n = 3. 
2. `fd=open("/ooopsy",O_WRONLY|O_CREAT|O_TRUNC,0666);` - fd returns as -1. The errno is equal to 13, which corresponds to "Permission denied".
3. `char buf[3]; for (;;) printf("%d\n", read(fd,buf,3)); /* fd is a file on disk open for reading; the associate file is 10 bytes long*/` - There is no error when the code executes, fd returns as 10. 
4. `n=write(1,NULL,1);` - The code will have an error, aka n=-1. Errno is equal to 14, which corresponds to Bad address (NULL is not a valid address to write to).
5. `n=close(-1);` - This will return n=-1. Errno is equal to 9, which corresponds to Bad file descriptor.
