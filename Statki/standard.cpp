#include <iostream>

#define MAX_SHIPS 10
#define SHIP_SIZE 5
#define X 10
#define Y 21
#define CAR_DEFAULT 5
#define BAT_DEFAULT 4
#define CRU_DEFAULT 3
#define DES_DEFAULT 2

using namespace std;

enum output {
    NO_ERRORS,
    PLAYER_WON,
    INVALID_DIRECTION,
    NOT_IN_STARTING_POSITION,
    SHIP_ALREADY_PRESENT,
    ALL_ALREADY_SET,
    FILD_DOES_NOT_EXIST,
    NOT_ALL_SHIPS_PLACED,
};
enum directions {
    N,
    E,
    S,
    W,
};
enum shipClass {
    CAR = CAR_DEFAULT,
    BAT = BAT_DEFAULT,
    CRU = CRU_DEFAULT,
    DES = DES_DEFAULT,
};

struct player_boundaries {
    int y[2];
    int x[2];
};
struct coords {
    int x, y;
};
struct ship {
    shipClass type{};
    int size = SHIP_SIZE;
    char ship_elements[SHIP_SIZE]{};
    coords position[SHIP_SIZE*10]{};
};
struct player {
    ship ships[MAX_SHIPS*10];
    int CAR[2] = {1,1};
    int BAT[2] = {2,2};
    int CRU[2] = {3,3};
    int DES[2] = {4,4};
    int shipIndex[4] = {};
};

bool compare(char array1[], const char array2[]) {
    int i = 0;
    while (array1[i] != '\0' || array2[i] != '\0') {
        if (array1[i] != array2[i])
            return 0;
        i++;
    }

    return 1;
}
void boundaries(player_boundaries *P, int minX, int maxX, int minY, int maxY) {
    (*P).x[0] = minX;
    (*P).x[1] = maxX;
    (*P).y[0] = minY;
    (*P).y[1] = maxY;  
}

void PRINT(char board[X][Y], int* aRemaining, int* bRemaining) {
   
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            cout << board[j][i];
        }
        cout << '\n';
    }



    cout << "PARTS REMAINING:: A : " << *aRemaining << " B : " << *bRemaining << '\n';
}
void initialize_board(char board[X][Y]){
    for(int i=0; i<Y; i++)
        for(int j=0; j<X; j++)
            board[j][i] = ' ';
}
bool STARTING_POSITION(player_boundaries *P, int y, int x, directions Dd, shipClass sC) {
    int Bx[2], By[2];
    Bx[0] = (*P).x[0];
    Bx[1] = (*P).x[1];
    By[0] = (*P).y[0];
    By[1] = (*P).y[1];
    
    if (x > Bx[1] || x<Bx[0] || y>By[1] || y < By[0])
        return 1;
    
    switch (Dd) {
    case N: {
        if ((y + sC - 1 < By[0]) || (y + sC - 1 > By[1]) )
            return 1;
        else
            return 0;
    }
    case E: {
        if ((x - sC + 1 < Bx[0]) || (x - sC + 1 > Bx[1]))
            return 1;
        else
            return 0;
    }
    case S: {
        if ((y - sC + 1 > By[1]) || (y - sC + 1 < By[0]))
            return 1;
        else
            return 0;
    }
    case W: {
        if ((x + sC - 1 > Bx[1]) || (x + sC - 1 < Bx[0]))
            return 1;
        else
            return 0;
    }
    }
    return 0;
}
bool ALREADY_PRESENT(player *P,shipClass C, int i){
    switch(C) {
        case CAR:{
            if(i == (*P).shipIndex[0]) {
                (*P).shipIndex[0]++;
                return 0;
            }
            else   
                return 1;
        }
        case BAT:{
            if(i == (*P).shipIndex[1]) {
                (*P).shipIndex[1]++;
                return 0;
            }
            else   
                return 1;
        }
        case CRU:{
            if(i == (*P).shipIndex[2]) {
                (*P).shipIndex[2]++;
                return 0;
                }
            else   
                return 1;
        }
        case DES:{
            if(i == (*P).shipIndex[3]) {
                (*P).shipIndex[3]++;
                return 0;
            }
            else   
                return 1;

        }
    }
    return 0;
}
bool ALREADY_SET(player *P, shipClass C){
    switch(C){
        case CAR:{
            (*P).CAR[1] --;
            break;
        }
        case BAT:{
            (*P).BAT[1] --;
            break;
        }
        case CRU:{
            (*P).CRU[1] --;
            break;
        }
        case DES:{
            (*P).DES[1] --;
            break;
        }
    }
    if((*P).CAR[1]<0 || (*P).BAT[1]<0 || (*P).CRU[1]<0 || (*P).DES[1]<0)
        return true;
    else
        return false;
}

