#ifndef PLAYGAME_H
    #define PLAYGAME_H
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <time.h>
    #include <stdbool.h>
    #include "makeboard.h"
    void play_game(BOARD b);
    bool game_over(BOARD b);
    bool game_lost(BOARD b);
    bool game_won(BOARD b);
    void print_board(BOARD b);
    void destroy_board(BOARD* b);
    void play_option_mark(BOARD* b, int user_row, int user_col, int action);
    void play_option_question(BOARD* b, int user_row, int user_col, int action);
    void play_option(BOARD* b, int user_row, int user_col, int action);
    void recursive_reveal(BOARD* b, int user_row, int user_col, bool to_end);
    
#endif
