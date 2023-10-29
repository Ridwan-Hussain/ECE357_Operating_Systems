CC=gcc

pager: wordgen.c wordsearch.c pager.c
	$(CC) -o wordgen wordgen.c
	$(CC) -o wordsearch wordsearch.c
	$(CC) -o pager pager.c

full1M: wordgen.c wordsearch.c pager.c
	./wordgen 1000000 | ./wordsearch dict.txt | ./pager

fullinf: wordgen.c wordsearch.c pager.c
	./wordgen | ./wordsearch dict.txt | ./pager
