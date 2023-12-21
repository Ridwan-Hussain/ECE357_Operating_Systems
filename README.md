Github repository with homework submissions for ECE357 - Operating Systems with Professor Hak during Fall 2023 semester.

## Programs
- Meow 
    - A cat duplicate that allows for user input and standard input and prints content to a file or standard out
- Hunt
    - Given a target file, recursively goes through all directories and find files that matches the target file
    - A match is defined as a file with the same size and same content byte-by-byte
- rsh
    - Made my own shell that takes in user inputs and execs the programs on the bash shell
    - Made my own pwd and cd command
    - Shell is able to redirect stdin, stdout, and stderr
- Pager
    - Made recreated a simple version of pager (older version of less) with multiple files. 
        - wordgen.c generates a list of random words
        - wordsearch.c checks a user input for a word and sees if it's in the supplied dictionary.
        - pager.c lists the output from standard input in increments of 23 lines.
        - launcher.c combines all the different files together through pipes.
- Smear
    - Simple find and replace program that takes in two string inputs, one is the target and the other is the replacement, and does the find and replace on all the files given as inputs.
