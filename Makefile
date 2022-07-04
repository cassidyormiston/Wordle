all: wordle
wordle: wordle.c wordle.h
	gcc -pedantic -Wall -g -std=gnu99 -o $@ $<

clean: 
	rm wordle
