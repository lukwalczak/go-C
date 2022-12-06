#include<stdio.h>
#include<stdlib.h>
#include"conio2.h"
#include<windows.h>

#define TEXTX 2
#define TEXTY 1
#define BOARDSTARTX 50
#define BOARDSTARTY 2

/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   with well defined identifiers */

typedef struct gameBoard {
    int x;
    int y;
    char value;
} gameBoard;

typedef struct player {
    float points;
    char playerStone;
} player;

typedef struct game {
    int turn;
    player player1;
    player player2;
} game;

gameBoard **createNewGameBoard(int boardSize);

void freeGameBoardMemory(gameBoard **, int boardSize);

void displayConsoleMovement(int x, int y, game game);

void displayGameBoard(int boardSize, gameBoard **gameBoard);

void initConsole();

void displayBoardSizeChoice(int textX, int textY);

void placeStone(gameBoard **gameBoard, game *game, int x, int y, int boardSize);

int calculateStoneLiberties(gameBoard **gameBoard, game *game, int x, int y, int boardSize);

int calculateDirectionalLiberty(gameBoard **gameBoard, int x, int y, int xOffset, int yOffset, char playerStone,
                                char oppositeStone,
                                int boardSize);

void recalculateEveryStoneLiberties(gameBoard **gameBoard, game *game, int boardSize);

void captureStone(gameBoard **gameBoard, game *game, int x, int y);

int chooseGameBoardSize();

int selectCustomBoardSize();

int gameInstance();

int main() {
    int k = 0;
    do {
        k = gameInstance();
    } while (k == 0);
}

