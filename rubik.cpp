#include <cstdio>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
using namespace std;

#include "queue.hpp"
#define MAX 11022480
struct board {
    int e[8][3];};
struct in_board {
    int e[6][4];};
std::vector<int> src_id; // 6 14 15 35 66 165 385
std::vector<int> ori_src_id; // 6 15 66 165 14 35 385 
board src;

int max(int a, int b){
    if(a > b) return a;
    else return b;
}

int pow(int b , int p){
    int ans = 1;
    for(int i = 0; i < p; i++){
        ans *= b;
    }
    return ans;
}

int fact(int n)
{
    int p = 1;
    for (int i = 2; i <= n; ++i) {
        p *= i;
    }
    return p;
}

vector<int> cubelet_id(const board& b){
    vector<int> id;
    for(int i = 0; i < 8; ++i){
        int t = 1;
        if(i != 6){
            for(int j = 0; j < 3; ++j){
                switch(b.e[i][j]){
                    case 0: t *= 1; break;
                    case 1: t *= 2; break;
                    case 2: t *= 3; break;
                    case 3: t *= 5; break;
                    case 4: t *= 7; break;
                    case 5: t *= 11; break;
                }
            }
            id.push_back(t);
        }
    }
    return id;
}

//1 3 5 6 2 4 7
vector<int> rank_func(vector<int>& bcube_id){
    vector<int> sorted_id = bcube_id;
    vector<int> cubelet_rank;
    std::sort(sorted_id.begin(), sorted_id.end());
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++){
            if(bcube_id[i] == sorted_id[j]){
                cubelet_rank.push_back(1+j);
            }
        }
    }
    return cubelet_rank;
}

// 180 -> 1 3 5 6 2 4 7
int position_ord(const vector<int>& rank){
    int a = 0;
    for (int i = 0; i < 6; i++) {
        int smaller_count = 0;
        for (int j = i + 1; j < 7; j++) {
            if (rank[j] < rank[i]) {
                smaller_count++;
            }
        }
        a += smaller_count * fact(7 - i - 1);
    }
    return a;
}

int cubelet_ord(int a, int b, int c){
    if(max(a, b) == a && max(a, c) == a) return 2;
    else if (max(a, b) == b && max(a, c) == c) return 0;
    else return 1;
    assert(0);
}

//397377
int ord(const board& b){
    vector<int> bcube_id = cubelet_id(b);  // 6 15 66 165 14 35 385
    vector<int> cubelet_rank = rank_func(bcube_id);     // 1 3 5 6 2 4 7
    int pos_ord = pow(3,7)*position_ord(cubelet_rank);  // 395847
    int x = 0, cub_ord = 0; // 5904

    for(int i = 0; i < 8; i++){
        if(i != 6){
            cub_ord += (cubelet_ord(b.e[i][0], b.e[i][1], b.e[i][2]) * pow(3, x));
            x++;
        }
    }
    return (pos_ord + cub_ord);
    assert(0);
}

vector<int> permutation(int n, int r) {
    vector<int> result;
    vector<int> nums;
    vector<int> used;

    for (int i = 0; i < n; i++) {
        nums.push_back(i + 1);
        used.push_back(0);
    }

    for (int i = 0; i < n; i++) {
        int f = fact(n - 1 - i);
        int index = (r - 1) / f;
        r = r - index * f;

        for (int j = 0; j < n; j++) {
            if (!used[j]) {
                if (index == 0) {
                    result.push_back(nums[j]);
                    used[j] = 1;
                    break;
                }
                index--;
            }
        }
    }
    return result;
}

// 6 15 66 165 14 35 385  from  1 3 5 6 2 4 7
vector<int> decode_id(vector<int>& perm_rank){
    vector<int> bcube_id;
    for(int i = 0; i < 7; i++){
        bcube_id.push_back(src_id[perm_rank[i]-1]);
    }
    return bcube_id;
}

