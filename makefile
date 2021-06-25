CC = gcc
CFLAGS = -lm -Wall -g

app: main.c process.o find_word_from_index.o find_word_from_text.o
	$(CC) $(CFLAGS) main.c process.o find_word_from_index.o find_word_from_text.o -o app

process.o: process.c
	$(CC) $(CFLAGS) -c process.c

find_word_from_index.o: find_word_from_index.c
	$(CC) $(CFLAGS) -c find_word_from_index.c

find_word_from_text.o: find_word_from_text.c
	$(CC) $(CFLAGS) -c find_word_from_text.c

claen:
	rm *.o app