int gameInstance() {
    int zn = 0, x = BOARDSTARTX, y = BOARDSTARTY, attr = 7, back = 0, zero = 0, consoleWidth = 120, consoleHeight = 30, boardSize = 13;
    char txt[32];
    gameBoard **mainGameBoard;
    player player1 = {0, 'X'}, player2 = {0, 'O'};
    game mainGame = {0, player1, player2};
    initConsole();
    boardSize = chooseGameBoardSize();
    mainGameBoard = createNewGameBoard(boardSize);
    clrscr();
    do {
        displayConsoleMovement(x, y, mainGame);
        if (zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
        else sprintf(txt, "key code: 0x%02x", zn);
        gotoxy(TEXTX, 12);
        cputs(txt);
        displayGameBoard(boardSize, mainGameBoard);
        gotoxy(x, y);
        textcolor(attr);
        textbackground(back);
        putch('*');
        zero = 0;
        zn = getch();
        if (zn == 0) {
            gotoxy(x, y);
            zero = 1;        // if this is the case then we read
            zn = getch();        // the next code knowing that this
            if (zn == 0x48) {
                if (y - 1 >= BOARDSTARTY)y--;
            } else if (zn == 0x50) {
                if (y <= BOARDSTARTY + boardSize)y++;
            } else if (zn == 0x4b) {
                if (x - 1 >= BOARDSTARTX)x--;
            } else if (zn == 0x4d) {
                if (x <= BOARDSTARTX + boardSize)x++;
            }
        } else if (zn == ' ') attr = (attr + 1) % 16;
        else if (zn == 0x0d) back = (back + 1) % 16;    // enter key is 0x0d or '\r'
        else if (zn == 0x69) {
            placeStone(mainGameBoard, &mainGame, x, y, boardSize);
            recalculateEveryStoneLiberties(mainGameBoard, &mainGame, boardSize);
        } else if (zn == 0x6e) {
            return 0;
        } else if (zn == 0x51) {
            freeGameBoardMemory(mainGameBoard, boardSize);
            return 1;
        }
    } while (zn != 'q');
    _setcursortype(_NORMALCURSOR);
}

void initConsole() {
    settitle("Lukasz, Walczak, 193530");
    _setcursortype(_NOCURSOR);
    Conio2_Init();
};

int chooseGameBoardSize() {
    int zn = 0, x = 9, y = 5, textX = 10, textY = 4;
    do {
        displayBoardSizeChoice(textX, textY);
        gotoxy(x, y);
        putch('*');
        zn = getch();
        if (zn == 0) {
            zn = getch();
            if (zn == 0x48) {
                if (y - 1 > textY)y--;
            } else if (zn == 0x50) {
                if (y + 1 < textY + 5)y++;
            }
        }
    } while (zn != 0x0d);
    switch (y) {
        case 5: //9x9
            return 9;
        case 6: //13x13
            return 13;
        case 7: // 19x19
            return 19;
        case 8: // custom
            return selectCustomBoardSize();
        default:
            return 13;
    };
}

void displayBoardSizeChoice(int textX, int textY) {
    clrscr();
    textbackground(BLACK);
    gotoxy(textX, textY);
    cputs("Choose board size");
    gotoxy(textX, textY + 1);
    cputs("9x9");
    gotoxy(textX, textY + 2);
    cputs("13x13");
    gotoxy(textX, textY + 3);
    cputs("19x19");
    gotoxy(textX, textY + 4);
    cputs("custom");
    gotoxy(textX, textY + 5);
}

void displayConsoleMovement(int x, int y, game game) {
    char txt[32];
    textbackground(BLACK);
    clrscr();
    textcolor(7);
    gotoxy(TEXTX, TEXTY); //rows and column are numbered starting with 1
    cputs("q       = exit");
    gotoxy(TEXTX, TEXTY + 1);
    cputs("cursors = moving");
    gotoxy(TEXTX, TEXTY + 2);
    cputs("space   = change color");
    gotoxy(TEXTX, TEXTY + 3);
    cputs("enter   = change background color");
    gotoxy(TEXTX, TEXTY + 4);
    cputs("i   = place new stone");
    gotoxy(TEXTX, TEXTY + 5);
    cputs("enter   = confirm move");
    gotoxy(TEXTX, TEXTY + 6);
    cputs("n   = new game");
    gotoxy(TEXTX, TEXTY + 7);
    cputs("s   = save game");
    gotoxy(TEXTX, TEXTY + 8);
    cputs("f   = end game");
    gotoxy(TEXTX, TEXTY + 9);
    cputs("current position");
    gotoxy(TEXTX, TEXTY + 10);
    sprintf(txt, "x:%d, y:%d", x - BOARDSTARTX - 1, y - BOARDSTARTY - 1);
    cputs(txt);
    sprintf(txt, "turn: %d", game.turn);
    gotoxy(TEXTX, TEXTY + 12);
    cputs(txt);
    sprintf(txt, "player1 points: %.2lf", game.player1.points);
    gotoxy(TEXTX, TEXTY + 14);
    cputs(txt);
    sprintf(txt, "player2 points: %.2lf", game.player2.points);
    gotoxy(TEXTX, TEXTY + 16);
    cputs(txt);
}

void displayGameBoard(int boardSize, gameBoard **gameBoard) {
    int i, j;
    for (i = 0; i < boardSize + 2; i++) {
        for (j = 0; j < boardSize + 2; j++) {
            if ((i == 0 & j == 0) | (i == boardSize + 1 & j == boardSize + 1)) {
                gotoxy(i + BOARDSTARTX, j + BOARDSTARTY);
                putch('/');
            } else if ((i == 0 & j == boardSize + 1) | (i == boardSize + 1 & j == 0)) {
                gotoxy(i + BOARDSTARTX, j + BOARDSTARTY);
                putch('\\');
            } else if (i == 0) {
                gotoxy(i + BOARDSTARTX, j + BOARDSTARTY);
                putch('|');
            } else if (i == boardSize + 1) {
                gotoxy(i + BOARDSTARTX, j + BOARDSTARTY);
                putch('|');
            } else if (j == boardSize + 1) {
                gotoxy(i + BOARDSTARTX, j + BOARDSTARTY);
                putch('-');
            } else if (j == 0) {
                gotoxy(i + BOARDSTARTX, j + BOARDSTARTY);
                putch('-');
            }
        }
    }
    for (i = 0; i < boardSize; i++) {
        for (j = 0; j < boardSize; j++) {
            gotoxy(BOARDSTARTX + i + 1, BOARDSTARTY + j + 1);
            putch(gameBoard[i][j].value);
        }
    }
};

gameBoard **createNewGameBoard(int boardSize) {
    gameBoard **newGameBoard = NULL;
    newGameBoard = malloc(sizeof(gameBoard *) * boardSize);
    for (int i = 0; i < boardSize; i++) {
        newGameBoard[i] = malloc(sizeof(gameBoard) * boardSize);
    }
    if (newGameBoard == NULL) {
        printf("nie udało sie\n");
        return 0;
    }
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            newGameBoard[i][j].value = '+';
            newGameBoard[i][j].x = i;
            newGameBoard[i][j].y = j;
        }
    }
    return newGameBoard;
};

