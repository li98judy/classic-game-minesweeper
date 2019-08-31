#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "makeboard.h"

//FIXME: error message
void read_args(int argc){
    
    if (argc < 4){
        printf("Not enough arguments. Usage:\n");
        //FIXME: error message
        printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        exit(0);
    }
    if (argc > 5){
        printf("Too many arguments. Usage:\n");
        //FIXME: error message
        printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        exit(0);
    }
}


BOARD args_into_board(int argc, char** argv){
    BOARD b;
    
    int rows = 0;
    int cols = 0;
    int mines = 0;
    
    // when no seed is given
    if (argc == 4) {
        sscanf(argv[1], "%d", &rows);
        sscanf(argv[2], "%d", &cols);
        sscanf(argv[3], "%d", &mines);
        
        //FIXME: error message
        if(mines > (rows * cols)){
            printf("Too many mines");
            exit(0);
        }
        
        b.num_rows = rows;
        b.num_cols = cols;
        b.mines = mines;
        b = create_board(b);
        
        // places mines in gameboard
        create_mines_time(&b);
        
        // counts how many surrounding mines it has
        count_mines_around_tile(&b);
    }
    
    // when seed is given
    else if (argc == 5) {
        int seed = 0;
        
        sscanf(argv[1], "%d", &rows);
        sscanf(argv[2], "%d", &cols);
        sscanf(argv[3], "%d", &mines);
        sscanf(argv[4], "%d", &seed);
        
        //FIXME: error message
        if(mines > (rows * cols)){
            printf("Too many mines");
            exit(0);
        }
        
        b.num_rows = rows;
        b.num_cols = cols;
        b.mines = mines;
        b.seed = seed;
        b = create_board(b);
        
        // places mines in gameboard
        create_mines_seed(&b);
        
        // counts how many surrounding mines it has
        count_mines_around_tile(&b);
    }
    
    else{
        exit(0);
    }
    return b;
}

BOARD create_board(BOARD b) {
    int i = 0;
    int k = 0;
    
    b.board = (TILE**) malloc(b.num_rows * sizeof(TILE*));
    
    for (i = 0; i < b.num_rows; i++) {
        b.board[i] = (TILE*) malloc(b.num_cols * sizeof(TILE));
        
        // makes all tiles blank
        for(k = 0; k < b.num_cols; k++) {
            // 0 means a blank tile
            b.board[i][k].is_mine = false;
            b.board[i][k].num_surrounding_mines = 0;
            b.board[i][k].tile_visibility = concealed;
            
        }
    }
    
    return b;
}

//FIXME: potential problem
void create_mines_time(BOARD* b){
    int i = 0;
    int x = 0;
    int y = 0;
    
    srand(time(0));
    
    for(i = 0; i < b->mines; i++) {
        // randomly find the point of mine
        do{
            x = b->num_rows - rand() % b->num_rows - 1;
            y = rand() % b->num_cols;
            
        } while (b->board[x][y].is_mine == true);
        
        b->board[x][y].is_mine = true;
        
        printf("Placing mine at %d, %d\n", b->num_rows - x - 1, y);
    }
    printf("There are %d mines left\n", b->mines);
}

void create_mines_seed(BOARD* b) {
    int i = 0;
    int x = 0;
    int y = 0;
    
    srand(b->seed);
    
    for(i = 0; i < b->mines; i++){
        // randomly find the point of mine
        do{
            x = b->num_rows - rand() % b->num_rows - 1;
            y = rand() % b->num_cols;
        
        } while (b->board[x][y].is_mine == true);
        
        b->board[x][y].is_mine = true;
        
        printf("Placing mine at %d, %d\n", b->num_rows - x - 1, y);
    }
    printf("There are %d mines left\n", b->mines);
}

void count_mines_around_tile(BOARD* b){
    int i = 0;
    int k = 0;
    int count = 0;
    
    for (i = 0; i < b->num_rows; i++) {
        for (k = 0; k < b->num_cols; k++) {
            if (b->board[i][k].is_mine == true) {
                continue;
            }
            else {
                // #1 condition to check if it's out of bounds
                if ((i - 1 >= 0) && (k - 1 >= 0)) {
                    if (b->board[i - 1][k - 1].is_mine == true){
                        count++;
                    }// check if there is a mine
                    
                }
                // #2 condition to check if it's out of bounds
                if (i - 1 >= 0) {
                    if (b->board[i - 1][k].is_mine == true) {
                        count++;    
                    }
                }
                // #3 condition to check if it's out of bounds
                if ((i - 1 >= 0) && (k + 1 < b->num_cols)) {
                    if (b->board[i - 1][k + 1].is_mine == true) {
                        count++;
                    }
                }
                
                // #4 condition to check if it's out of bounds
                if (k - 1 >= 0) {
                    if (b->board[i][k - 1].is_mine == true) {
                        count++;
                    }
                }
                
                // #5 condition to check if it's out of bounds
                if (k + 1 < b->num_cols) {
                    if (b->board[i][k + 1].is_mine == true) {
                        count++;
                    }
                }
                
                // #6 condition to check if it's out of bounds
                if ((i + 1 < b->num_rows) && (k - 1 >= 0)) {
                    if (b->board[i + 1][k - 1].is_mine == true) {
                        count++;
                    }
                }
                
                // #7 condition to check if it's out of bounds
                if (i + 1 < b->num_rows) {
                    if (b->board[i + 1][k].is_mine == true) {
                        count++;
                    }
                }
                
                // #8 condition to check if it's out of bounds
                if ((i + 1 < b->num_rows) && (k + 1 < b->num_cols)) {
                    if (b->board[i + 1][k + 1].is_mine == true) {
                        count++;
                    }    
                }
                
                // places the count of mines on current tile
                b->board[i][k].num_surrounding_mines = count;
            }
            
            // reset the count to check the next tile
            count = 0;
        }
    }
    return;
}
