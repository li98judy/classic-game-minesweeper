#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "playgame.h"
#include "makeboard.h"
void print_board(BOARD b) {
    int i = 0;
    int k = 0;
    int m = 0;
    
    
    for (i = 0; i < b.num_rows; ++i) {
        printf("%d ", b.num_rows - 1 - i);
        
        for (k = 0; k < b.num_cols; ++k) {
            if (b.board[i][k].tile_visibility == concealed) {
                printf("# ");
            }
            else if (b.board[i][k].tile_visibility == marked) {
                printf("! ");
            }
            else if (b.board[i][k].tile_visibility == question) {
                printf("? ");
            }
            //FIXME: call another function bc revealed can be anything
            else if (b.board[i][k].tile_visibility == revealed) {
                if (b.board[i][k].is_mine == true) {
                    printf("* ");
                }
                else if (b.board[i][k].num_surrounding_mines > 0) {
                    printf("%d ", b.board[i][k].num_surrounding_mines);    
                }
                else if (b.board[i][k].num_surrounding_mines == 0) {
                    printf("0 ");
                }
            }
        }
        printf("\n"); 
    }
    
    printf("  ");
    
    // prints out the numbers that will label the columns
    for (m = 0; m < b.num_cols; m++) {
        // prints out the numbers that will label the rows
        printf("%d ", m);
    }
    
    printf("\n");
}

bool game_over(BOARD b){
    return game_lost(b) || game_won(b);
}

// user looses, if they click a mine
bool game_lost(BOARD b){
    int i = 0;
    int k = 0;
    
    for(i = 0; i < b.num_rows; i++){
        for(k = 0; k < b.num_cols; k++){
            if((b.board[i][k].is_mine == true) && (b.board[i][k].tile_visibility == revealed)){
                return true;
            }
        }
    }
    
    return false;
}

// user wins, if tiles are all revealed, and all the mines have a ! mark
bool game_won(BOARD b){
    int i = 0;
    int k = 0;
    int count = 0;
    
    for(i = 0; i < b.num_rows; i++){
        for(k = 0; k < b.num_cols; k++){
            if((b.board[i][k].is_mine == true) && (b.board[i][k].tile_visibility != marked)){
                return false;
            }
            if((b.board[i][k].is_mine == true) && (b.board[i][k].tile_visibility == marked)){
                count++;
            }
            else if(b.board[i][k].tile_visibility == revealed){
                count++;
            }
        }
    }
    
    if (count == b.num_cols * b.num_rows){
        return true;
    }
    else{
        return false;
    }
}

void recursive_reveal(BOARD* b, int user_row, int user_col, bool to_end){
    if(user_row < 0 || user_row > b->num_rows - 1 || user_col < 0 || user_col > b->num_cols - 1){
        return;
    }
    else if(b->board[user_row][user_col].tile_visibility == marked){
        return;
    }
    else if(b->board[user_row][user_col].tile_visibility == question){
        return;
    }
    else if(b->board[user_row][user_col].tile_visibility != concealed && to_end == true){
        return;
    }
    else if(b->board[user_row][user_col].is_mine == true){
        return;
    }
    else if(b->board[user_row][user_col].num_surrounding_mines == 0 && to_end == false){
        b->board[user_row][user_col].tile_visibility = revealed;
        recursive_reveal(b, user_row - 1, user_col - 1, true);
        recursive_reveal(b, user_row - 1, user_col, true);
        recursive_reveal(b, user_row - 1, user_col + 1, true);
        recursive_reveal(b, user_row, user_col - 1, true);
        recursive_reveal(b, user_row, user_col + 1, true);
        recursive_reveal(b, user_row + 1, user_col - 1, true);
        recursive_reveal(b, user_row + 1, user_col, true);
        recursive_reveal(b, user_row + 1, user_col + 1, true);
    }
    else if(b->board[user_row][user_col].num_surrounding_mines == 0 && to_end == true){
        b->board[user_row][user_col].tile_visibility = revealed;
        recursive_reveal(b, user_row - 1, user_col - 1, true);
        recursive_reveal(b, user_row - 1, user_col, true);
        recursive_reveal(b, user_row - 1, user_col + 1, true);
        recursive_reveal(b, user_row, user_col - 1, true);
        recursive_reveal(b, user_row, user_col + 1, true);
        recursive_reveal(b, user_row + 1, user_col - 1, true);
        recursive_reveal(b, user_row + 1, user_col, true);
        recursive_reveal(b, user_row + 1, user_col + 1, true);
    }
    else{
        b->board[user_row][user_col].tile_visibility = revealed;
    }
    return;    
}

void play_option_question(BOARD* b, int user_row, int user_col, int action) {
    // Enter Action
    // 0. UnQuestion
    // 1. Cancel
    if (action == 0) {
        b->board[user_row][user_col].tile_visibility = concealed;
    }

}

