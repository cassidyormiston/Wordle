#include "wordle.h"

/*
 * Arguments: a length for the word to be guessed
 *            the number of attempts the user gets to complete the game
 *            the dictionary the user wishes to use
 *            All are optional with defaults being provided
 * Returns: 0 if the game was played correctly
 *          1 if the input was incorrect
 *          2 if the dictionary can't be opened
 *          3 is the user runs out of attempts
 * */
int main(int argc, char** argv) {
    /* check argc is a number between 0 and 5 and return status 1*/
    if (argc < 0 || argc > 6) {
        fprintf(stderr, ERR_INPUT_STRUCT);
        return 1;
    }

    int statusNumber = valid_arguments(argc, argv);
    if (statusNumber == 0) {
        return 0;
    } else if (statusNumber == 1) {
        return 1;
    } else if (statusNumber == 2) {
        return 2;
    } else if (statusNumber == 3) {
        return 3;
    }
    return 0;
}

/*
 * Checks that all the arguments provided by the user are legal
  * Arguments: argc - the number of arguments priovided by the user
 *            argv - An array of each argument provided by the user
 * Returns: integer that is the return status for the main to execute.
 */
int valid_arguments(int argc, char** argv) {
    int gameSize = 0;
    int numberAttempts = 0;
    int containsLen = 0;
    int containsMax = 0;
    char* dictionary = NULL;
    // check that the arguments are legal
    for (int i = 1; i < argc; i++) {
        char* argument = argv[i];
        char* incrementedArgument = argv[i + 1];
        char* decrementedArgument = argv[i - 1];
        if (!strcmp(argument, " ")) {
            // argument is an empty string
            fprintf(stderr, ERR_INPUT_STRUCT);
            return 1;
        } else if (argc == 1) {
            // break to the default conditions
            break;
        } else if (argc == 2) {
            // is -len or -max 
            if (!strcmp(argument, "-len") || !strcmp(argument, "-max") || 
                    starts_with_dash(argument)) {
                fprintf(stderr, ERR_INPUT_STRUCT);
                return 1;
            } else {
                // to be treated as a dictionary
                if (!fopen(argument, "r")) {
                    fprintf(stderr,
                            "wordle: dictionary file \"%s\" cannot be opened\n"
                            , argument);
                    return 2;
                } else {
                    dictionary = argument;
                } 
            }
        } else {
            // argument is either -len or -max and their conditions
            if (!strcmp(argument, "-len") && containsLen == 0) {
                containsLen = 1;
                // the integer following -len is between 3-9 inclusive
                if (i + 1 < argc && atoi(incrementedArgument) >= 3 && 
                        atoi(incrementedArgument) <= 9 &&
                        strlen(incrementedArgument) == 1) {
                    gameSize = atoi(incrementedArgument);
                } else {
                    // errors out if the next argument is not an integer
                    // between 3 and 9
                    fprintf(stderr, ERR_INPUT_STRUCT);
                    return 1;
                }
            // is max followed by a 3 - 9 inclusive
            } else if (!strcmp(argument, "-max") && containsMax == 0) {
                containsMax = 1;
                // the integer following -max is between 3-9
                if (atoi(incrementedArgument) >= 3 && 
                        atoi(incrementedArgument) <= 9 && 
                        strlen(incrementedArgument) == 1) {
                    numberAttempts = atoi(incrementedArgument);
                } else {
                    // errors out if next argument is not an integer between
                    // 3 and 9
                    fprintf(stderr, ERR_INPUT_STRUCT);
                    return 1;
                }
            // when the loop comes to an integer that follows a -len or
            // -max skip to the next argument
            // This argument has already been dealt with
            } else if ((!strcmp(decrementedArgument, "-len") || 
                    !strcmp(decrementedArgument, "-max")) && 
                    strlen(argument) == 1) {
                continue;
            // starts with a dash and not the last argument
            } else if (starts_with_dash(argument) && 
                    strcmp(argument, argv[argc - 1])) {
                fprintf(stderr, ERR_INPUT_STRUCT);
                return 1;
            } else if (strcmp(argument, argv[argc - 1]) && 
                    strlen(argument) > 1) {
                fprintf(stderr, ERR_INPUT_STRUCT);
                return 1;
            } else {
                // try to open the file 
                if (!fopen(argument, "r")) {
                    fprintf(stderr, "wordle: dictionary file \"%s\" "
                            "cannot be opened\n", argument);
                    return 2;
                } else {
                    dictionary = argument;
                }
            }
        }
    }
    return starting_arguments(gameSize, numberAttempts, dictionary);
}

