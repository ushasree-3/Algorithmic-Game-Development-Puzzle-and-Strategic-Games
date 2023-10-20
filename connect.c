#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* Implement connect 4 on a 5 (columns) x 4 (rows) board. */
enum {
    EMPTY = 0,
    RED = 1,
    BLUE = 2,
};
typedef char board_t[4][5];
typedef char player_t;
typedef struct {
    int col;
    int score;
} move_t;
#define MAX_ORD (57258302)
uint8_t computed_moves[MAX_ORD];

int has_won(board_t board, player_t player)
{
    for(int col = 0; col < 5; col++){
        if(board[0][col] == player &&
           board[1][col] == player &&
           board[2][col] == player &&
           board[3][col] == player) return 1;  
    }

    for(int i = 0; i < 2; i++){
        for(int row = 0; row < 4; row++){
            if(board[row][0+i] == player &&
           board[row][1+i] == player &&
           board[row][2+i] == player &&
           board[row][3+i] == player ) return 1;  
        }
    }

    for(int i = 0; i < 2; i++){
        if(board[0][0+i] == player &&
           board[1][1+i] == player &&
           board[2][2+i] == player &&
           board[3][3+i] == player) return 1;
    }

    for(int i = 0; i < 2; i++){
        if(board[3][0+i] == player &&
           board[2][1+i] == player &&
           board[1][2+i] == player &&
           board[0][3+i] == player) return 1;
    }
    return 0;
}

int is_full(board_t board)
{
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 5; col++){
            if(board[row][col] == '.') return 0;
        }
    }
    return 1;
}

player_t other_player(player_t player)
{
    if(player == 'R') return 'B';
    else return 'R';
}

void print_board(board_t board)
{
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 5; col++){
            printf("%c ", board[row][col]);
        }
        printf("\n");
    }
}

void init_board(board_t board)
{
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 5; col++){
            board[row][col] = '.';
        }
        printf("\n");
    }
}

int empty_row(board_t board, int col)
{
    for(int r = 3; r >= 0; r--){
        if(board[r][col] == '.') return r;
    }
    return -1;
}

uint8_t encode_move(move_t m, board_t board)
{
    uint8_t b = 0;
    int row = empty_row(board, m.col);
    assert(row != -1);
    b |= row;

    assert(0 <= m.col && m.col <= 4);
    b |= m.col << 2;

    switch (m.score) {
    case -1: b |= 1 << 7; break;
    case 0: b |= 1 << 6; break;
    case 1: b |= 1 << 5; break;
    }

    return b;
}

move_t decode_move(uint8_t b, board_t board)
{
    move_t m;
    int row = empty_row(board, m.col);

    row = b & 0x3;
    m.col = (b & 0x1C) >> 2;
    if (b & 0x20) m.score = 1;
    if (b & 0x40) m.score = 0;
    if (b & 0x80) m.score = -1;
    return m;
}

int colord(board_t board, int col){
    if(board[3][col]=='.'){
        return 0;
    }
    else if(board[2][col]=='.'){
        if(board[3][col]=='R'){
            return 1;
        } else{
            return 2;
        }
    }
    else if(board[1][col]=='.'){
        if(board[3][col]=='R'){
            if(board[2][col]=='R'){
                return 3;
            }
            else{
                return 4;
            }
        }
        else{
            if(board[2][col]=='R'){
                return 5;
            }
            else{
                return 6;
            }
        }
    }
    else if(board[0][col]=='.'){
        if(board[3][col]=='R' && board[2][col]=='R'){
            if(board[1][col]=='R'){
                return 7;
            }
            else{
                return 8;
            }
        }
        if(board[3][col]=='R' && board[2][col]=='B'){
            if(board[1][col]=='R'){
                return 9;
            }
            else{
                return 10;
            }
        }
        if(board[3][col]=='B' && board[2][col]=='R'){
            if(board[1][col]=='R'){
                return 11;
            }
            else{
                return 12;
            }
        }
        if(board[3][col]=='B' && board[2][col]=='B'){
            if(board[1][col]=='R'){
                return 13;
            }
            else{
                return 14;
            }
        }
    }
    else {
        if(board[3][col]=='R' && board[2][col]=='R'){
            if(board[1][col]=='R' && board[0][col]=='R'){
                return 15;
            }
            else if(board[1][col]=='R' && board[0][col]=='B'){
                return 16;
            }
            else if(board[1][col]=='B' && board[0][col]=='R'){
                return 17;
            }
            else{
                return 18;
            }
        }
        if(board[3][col]=='R' && board[2][col]=='B'){
            if(board[1][col]=='R' && board[0][col]=='R'){
                return 19;
            }
            else if(board[1][col]=='R' && board[0][col]=='B'){
                return 20;
            }
            else if(board[1][col]=='B' && board[0][col]=='R'){
                return 21;
            }
            else{
                return 22;
            }
        }
        if(board[3][col]=='B' && board[2][col]=='R'){
            if(board[1][col]=='R' && board[0][col]=='R'){
                return 23;
            }
            else if(board[1][col]=='R' && board[0][col]=='B'){
                return 24;
            }
            else if(board[1][col]=='B' && board[0][col]=='R'){
                return 25;
            }
            else{
                return 26;
            }
        }
        if(board[3][col]=='B' && board[2][col]=='B'){
            if(board[1][col]=='R' && board[0][col]=='R'){
                return 27;
            }
            else if(board[1][col]=='R' && board[0][col]=='B'){
                return 28;
            }
            else if(board[1][col]=='B' && board[0][col]=='R'){
                return 29;
            }
            else{
                return 30;
            }
        }
    }
}
long ord(board_t board)
{
    long i=0;
    i=(colord(board,4)*1)+(colord(board,3)*31)+(colord(board,2)*31*31)+(colord(board,1)*31*31*31)+(colord(board,0)*31*31*31*31);
    return i;
}