board decode(int ord){
    int position_ord = ord / pow(3,7); // 180
    int cub_ord = ord % pow(3,7); // 1530

    vector<int> cubelet_rank = permutation(7, position_ord + 1); // 1 3 5 6 2 4 7
    vector<int> bcube_id = decode_id(cubelet_rank); // 6 15 66 165 14 35 385

    board b;
    vector<int> prime = {1,2,3,5,7,11};
    int i = 0, j = 0, k = 0, t = 0;
    while(i < 8){
        if(i == 6){b.e[6][0] = src.e[6][0]; b.e[6][1] = src.e[6][1]; b.e[6][2] = src.e[6][2]; i++; continue;}
        j = 0;
        while(j < 3){
            k = 5;
            while(k >= 0){
                int x = prime[k];
                t = i; if(i == 7)t = 6;
                if(bcube_id[t] % x == 0 && j < 3){
                    if(x == 11) {b.e[i][j] = 5;}
                    else if(x == 7) {b.e[i][j] = 4;}
                    else if(x == 5) {b.e[i][j] = 3;}
                    else if(x == 3) {b.e[i][j] = 2;}
                    else if(x == 2) {b.e[i][j] = 1;}
                    else if(x == 1) {b.e[i][j] = 0;}
                    j++;
                }
                k--;
            }
        }
        i++;
    }

    vector<int> score2;
    int score = 0, x = 0, y = 0, z = 0, m = 0, n = 0; 
    for(int i = 7; i >= 0; i--){
        n = i;
        if(i == 7) {n--;}
        else if(i == 6) {continue;}
        score = cub_ord / pow(3, n);
        cub_ord = cub_ord % pow(3, n);
        
        x = b.e[i][0], y = b.e[i][1]; z = b.e[i][2];
        vector<int> order = {x, y, z};
        sort(order.begin(), order.end());
        if(score == 2) m = order[2];
        else if(score == 1) m = order[1];
        else m = order[0];
        score2.push_back(m);
    }
    std::reverse(score2.begin(), score2.end());

    int w = 0;
    vector<int> sense2;
    for(int u = 0; u < 7; u++){
        for(int v = 0; v < 7; v++){
            w = v;
            if(v == 6){ w = 7;}
            if(bcube_id[u] == ori_src_id[v]){
                for(int l = 0; l < 3; l++){
                    if(score2[u] == src.e[w][l]){
                        for(int k = 0; k < 3; k++){
                            b.e[u][k] = src.e[w][(l+k)%3];
                        }
                    }
                }

                if( ((u == 0 || u == 3 || u == 5) && (v == 0 || v == 3 || v == 5)) || 
                ((u == 1 || u == 2 || u == 4 || u == 6) && (v == 1 || v == 2 || v == 4 || v == 6)) ) sense2.push_back(1);
                else sense2.push_back(-1);
                if(sense2[u] == -1){
                    int temp = b.e[u][1]; 
                    b.e[u][1] = b.e[u][2];
                    b.e[u][2] = temp;
                }
            }
        }
    }
    b.e[7][0] = b.e[6][0]; b.e[7][1] = b.e[6][1]; b.e[7][2] = b.e[6][2];
    b.e[6][0] = src.e[6][0]; b.e[6][1] = src.e[6][1]; b.e[6][2] = src.e[6][2];
    return b;
}

void print_board(const board& b)
{
    for (int c = 0; c < 8; ++c){
        for (int s = 0; s < 3; ++s){
            printf("%3d ", b.e[c][s]);
        }
        printf("\n");
    }
}

void read_board(board& b)
{
    for (int c = 0; c < 8; ++c) {
        for (int s = 0; s < 3; ++s) {
            scanf("%d", &b.e[c][s]);
        }
    }
}

enum move { R = 1, U = 2, F = 3, Ra = 4, Ua = 5, Fa = 6, R2 = 7, U2 = 8, F2 =  9 };

board update_board(board old_board,int a, int b, int c, int d, int face, int sense){
    board new_board = old_board;
    int f = 0, s = 0, x = 0;

    //f, s, x represent a particular side of cubelet.
    // f and s: sides of cubelet involved in side changes; x is the side of cubelet involved in face change. 
    switch(face){
        case R: f = 0; s = 2; x = 1; break;
        case U: f = 1; s = 2; x = 0; break;
        case F: f = 0; s = 1; x = 2; break;
    }

    std::vector<int> cubelets = { a, b, c, d};
    for(int i = 0; i < 4; i++){
        //sides
        if(sense != 2){
        new_board.e[cubelets[i]][f] = old_board.e[cubelets[(i+sense+4)%4]][s];
        new_board.e[cubelets[i]][s] = old_board.e[cubelets[(i+sense+4)%4]][f];
        }else {
        new_board.e[cubelets[i]][f] = old_board.e[cubelets[(i+sense)%4]][f];
        new_board.e[cubelets[i]][s] = old_board.e[cubelets[(i+sense)%4]][s];
        }

        //face
        new_board.e[cubelets[i]][x] = old_board.e[cubelets[(i+sense+4)%4]][x];
    }
    return new_board;
    assert(0);
}