/*
 * Starts the game with the provided arguments
 * Arguments: gameSize: the game size the user wishes to use
 *            numberAttempts: the number of attempts the user has to complete
 *            the game.
 *            dictionary: the dictionary file path
 * Returns: an integer that represents an exit code
 */
int starting_arguments(int gameSize, int numberAttempts, char* dictionary) {
    // Applying a default 5 letters if len does not exist
    if (!gameSize) {
        gameSize = DEFAULT_LEN;
    }
    
    // Applying a default 6 attempts is max does not exist
    if (!numberAttempts) {
        numberAttempts = DEFAULT_ATT;
    }
        
    // Applying a default dictionary if one is not provided
    if (dictionary == NULL) {
        char defaultDictionary[] = DEFAULT_DICT;
        dictionary = defaultDictionary;
    }
    
    // start the game with the valid inputs
    return game(gameSize, numberAttempts, dictionary);
}

/*
 * Starts the game play 
 * Arguments: gameSize - the length of the word to be guessed 3-9 inclusive
 *            attempts - the number of attempts the user is allowed to guess
 *            the word
 *            dictionary - a file path to the desired dictionary to be used
 * Returns: void 
 */
int game(int gameSize, int numberAttempts, char* dictionary) {
    fprintf(stdout, "Welcome to Wordle!\n");
    // get a random word 
    char* answer = generate_random_word(dictionary, gameSize);
    printf("%s", answer);
    if (answer == NULL) {
        fprintf(stdout, "The dictionary provided does not contain a number of"
                "the requested size\n");
        return 2;
    }
    // guessed will evaluate as 1 when guess and answer are equal
    int guessed = 0;
    int takenAttempts = numberAttempts;
    char* guess = calloc(MAX_INPUT, sizeof(char));
    char* hintString = calloc(gameSize + 1, sizeof(char));
    char** storedDictionary = store_memory(dictionary, gameSize);
    // setting the default hintString to all dashes
    for (int i = 0; i < gameSize; i++) {
        hintString[i] = '-';  
    }    
    while (guessed == 0 && takenAttempts != 0) {
        // If on last attempt print (last attempt)
        if (takenAttempts == 1) {
            fprintf(stdout, "Enter a %d letter word (last attempt):\n",
                    gameSize);
        } else {
            // regular output message
            fprintf(stdout, "Enter a %d letter word (%d attempts remaining):\n"
                    , gameSize, takenAttempts);
        }
        // check if eof has occured in stdin
        if (!fgets(guess, MAX_INPUT, stdin)) {
            fprintf(stderr, "Bad luck - the word is \"%s\".\n", answer);
            free_dictionary(storedDictionary, guess, hintString, answer);
            return 3;
        }

        if (guess[strlen(guess) - 1] != '\n') {
            guess[strlen(guess)] = '\n';
            guess[strlen(guess) + 1] = '\0';
        }

        if (strlen(guess) != gameSize + 1) {
            // the guess was not of the right size
            fprintf(stdout, "Words must be %d letters long - try again.\n",
                    gameSize);
            continue;
        } else if (contains_illegal_arguments(guess)) {
            // the guess contains an illegal character or digit
            fprintf(stdout, SYMBOLS);
            continue;
        } else if (in_dictionary(storedDictionary, to_lower(guess)) == 2) {
            // guess correct length and no illegal chars
            fprintf(stdout, NOT_IN_DICT);
            continue;
        } else if (correct_guess(guess, answer)) {
            // the guess is correct
            fprintf(stdout, "Correct!\n");
            guessed = 1;
        } else {
            // give the user hints based off their guess
            hintString = hints(guess, answer, hintString);
            fprintf(stdout, "%s\n", hintString);
        }
        takenAttempts--;
    }
    return game_summary(takenAttempts, storedDictionary, guess, hintString,
            answer, guessed);
}

