#include <iostream>
#include <set>
#include <vector>
#include <stdlib.h>

using namespace std;

struct coord {
    int x;
    int y;
};

class Cell {
    public:
        coord pos;
        int value;
        bool bomb;

        void setValue(int bombAmount) {
            value = bombAmount;
        }
};

class Board {
    

    private:
        vector<vector<Cell>> cells;
        int w;
        int h;
        int bombAmount;
        void generateBoard() {
            generateEmptyBoard();
            spreadBombs();
            fillValues();
        }

        void generateEmptyBoard() {
            
            // Generates the board, with an extra padding of cells around the border to assist in counting the bombs
            for (int i = 0; i<=h+1; i++) {
                vector<Cell> vect(w+2, Cell{});
                cells.push_back(vect);
            }
        }

        void spreadBombs() {
            // Currently just a naice implementation that generates random coordinates until n bombs were placed
            int placed = 0;
            while (placed != bombAmount) {
                int x = rand() % w;
                int y = rand() % h;

                Cell* cell = &cells[y+1][x+1];
                if (!cell->bomb) {
                    cell->bomb = true;
                    placed++;
                }
            }
        }

        void fillValues() {
            for (int i = 1; i<=h; i++) {
                for (int j = 1; j<=w; j++) {
                    int amt = 0;
                    for (int yOffset = -1; yOffset<2; yOffset++) {
                        for (int xOffset = -1; xOffset<2; xOffset++) {
                            if(!(xOffset == 0 && yOffset == 0)) {
                                amt += cells[i+yOffset][j+xOffset].bomb;
                            }
                        }
                    }
                    cells[i][j].setValue(amt);
                }
            }
        }

    public:
        Board(int width, int height, int amount) {
            w = width;
            h = height;
            bombAmount = amount;
            // TODO: Better seed generation that isn't tied to seconds
            srand(time(NULL));
            generateBoard();
        }

        void printBoard() {
            for (int y = 1; y<=w; y++) {
                for (int x = 1; x<=h; x++) {
                    Cell* cell = &cells[y][x];
                    if (!cell->bomb){
                        cout << cell->value << " | ";
                    } else {
                        cout << "⏺" << " | ";
                    }
                }
                cout << endl;
            }
        }
};

int main() {
    Board board{10,10, 20};
    board.printBoard();
}