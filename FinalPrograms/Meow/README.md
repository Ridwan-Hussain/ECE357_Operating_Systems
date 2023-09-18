Meow is a program similar to cat on a linux shell.

1. Run `gcc -o meow meow.c`
2. Can run `./meow -o output`; reads stdin and prints to outFile.
3. Can run `./meow -o output2 file1 - file2 -`; prints to output2 the contents of file1, stdin, file2, then stdin again.
4. Can run `./meow file3`; prints file3 on the terminal (with the byte count being greater than the buffer size).
5. Can run `./meow -o binOut meow` to test the binary file.