/*
 * Returns the correct status output based of the arguments provided
 * Arguments: takenAttempts: the amount of attempts taken by the user
 *            storedDictionary: the dictionary stored in memory
 *            guess: the users guess
 *            hintString: the stirng returned with hints to the answer
 *            answer: the answer to the game
 *            guessed: 1 if the user guessed correctly
 *                     0 if the user failed
 * Returns: An integer that given arguments uses to determine the exit status.
 */
int game_summary(int takenAttempts, char** storedDictionary, char* guess,
        char* hintString, char* answer, int guessed) {
    if (takenAttempts == 0) {
        // Ran out of attempts
        fprintf(stderr, "Bad luck - the word is %s\n", answer);
        free_dictionary(storedDictionary, guess, hintString, answer);
        return 3;
    } else if (guessed) {
        free_dictionary(storedDictionary, guess, hintString, answer);
        return 0;
    } else {
        free_dictionary(storedDictionary, guess, hintString, answer);
        return 0;
    }
}

/*
 * Checks the guess if the user has inputted any illegal punctuation,
 * digits or spaces
 * e.g. 1, 2, 3, ', !, ., ' '.
 * Arguments: guess - the users input
 * Returns: An integer 1 or 0 to allow for a conditional response which 
 *          is utilised in game
 *          1 - contains an illegal character
 *          0 - the guess contains only characters
 */
int contains_illegal_arguments(char* guess) {
    // loop through the string to determine if any are not apart of the
    // alphabet
    for (int i = 0; i < strlen(guess) - 1; i++) {
        if (isalpha((char) guess[i]) == 0) {
            return 1;
        } 
    }
    return 0;
}

/*
 * Checks if the given guess is apart of the provided  dictionary
 * Arguments: guess - the user's guess as a string
 *            dictionary - the path for the chosen dictionary
 * Returns: Either a 1 or 0 to allow for a conditional response 
 *          which is utilised in game
 *          1 - the guess is in the dictionary
 *          0 - the guess is not present in the dictionary and 
 *              therefore not a valid word
 */
int in_dictionary(char** dictionary, char* guess) {
    // loop through the dictionary to check if the guess is a real word
    int match = 0;
    int increment = 0;
    while (dictionary[increment] != NULL) {
       if (strcmp(dictionary[increment], to_lower(guess)) == 0) {
            match =1;
            return match;
       }
       increment++;     
    }
    match = 2;
    return match;
}

/*
 * Compares the guess to the answer by converting the guess to lower case.
 * Arguments: guess - the user's guess as a string
 *            answer - the random word answer as a string
 * Returns: Either 1 or 0 to allow for a conditional response
 *          which is utilised in game
 *          1 - the guess was incorrect
 *          0 - the guess was correct
 */