move_t best_move(board_t board, player_t player)
{
    move_t response;
    move_t candidate;
    int no_candidate = 1;

    assert(!is_full(board));
    assert(!has_won(board, player));
    assert(!has_won(board, other_player(player)));

    long ord_val = ord(board);
    
    if (computed_moves[ord_val]) {
        return decode_move(computed_moves[ord_val], board);
    }
    
    for (int col = 0; col < 5; ++col) {
        int row = empty_row(board, col);
        if(row == -1){
            continue;
        } 
        if (board[row][col] == '.') {
            board[row][col] = player;
            if (has_won(board, player)) {
                board[row][col] = '.';
                candidate = (move_t) {
                    .col = col,
                    .score = 1
                    };
                computed_moves[ord_val] = encode_move(candidate, board);
                return candidate;
            }
            board[row][col] = '.';
        }
    }

    for (int col = 0; col < 5; ++col) {
        int row = empty_row(board, col);
        if(row == -1){
            continue;
        } 
        if (board[row][col] == '.') {
            board[row][col] = player;
            if (is_full(board)) {
                board[row][col] = '.';
                candidate = (move_t) {
                    .col = col,
                    .score = 0
                    };
                computed_moves[ord_val] = encode_move(candidate, board);
                return candidate;
            }

            response = best_move(board, other_player(player));
            board[row][col] = '.';
            if (response.score == -1) {
                candidate = (move_t) {
                    .col = col,
                    .score = 1
                    };
                computed_moves[ord_val] = encode_move(candidate, board);
                return candidate;
            } else if (response.score == 0) {
                candidate = (move_t) {
                    .col = col,
                    .score = 0
                };
                no_candidate = 0;
            } else {
                if (no_candidate) {
                    candidate = (move_t) {
                        .col = col,
                        .score = -1
                    };
                    no_candidate = 0;
                }
            }
        }
    }
    
    computed_moves[ord_val] = encode_move(candidate, board);
    return candidate;
}

int main()
{
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */

    int move, row, col;
    board_t board;
    move_t response;
    player_t current;
    int player_turn;
    char colour;

    printf("Enter player:\n");
    printf("1 to play as first player or 2 to play as second player\n");
    scanf("%d", &player_turn);
    printf("select colour:\n");
    printf("R for RED or B for BLUE\n");
    scanf(" %c", &colour);

    if(player_turn == 1) current = colour;
    else if(player_turn == 2) current = other_player(colour);

    init_board(board);
    while (1) {
        printf("\n");
        print_board(board);
        printf("\n");
        if (player_turn == 1) {
            printf("Enter your move: ");
            scanf("%d", &move);
            col = move % 5;
            row = empty_row(board, col);
            assert(board[row][col] == '.');
            board[row][col] = current;
        } else if(player_turn == 2){
            response = best_move(board, current);
            row = empty_row(board ,response.col);
            board[row][response.col] = current;
        }
        if (has_won(board, current)) {
            print_board(board);
            printf("Player %c has won!\n", current);
            break;
        } else if (is_full(board)) {
            print_board(board);
            printf("Draw.\n");
            break;
        }
        current = other_player(current);
        player_turn = (3 - player_turn);
    }
    return 0;
}