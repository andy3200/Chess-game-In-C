#include "hw4.h"

void initialize_game(ChessGame *game) {
    game->capturedCount = 0;
    game->moveCount = 0;
    game->currentPlayer = WHITE_PLAYER;
    char initial_board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
}
/// @brief checks whether the piece is white or not 
/// @param c the piece  
/// @return  return 1 if true return 0 if black 
int check_white(char c) {
    return (c >= 'A' && c <= 'Z');
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    (void)fen;
    (void)game;
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int move_direction_vertical;
    int move_direction_horizontal;
    int eating = 0;
    move_direction_horizontal = dest_col - src_col; // - means left + means right 
    move_direction_vertical = dest_row - src_row; // - means moving up. + means moving down 
    int net_movement_vertical = abs(move_direction_vertical);
    int net_movement_horizontal = abs(move_direction_horizontal);
    
    if((net_movement_horizontal == 0) && (net_movement_vertical) ==0){//not moving at all 
        return false; 
    }

    if((game->currentPlayer == WHITE_PLAYER) && (move_direction_vertical > 0)){ //white but moving down 
        return false ;
    }
    if((game->currentPlayer == BLACK_PLAYER) && (move_direction_vertical < 0)){ //black but moving up 
        return false ;
    }
    if((net_movement_horizontal != 0)){ // the pawn is eating something. 
        if(net_movement_vertical != 1){ //move horizontal but not vertical
            return false;
        }
        if(net_movement_horizontal > 1){ //moving horizontally more than 1 space left or right 
            return false;
        }
        eating = 1;
    }
    if(net_movement_vertical == 2){ //move up or down by 2 
        if(((game->currentPlayer == WHITE_PLAYER) && (src_row == 6)) ||((game->currentPlayer == BLACK_PLAYER) && (src_row == 1)) ){
            //valid, now check if there is something in between
            if(game->currentPlayer == WHITE_PLAYER){
                if(game->chessboard[dest_row+1][dest_col] != '.'){ //thing in between is not empty 
                    return false;
                }else if(game->chessboard[dest_row][dest_col] != '.'){ //destination is not empty 
                    return false; 
                }
                else{//valid move 
                    return true; 
                }
            }
            if(game->currentPlayer == BLACK_PLAYER){
                if(game->chessboard[dest_row-1][dest_col] != '.'){
                    return false;
                }else{
                    return true; 
                }
            }


        }else if(net_movement_horizontal != 0){ //move 2 vertically but also moving to left or right 
            return false; 
        }
        else{
            return false; // moved 2 but not in starting position 
        }
    }
    if(net_movement_vertical> 2){ // cant move more than 2 
        return false;
    }
    if(eating){
        if( (game->currentPlayer == WHITE_PLAYER) && (check_white(game->chessboard[dest_row][dest_col]) == 1) ){ // white eat white 
            return false;
        }
        else if((game->currentPlayer == BLACK_PLAYER) && (check_white(game->chessboard[dest_row][dest_col]) == 0)){//black eat black
            return false;
        }else{
            return true;
        }
    }
    if(game->chessboard[dest_row][dest_col] != '.'){ //destination not empty 
        return false;
    }else{
        return true; 
    }
    
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    (void)fen;
    (void)game;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    (void)move;
    (void)parsed_move;
    return -999;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return -999;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    (void)game;
    (void)username;
    (void)db_filename;
    return -999;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    (void)game;
    (void)username;
    (void)db_filename;
    (void)save_number;
    return -999;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
