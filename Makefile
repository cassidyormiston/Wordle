all: wordle
wordle: wordle.c wordle.h
	gcc -pedantic -Wall -g -std=gnu99 -o $@ -I/local/courses/csse2310/include -L/local/courses/csse2310/lib -lcsse2310a1 $<

clean: 
	rm wordle