//All 9 operations
//1,clockwise turn of the right face of the cube
board Right(const board& b)
{
    board o = update_board(b, 1, 3, 7, 5, R, 1);
    return o;
    assert(0);
}

//2,clockwise turn of the up (top) face of the cube
board Up(const board& b)
{
    board o = update_board(b, 0, 1, 5, 4, U, 1);
    return o;
}

//3,clockwise turn of the frount face of the cube.
board Front(const board& b)
{
    board o = update_board(b, 0, 2, 3, 1, F, 1);
    return o;
}

//4,anti clockwise turn of the right face of the cube
board Right_a(const board& b)
{
    board o = update_board(b, 1, 3, 7, 5, R, -1);
    return o;
    assert(0);
}

//5,anti clockwise turn of the up (top) face of the cube
board Up_a(const board& b)
{
    board o = update_board(b, 0, 1, 5, 4, U, -1);
    return o;
}

//6,anti clockwise turn of the frount face of the cube.
board Front_a(const board& b)
{
    board o = update_board(b, 0, 2, 3, 1, F, -1);
    return o;
}

//7,clockwise turn of the right face of the cube
board Right2(const board& b)
{
    board o = update_board(b, 1, 3, 7, 5, R, 2);
    return o;
}

//8,clockwise turn of the up (top) face of the cube
board Up2(const board& b)
{
    board o = update_board(b, 0, 1, 5, 4, U, 2);
    return o;
}
//9,clockwise turn of the frount face of the cube.
board Front2(const board& b)
{
    board o = update_board(b, 0, 2, 3, 1, F, 2);
    return o;
}

bool is_dest(const board &b, const board &src){
    vector<int> face;

    //bottom face
    face = { 2, 3, 6, 7};
    for(int x:face){
        if(b.e[x][0] != src.e[6][0]) return false;
    }
    //left face
    face = { 0, 2, 4, 6};
    for(int x:face){
        if(b.e[x][1] != src.e[6][1]) return false;
    }
    //back face
    face = { 4, 5, 6, 7};
    for(int x:face){
        if(b.e[x][2] != src.e[6][2]) return false;
    }

    //top face
    if(b.e[0][0] != b.e[1][0] || b.e[0][0] != b.e[4][0] || b.e[0][0] != b.e[5][0]) return false;
    //right face
    if(b.e[1][1] != b.e[3][1] || b.e[1][1] != b.e[5][1] || b.e[1][1] != b.e[7][1]) return false;
    //front face
    if(b.e[0][2] != b.e[1][2] || b.e[0][2] != b.e[2][2] || b.e[0][2] != b.e[3][2]) return false;
    else {
        return true;
        }
    assert(0);
}

bool check_is_same(const board& b, const board &src)
{
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 3; ++c)
            if (b.e[r][c] != src.e[r][c]) return false;

    return true;
}

