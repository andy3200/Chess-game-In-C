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
    for(int x = 0; x < 8; x++){
        for(int y = 0; y <8 ; y++){
            game->chessboard[x][y] = initial_board[x][y];
        }
    }
}
/// @brief checks whether the piece is white or not 
/// @param c the piece  
/// @return  return 1 if true return 0 if black 
int check_white(char c) {
    return (c >= 'A' && c <= 'Z');
}

//return 1 if it is within bounds and 0 if not. 
int check_bounds(int dest_row, int dest_col) {
    return ((dest_row >= 0 && dest_row < 8) && (dest_col >= 0 && dest_col < 8));
}

int check_eating(int dest_row, int dest_col, ChessGame *game){
    if( (game->currentPlayer == WHITE_PLAYER) && (check_white(game->chessboard[dest_row][dest_col]) == 1) ){ // white eat white 
        return 0;
    }
    else if((game->currentPlayer == BLACK_PLAYER) && (check_white(game->chessboard[dest_row][dest_col]) == 0)){//black eat black
        return 0;
    }
    if((game->chessboard[dest_row][dest_col] == 'K') || (game->chessboard[dest_row][dest_col] == 'k')){
        return 0;
    }
    return 1;
}

/* 
int check_basic(char piece, int dest_row, int dest_col, ChessGame *game){
     //check if player is using the right piece 
    if(check_white(piece) == 1 && game->currentPlayer != WHITE_PLAYER){
        return 0; 
    }
    else if(check_white(piece) == 0 && game->currentPlayer != BLACK_PLAYER){
        return 0; 
    }
    else{
        return 1; 
    }
    
}
*/


bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game){
    (void)piece;
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
        if(net_movement_vertical != 1){ //move horizontal but not vertical or move more than by vertical
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
        if(check_eating(dest_row,dest_col,game) == 0){ // some eating rules is violated 
            return false; 
        }
        else if((game->chessboard[dest_row][dest_col]) == '.'){//eating empty 
            return false; 
        }
        else{
            return true;
        }
    }
    //now you move up or down only by one 
    if(game->currentPlayer == WHITE_PLAYER){
        if(check_white(game->chessboard[dest_row][dest_col]) == 0){ //eating enemy 
            return false;
        }else{
            return true; //this should be invalid but handle it in part 4 
        }
    }else if(game->currentPlayer == BLACK_PLAYER){
        if(check_white(game->chessboard[dest_row][dest_col]) == 1){//eating enemy 
            return false; 
        }else{
            return true; // capturing ally. handle in part 4 
        }
    }else if(game->chessboard[dest_row][dest_col] == '.'){
        return true;
    }else{
        return false; 
    }
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int move_direction_vertical;
    int move_direction_horizontal;
    move_direction_horizontal = dest_col - src_col; // - means left + means right 
    move_direction_vertical = dest_row - src_row; // - means moving up. + means moving down 
    int net_movement_vertical = abs(move_direction_vertical);
    int net_movement_horizontal = abs(move_direction_horizontal);
    int row_move; 
    int col_move; 
    //check if moving diagonally 
    if(net_movement_horizontal > 0){
        if(net_movement_vertical != 0){ // moving diagonally 
            return false; 
        }
    }
    if(net_movement_vertical > 0){
        if(net_movement_horizontal != 0){ // moving diagonally 
            return false; 
        }
    }

    if(move_direction_horizontal <0){//moving left 
        col_move = -1;
        row_move = 0; 
    }else if(move_direction_horizontal >0){ //moving right 
        col_move = 1;
        row_move = 0;
    }
    if(move_direction_vertical <0){//moving up 
        row_move = -1;
        col_move = 0; 
    }else if(move_direction_horizontal >0){ //moving down 
        row_move = 1;
        col_move = 0;
    }

    //starting checking for obstructing piece 
    int row, col;
    for (row = src_row + row_move, col = src_col + col_move; row != dest_row || col != dest_col; row += row_move, col += col_move) {
        if (game->chessboard[row][col] != '.') {
            return false; // Path is blocked by another piece
        }
    }
    return true; 
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    int move_direction_vertical;
    int move_direction_horizontal;
    move_direction_horizontal = dest_col - src_col; // - means left + means right 
    move_direction_vertical = dest_row - src_row; // - means moving up. + means moving down 
    int net_movement_vertical = abs(move_direction_vertical);
    int net_movement_horizontal = abs(move_direction_horizontal);
    if((net_movement_horizontal == 1 && net_movement_vertical == 2) || (net_movement_horizontal == 2 && net_movement_vertical == 1)) {
        return true;
    }else{
        return false; 
    }
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int move_direction_vertical;
    int move_direction_horizontal;
    move_direction_horizontal = dest_col - src_col; // - means left + means right 
    move_direction_vertical = dest_row - src_row; // - means moving up. + means moving down 
    int net_movement_vertical = abs(move_direction_vertical);
    int net_movement_horizontal = abs(move_direction_horizontal);
    int row_move;
    int col_move; 
    if(net_movement_horizontal != net_movement_vertical){ //not moving diagonally 
        return false; 
    }
    if(move_direction_horizontal > 0){
        col_move = 1;
    }else{
        col_move = -1;
    }
    if(move_direction_vertical > 0){
        row_move = 1; 
    }else{
        row_move = -1;
    }
    int x,y; 
    for( x = src_row + row_move, y = src_col + col_move; x != dest_row && y != dest_col; x += row_move, y += col_move) {
            if (game->chessboard[x][y] != '.') {//is blocked 
                return false; 
            }
    }
    return true; 
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int move_direction_vertical;
    int move_direction_horizontal;
    move_direction_horizontal = dest_col - src_col; // - means left + means right 
    move_direction_vertical = dest_row - src_row; // - means moving up. + means moving down 
    int net_movement_vertical = abs(move_direction_vertical);
    int net_movement_horizontal = abs(move_direction_horizontal);

    if (net_movement_horizontal == net_movement_vertical || src_row == dest_row || src_col == dest_col) { // check if it moves diagonally or vertical or horizontal; 
        if(net_movement_horizontal == net_movement_vertical){//move like bishop diagonally 
            return is_valid_bishop_move(src_row,src_col,dest_row,dest_col,game);
        }else if(src_row == dest_row || src_col == dest_col){//move like rook vertical or horizontal
            return is_valid_rook_move(src_row,src_col,dest_row,dest_col,game);
        }else{ //this never occurs 
            return false;
        }
    }else{
        return false;
    }
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    int move_direction_vertical;
    int move_direction_horizontal;
    move_direction_horizontal = dest_col - src_col; // - means left + means right 
    move_direction_vertical = dest_row - src_row; // - means moving up. + means moving down 
    int net_movement_vertical = abs(move_direction_vertical);
    int net_movement_horizontal = abs(move_direction_horizontal);
    if ((net_movement_horizontal == 1 && net_movement_vertical ==1) || (net_movement_horizontal == 1 && net_movement_vertical== 0) || (net_movement_horizontal == 0 && net_movement_vertical== 1)) {
        return true; 
    }else{
        return false;
    }
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    char given_piece = game->chessboard[src_row][src_col];
    if(piece != given_piece){ // piece not = corresponding location 
        return false;
    }
    if(given_piece == '.'){// no piece at given location 
        return false; 
    }
    if((given_piece == 'p') || (given_piece == 'P')){
        return is_valid_pawn_move(piece,src_row,src_col,dest_row,dest_col,game);
    }else if((given_piece == 'r') || (given_piece == 'R')){
        return is_valid_rook_move(src_row,src_col,dest_row,dest_col,game);
    }else if((given_piece == 'n') || (given_piece == 'N')){
        return is_valid_knight_move(src_row,src_col,dest_row,dest_col);
    }else if((given_piece == 'b') || (given_piece == 'B')){
        return is_valid_bishop_move(src_row,src_col,dest_row,dest_col,game);
    }else if((given_piece == 'q') || (given_piece == 'Q')){
        return is_valid_queen_move(src_row,src_col,dest_row,dest_col,game);
    }else if((given_piece == 'k') || (given_piece == 'K')){
        return is_valid_king_move(src_row,src_col,dest_row,dest_col);
    }else{//this should never happen 
        return false; 
    }
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    (void)fen;
    (void)game;
}
void chessboard_to_fen(char fen[], ChessGame *game) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        fen[i] = 0; // Set each element to 0
    }
    int fen_index = 0;
    char piece;
    for (int x = 0; x < 8; x++) {
        int empty_count = 0;
        for (int y = 0; y < 8; y++) {
            piece = game->chessboard[x][y];
            if(piece == '.' && y == 7){// reaching last col and it's empty 
                empty_count++;
                fen[fen_index]= (char)(empty_count);
                fen_index++;
            } 
            else if(piece == '.'){
                empty_count++;
            }else{
                if(empty_count>0){//there are empty before the current index 
                    fen[fen_index] = (char)(empty_count);
                    empty_count = 0;
                    fen_index++;
                    fen[fen_index] = piece;
                    fen_index++;
                }else{//justp put in the piece
                    fen[fen_index] = piece;
                    fen_index++;
                }

            }
        }
        if (x < 7) { //if its not row 7 then add the / 
            fen[fen_index] = '/';
            fen_index++;
        }
    }
    //done converting the board. now indicate next turn. 
    fen[fen_index] = ' ';
    fen_index++;
    if(game->currentPlayer == WHITE_PLAYER){
        fen[fen_index] = 'w';
        fen_index++;
    }else{
        fen[fen_index] = 'b';
        fen_index++;
    }
    fen[fen_index] = '\0'; 
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
