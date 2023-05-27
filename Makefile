# Makefile for Proxy Lab 
#
# You may modify this file any way you like (except for the handin
# rule). You instructor will type "make" on your specific Makefile to
# build your proxy from sources.

CC := gcc
CFLAGS := -g -Wall
LDFLAGS = -lcsapp

.PHONY: all
all: proxy

proxy.o: proxy.c 
	$(CC) $(CFLAGS) -c $^ -o $@

proxy: proxy.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz *.txt

