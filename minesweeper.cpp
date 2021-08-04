#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std;

struct coord {
    int x;
    int y;
};

class Cell {
    public:
        coord pos;
        int value = 0;
        bool bomb;
        bool visible = false;
        bool flag = false;
        bool onBoard = false;

        void setValue(int bombAmount) {
            value = bombAmount;
        }

        void setVisible(bool visibility) {
            visible = visibility;
        }

        void toggleFlag() {
            flag = !flag;
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
                    cells[i][j].onBoard = true;
                }
            }
        }

    public:
        Board(int width, int height, int amount) {
            w = width;
            h = height;
            if ((w * h) < amount) {
                bombAmount = w*h;
            } else {
                bombAmount = amount; 
            }
            // TODO: Better way to generate the seed that is not seconds
            srand(time(NULL));
            generateBoard();
        }

        void printBoard() {
            for (int y = 0; y<=w; y++) {
                if (y != 0) {
                    cout << setw(3) << y << " ";
                    cout << "| ";
                } else {
                    cout << "     ";
                }

                for (int x = 1; x<=h; x++) {
                    if ( y == 0) {
                        cout << setw(3);
                        cout << std::left;
                        cout << x << " ";
                    } else {
                        Cell* cell = &cells[y][x];
                        if (cell -> flag) {
                            cout << "🚩" << "| ";
                        }
                        else if (!cell->visible) {
                            cout << "█" << " | ";
                        }
                        else if (!cell->bomb){
                            cout << cell->value << " | ";
                        } else {
                            cout << "💣" << "| ";
                        }
                    }
                }
                cout << endl;
            }
        }

        int getWidth() {
            return w;
        }
        int getHeight() {
            return h;
        }

        Cell* getCell(int x, int y) {
            return &cells[y][x];
        }

        void revealCell(int x, int y) {
            if (x > 0 
            && x <= w 
            && y > 0 
            && y <= h 
            && getCell(y,x)->onBoard ) {
                getCell(y,x)->setVisible(true);
                // Recursively flood fill the visibility
                // if the cell has no neighbouring bomb
                // FIXME: Sometimes broken, but good enough
                
                if (getCell(y,x) -> value == 0){
                    for (int yOffset = -1; yOffset<2; yOffset++) {
                        for (int xOffset = -1; xOffset<2; xOffset++) {
                            if(!(xOffset == 0 && yOffset == 0)) {
                                Cell* c = getCell(y+yOffset, x+xOffset);
                                if (c->onBoard) {
                                    if (!(c -> visible)) {
                                        revealCell(x+xOffset, y+yOffset);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        void revealBoard() {
            for (int y = 0; y<=w; y++) {
                if (y != 0) {
                    cout << setw(3) << y << " ";
                    cout << "| ";
                } else {
                    cout << "     ";
                }

                for (int x = 1; x<=h; x++) {
                    if ( y == 0) {
                        cout << setw(3);
                        cout << std::left;
                        cout << x << " ";
                    } else {
                        Cell* cell = &cells[y][x];
                        if (!cell->bomb){
                            cout << cell->value << " | ";
                        } else {
                            cout << "💣" << "| ";
                        }
                    }
                }
                cout << endl;
            }
        }

        int countVisible() {
            int amt = 0;
            for (int x = 1; x<=h;x++) {
                for (int y = 1; y<=w;y++) {
                    amt += getCell(x,y)->visible;
                }
            }
            return amt;
        }

        bool isDone() {
            return (w*h)-bombAmount == countVisible();
        }
};

Board initGame() {
    int width = 0;
    int height = 0;
    int bombs = 0;
    cout << "Welcome to Ritz's totally amazing Minesweeper that I definitely always wanted to implement as a CLI program!" << endl;
    cout << "How wide do you want your board to be?" << endl;
    do{
        if (!(cin >> width)) {
            cout << "I sadly don't see a fitting number. Please try again!" << endl;
        }
    } while (width == 0);

    cout << "How high do you want your board to be?" << endl;
    do{
        if (!(cin >> height)) {
            cout << "I sadly don't see a fitting number. Please try again!" << endl;
        }
    } while (height == 0);

    cout << "How many bombs would you like on your board?" << endl;
    do{
        if (!(cin >> bombs)) {
            cout << "I sadly don't see a fitting number. Please try again!" << endl;
        }
    } while (height == 0);
    return Board{width, height, bombs};
}

void gameLoop(Board board) {
    while (true) {
        int row = 0;
        int column = 0;
        bool flag = false;

        cout << "What row do you want to interact with?" << endl;
        do {
            if (!(cin >> row) || row > board.getHeight() || row < 1) {
                cout << "That isn't a valid row. Please try again!" << endl;
            }
        } while (row == 0);

        cout << "What column do you want to interact with?" << endl;
        do {
            if (!(cin >> column) || column > board.getWidth() || column < 1) {
                cout << "That isn't a valid column. Please try again!" << endl;
                column = 0;
            }
        } while (column == 0);

        cout << "Would you like to flag (f) this spot or to reveal (r) it?" << endl;

        string entry = "";
        do {
            if (!(cin >> entry) || (entry != "f" && entry != "r") ) {
                cout << "That isn't a valid action. Please try again!" << endl;
                entry = "";
            }
        } while (entry == "");

        flag = entry == "f";

        cout << board.getCell(column,row)->onBoard << endl;
        if (flag) {
            board.getCell(column, row)->toggleFlag();
        } else {
            if (!(board.getCell(column,row)->bomb)){
                cout << "AAA" << endl;
                board.revealCell(row,column);
            } else {
                cout << "BOOM! You hit a bomb!" << endl;
                board.revealBoard();
                break;
            }
        }

        if (board.isDone()) {
            cout << "Congratulations! You finished the board!" << endl;
            board.revealBoard();
            break;
        } else {
            board.printBoard();
        }
    }
    
}

int main() {
    Board board = initGame();
    board.printBoard();
    gameLoop(board);
}