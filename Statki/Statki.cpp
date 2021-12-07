#include <iostream>
#include <math.h>

#define MAX_SHIPS 10
#define SHIP_SIZE 5
#define CAR_DEFAULT 5
#define BAT_DEFAULT 4
#define CRU_DEFAULT 3
#define DES_DEFAULT 2

using namespace std;

int X, Y;
bool EXTENDED_SHIPS = false;

enum output {
    NO_PROBLEMS,
    PLAYER_WON,
    NOT_IN_STARTING_POSITION,
    SHIP_ALREADY_PRESENT,
    ALL_ALREADY_SET,
    FILD_DOES_NOT_EXIST,
    NOT_ALL_SHIPS_PLACED,
    PLACED_SHIP_ON_REEF,
    SHIP_TO_CLOSE,
    REEF_PLACED_OF_BOARD,
    SHIP_MOVED_ALREADY,
    SHIP_CANNOT_MOVE,
    SHIP_CANNOT_SHOOT,
    SHOOTING_TOO_FAR,
    TOO_MANY_SHOOTS,

    END,
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

struct playerBoundaries {
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
    coords position[SHIP_SIZE * 10]{};
    directions D;
    int moves = 0;
    int shoots = 1;
};
struct player {
    ship ships[MAX_SHIPS * 10];
    int CAR[2] = { 1,1 };
    int BAT[2] = { 2,2 };
    int CRU[2] = { 3,3 };
    int DES[2] = { 4,4 };
    int placedShips[4] = {};
    playerBoundaries boundaries;
};

int* dynamic_array(int x) {
    int* dynArray = new int[x+1];
    for (int i = 0; i < x+1; i++) {
        dynArray[i] = -1;
    }

    return dynArray;
}
char** BOARD_SIZE(int y, int x) {
    char** new_board;
    new_board = new char* [y];
    for (int i = 0; i < y; i++)
        new_board[i] = new char[x];
    return new_board;
}
bool compare(char array1[], const char array2[]) {
    int i = 0;
    while (array1[i] != '\0' || array2[i] != '\0') {
        if (array1[i] != array2[i])
            return 0;
        i++;
    }

    return 1;
}
void boundaries(player* P, int minX, int maxX, int minY, int maxY) {
    (*P).boundaries.x[0] = minX, (*P).boundaries.x[1] = maxX;
    (*P).boundaries.y[0] = minY, (*P).boundaries.y[1] = maxY;
}
int distance(coords P1, coords P2){
    return sqrt(((P1.y - P2.y)*(P1.y - P2.y)) + ((P1.x - P2.x)*(P1.x - P2.x))) ;
}
directions convertToDirections(char D) {
    if (D == 'N')
        return N;
    else if (D == 'E')
        return E;
    else if (D == 'S')
        return S;
    else if (D == 'W')
        return W;
    return N;
}
shipClass convertToShipClass(char C[4]) {
    if (compare(C, "CAR"))
        return CAR;
    else if (compare(C, "BAT"))
        return BAT;
    else if (compare(C, "CRU"))
        return CRU;
    else if (compare(C, "DES"))
        return DES;
    return CAR;
}
void PRINT(char** board, int* aRemaining, int* bRemaining, bool x) {

    if(x){
        cout<<"  ";
        for(int i = 0; i< X; i++)
            cout<<i;
        cout<<'\n';
        for (int i = 0; i < Y; i++) {
            if(i<10)
                cout<<0<<i;
            else
                cout<<i;
            
            for (int j = 0; j < X; j++) {
                
                    cout << board[i][j];
            }
            cout << '\n';
        }
    }
    else{
        for (int i = 0; i < Y; i++) {
            for (int j = 0; j < X; j++) {
                if(board[i][j] == '!' || board[i][j] == '@' || board[i][j] == '%')
                    cout<<"+";
                else
                    cout << board[i][j];
            }
            cout << '\n';
        }
    }



    cout << "PARTS REMAINING:: A : " << *aRemaining << " B : " << *bRemaining << '\n';
}
void initialize_board(char** board) {
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            board[i][j] = ' ';
}
bool STARTING_POSITION(player* P, int y, int x, directions Dd, shipClass sC) {
    int Bx[2], By[2];
    Bx[0] = (*P).boundaries.x[0];
    Bx[1] = (*P).boundaries.x[1];
    By[0] = (*P).boundaries.y[0];
    By[1] = (*P).boundaries.y[1];

    if (x > Bx[1] || x<Bx[0] || y>By[1] || y < By[0])
        return 1;

    switch (Dd) {
    case N: {
        if ((y + sC - 1 < By[0]) || (y + sC - 1 > By[1]))
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
bool ALREADY_PRESENT(player* P, shipClass C, int i, int* iCAR, int* iBAT, int* iCRU, int* iDES, int *index) {
    switch (C) {
    case CAR: {
        if (iCAR[i] == -1) {
            iCAR[i] = (*index);
            (*P).placedShips[0]++;
            return 0;
        }
        else
            return 1;
    }
    case BAT: {
        if (iBAT[i] == -1) {
            iBAT[i] = (*index);
            (*P).placedShips[1]++;
            return 0;
        }
        else
            return 1;
    }
    case CRU: {
        if (iCRU[i] == -1) {
            iCRU[i] = (*index);
            (*P).placedShips[2]++;
            return 0;
        }
        else
            return 1;
    }
    case DES: {
        if (iDES[i] == -1) {
            iDES[i] = (*index);
            (*P).placedShips[3]++;
            return 0;
        }
        else
            return 1;

    }
    }
    return 0;
}
bool ALREADY_SET(player* P, shipClass C) {
    switch (C) {
    case CAR: {
        (*P).CAR[1] --;
        break;
    }
    case BAT: {
        (*P).BAT[1] --;
        break;
    }
    case CRU: {
        (*P).CRU[1] --;
        break;
    }
    case DES: {
        (*P).DES[1] --;
        break;
    }
    }
    if ((*P).CAR[1] < 0 || (*P).BAT[1] < 0 || (*P).CRU[1] < 0 || (*P).DES[1] < 0)
        return true;
    else
        return false;
}
bool SHIP_ON_REEF(char** board, int y, int x, directions D, shipClass C) {
    switch (D) {
    case N: {
        for (int i = 0; i < C; i++) {
            if (board[y + i][x] == '#')
                return 1;
        }
        return 0;
    }
    case E: {
        for (int i = 0; i < C; i++) {
            if (board[y][x - i] == '#')
                return 1;
        }
        return 0;
    }
    case S: {
        for (int i = 0; i < C; i++) {
            if (board[y - i][x] == '#')
                return 1;
        }
        return 0;
    }
    case W: {
        for (int i = 0; i < C; i++) {
            if (board[y][x + i] == '#')
                return 1;
        }
        return 0;
    }
    }
    return 0;
}
bool SHIPS_TO_CLOSE(char** board, int y, int x, directions D, shipClass C) {
    switch (D) {
    case N: {
        for (int i = y - 1; i < y + C; i++) {
            for (int j = x - 1; j < x + 2; j++) {
                if (i >= 0 && i < Y && j >= 0 && j < X)
                    if (board[i][j] != ' ' && board[i][j] != '#')
                        return 1;
            }
        }
        return 0;
    }
    case E: {
        for (int i = y - 1; i < y + 2; i++) {
            for (int j = x - C; j < x + 1; j++) {
                if (i >= 0 && i < Y && j >= 0 && j < X)
                    if (board[i][j] != ' ' && board[i][j] != '#')
                        return 1;
            }
        }
        return 0;
    }
    case S: {
        for (int i = y - C; i < y + 1; i++) {
            for (int j = x - 1; j < x + 2; j++) {
                if (i >= 0 && i < Y && j >= 0 && j < X)
                    if (board[i][j] != ' ' && board[i][j] != '#')
                        return 1;
            }
        }
        return 0;
    }
    case W: {
        for (int i = y - 1; i < y + 2; i++) {
            for (int j = x - 1; j < x + C; j++) {
                if (i >= 0 && i < Y && j >= 0 && j < X)
                    if (board[i][j] != ' ' && board[i][j] != '#')
                        return 1;
            }
        }
        return 0;
    }
    }
    return 1;
}
void SET_FLEET(player* P, int a1, int a2, int a3, int a4,int *&iCAR, int *&iBAT, int *&iCRU, int *&iDES) {
    (*P).CAR[0] = a1, (*P).CAR[1] = a1;
    (*P).BAT[0] = a2, (*P).BAT[1] = a2;
    (*P).CRU[0] = a3, (*P).CRU[1] = a3;
    (*P).DES[0] = a4, (*P).DES[1] = a4;
    //delete [] iCAR, delete[] iBAT, delete[] iCRU, delete[] iDES;
    iCAR = dynamic_array(a1), iBAT = dynamic_array(a2), iCRU = dynamic_array(a3), iDES = dynamic_array(a4);

}
bool SHOOT_basic(player* P, char** board, int x, int y, int* wsk) {
    for (int i = 0; i < MAX_SHIPS; i++) {
        for (int j = 0; j < 5; j++) {
            if (x == (*P).ships[i].position[j].x && y == (*P).ships[i].position[j].y && (*P).ships[i].ship_elements[j] != 'x' && board[y][x] != 'x') {
                board[y][x] = 'x';
                (*wsk)--;
                (*P).ships[i].ship_elements[j] = 'x';
                return 0;
            }
        }
    }
    return 0;
}
output SHOOT_extended(player *P, player *En, char** board, int x, int y,char C[], int* ERemaining, int i, int *ShipClassIndex, shipClass sC){
    int index = ShipClassIndex[i];
    if((*P).ships[index].shoots > sC){
        cout<<"INVALID OPERATION "<<char(34)<<"SHOOT "<<i<<" "<<C<<" "<<y<<" "<<x<<char(34)<<": TOO MANY SHOOTS";
        return TOO_MANY_SHOOTS;
    }
    if((*P).ships[index].ship_elements[1] != '!'){
        cout<<"INVALID OPERATION "<<char(34)<<"SHOOT "<<i<<" "<<C<<" "<<y<<" "<<x<<char(34)<<": SHIP CANNOT SHOOT";
        return SHIP_CANNOT_SHOOT;
    }
    coords P1, P2;
    P1 = (*P).ships[index].position[1];
    P2.x = x, P2.y = y;
    if(distance(P1,P2) > sC && sC != CAR){
        cout<<"INVALID OPERATION "<<char(34)<<"SHOOT "<<i<<" "<<C<<" "<<y<<" "<<x<<char(34)<<": SHOOTING TOO FAR";
        return SHOOTING_TOO_FAR;
    }
    (*P).ships[index].shoots++;
    SHOOT_basic(En, board, x, y, ERemaining);
    return NO_PROBLEMS;
}
void PLACE_SHIP(char** board, player* &P, int* index, int x, int y, directions D, shipClass C, int* Remaining) {

    (*P).ships[*index].type = C;
    (*P).ships[*index].position[0].x = x;
    (*P).ships[*index].position[0].y = y;
    (*P).ships[*index].size = C;
    (*P).ships[*index].D = D;
    *Remaining = (*Remaining + (*P).ships[*index].size);
    if((*P).ships[*index].ship_elements[C-1] == '\0')
        (*P).ships[*index].ship_elements[C-1] = '%';
    if((*P).ships[*index].ship_elements[1] == '\0')
        (*P).ships[*index].ship_elements[1] = '!';
    if((*P).ships[*index].ship_elements[0] == '\0')
        (*P).ships[*index].ship_elements[0] = '@';
    switch (D) {
    case E: {
        for (int i = 0; i < (*P).ships[*index].size; i++) {
            if ((*P).ships[*index].ship_elements[i] == '\0')
                board[y][x - i] = '+';
            else
                board[y][x - i] = (*P).ships[*index].ship_elements[i];
            (*P).ships[*index].position[i].x = x - i;
            (*P).ships[*index].position[i].y = y;
        }
        break;
    }
    case W: {
        for (int i = 0; i < (*P).ships[*index].size; i++) {
            if ((*P).ships[*index].ship_elements[i] == '\0')
                board[y][x + i] = '+';
            else
                board[y][x + i] = (*P).ships[*index].ship_elements[i];
            (*P).ships[*index].position[i].x = x + i;
            (*P).ships[*index].position[i].y = y;
        }
        break;
    }
    case S: {
        for (int i = 0; i < (*P).ships[*index].size; i++) {
            if ((*P).ships[*index].ship_elements[i] == '\0')
                board[y - i][x] = '+';
            else
                board[y - i][x] = (*P).ships[*index].ship_elements[i];
            (*P).ships[*index].position[i].x = x;
            (*P).ships[*index].position[i].y = y - i;
        }
        break;
    }
    case N: {
        for (int i = 0; i < (*P).ships[*index].size; i++) {
            if ((*P).ships[*index].ship_elements[i] == '\0')
                board[y + i][x] = '+';
            else
                board[y + i][x] = (*P).ships[*index].ship_elements[i];
            (*P).ships[*index].position[i].x = x;
            (*P).ships[*index].position[i].y = y + i;
        }
        break;
    }
    }
    (*index)++;
}
bool OFF_BOARD(int y, int x, shipClass C, directions D){
    switch(D){
        case  N:{
            for(int i=0; i<C; i++){
            if(x < 0 || x >= X || y + i < 0 || y + i >= Y)
                return 1;
            }
            break;
        }
        case E:{
            for(int i=0; i<C; i++){
            if(x - i < 0 || x - i >= X || y < 0 || y >= Y)
                return 1;
            }
            break;
        }
        case S:{
            for(int i=0; i<C; i++){
            if(x < 0 || x >= X || y - i < 0 || y - i >= Y)
                return 1;
            }
            break;
        }
        case W:{
            for(int i=0; i<C; i++){
            if(x + i < 0 || x + i >= X || y < 0 || y >= Y)
                return 1;
            }
            break;
        }
    }
    return 0;
}
output MOVE(char **board, int i, char C[], char M, player *&P, int *shipClassIndex){
    int index = shipClassIndex[i], x = 0, y = 0,blank = 10;
    directions D = (*P).ships[index].D;
    shipClass sC = convertToShipClass(C);
    if((*P).ships[index].ship_elements[sC-1] != '%'){
        cout<<"INVALID OPERATION "<<char(34)<<"MOVE "<< i << " " << C << " " << M << char(34) <<": SHIP CANNOT MOVE";
        return SHIP_CANNOT_MOVE;
    }
    if((*P).ships[index].moves == 3 || ((*P).ships[index].moves == 2 && sC == CAR)){
        cout<<"INVALID OPERATION "<<char(34)<<"MOVE "<< i << " " << C << " " << M << char(34) <<": SHIP MOVED ALREADY";
        return SHIP_MOVED_ALREADY;
    }
    for(int j=0; j<sC; j++){
            x = (*P).ships[index].position[j].x, y = (*P).ships[index].position[j].y;
            board[y][x] = ' ';
        }
    switch(M){
        case 'F':{
            switch(D){
            case N:{
                x = (*P).ships[index].position[0].x, y = (*P).ships[index].position[0].y-1;
                break;
            }
            case E:{
                x = (*P).ships[index].position[0].x+1, y = (*P).ships[index].position[0].y;
                break;
            }
            case S:{
                x = (*P).ships[index].position[0].x, y = (*P).ships[index].position[0].y+1;
                break;
            }
            case W:{
                x = (*P).ships[index].position[0].x-1, y = (*P).ships[index].position[0].y;
                break;
            }
        }
            break;
        }
        case 'R':{
            switch(D){
            case N:{
                x = (*P).ships[index].position[0].x, y = (*P).ships[index].position[0].y-1, D = W;
                (*P).ships[index].D = W;
                break;
            }
            case E:{
                x = (*P).ships[index].position[0].x+1, y = (*P).ships[index].position[0].y, D = N;
                (*P).ships[index].D = N;
                break;
            }
            case S:{
                x = (*P).ships[index].position[0].x, y = (*P).ships[index].position[0].y+1, D = E;
                (*P).ships[index].D = E;
                break;
            }
            case W:{
                x = (*P).ships[index].position[0].x-1, y = (*P).ships[index].position[0].y, D = S;
                (*P).ships[index].D = S;
                break;
            }
        }
            break;
        }
        case 'L':{
            switch(D){
            case N:{
                x = (*P).ships[index].position[0].x, y = (*P).ships[index].position[0].y-1, D = E;
                (*P).ships[index].D = E;
                break;
            }
            case E:{
                x = (*P).ships[index].position[0].x+1, y = (*P).ships[index].position[0].y, D = S;
                (*P).ships[index].D = S;
                break;
            }
            case S:{
                x = (*P).ships[index].position[0].x, y = (*P).ships[index].position[0].y+1, D = W;
                (*P).ships[index].D = W;
                break;
            }
            case W:{
                x = (*P).ships[index].position[0].x-1, y = (*P).ships[index].position[0].y, D = N;
                (*P).ships[index].D = N;
                break;
            }
        }
            break;
        }
    }  
    if(SHIPS_TO_CLOSE(board,y,x,D,sC)){
        cout<<"INVALID OPERATION "<<char(34)<<"MOVE "<< i << " " << C << " " << M << char(34) <<": PLACING SHIP TOO CLOSE TO OTHER SHIP";
        return SHIP_TO_CLOSE;
    }
    else if(OFF_BOARD(y,x,sC,D)){
        cout<<"INVALID OPERATION "<<char(34)<<"MOVE "<< i << " " << C << " " << M << char(34) <<": SHIP WENT FROM BOARD";
        return NOT_IN_STARTING_POSITION;
    }
    else if(SHIP_ON_REEF(board,y,x,D,sC)){
        cout<<"INVALID OPERATION "<<char(34)<<"MOVE "<< i << " " << C << " " << M << char(34) <<": PLACING SHIP ON REEF";
        return PLACED_SHIP_ON_REEF;
    }
        (*P).ships[index].moves ++;
        PLACE_SHIP(board,P,&index,x,y,D,sC,&blank);
        return NO_PROBLEMS;
}
bool victory_condition(int* wsk) {
    if (*wsk)
        return 0;
    return 1;
}
void end_of_the_round(player *P){
    for(int i=0; i<MAX_SHIPS; i++){
        (*P).ships[i].moves = 0;
        (*P).ships[i].shoots = 1;
    }
}
output SHIP(char** board, player* P, int y, int x, directions D, shipClass C, char status[SHIP_SIZE], int* index, int* Remaining, int i, int *iCAR, int *iBAT, int *iCRU, int *iDES){

    for (int j = 0; j < C; j++)
        if (status[j] == '1') {
            (*P).ships[*index].ship_elements[j] = '+';
        }
        else {
            (*P).ships[*index].ship_elements[j] = 'x';
            (*Remaining)--;
        }
    if (STARTING_POSITION(P, y, x, D, C) == 1)
        return NOT_IN_STARTING_POSITION;
    else if (ALREADY_SET(P, C))
        return ALL_ALREADY_SET;
            else if(ALREADY_PRESENT(P,C,i,iCAR,iBAT,iCRU,iDES,index))
                    return SHIP_ALREADY_PRESENT;
    else if (SHIP_ON_REEF(board, y, x, D, C))
        return PLACED_SHIP_ON_REEF;
    else if (SHIPS_TO_CLOSE(board, y, x, D, C)) 
        return SHIP_TO_CLOSE;
    else
        PLACE_SHIP(board, P, index, x, y, D, C, Remaining);
    return NO_PROBLEMS;
}
output reef(char **board, int y, int x){
    if(y>=Y || y<0 || x>=X || x<0){
        cout<<"INVALID OPERATION "<<char(34)<<"REEF "<<y<<" "<<x<<char(34)<<": REEF IS NOT PLACED ON BOARD";
        return REEF_PLACED_OF_BOARD;
    }
    board[y][x] = '#';
    return NO_PROBLEMS;
}
void next_player(char P, int *playerIndex){
    if (P == 'A')
        *playerIndex = 0;
    else if (P == 'B')
        *playerIndex = 1;
}

output invalid_operation_shoot(player *P, player *En, int y, int x){
    if ((((*P).CAR[0] - (*P).placedShips[0]) != 0 || ((*P).BAT[0] - (*P).placedShips[1]) != 0 || ((*P).CRU[0] - (*P).placedShips[2]) != 0 || ((*P).DES[0] - (*P).placedShips[3]) != 0) || (((*En).CAR[0] - (*En).placedShips[0]) != 0 || ((*En).BAT[0] - (*En).placedShips[1]) != 0 || ((*En).CRU[0] - (*En).placedShips[2]) != 0 || ((*En).DES[0] - (*En).placedShips[3]) != 0)) {
        cout << "INVALID OPERATION " << char(34) << "SHOOT " << y << " " << x << char(34) << ": NOT ALL SHIPS PLACED";
        return NOT_ALL_SHIPS_PLACED;
        }
        if (y >= Y || x >= X) {
            cout << "INVALID OPERATION " << char(34) << "SHOOT " << y << " " << x << char(34) << ": FIELD DOES NOT EXIST";
            return FILD_DOES_NOT_EXIST;
        }
        return NO_PROBLEMS;
}
output invalid_operation_place_ship(char **board, player *P, int y, int x, char D, char C[], int i, int *index, int *iCAR, int *iBAT, int *iCRU, int *iDES){
    directions Dd = convertToDirections(D);
    shipClass sC = convertToShipClass(C);
    if (STARTING_POSITION(P, y, x, Dd, sC) == 1) {
        cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": NOT IN STARTING POSITION";
        return NOT_IN_STARTING_POSITION;
    }
    else if (ALREADY_PRESENT(P, sC, i, iCAR, iBAT, iCRU, iDES,index)) {
        cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": SHIP ALREADY PRESENT";
        return SHIP_ALREADY_PRESENT;
    }
    else if (ALREADY_SET(P, sC)) {
        cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": ALL SHIPS OF THE CLASS ALREADY SET";
        return ALL_ALREADY_SET;
    }
    else if (SHIP_ON_REEF(board, y, x, Dd, sC)) {
        cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": PLACING SHIP ON REEF";
        return PLACED_SHIP_ON_REEF;
    }
    else if (SHIPS_TO_CLOSE(board, y, x, Dd, sC)) {
        cout << "INVALID OPERATION " << char(34) << "PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << char(34) << ": PLACING SHIP TOO CLOSE TO OTHER SHIP";
        return SHIP_TO_CLOSE;
    }
    return NO_PROBLEMS;
}
output invalid_operation_ship(output shipOutput, char P, int y, int x, char D, int i, char C[], char status[]){
    switch (shipOutput) {
                case NOT_IN_STARTING_POSITION: {
                    cout << "INVALID OPERATION " << char(34) << "SHIP " << P << " " << y << " " << x << " " << D << " " << i << " " << C << " " << status << char(34) << ": NOT IN STARTING POSITION";
                    return shipOutput;
                }
                case ALL_ALREADY_SET: {
                    cout << "INVALID OPERATION " << char(34) << "SHIP " << P << " " << y << " " << x << " " << D << " " << i << " " << C << " " << status << char(34) << ": ALL SHIPS OF THE CLASS ALREADY SET";
                    return shipOutput;
                }
                case SHIP_ALREADY_PRESENT: {
                    cout << "INVALID OPERATION " << char(34) << "SHIP " << P << " " << y << " " << x << " " << D << " " << i << " " << C << " " << status << char(34) << ": SHIP ALREADY PRESENT";
                    return shipOutput;
                }
                case PLACED_SHIP_ON_REEF: {
                    cout << "INVALID OPERATION " << char(34) << "SHIP " << P << " " << y << " " << x << " " << D << " " << i << " " << C << " " << status << char(34) << ": PLACING SHIP ON REEF";
                    return shipOutput;
                }
                case SHIP_TO_CLOSE: {
                    cout << "INVALID OPERATION " << char(34) << "SHIP " << P << " " << y << " " << x << " " << D << " " << i << " " << C << " " << status << char(34) << ": PLACING SHIP TOO CLOSE TO OTHER SHIP";
                    return shipOutput;
                }
                default: {
                    return NO_PROBLEMS;
                }
            }
}
//state i playerX rozbić ograniczenie na strzelanie!!
output state(char** board, player* A, player* B, int* aRemaining, int* bRemaining, int* a, int* b, int* playerIndex, int* aCAR, int *aBAT, int *aCRU, int *aDES, int* bCAR, int *bBAT, int *bCRU, int *bDES) {
    char stateCommand[10], status[SHIP_SIZE], C[4];
    shipClass sC;
    directions Dd;
    int x, y, a1, a2, a3, a4;
    bool extprt;
    char P, D;
    while (cin >> stateCommand) {
        if (compare(stateCommand, "PRINT")) {
            cin >> extprt;
            PRINT(board, aRemaining, bRemaining,extprt);
        }
        else if (compare(stateCommand, "SET_FLEET")) {
            cin >> P >> a1 >> a2 >> a3 >> a4;
            if (P == 'A')
                SET_FLEET(A, a1, a2, a3, a4,aCAR,aBAT,aCRU,aDES);
            else if (P == 'B')
                SET_FLEET(B, a1, a2, a3, a4,bCAR,bBAT,bCRU,bDES);
            else
                break;
        }
        else if (compare(stateCommand, "BOARD_SIZE")) {
            cin >> y >> x;
            Y = y;
            X = x;
            delete[] * board;
            delete[] board;
            board = BOARD_SIZE(y, x);
            initialize_board(board);
            boundaries(A, 0, x - 1, 0, y / 2 - 1);
            boundaries(B, 0, x - 1, y / 2 + 1, y - 1);
        }
        else if (compare(stateCommand, "SHIP")) {
            int i;
            cin >> P >> y >> x >> D >> i >> C >> status;

            Dd = convertToDirections(D);
            sC = convertToShipClass(C);
            output shipOutput;
            if (P == 'A')
                shipOutput = SHIP(board, A, y, x, Dd, sC, status, a, aRemaining, i,aCAR,aBAT,aCRU,aDES);
            else
                shipOutput = SHIP(board, B, y, x, Dd, sC, status, b, bRemaining, i,bCAR,bBAT,bCRU,bDES);
            if(invalid_operation_ship(shipOutput,P,y,x,D,i,C,status) != NO_PROBLEMS)
                return shipOutput;
        }
        else if (compare(stateCommand, "INIT_POSITION")) {
            int y1, y2, x1, x2;
            cin >>P >> y1 >> x1 >> y2 >> x2;
            if(P == 'A')
                boundaries(A,x1,x2,y1,y2);
            else
                boundaries(B,x1,x2,y1,y2);
        }
        else if (compare(stateCommand, "REEF")) {
            cin >> y >> x;
            if(reef(board,y,x) != NO_PROBLEMS)
                return REEF_PLACED_OF_BOARD;
        }
        else if (compare(stateCommand, "NEXT_PLAYER")) {
            cin >> P;
            next_player(P,playerIndex);
        }
        else if (compare(stateCommand, "EXTENDED_SHIPS")){
            EXTENDED_SHIPS = true;
        }
        //do zrobienia
        else if (compare(stateCommand, "SET_AI_PLAYER")){
            cin>>P;
            return END;
        }
        //do zrobienia
        else if (compare(stateCommand, "SRAND")){
            cin>>x;
            return END;
        }
        //do zrobienia
        else if(compare(stateCommand, "SAVE")){
            return END;
        }
        else if (compare(stateCommand, "[state]")) {
            return NO_PROBLEMS;
        }
    }
    return NO_PROBLEMS;
}

output playerX(player* P, player* En, char** board, int* index, int* Remaining, int* ERemaining, int* iCAR, int* iBAT, int* iCRU, int* iDES) {
    char D,M;
    char playerCommand[20], C[4];
    int x, y, i;
    directions Dd;
    shipClass sC;
    output IO = NO_PROBLEMS;
    while (cin >> playerCommand) {
        if (compare(playerCommand, "PLACE_SHIP")) {
            cin >> y >> x >> D >> i >> C;

            Dd = convertToDirections(D);
            sC = convertToShipClass(C);
            IO = invalid_operation_place_ship(board,P,y,x,D,C,i,index,iCAR,iBAT,iCRU,iDES);
            if(IO != NO_PROBLEMS)
                return IO;
            else
                PLACE_SHIP(board, P, index, x, y, Dd, sC, Remaining);

        }
        else if (compare(playerCommand, "SHOOT")) {
            if(EXTENDED_SHIPS == true){
                cin>>i>>C>>y>>x;
                sC = convertToShipClass(C);
                switch(sC){
                    case CAR:{
                        IO = SHOOT_extended(P,En,board,x,y,C,ERemaining,i,iCAR,sC);
                        break;
                    }
                    case BAT:{
                        IO = SHOOT_extended(P,En,board,x,y,C,ERemaining,i,iBAT,sC);
                        break;
                    }
                    case CRU:{
                        IO = SHOOT_extended(P,En,board,x,y,C,ERemaining,i,iCRU,sC);
                        break;
                    }
                    case DES:{
                        IO = SHOOT_extended(P,En,board,x,y,C,ERemaining,i,iDES,sC);
                        break;
                    }
                }
                if(IO !=NO_PROBLEMS)
                    return IO;
            }
            else{
                cin >> y >> x;
                IO = invalid_operation_shoot(P,En,y,x);
                if(IO != NO_PROBLEMS)
                    return IO;
                SHOOT_basic(En, board, x, y, ERemaining);
            }
            if (victory_condition(ERemaining)) {
                return PLAYER_WON;
            }
        }
        else if (compare(playerCommand, "MOVE")) {
            cin>>i>>C>>M;
            sC = convertToShipClass(C);
            output move = NO_PROBLEMS;
            switch(sC){
                case CAR:{
                    move = MOVE(board,i,C,M,P,iCAR);
                    break;
                }
                case BAT:{
                    move = MOVE(board,i,C,M,P,iBAT);
                    break;
                }
                case CRU:{
                    move = MOVE(board,i,C,M,P,iCRU);
                    break;
                }
                case DES:{
                    move = MOVE(board,i,C,M,P,iDES);
                    break;
                }
                default:{

                }
            }
            if(move != NO_PROBLEMS)
                return move;
        }
        //do zrobienia
        else if (compare(playerCommand, "SPY")) {
            cin>>y>>x;
            return END;
        }
        else if (compare(playerCommand, "[playerA]")) {
            end_of_the_round(P);
            return NO_PROBLEMS;
        }
        else if (compare(playerCommand, "[playerB]")) {
            end_of_the_round(P);
            return NO_PROBLEMS;
        }
    }
    return NO_PROBLEMS;
}

int main() {
    output test;
    X = 10, Y = 21;
    char** board = BOARD_SIZE(Y, X);
    initialize_board(board);
    player A, B;
    player* wskA, * wskB;
    wskA = &A, wskB = &B;
    int* aCAR = dynamic_array(A.CAR[0]), * aBAT = dynamic_array(A.BAT[0]), * aCRU = dynamic_array(A.CRU[0]), * aDES = dynamic_array(A.DES[0]);
    int* bCAR = dynamic_array(B.CAR[0]), * bBAT = dynamic_array(B.BAT[0]), * bCRU = dynamic_array(B.CRU[0]), * bDES = dynamic_array(B.DES[0]);
    boundaries(wskA, 0, X - 1, 0, Y / 2 - 1);
    boundaries(wskB, 0, X - 1, Y / 2 + 1, Y - 1);
    char command[10];
    int a = 0, b = 0, aRemaining = 0, bRemaining = 0, p = 0;
    int* wsk, * awsk, * bwsk;
    awsk = &aRemaining, bwsk = &bRemaining;
    while (cin >> command) {
        if (compare(command, "[state]")) {
            wsk = &p;
            if (state(board, &A, &B, awsk, bwsk, &a, &b, wsk,aCAR,aBAT,aCRU,aDES,bCAR,bBAT,bCRU,bDES) != NO_PROBLEMS)
                return 1;
        }
        else if (compare(command, "[playerA]")) {
            if (p % 2 == 1) {
                cout << "INVALID OPERATION " << char(34) << command << " " << char(34) << ": THE OTHER PLAYER EXPECTED";
                return 1;
            }
            p++;
            wsk = &a;
            test = playerX(wskA, wskB, board, wsk, awsk, bwsk, aCAR, aBAT, aCRU, aDES);
            if (test == PLAYER_WON) {
                cout << "A won";
                return 0;
            }
            else if (test != NO_PROBLEMS)
                return 1;
        }
        else if (compare(command, "[playerB]")) {
            if (p % 2 == 0) {
                cout << "INVALID OPERATION " << char(34) << command << " " << char(34) << ": THE OTHER PLAYER EXPECTED";
                return 1;
            }
            p++;
            wsk = &b;
            test = playerX(wskB, wskA, board, wsk, bwsk, awsk, bCAR, bBAT, bCRU, bDES);
            if (test == PLAYER_WON) {
                cout << "B won";
                return 0;
            }
            else if (test != NO_PROBLEMS)
                return 1;
        }
    }

    return 0;
}
