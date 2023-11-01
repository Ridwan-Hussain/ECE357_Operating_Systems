CC=gcc

pager: wordgen.c wordsearch.c pager.c launcher.c
	$(CC) -o wordgen wordgen.c -g
	$(CC) -o wordsearch wordsearch.c -g
	$(CC) -o pager pager.c -g
	$(CC) -o launcher launcher.c -g

full1M: wordgen.c wordsearch.c pager.c
	./wordgen 1000000 | ./wordsearch dict.txt | ./pager

fullinf: wordgen.c wordsearch.c pager.c
	./wordgen | ./wordsearch dict.txt | ./pager

full3k: wordgen.c wordsearch.c pager.c
	./wordgen 3000 | ./wordsearch dict.txt | ./pager

full100k: wordgen.c wordsearch.c pager.c
	./wordgen 100000 | ./wordsearch dict.txt | ./pager

good1M: wordgen.c wordsearch.c pager.c
	./wordgen 1000000 | ./wordsearch words.txt | ./pager

goodinf: wordgen.c wordsearch.c pager.c
	./wordgen | ./wordsearch words.txt | ./pager

good100k: wordgen.c wordsearch.c pager.c
	./wordgen 100000 | ./wordsearch words.txt | ./pager
