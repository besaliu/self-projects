CC = gcc
CFLAGS = -Wall -Wextra -g -Wunused-parameter

crawler: crawler.o pagedir.o url.o bag.o hashtable.o set.o curl.o
	$(CC) $(CFLAGS) -o crawler crawler.o pagedir.o url.o bag.o hashtable.o set.o curl.o -lcurl

crawler.o: crawler.c crawler.h
	$(CC) $(CFLAGS) -c crawler.c

# Add rules for other object files

clean:
	rm -f *.o crawler

