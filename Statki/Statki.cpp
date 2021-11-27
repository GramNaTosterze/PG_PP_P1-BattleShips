#include <iostream>

#define MAX_SHIPS 10
#define X 21
#define Y 10
#define SHIP_SIZE 5

using namespace std;

enum kierunek {
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

struct coords {
    int x;
    int y;
};

struct ship {
    char type[3];
    int ship_spaces;
    int ship_elements[SHIP_SIZE];
    coords position[4];
    bool trafiony = false;
};

struct player {
    ship ships[MAX_SHIPS];
    int five = 1;
    int four = 2;
    int three = 3;
    int two = 4;
};

void wship(ship ships, char type[3]) {
    for(int i=0; i<3; i++)
        ships.type[i] = type[i];
    ships.ship_spaces = SHIP_SIZE;
    for (int i = 0; i < ships.ship_spaces; i++) {
        ships.ship_elements[i] = '+';
    }

}

void place(char board[X][Y], ship ships, int x, int y) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; i++) {

        }
    }
}

void printState(char board[X][Y]) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            cout << board[i][j];
        }
        cout << '\n';
    }
}

int main() {
    int i = 0, x,y, a=0, b=0;
    char board[X][Y];
    player A, B;
    char command;

    cin >> command;
    while (true) {
        switch (command) {
            case 'P': {
                printState(board);
                break;
            }
            case 'a': {
                if (i % 2) {
                    cout << "PLAYER A\n";
                    cin >> A.ships[a].type;
                    cin >> A.ships[a].position[1].x >> A.ships[a].position[1].y;

                    a++;
                }
                else {
                    cout << "PLAYER B\n";
                    cin >> B.ships[a].type;
                    cin >> B.ships[a].position[1].x >> A.ships[a].position[1].y;
                }
                cin >> x >> y;

                i++;
                break;
            }
            case 's': {


                break;
            }
            default: {

                break;
            }
        }
    }

    return 0;
}