#include <stdio.h>
#include <assert.h>

/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */

int next(int k){
    if(0 <= k && k < 5) return 5;
    else if(5 <= k && k < 9) return 9;
    else if(9 <= k && k < 12) return 12;
    else if(12 <= k && k < 14) return 14;
}

int has_lost(board_t board, player_t player)
{
    int l = 0, m = 1, r = 5;
    while(m < 14){
        if(r > 14){
            l = next(l);
            m = l + 1;
            r = next(m);
            continue;
        }
        if(m == next(l)){
            l++;
            m = l + 1; 
            continue;
        }

        if (board[l] == player &&
            board[m] == player &&
            board[r] == player) return 1;
        m++;
        r++;
    }
    return 0;
}

int is_full(board_t board)
{
    for(int i = 0; i < 15; i++){
        if(board[i] == NO) return 0;
    }
    return 1;
}

int is_empty(board_t board){
    for(int i = 0; i < 15; i++){
        if(board[i] != NO) return 0;
    }
    return 1;
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

player_t other_player(player_t player){
    if(player == RED) return BLUE;
    else if(player == BLUE) return RED;
}

void print_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        if(board[i] == RED) printf("R  ");
        else if(board[i] == BLUE) printf("B  ");
        else printf(".  ");
    }
    printf("\n");
}

void init_board(board_t board){
    for(int i = 0; i < 15; i++){
        board[i] = NO;
    }
}

move_t best_move(board_t board, player_t player)
{
    move_t response;
    move_t candidate;
    move_t lose_move;
    int no_candidate = 1;
    int no_win = 0;
    int lose = 1;
    
    for(int i = 0; i < 15; i++){
        if(board[i] == NO){
            board[i] = player;
            
            if(has_lost(board, player)){
                board[i] = NO;
                continue;
            }
            
            else if(!is_full(board)){
                for(int j = 0; j < 15; j++){
                    if(board[j] == NO){
                        board[j] = other_player(player);
                        if(!has_lost(board,other_player(player))){
                            no_win = 1;
                        }
                    board[j] = NO;
                    }
                }
                if(no_win == 0){
                    board[i] = NO;
                    return (move_t){i,1};
                }
            }
            board[i] = NO;
        }
    }
  
    for(int i = 0; i < 15; i++){
        if(board[i] == NO){
            board[i] = player;
            if(has_lost(board, player)){
                board[i] = NO;
                continue;
            }
            response = best_move(board, other_player(player));
            if(response.score == -1){
                board[i] = NO;
                assert(board[i] == 0);
                return (move_t){i,1};
            }
            
            board[i] = NO;
        }
    }
    
        for(int i = 0; i < 15; i++){
        if(board[i] == NO){
            board[i] = player;
            if(!has_lost(board, player) && lose == 1){
                lose = 0;
                candidate = (move_t){i,-1};
            }
            lose_move = (move_t){i,-1};
            board[i] = NO;
        }
    }
    if(lose == 1){
        return lose_move;
    }
    return candidate;
}

int main()
{
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */

    board_t board;
    player_t current;
    move_t response;
    init_board(board);
    int move;
    char colour;
    int player_turn;

    printf("Enter player:\n");
    printf("1 to play as first player or 2 to play as second player\n");
    scanf("%d", &player_turn);
    printf("select colour:\n");
    printf("R for RED or B for BLUE\n");
    scanf(" %c", &colour);
    
    assert(colour == 'R' || colour == 'B');
    if(colour == 'R') current = RED;
    else if(colour == 'B') current = BLUE;
    
    if(player_turn == 2) current = other_player(current);

    while(1){
        if(player_turn == 1){
            printf("0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 \n");
            print_board(board);
            printf("\n------------------\n");
            printf("\n");
            printf("Enter move: ");
            scanf("%d", &move);
            assert(move >= 0 && move < 15);
            assert(board[move] == NO);
            printf("your move is %d\n", move);
            board[move] = current;
        }else if(player_turn == 2){
            if (is_empty(board)) board[0] = current;
            else {
                response = best_move(board, current);
                printf("computer move is %d\n", response.line);
                board[response.line] = current;
            }
        }
        if(has_lost(board, current)){
            printf("0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 \n");
            print_board(board);
            printf("player %s has lost\n", (current == RED) ? "RED" : "BLUE");
            break;
        }
        if(is_full(board)){
            printf("0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 \n");
            print_board(board);
            printf("DRAW.\n");
            break;
        }
        current = other_player(current);
        player_turn = 3 - player_turn;
    } 
    return 0;
}