void SET_FLEET(player *P, int a1, int a2, int a3, int a4) {
    (*P).CAR[0] = a1;
    (*P).CAR[1] = a1;
    (*P).BAT[0] = a2;
    (*P).BAT[1] = a2;
    (*P).CRU[0] = a3;
    (*P).CRU[1] = a3;
    (*P).DES[0] = a4;
    (*P).DES[1] = a4;
}
bool SHOOT(player *P, char board[X][Y], int x, int y, int *wsk) {
    for (int i = 0; i < MAX_SHIPS; i++) {
        for (int j = 0; j < MAX_SHIPS; j++) {
            if (x == (*P).ships[i].position[j].x && y == (*P).ships[i].position[j].y && (*P).ships[i].ship_elements[j] != 'x' && board[x][y] != 'x') {
                board[x][y] = 'x';
                (*wsk)--;
                (*P).ships[i].ship_elements[j] = 'x';
                return 0;
            }
        }
    }
    return 0;
}
void PLACE_SHIP(char board[X][Y], player *P, int* index, int x, int y, directions D, shipClass C, int* Remaining) {

    (*P).ships[*index].type = C;
    (*P).ships[*index].position[0].x = x;
    (*P).ships[*index].position[0].y = y;
    (*P).ships[*index].size = C;
    *Remaining = (*Remaining + (*P).ships[*index].size);
    switch (D) {  
    case E: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x - i][y] = '+';
            (*P).ships[*index].position[i].x = x-i;
            (*P).ships[*index].position[i].y = y;
        }
        break;
    }
    case W: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x + i][y] = '+';
            (*P).ships[*index].position[i].x = x+i;
            (*P).ships[*index].position[i].y = y;
        }
        break;
    }
    case S: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x][y - i] = '+';
            (*P).ships[*index].position[i].x = x;
            (*P).ships[*index].position[i].y = y-i;
        }
        break;
    }
    case N: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x][y + i] = '+';
            (*P).ships[*index].position[i].x = x;
            (*P).ships[*index].position[i].y = y+i;
        }
        break;
    }
    }
    (*index)++;
}
bool victory_condition (int *wsk){
    if(*wsk)
        return 0;
    return 1;
}

void state(char board[X][Y], player *A, player *B, int* aRemaining, int* bRemaining, player_boundaries *bA, player_boundaries *bB) {
    char command[10];
    int x,y,a1, a2, a3, a4;
    char P;
    while (cin >> command) {
        if (compare(command, "PRINT")) {
            cin >> x;
            PRINT(board, aRemaining, bRemaining);
        }
        else if (compare(command, "SET_FLEET")) {
            cin >> P >> a1 >> a2 >> a3 >> a4;
            if (P == 'A')
                SET_FLEET(A, a1, a2, a3, a4);
            else if (P == 'B')
                SET_FLEET(B, a1, a2, a3, a4);
            else
                break;
        }
        else if(compare(command,"BOARD_SIZE")){
            cin>>y>>x;
            boundaries(bA, 0, X-1, 0, Y/2 -1);
            boundaries(bB, 0, X-1, Y/2+1, Y-1);
        }
        else if (compare(command, "NEXT_PLAYER")) {
            break;
        }
        else if (compare(command, "[state]")) {
            break;
        }
    }
}

