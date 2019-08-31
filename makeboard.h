#ifndef MAKEBOARD_H
    #define MAKEBOARD_H

   typedef enum {revealed, concealed, marked, question} VISIBILITY;

    typedef struct TILE_STATE_STRUCT {
        // visible (revealed / concealed), mine, surrounding mines (number), marked (! mark or ? mark)
        VISIBILITY tile_visibility;
    
        // true if there is a mine, false if there is no mine
        bool is_mine;
    
        // keeps track of the total number of surrounding mines per tile
        int num_surrounding_mines;
    } TILE;

    typedef struct BOARD_STATE_STRUCT {
        int num_rows;
        int num_cols;
        TILE** board;
        int mines;
        int seed;
    } BOARD;

    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <time.h>
    #include <stdbool.h>
    void read_args(int argc);
    BOARD args_into_board(int argc, char** argv);
    BOARD create_board(BOARD b);
    void create_mines_seed(BOARD* b);
    void create_mines_time(BOARD* b);
    void count_mines_around_tile(BOARD* b);

    
#endif
