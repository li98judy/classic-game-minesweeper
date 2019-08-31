#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "makeboard.h"
#include "playgame.h"


/*  Arguments on the commandline:
        1. The number of rows
        2. The number of columns
        3. The number of mines 
        4. Optionally a seed to the random number generator
*/


int main(int argc, char** argv) {
    read_args(argc);
    
    // makes gameboard and places the mines inside the board
    BOARD b = args_into_board(argc, argv);
    
    play_game(b);
    
    return 0;
}