int correct_guess(char* guess, char* answer) {

    // compares the answer and the guess
    guess = to_lower(guess);
    int count = 0;
    for (int i = 0; i < strlen(answer); i++) {
        if (guess[i] == answer[i]) {
            count++;
        }
    }
    
    // if the count is equal to guess return true
    if (count == strlen(answer)) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Converts the given word to lower case letters 
 * Arguments: guess - the user's guess as a string
 * Returns: the string with all characters in their lowercase form
 */
char* to_lower(char* guess) {
    // convert to lower case letters
    for (int i = 0; i < strlen(guess); i++) {
        guess[i] = tolower(guess[i]);
    }
    return guess;
}

/*
 * Provides the user with hints based off their guess
 * Arguments: guess - the user's guess as a string
 *            answer - the answer to the wordle 
 *            givenHints - the string with which the hints are shown.
 *                         Always starts as all dashes.
 * Returns: the updated hints string.
 */
char* hints(char* guess, char* answer, char* givenHints) {
    // set positions to '-'
    for (int i = 0; i < strlen(answer); i++) {
        givenHints[i] = '-';
    }

    // look for exact match
    for (int i = 0; i < strlen(guess); i++) {
        if (answer[i] == guess[i]) {
            givenHints[i] = toupper(answer[i]);
        }
    }
    
    // find how many positions open
    int amountOfDash = 0;
    for (int i = 0; i < strlen(guess) - 1; i++) {
        if (givenHints[i] == '-') {
            amountOfDash++;
        }
    }

    int openPositions[amountOfDash];
    int increment = 0; 
    for (int j = 0; j < strlen(guess) - 1; j++) {
        // find the open positions e.g. '-'
        if (givenHints[j] == '-') {
            openPositions[increment] = j;
            increment++;
        }
    }

    // loop through all the characters in guess to see if
    // they match a character in an open position
    for (int k = 0; k < sizeof(openPositions) / sizeof(int); k++) {
        int positionSearching = openPositions[k];
        char searchingCharacter = answer[positionSearching];
        // dealing with multiple same characters
        int alreadyFound = 0;
        for (int z = 0; z < amountOfDash; z++) {
            if (guess[openPositions[z]] == searchingCharacter
                    && !alreadyFound) {
                givenHints[openPositions[z]] = tolower(searchingCharacter);
                alreadyFound = 1;
            }
        }
    }

    return givenHints;
}

/*
 * Checks to see if the argument starts with a dash
 * Arguments: lineArgument - the argument on the command line
 * Returns: int - 1 contains dash
 *              - 0 doesn't contains dash
 */
int starts_with_dash(char* lineArgument) {
    if (lineArgument[0] == '-') {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Stores the dictionary to memory so the games doesn't have to load in 
 * the dictionary every time it checks a word
 * Arguments: dictionary - a pointer to the dictionary file string
 *            gameSize - the size of words the game is playing
 * Returns:   char** - the dictionary containing all the words at the given 
 *                     game length
 */
char** store_memory(char* dictionary, int gameSize) {
    char** storedDictionary = malloc(sizeof(char*));
    char*  word;
    char line[MAX_INPUT];
    int numberOfValidLines = 1;
    FILE* openDictionary = fopen(dictionary, "r");
    while (fgets(line, MAX_INPUT, openDictionary) != NULL) {
        if (strlen(line) == gameSize + 1) {
            // memory error to ask if I have time
            word = strndup(line, gameSize+1);
            storedDictionary = realloc(storedDictionary,
                    sizeof(char*) * numberOfValidLines);
            storedDictionary[numberOfValidLines - 1] = word;
            numberOfValidLines++;
        } else {
            continue;
        }
    }

    storedDictionary = realloc(storedDictionary,
            sizeof(char*) * numberOfValidLines);
    storedDictionary[numberOfValidLines - 1] = NULL;
    fclose(openDictionary);
    return storedDictionary;
}

/*
 * Free's all the provided arguments memory
 */
void free_dictionary(char** dictionary, char* guess, char* hintString, 
        char* answer) {
    int increment = 0;
    while (dictionary[increment] != NULL) {
        free(dictionary[increment]);
        increment++;
    }
    free(dictionary);
    free(guess);
    free(hintString);
    free(answer);
}

/*
 * Generates a random word from the given dictionary
 * Arguments:
 *      char* dictionary - dictionary path
 *      int length - length of word to be searched
 * Returns:
 *      char* - the random word
 *      void - if the supplied dictionary does not have a word of the given 
 *      length
 */
char* generate_random_word(char* dictionary, int length) {
    FILE* dict = fopen(dictionary, "r");
    // list of all the words with the given length
    char** words = malloc(sizeof(char*)*1000000);
    int counter = 0;
    char buffer[10];
    char* word;
    time_t t;
    while (fgets(buffer, 10, dict) != NULL) {
        if (strlen(buffer) == length+1) {
            word = strndup(buffer, length+1);
            words[counter] = word;
            counter++;
        }
    }
    if (counter == 0) {
        return NULL;
    }
    srand((unsigned) time(&t));
    int randomNumber = rand()%counter;
    return words[randomNumber];
}

