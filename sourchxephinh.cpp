#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

const int SCREEN_WIDTH = 12;
const int SCREEN_HEIGHT = 22;
const int BLOCK_SIZE = 4;

int screen[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};

class Block {
public:
    int shape[BLOCK_SIZE][BLOCK_SIZE];
    int color;
    int x, y;

    void draw() {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (shape[i][j] == 1) {
                    COORD position = {SHORT(x * 2 + j * 2), SHORT(y + i)};
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
                    cout << "[]";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset color
                }
            }
        }
    }

    void clear() {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (shape[i][j] == 1) {
                    COORD position = {SHORT(x * 2 + j * 2), SHORT(y + i)};
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
                    cout << "  ";
                }
            }
        }
    }

    void rotate() {
        Block temp = *this;

        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                shape[i][j] = temp.shape[BLOCK_SIZE - j - 1][i];
            }
        }
    }
};

Block currentBlock;

void mergeBlock() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (currentBlock.shape[i][j] == 1) {
                screen[currentBlock.y + i][currentBlock.x / 2 + j] = currentBlock.color;
            }
        }
    }
}

void generateNewBlock() {
    currentBlock.x = SCREEN_WIDTH / 2 - BLOCK_SIZE / 2 * 2;
    currentBlock.y = 0;
    currentBlock.color = rand() % 7 + 1; // Random color
    int shapeIndex = rand() % 4; // Random shape
    int shapes[4][BLOCK_SIZE][BLOCK_SIZE] = {
        {{1, 1, 1, 1},
         {0, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},

        {{1, 1, 1, 0},
         {1, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},

        {{1, 1, 0, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},

        {{1, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}}
    };

    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            currentBlock.shape[i][j] = shapes[shapeIndex][i][j];
        }
    }
}

void drawScreen() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH * 2; j += 2) {
            COORD position = {SHORT(j), SHORT(i)};
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
            int color = screen[i][j / 2];
            switch (color) {
                case 0:
                    cout << "  ";
                    break;
                case 1:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                    cout << "[]";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    break;
                // Add more color cases if needed
            }
        }
    }
}

int checkCollision() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            if (currentBlock.shape[i][j] == 1) {
                if (screen[currentBlock.y + i][currentBlock.x / 2 + j] != 0) {
                    return 1; // Collision detected
                }
            }
        }
    }
    return 0; // No collision
}

int main() {
    srand((unsigned int)time(NULL));

    while (1) {
        system("cls");
        int keyPressed = 0;
        if (_kbhit()) {
            keyPressed = _getch();
        }

        currentBlock.clear();

        switch (keyPressed) {
            case 'a':
                if (currentBlock.x > 0) {
                    currentBlock.x -= 2;
                }
                break;
            case 'd':
                if (currentBlock.x < SCREEN_WIDTH - BLOCK_SIZE * 2) {
                    currentBlock.x += 2;
                }
                break;
            case 's':
                if (!checkCollision()) {
                    currentBlock.y += 1;
                }
                break;
            case 'w':
                currentBlock.rotate();
                if (checkCollision()) {
                    // Revert rotation if it causes collision
                    for (int i = 0; i < 3; i++) {
                        currentBlock.rotate();
                    }
                }
                break;
            case 'q':
                return 0;
        }

        if (!checkCollision()) {
            currentBlock.y += 1;
        } else {
            mergeBlock();
            generateNewBlock();
        }

        drawScreen();
        currentBlock.draw();
        Sleep(200); // Adjust speed
    }

    return 0;
}