void play_option_mark(BOARD* b, int user_row, int user_col, int action){
    // Enter Action
    // 0. UnMark
    // 1. Cancel
    
    if(action == 0){
        b->board[user_row][user_col].tile_visibility = concealed;
        b->mines = b->mines + 1;
    }
}
void play_option(BOARD* b, int user_row, int user_col, int action){
    // Enter Action
    // 0. Reveal
    // 1. Question
    // 2. Mark
    // 3. Cancel
    
    if(action == 0){
        if(b->board[user_row][user_col].num_surrounding_mines == 0){
            recursive_reveal(b, user_row, user_col, false);
        }
        else{
            b->board[user_row][user_col].tile_visibility = revealed;
        }
    }
    else if (action == 1) {
        b->board[user_row][user_col].tile_visibility = question;
    }
    else if(action == 2){
        // case #1: the ! mark actually marks a mine that is underneath, so we need to update the # of mines left
        // case #2: the ! mark does not mark a mine underneath, so DO NOT update the # of mines left
        b->board[user_row][user_col].tile_visibility = marked;
        b->mines = b->mines - 1;
    }
}

void play_game(BOARD b) {
    int user_row = 0;
    int row = 0;
    int col = 0;
    int action = 0;
    
    int i = 0;
    int k = 0;
    int argsRowColRead = 0;
    int argsActionRead = 0;
    
    //print board after it has been created
    print_board(b);
    
    // while loop
    while(!game_over(b)) {
        do {
            // gets move from user
            printf("Enter row a row between 0-%d and a column between 0-%d: ", b.num_rows - 1, b.num_cols - 1);
            argsRowColRead = scanf(" %d %d", &user_row, &col);
            row = b.num_rows - user_row - 1;
        } while (user_row < 0 || user_row > b.num_rows - 1 || col < 0 || col > b.num_cols - 1);
        
        if (argsRowColRead < 2 || argsRowColRead > 2) {
            continue;
        }
        if (b.board[row][col].tile_visibility == revealed) {
            printf("This tile is already revealed.\n");
            continue;
        }
        // depending on the tile, if there is already a ? mark or ! mark, then the user can choose to remove or cancel their move
        if (b.board[row][col].tile_visibility == marked) {
            printf("Enter Action\n0. UnMark\n1. Cancel\n");
            printf("Action: ");
            argsActionRead = scanf(" %d", &action);
            if (argsActionRead > 1 || argsActionRead < 1) {
                continue;
            }
            if(action < 0 || action > 1){
                continue;
            }
            else if(action == 1){
                continue;
            }
            else {
                play_option_mark(&b, row, col, action);
            }
        }
        else if (b.board[row][col].tile_visibility == question) {
            printf("Enter Action\n0. UnQuestion\n1. Cancel\n");
            printf("Action: ");
            argsActionRead = scanf(" %d", &action);
            if (argsActionRead > 1) {
                continue;
            }
            if(action < 0 || action > 1){
                continue;
            }
            if(action == 1){
                continue;
            }
            play_option_question(&b, row, col, action);
        }
        else if (b.board[row][col].is_mine == true && b.board[row][col].tile_visibility == concealed) {
            printf("Enter Action\n0. Reveal\n1. Question\n2. Mark\n3. Cancel\n");
            printf("Action: ");
            argsActionRead = scanf(" %d", &action);
            if (argsActionRead > 1) {
                continue;
            }
            if(action < 0 || action > 3){
                continue;
            }
            if(action == 3){
                continue;
            }
            else if (action == 1 || action == 2) {
                play_option(&b, row, col, action);
            }
            else if (action == 0) {
                b.board[row][col].tile_visibility = revealed;
                for (i = 0; i < b.num_rows; i++) {
                    for (k = 0; k < b.num_cols; k++) {
                        b.board[i][k].tile_visibility = revealed;
                    }
                }
                print_board(b);
                printf("You Lost :(\n");
                exit(0);
            }
        }
        else{
            printf("Enter Action\n0. Reveal\n1. Question\n2. Mark\n3. Cancel\n");
            printf("Action: ");
            scanf(" %d", &action);
            if(action < 0 || action > 3){
                continue;
            }
            if(action == 3){
                continue;
            }
            if (action == 0 && b.board[row][col].tile_visibility == revealed) {
                printf("This tile is already revealed.\n");
                continue;
            }
            play_option(&b, row, col, action);
        }
        
        if (game_lost(b) ==  true) {
            for (i = 0; i < b.num_rows; i++) {
                for (k = 0; k < b.num_cols; k++) {
                    b.board[i][k].tile_visibility = revealed;
                }
            }
            print_board(b);
            printf("You Lost :(\n");
            exit(0);
        }
        else if (game_won(b) == true){
            for(i = 0; i < b.num_rows; i++){
                for(k = 0; k < b.num_cols; k++){
                    b.board[i][k].tile_visibility = revealed;
                }
            }
            print_board(b);
            printf("You Won!!\n");
            exit(0);
        }
        
        // displays the board
        printf("There are %d mines left\n", b.mines);
        print_board(b);
    }
    
    
    destroy_board(&b);
}
void destroy_board(BOARD* b) {
    int i = 0;
    
    for (i = 0; i < b->num_rows; i++) {
        free(b->board[i]);
    }
    
    free(b->board);
    
    return;
}
