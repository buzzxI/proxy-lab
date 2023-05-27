# Makefile for Proxy Lab 
#
# You may modify this file any way you like (except for the handin
# rule). You instructor will type "make" on your specific Makefile to
# build your proxy from sources.

.PHONY: all
all: clean build install run

.PHONY: build
build: 
	cmake -B build

.PHONY: install
install:
	cmake --build ./build
	cp ./build/proxy ./

.PHONY: run
run:
	./driver.sh >> log.txt

.PHONY: clean
clean:
	rm -rf proxy ./build log.txt proxy_log.txt