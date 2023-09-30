# Program 3 - Hunt

## To Test Program:
- Run `make`
- Run `./Hunt <targetfile> HuntingZone/`
    - <targetfile> can be target.txt, smallBigTarget.txt, bigTarget.txt, or Hunt (used as a binary fily tester)

- Synthax: ./Hunt <targetfile> <HuntingDirectory>
- Given a Target File within the Hunting Zone, the program attempts to find files that match the target file within a directory and goes through all entries of that directory recursively.
- A match is defined as the file and the target having the same amount of bytes and each corresponding bytes are the same for both files.
- The program also distinguishes between the original target, hard links to the targets, hard linkes to matches, and symbolic links.
- The program uses mmap, stat, dirent, and reports errors using errno (as well as strerror).

