#include <iostream>

#define MAX_SHIPS 10
#define Y 21
#define X 10
#define SHIP_SIZE 5
#define CAR_DEFAULT 5
#define BAT_DEFAULT 4
#define CRU_DEFAULT 3
#define DES_DEFAULT 2

using namespace std;

enum output {
    NO_ERRORS,
    INVALID_DIRECTION,
    NOT_IN_STARTING_POSITION,
    SHIP_ALREADY_PRESENT,
    ALL_ALREADY_SET,
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

struct coords {
    int x, y;
};
struct ship {
    shipClass type{};
    int size = SHIP_SIZE;
    char ship_elements[SHIP_SIZE]{};
    coords position[SHIP_SIZE]{};
    bool trafiony[SHIP_SIZE] = { false, false, false, false, false };
};
struct player {
    ship ships[MAX_SHIPS];
    int CAR = 1;
    int BAT = 2;
    int CRU = 3;
    int DES = 4;
    int BOUNDARIES_X[2];
    int BOUNDARIES_Y[2];
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
void initializeBoard(char board[X][Y]) {
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            board[j][i] = ' ';
}
void boundaries(player *P, int minX, int maxX, int minY, int maxY) {
    (*P).BOUNDARIES_X[0] = minX;
    (*P).BOUNDARIES_X[1] = maxX;
    (*P).BOUNDARIES_Y[0] = minY;
    (*P).BOUNDARIES_Y[1] = maxY;
}
void PLACE_SHIP(char board[X][Y], player *P, int* index, int x, int y, directions D, shipClass C, int* Remaining) {

    (*P).ships[*index].type = C;
    (*P).ships[*index].position[0].x = x;
    (*P).ships[*index].position[0].y = y;
    (*P).ships[*index].size = C;
    //initializeShip(P.ships[*index]);
    *Remaining = (*Remaining + (*P).ships[*index].size);
    switch (D) {  
    case E: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x - i][y] = '+';//P.ships[*index].ship_elements[i];
            (*P).ships->position[i+1].x = x-i;
            (*P).ships->position[i+1].y = y;
        }
        break;
    }
    case W: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x + i][y] = '+';//P.ships[*index].ship_elements[i];
            (*P).ships->position[i+1].x = x+i;
            (*P).ships->position[i+1].y = y;
        }
        break;
    }
    case S: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x][y - i] = '+';//P.ships[*index].ship_elements[i];
            (*P).ships->position[i+1].x = x;
            (*P).ships->position[i+1].y = y-i;
        }
        break;
    }
    case N: {
        for (int i = 0; i < (*P).ships[*index].size; i++){
            board[x][y + i] = '+';//P.ships[*index].ship_elements[i];
            (*P).ships->position[i+1].x = x;
            (*P).ships->position[i+1].y = y+i;
        }
        break;
    }
    }


    (*index)++;
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

void SET_FLEET(player *P, int a1, int a2, int a3, int a4) {
    (*P).CAR = a1;
    (*P).BAT = a2;
    (*P).CRU = a3;
    (*P).DES = a4;
}

bool SHOOT(player *P, char board[X][Y], int x, int y, int *wsk) {
    for (int i = 0; i < MAX_SHIPS; i++) {
        for (int j = 0; j < MAX_SHIPS; j++) {
            if (x == (*P).ships[i].position[j].x && y == (*P).ships[i].position[j].y) {
                board[x][y] = 'x';
                (*wsk)--;
                (*P).ships[i].trafiony[j] = true;
                return 0;
            }
        }
    }
    return 1;
}

void state(char board[X][Y], player *A, player *B, int* aRemaining, int* bRemaining) {
    char command[10];
    int x, a1, a2, a3, a4;
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

            //i++;
        }
        else if (compare(command, "NEXT_PLAYER")) {
            break;
        }
        else if (compare(command, "[state]")) {
            break;
        }
    }
}

bool STARTING_POSITION(int Bx[], int By[], int y, int x, directions Dd, shipClass sC) {
    if (x > Bx[1] || x<Bx[0] || y>By[1] || y < By[0])
        return 1;
    //cout<< Bx[0]<<Bx[1]<<" "<<By[0]<<By[1];

    
    //cout<<x<<" "<<y<<" "<<sC;
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
            (*P).CAR --;
            break;
        }
        case BAT:{
            (*P).BAT --;
            break;
        }
        case CRU:{
            (*P).CRU --;
            break;
        }
        case DES:{
            (*P).DES --;
            break;
        }
    }
    if((*P).CAR<0 || (*P).BAT<0 || (*P).CRU<0 || (*P).DES<0)
        return true;
    else
        return false;
}

output playerX(player *P,player *En, char board[X][Y], int* index, int* Remaining, int* ERemaining) {
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
            
            if (STARTING_POSITION((*P).BOUNDARIES_X, (*P).BOUNDARIES_Y, y, x, Dd, sC) == 1) {
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
            SHOOT(En,board,x,y,ERemaining);
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
    initializeBoard(board);
    player A, B;
    player *wskA, *wskB;
    //boundaries(A, 0, 9, 0, 9);
    //boundaries(B, 0, 9, 11, 21);

    A.BOUNDARIES_X[0] = 0;
    A.BOUNDARIES_X[1] = 9;
    A.BOUNDARIES_Y[0] = 0;
    A.BOUNDARIES_Y[1] = 9;

    B.BOUNDARIES_X[0] = 0;
    B.BOUNDARIES_X[1] = 9;
    B.BOUNDARIES_Y[0] = 11;
    B.BOUNDARIES_Y[1] = 21;
 
    char command[10];
    int a = 0, b = 0, aRemaining = 0, bRemaining = 0;
    int* wsk, * awsk, * bwsk;
    awsk = &aRemaining;
    bwsk = &bRemaining;
    wskA = &A;
    wskB = &B;
    while (cin >> command) {
        if (compare(command, "[playerA]")) {
            wsk = &a;
            test = playerX(wskA,wskB, board, wsk, awsk,bwsk);
            if (test != NO_ERRORS)
                return 1;
        }
        else if (compare(command, "[playerB]")) {
            wsk = &b;
            test = playerX(wskB,wskA, board, wsk, bwsk,awsk);
            if (test != NO_ERRORS)
                return 1;
        }
        else if (compare(command, "[state]")) {
            state(board, &A, &B, awsk, bwsk);
        }
    }

    return 0;
}


