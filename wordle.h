#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <csse2310a1.h>

#define MAX_INPUT 100
#define ERR_INPUT_STRUCT "Usage: wordle [-len word-length] [-max max-guesses] [dictionary]\n"
#define DEFAULT_LEN 5
#define DEFAULT_ATT 6
#define DEFAULT_DICT "/usr/share/dict/words"
#define SYMBOLS "Words must contain only letters - try again.\n"
#define NOT_IN_DICT "Word not found in the dictionary - try again.\n"

int valid_arguments(int argc, char** argv);

int starting_arguments(int gameSize, int numberAttempts, char* dictionary);

int game(int gameSize, int attempts, char* dictionary);

int game_summary(int takenAttempts, char** storedDictionary, char* guess, char* hintString, char*answer, int guessed);

int contains_illegal_arguments(char* guess);

int correct_guess(char* guess, char* answer);

int in_dictionary(char** dictionary, char* guess);

char* to_lower(char* guess);

char* hints(char* guess, char* answer, char* givenHints);

int starts_with_dash(char* lineArgument);

char** store_memory(char* dictionary, int gameSize);

void free_dictionary(char** dictionary, char* guess, char* hintString, char* answer);
