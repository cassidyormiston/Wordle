# Wordle
This program allows you to play the Wordle game via terminal interaction

This game is based off the NY Times game wordle see www.nytimes.com/games/wordle/ for more information

## How to Run

Within the same directory as the provided files: 

For the default game (5 letter random word from a default dictionary, 6 attempts) run:

`./wordle`

For custom game play

`./wordle -len [length of word] -max [number of attempts] [dictionary path]`

## How To Play

The program will determine a random word from the given or default dictionary of the desired word length that is known to the user. The program will repeatedly prompt the user to guess the answer until they get it right or they run out of attempts.
For incorrect guesses, if the guess is valid (i.e. is the right length and appears in the nomiated dictionary) then the program will report on which letters matched those in the answer (if any) - i.e which letters were in the right place, which were
in the wrong place, and which weren't in the answer at all. Invalid guesses don't count as an attempt - the user will be prompted to enter another guess.