std::vector<int> solve(const board& src)
{   
    queue<board,MAX> q;
    int visited[MAX];
    int parent[MAX];

    enqueue(q, src);
    visited[ord(src)] = R;
    
    while (!check_is_empty(q)) {
        board v = dequeue(q);
        if (is_dest(v, src)) {
            /* return the moves to get to u from src. */
            std::vector<int> moves;
            board c = v;
            int o = ord(c);
            while (!check_is_same(c, src)) {
                moves.push_back(visited[o]);
                c = decode(parent[o]);
                o = ord(c);
            }
            std::reverse(moves.begin(), moves.end());
            return moves;
            assert(0);
        }

        board r = Right(v);
        board u = Up(v);
        board f = Front(v);
        board ra = Right_a(v);
        board ua = Up_a(v);
        board fa = Front_a(v);
        board r2 = Right2(v);
        board u2 = Up2(v);
        board f2 = Front2(v);
        
        int r_ord = ord(r);
        int u_ord = ord(u);
        int f_ord = ord(f);
        int ra_ord = ord(ra);
        int ua_ord = ord(ua);
        int fa_ord = ord(fa);
        int r2_ord = ord(r2);
        int u2_ord = ord(u2);
        int f2_ord = ord(f2);

        if (!visited[r_ord]) {
            visited[r_ord] = R;
            parent[r_ord] = ord(v);
            enqueue(q, r);
        }
        if (!visited[u_ord]) {
            visited[u_ord] = U;
            parent[u_ord] = ord(v);
            enqueue(q, u);
        }
        if (!visited[f_ord]) {
            visited[f_ord] = F;
            parent[f_ord] = ord(v);
            enqueue(q, f);
        }
        if (!visited[ra_ord]) {
            visited[ra_ord] = Ra;
            parent[ra_ord] = ord(v);
            enqueue(q, ra);
        }
        if (!visited[ua_ord]) {
            visited[ua_ord] = Ua;
            parent[ua_ord] = ord(v);
            enqueue(q, ua);
        }
        if (!visited[fa_ord]) {
            visited[fa_ord] = Fa;
            parent[fa_ord] = ord(v);
            enqueue(q, fa);
        }
        if (!visited[r2_ord]) {
            visited[r2_ord] = R2;
            parent[r2_ord] = ord(v);
            enqueue(q, r2);
        }
        if (!visited[u2_ord]) {
            visited[u2_ord] = U2;
            parent[u2_ord] = ord(v);
            enqueue(q, u2);
        }
        if (!visited[f2_ord]) {
            visited[f2_ord] = F2;
            parent[f2_ord] = ord(v);
            enqueue(q, f2);
        }
    }
    printf("Cube is not solvable");
    return {0};
    assert(0);
}

void print_moves(const std::vector<int>& moves)
{
    for (auto m: moves) {
        switch (m) {
        case R : printf("R "); break;
        case U : printf("U "); break;
        case F : printf("F "); break;
        case Ra: printf("Ra "); break;
        case Ua: printf("Ua "); break;
        case Fa: printf("Fa "); break;
        case R2: printf("R2 "); break;
        case U2: printf("U2 "); break;
        case F2: printf("F2 "); break;
        }
    }
    printf("\n");
}

void read_input(in_board& b)
{
    for (int c = 0; c < 6; ++c) {
        for (int s = 0; s < 4; ++s) {
            scanf("%d", &b.e[c][s]);
        }
    }
}

board convert_board(in_board& in_src){
    board b;
    b.e[0][0] = in_src.e[0][2]; b.e[0][1] = in_src.e[1][1]; b.e[0][2] = in_src.e[2][0];
    b.e[1][0] = in_src.e[0][3]; b.e[1][1] = in_src.e[3][0]; b.e[1][2] = in_src.e[2][1];
    b.e[2][0] = in_src.e[5][0]; b.e[2][1] = in_src.e[1][3]; b.e[2][2] = in_src.e[2][2];
    b.e[3][0] = in_src.e[5][1]; b.e[3][1] = in_src.e[3][2]; b.e[3][2] = in_src.e[2][3];
    b.e[4][0] = in_src.e[0][0]; b.e[4][1] = in_src.e[1][0]; b.e[4][2] = in_src.e[4][1];
    b.e[5][0] = in_src.e[0][1]; b.e[5][1] = in_src.e[3][1]; b.e[5][2] = in_src.e[4][0];
    b.e[6][0] = in_src.e[5][2]; b.e[6][1] = in_src.e[1][2]; b.e[6][2] = in_src.e[4][3];
    b.e[7][0] = in_src.e[5][3]; b.e[7][1] = in_src.e[3][3]; b.e[7][2] = in_src.e[4][2];
    return b;
}

int main()
{
    in_board in_src;
    read_input(in_src);
    src = convert_board(in_src);
    src_id = cubelet_id(src);
    ori_src_id = src_id;
    std::sort(src_id.begin(), src_id.end());
    
    auto moves = solve(src);
    print_moves(moves);
    return 0;
}