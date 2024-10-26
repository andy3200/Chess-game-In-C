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
    return 1;
}




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
                }else if(game->chessboard[dest_row][dest_col] != '.'){ //destination is not empty 
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
        if(game->chessboard[dest_row][dest_col] == '.'){
            return true;
        }
        else if(check_white(game->chessboard[dest_row][dest_col]) == 0){ //eating enemy 
            return false;
        }else{
            return false; 
        }
    }else if(game->currentPlayer == BLACK_PLAYER){
        if(game->chessboard[dest_row][dest_col] == '.'){
            return true;
        }
        else if(check_white(game->chessboard[dest_row][dest_col]) == 1){//eating enemy 
            return false; 
        }else{
            return false; // capturing ally. handle in part 4 
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
    }else if(move_direction_vertical >0){ //moving down 
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
    int row = 0, col = 0;
    char piece;
    int empty; 
    for (int x = 0; fen[x] != '\0'; x++) {//loop through  the fen array 
        piece = fen[x];
        if (piece >= '1' && piece <= '8'){//we encounter an integer 
            empty = piece - '0'; // cast it to int
            for (int i = 0; i < empty; i++) {
                game->chessboard[row][col]= '.';
                col++;
            }
        }
        else if(piece == '/'){ //move to next row 
            col = 0; //reset col 
            row++; //increase row
        }else if(piece == ' '){ // we stop reading
            piece = fen[x+1];
            if(piece == 'b'){
                game->currentPlayer = BLACK_PLAYER;
            }else{
                game->currentPlayer = WHITE_PLAYER;
            }
            break; // done
        }
        else{//we encounter a piece 
            game->chessboard[row][col] = piece;
            col++;
        }
    }
}
void chessboard_to_fen(char fen[], ChessGame *game) {
    //this might be unnecessary 
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
                fen[fen_index]= empty_count + '0';;
                fen_index++;
            } 
            else if(piece == '.'){
                empty_count++;
            }else{
                if(empty_count>0){//there are empty before the current index 
                    fen[fen_index] = empty_count + '0';;
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
    //clear the buffer
    for (int i = 0; i < 3; i++) {
        parsed_move->startSquare[i] = '\0';
    }
    for (int x = 0; x < 4; x++) {
        parsed_move->endSquare[x] = '\0';
    }
    
    

    int promoting = 0;
    int length = strlen(move);
    if(length != 4 && length != 5){
        return PARSE_MOVE_INVALID_FORMAT;
    }
    char src_col = move[0];
    char src_row = move[1];
    char dest_col = move[2];
    char dest_row = move[3];
    if(length == 5){//there is promotion
        promoting = 1;
    }
    if (src_col < 'a' || src_col > 'h' || dest_col < 'a' || dest_col > 'h') {
        return PARSE_MOVE_INVALID_FORMAT ;
    }
    if ( src_row < '1' || src_row > '8' || dest_row < '1' || dest_row > '8'){
        return PARSE_MOVE_OUT_OF_BOUNDS;
    }

    if(promoting){
        //int move_direction_vertical = dest_row - src_row; // + means moving up. - means moving down 
        char promoting_piece = move[4];
        if(dest_row != '8' && dest_row != '1'){
            return PARSE_MOVE_INVALID_DESTINATION;
        }
        if (promoting_piece != 'q' && promoting_piece != 'r' && promoting_piece != 'b' && promoting_piece != 'n') {
            return PARSE_MOVE_INVALID_PROMOTION;
        }
        if(((dest_row- src_row)!= 1) && ((dest_row- src_row)!= -1)){//when you move more than 1 to get to dest 
            return PARSE_MOVE_INVALID_FORMAT; // i created this case myself 
        }
        parsed_move->startSquare[0]=src_col;
        parsed_move->startSquare[1]=src_row;
        parsed_move->endSquare[0]= dest_col;
        parsed_move->endSquare[1]= dest_row;
        parsed_move->endSquare[2]= promoting_piece;
        return 0;
    }
    parsed_move->startSquare[0]=src_col;
    parsed_move->startSquare[1]=src_row;
    parsed_move->endSquare[0]= dest_col;
    parsed_move->endSquare[1]= dest_row;
    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    //parse the letters into corresponding int (the mapping)
    int src_col =  move->startSquare[0]- 'a';
    int src_row = '8' - move->startSquare[1];
    int dest_col = move->endSquare[0] - 'a';
    int dest_row = '8' - move->endSquare[1];
    char src_piece = game->chessboard[src_row][src_col];
    int length_endsquare = strlen(move->endSquare);
    int captured_pieces_index = 0;
    if (validate_move) {
        if((is_client && game->currentPlayer == BLACK_PLAYER) || (is_client == 0 && game->currentPlayer == WHITE_PLAYER)){ 
            return MOVE_OUT_OF_TURN;
        }
        if(src_piece == '.'){
            return MOVE_NOTHING;
        }
        if((check_white(src_piece) == 1) && (is_client == 0)){ //black player trying to move white piece
            return MOVE_WRONG_COLOR;
        }   
        if((check_white(src_piece) == 0) && (is_client == 1)){ // white player moving black piece 
            return MOVE_WRONG_COLOR;
        }
        if(game->chessboard[dest_row][dest_col] != '.' && check_eating(dest_row,dest_col,game) == 0){
            return MOVE_SUS;
        }
        if((length_endsquare == 3) && (src_piece!= 'p' && src_piece != 'P')){
            return MOVE_NOT_A_PAWN;
        }
        if((src_piece == 'p' || src_piece == 'P') && (length_endsquare == 2)){
            if(src_piece== 'p' && dest_row == 7){
                return MOVE_MISSING_PROMOTION;
            }
            if(src_piece == 'P' && dest_row == 0){
                return MOVE_MISSING_PROMOTION;
            }
        }
        if(is_valid_move(src_piece,src_row,src_col,dest_row,dest_col,game) == false){
            return MOVE_WRONG;
        }
    }
    if(length_endsquare == 3){//promotion occur 
        char promoted_into = move->endSquare[2];
        game->chessboard[src_row][src_col] = '.';
        if(check_white(src_piece)){//if it's white
            if(game->chessboard[dest_row][dest_col]!= '.'){//eating and promoting occurs
                if(check_eating(dest_row,dest_col,game)== 0){
                    return MOVE_SUS;
                }
                game->capturedPieces[captured_pieces_index] = game->chessboard[dest_row][dest_col];
                captured_pieces_index++;
                game->capturedCount++;
                game->chessboard[dest_row][dest_col] = promoted_into - 32; //assign and turn it into upper case 
            }else{
                game->chessboard[dest_row][dest_col] = promoted_into - 32; //assign and turn it into upper case 
            }
        }
        if(check_white(src_piece)== 0){//if it's black
            if(game->chessboard[dest_row][dest_col]!= '.'){//eating and promoting occurs
                if(check_eating(dest_row,dest_col,game)== 0){
                    return MOVE_SUS;
                }
                game->capturedPieces[captured_pieces_index] = game->chessboard[dest_row][dest_col];
                captured_pieces_index++;
                game->capturedCount++;
                game->chessboard[dest_row][dest_col] = promoted_into;
            }else{
                game->chessboard[dest_row][dest_col] = promoted_into; 
            }
        }
        game->moves[game->moveCount] = *move;
        game->moveCount++;
    }else{//move regularly
        game->chessboard[src_row][src_col] = '.'; 
        if(game->chessboard[dest_row][dest_col]!= '.'){//eating occur
            game->capturedPieces[captured_pieces_index] = game->chessboard[dest_row][dest_col];
            captured_pieces_index++;
            game->capturedCount++;
            game->chessboard[dest_row][dest_col] = src_piece; //assign 
        }else{ //eating doesn't occur 
            game->chessboard[dest_row][dest_col] = src_piece; //assign 
        }
        game->moves[game->moveCount] = *move;
        game->moveCount++;
    }
    if(game->currentPlayer == WHITE_PLAYER){
        game->currentPlayer = BLACK_PLAYER;
    }else{
        game->currentPlayer = WHITE_PLAYER;
    }
    return 0;
    
  
    
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char command[50];
    char arg1[BUFFER_SIZE];
    char arg2[50];
    int number_read = 0;
    if ( (number_read = sscanf(message, "/%s %s %s", command, arg1, arg2)) < 1) {
        return COMMAND_UNKNOWN; 
    }
    if(strcmp(command, "move") == 0){
        ChessMove parsed_move;
        if(number_read != 2){ // different args number read
            return COMMAND_ERROR;
        }
        if(parse_move(arg1,&parsed_move) == 0){ // successful parse;
            if(make_move(game,&parsed_move,is_client,true) == 0 ){ // valid move 
                send(socketfd, message, strlen(message), 0);
                return COMMAND_MOVE;
            }else{
                return COMMAND_ERROR;
            }
        }else{
            return COMMAND_ERROR;
        }
    }else if (strcmp(command, "forfeit") == 0) {
        if(number_read != 1){ // different args number read
            return COMMAND_ERROR;
        }
        send(socketfd, message, strlen(message), 0);
        return COMMAND_FORFEIT;
    }else if (strcmp(command, "chessboard") == 0) {
        if(number_read != 1){ // different args number read
            return COMMAND_ERROR;
        }
        display_chessboard(game);
        return COMMAND_DISPLAY;
    }else if (strcmp(command, "import") == 0) {
        number_read= sscanf(message, "/%s %[^\n]", command, arg1);
        if(number_read != 2){ // different args number read
            return COMMAND_ERROR;
        }
        if (is_client == false) {
            fen_to_chessboard(arg1, game);
            send(socketfd, message, strlen(message), 0);
            return COMMAND_IMPORT;
        }else{
            return COMMAND_ERROR;
        }
    }else if (strcmp(command, "load") == 0) {
        if(number_read != 3){ // different args number read
            return COMMAND_ERROR;
        }
        int save_number = atoi(arg2); // turn the save number from character to int 
        if (load_game(game, arg1, "game_database.txt", save_number) == 0) {
            send(socketfd, message, strlen(message), 0);
            return COMMAND_LOAD;
        }else {
            return COMMAND_ERROR; 
        }
    }else if (strcmp(command, "save") == 0) {
        if(number_read != 2){ // different args number read
            return COMMAND_ERROR;
        }
       
        if (save_game(game, arg1, "game_database.txt") == 0) {
            send(socketfd, message, strlen(message), 0);
            return COMMAND_SAVE;
        } else {
            return COMMAND_ERROR; 
        }
    }else{
        return COMMAND_UNKNOWN;
    }
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char command[50];
    char arg1[BUFFER_SIZE];
    char arg2[50];
    int number_read = 0;
    if ( (number_read = sscanf(message, "/%s %s %s", command, arg1, arg2)) < 1) {
        return COMMAND_UNKNOWN; 
    }
    if(strcmp(command, "move") == 0){
        ChessMove parsed_move;
        if(number_read != 2){ // different args number read
            return COMMAND_ERROR;
        }
        if(parse_move(arg1,&parsed_move) == 0){ // successful parse;
            if(make_move(game,&parsed_move,is_client,false) == 0 ){ // valid move 
                send(socketfd, message, strlen(message), 0);
                return COMMAND_MOVE;
            }else{
                return COMMAND_ERROR;
            }
        }else{
            return COMMAND_ERROR;
        }
    }
    else if (strcmp(command, "forfeit") == 0) {
        if(number_read != 1){ // different args number read
            return COMMAND_ERROR;
        }
        close(socketfd);
        return COMMAND_FORFEIT;
    }else if (strcmp(command, "import") == 0) {
        number_read= sscanf(message, "/%s %[^\n]", command, arg1);
        if(number_read != 2){ // different args number read
            return COMMAND_ERROR;
        }
        if (is_client == true) {
            fen_to_chessboard(arg1, game);
            return COMMAND_IMPORT;
        }else{
            return COMMAND_ERROR;
        }
    }else if (strcmp(command, "load") == 0) {
        if(number_read != 3){ // different args number read
            return COMMAND_ERROR;
        }
        int save_number = atoi(arg2); // turn the save number from character to int 
        if (load_game(game, arg1, "game_database.txt", save_number) == 0) {
            send(socketfd, message, strlen(message), 0);
            return COMMAND_LOAD;
        }else {
            return COMMAND_ERROR; 
        }
    }else{
        return -1;
    }
    
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    char fen[BUFFER_SIZE];
    if(username == NULL || username[0] == '\0' || strchr(username, ' ') != NULL){//invalid username 
        return -1;
    }
    FILE *file = fopen(db_filename, "a");
    if (file == NULL) {
        return -1;
    }
    chessboard_to_fen(fen, game);
    fprintf(file, "%s:%s\n", username, fen);
    fclose(file);
    return 0;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    char each_line[BUFFER_SIZE+51];
    int game_count =0; 
    if(save_number ==0){
        return -1;
    }
    FILE *file = fopen(db_filename, "r");
    if (file == NULL) {
        return -1;
    }
    if(username == NULL || username[0] == '\0' || strchr(username, ' ') != NULL){//invalid username 
        return -1;
    }
    while (fgets(each_line, BUFFER_SIZE+51, file)){
        char* extracted_username = strtok(each_line, ":");
        char* fen = strtok(NULL, ":");
        if (strcmp(extracted_username, username) == 0) {//we found the username 
            game_count++;
            if(game_count == save_number){//we found the game corresponding to the user 
                fen_to_chessboard(fen,game);
                fclose(file);
                return 0;
            }
        }
    }
    return -1;
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