void freeGameBoardMemory(gameBoard **gameBoardToFree, int boardSize) {
    for (int i = 0; i < boardSize; i++) {
        free(gameBoardToFree[i]);
    }
    free(gameBoardToFree);
};

void placeStone(gameBoard **gameBoard, game *game, int x, int y, int boardSize) {
    int normalizedX = x - BOARDSTARTX - 1, normalizedY = y - BOARDSTARTY - 1;
    char stoneToPlace;
    if (normalizedX >= 0 & normalizedX <= boardSize - 1 & normalizedY >= 0 & normalizedY <= boardSize - 1) {
        if (gameBoard[normalizedX][normalizedY].value == '+') {
            if (game->turn % 2 == 0)
                stoneToPlace = game->player1.playerStone;
            else
                stoneToPlace = game->player2.playerStone;

            if (calculateStoneLiberties(gameBoard, game, normalizedX, normalizedY, boardSize) != 0) {
                gameBoard[normalizedX][normalizedY].value = stoneToPlace;
                game->turn += 1;
            }
        }
    }
}

void recalculateEveryStoneLiberties(gameBoard **gameBoard, game *game, int boardSize) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (calculateStoneLiberties(gameBoard, game, i, j, boardSize) == 0) {
                captureStone(gameBoard, game, i, j);
            }
        }
    }
}

void captureStone(gameBoard **gameBoard, game *game, int x, int y) {
    char c = gameBoard[x][y].value, z;
    if (game->turn % 2 == 0)
        z = game->player2.playerStone;
    else
        z = game->player1.playerStone;
    if (c != '+' & c != z) {
        gameBoard[x][y].value = '+';
        if (game->turn % 2 == 0)
            game->player2.points += 1;
        else
            game->player1.points += 1;
    }
}

int calculateStoneLiberties(gameBoard **gameBoard, game *game, int x, int y, int boardSize) {
    int liberties = 0;
    char oppositeStone, playerStone;
    if (game->turn % 2 == 0) {
        oppositeStone = game->player2.playerStone;
        playerStone = game->player1.playerStone;
    } else {
        oppositeStone = game->player1.playerStone;
        playerStone = game->player2.playerStone;
    }
    liberties += calculateDirectionalLiberty(gameBoard, x, y, 1, 0, playerStone, oppositeStone, boardSize);
    liberties += calculateDirectionalLiberty(gameBoard, x, y, -1, 0, playerStone, oppositeStone, boardSize);
    liberties += calculateDirectionalLiberty(gameBoard, x, y, 0, 1, playerStone, oppositeStone, boardSize);
    liberties += calculateDirectionalLiberty(gameBoard, x, y, 0, -1, playerStone, oppositeStone, boardSize);
    return liberties;
}

int calculateDirectionalLiberty(gameBoard **gameBoard, int x, int y, int xOffset, int yOffset, char playerStone,
                                char oppositeStone,
                                int boardSize) {
    switch (xOffset) {
        case 1: {
            if ((x + xOffset) >= boardSize)
                return 0;
            break;
        }
        case (-1): {
            if ((x + xOffset) < 0)
                return 0;
            break;
        }
        default:
            break;
    }
    switch (yOffset) {
        case 1: {
            if (y - yOffset < 0)
                return 0;
            break;
        }
        case (-1): {
            if (y - yOffset >= boardSize)
                return 0;
            break;
        }
        default:
            break;
    }
    if (gameBoard[x + xOffset][y - yOffset].value != oppositeStone) {
        return 1;
    }
    return 0;
}

int selectCustomBoardSize() {
    int i = 0;
    char txt[32], zn;
    clrscr();
    gotoxy(TEXTX, TEXTY);
    cputs("Choose Custom Board Size:\n");
    gotoxy(TEXTX, TEXTY + 1);
    do {
        zn = getche();
        txt[i] = zn;
        i++;
    } while (zn != 0x0d);
    cputs(txt);
    return atoi(txt);
}