CFLAGS = -Wall -Wextra -Wpedantic

smear: smear.c file1 file2 file3
	gcc -o smear smear.c $(CFLAGS)
	./smear int INT file1 file2 file3

reset: file1 file2 file3 origFile1 origFile2 origFile3
	cp origFile1 file1
	cp origFile2 file2
	cp origFile3 file3