output playerX(player *P,player *En, char board[X][Y], int* index, int* Remaining, int* ERemaining, player_boundaries *bP) {
    char D;
    char playerCommand[20], C[4];
    int x, y, i;
    directions Dd;
    shipClass sC;
    while (cin >> playerCommand) {
        if (compare(playerCommand, "PLACE_SHIP")) {
            cin >> y >> x >> D >> i >> C;
            if (compare(C, "CAR"))
                sC = CAR;
            else if (compare(C, "BAT"))
                sC = BAT;
            else if (compare(C, "CRU"))
                sC = CRU;
            else if (compare(C, "DES"))
                sC = DES;

            if (D == 'N')
                Dd = N;
            else if (D == 'E')
                Dd = E;
            else if (D == 'S')
                Dd = S;
            else if (D == 'W')
                Dd = W;
            else
                return INVALID_DIRECTION;

            if (STARTING_POSITION(bP, y, x, Dd, sC) == 1) {
                cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": NOT IN STARTING POSITION";
                return NOT_IN_STARTING_POSITION;
            }
            else if(ALREADY_PRESENT(P,sC,i)){
                cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": SHIP ALREADY PRESENT";
                return SHIP_ALREADY_PRESENT;
            }
            else if(ALREADY_SET(P,sC)){
                cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": ALL SHIPS OF THE CLASS ALREADY SET";
                return ALL_ALREADY_SET;
            }
            else
                PLACE_SHIP(board, P, index, x, y, Dd, sC, Remaining);

        }
        else if (compare(playerCommand, "SHOOT")) {
            cin >> y >> x;
            
            if((((*P).CAR[0] - (*P).shipIndex[0])!=0 || ((*P).BAT[0] - (*P).shipIndex[1])!=0 || ((*P).CRU[0] - (*P).shipIndex[2])!=0 || ((*P).DES[0] - (*P).shipIndex[3])!=0) || (((*En).CAR[0] - (*En).shipIndex[0])!=0 || ((*En).BAT[0] - (*En).shipIndex[1])!=0 || ((*En).CRU[0] - (*En).shipIndex[2])!=0 || ((*En).DES[0] - (*En).shipIndex[3])!=0)){
                cout<<"INVALID OPERATION "<<char(34)<<"SHOOT "<<y<<" "<<x<<char(34)<<": NOT ALL SHIPS PLACED";
                return NOT_ALL_SHIPS_PLACED;
            }
            if(y>=Y || x>=X) {
                cout<<"INVALID OPERATION "<<char(34)<<"SHOOT "<<y<<" "<<x<<char(34)<<": FIELD DOES NOT EXIST";
                return FILD_DOES_NOT_EXIST;
            }
            SHOOT(En,board,x,y,ERemaining);
                
            if(victory_condition(ERemaining)){
                return PLAYER_WON;
            }
        }
        else if (compare(playerCommand, "[playerA]")) {
            return NO_ERRORS;
        }
        else if (compare(playerCommand, "[playerB]")) {
            return NO_ERRORS;
        }
    }
    return NO_ERRORS;
}

int main() {
    output test;
    char board[X][Y];
    initialize_board(board);
    player A, B;
    player *wskA, *wskB;
    wskA = &A;
    wskB = &B;
    player_boundaries cbA, cbB;
    player_boundaries *bA, *bB;
    bA=&cbA; bB=&cbB;
    boundaries(bA, 0, X-1, 0, Y/2 -1);
    boundaries(bB, 0, X-1, Y/2+1, Y-1);
 
    char command[10];
    int a = 0, b = 0, aRemaining = 0, bRemaining = 0, p=0;
    int* wsk, * awsk, * bwsk;
    awsk = &aRemaining;
    bwsk = &bRemaining;
    while (cin >> command) {
        
        if (compare(command, "[state]")) {
            state(board, &A, &B, awsk, bwsk,bA,bB);
        }
        else if (compare(command, "[playerA]")) {
            if(p%2==1){
                cout<<"INVALID OPERATION "<<char(34)<<command<<" "<<char(34)<<": THE OTHER PLAYER EXPECTED";
                return 1;
            }
            p++;
            wsk = &a;
            test = playerX(wskA,wskB, board, wsk, awsk,bwsk,bA);
            if(test == PLAYER_WON){
                cout<<"A won";
                return 0;
            }
            else if (test != NO_ERRORS)
                return 1;
        }
        else if (compare(command, "[playerB]")) {
            if(p%2==0){
                cout<<"INVALID OPERATION "<<char(34)<<command<<" "<<char(34)<<": THE OTHER PLAYER EXPECTED";
                return 1;
            }
            p++;
            wsk = &b;
            test = playerX(wskB,wskA, board, wsk, bwsk,awsk,bB);
            if(test == PLAYER_WON){
                cout<<"B won";
                return 0;
            }
            else if (test != NO_ERRORS)
                return 1;
        }
    }

    return 0;
}

