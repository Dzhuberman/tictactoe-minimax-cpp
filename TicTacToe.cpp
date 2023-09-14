#include <iostream>
#include <vector>
#include <cmath>

std::vector<char> board;
int boardSize = 9;

const int size3x3 = 9;
const int size4x4 = 16;
const int size5x5 = 25;

int globalDepth = 3;

const char x =     'x';
const char o =     'o';
const char empty = '_';

bool isX = true;

char currentPlayer = x;

bool isGameOver = false;

//Filling vector with empty chars
void fillVector(int length);

//Board work
void printBoard();
bool isBoardFull();

//Making a turn
bool isFree(int position);
void makeMove();
void changeTurn();
int getInput();

//Check for wins
bool checkWin(char player);

//minimax functions
int minimax(int depth, bool isMaximizingPlayer, int boardSize, char bot, char player);
int getBestMove(char bot, char player);

//Gamemodes
//PVP - Player Versus Player
void PVP() {
    printf("You have chosen PVP match!!\n");
    printBoard();
    currentPlayer = x;

    while (!isGameOver) {
        printf("%c: your turn\n", currentPlayer);
        makeMove();

        if (checkWin(currentPlayer)) {
            printf("The winner is: %c\n", currentPlayer);
            isGameOver = true;
        }

        currentPlayer = isX ? x : o;
    }
}

//PVE - Player Versus Enviroment
void PVE() {
    printf("You have chosen PVE match!!\n");
    printBoard();
    currentPlayer = x;

    while (!isGameOver) {
        printf("%c: your turn\n", currentPlayer);
        if (isX) {
            makeMove();
        }
        else {
            int bestMove = getBestMove(o, x);
            board[bestMove] = o;
            printBoard();
            changeTurn();
        }

        if (checkWin(currentPlayer)) {
            printf("The winner is: %c\n", currentPlayer);
            isGameOver = true;
        }
        else if (isBoardFull()) {
            printf("Draw!\n");
            isGameOver = true;
        }

        currentPlayer = isX ? x : o;
    }
}

//EVE - Enviroment Versus Enviroment
void EVE() {
    printf("You have chosen EVE match?!\n");
    printBoard();
    currentPlayer = x;

    while (!isGameOver) {
        printf("%c: your turn\n", currentPlayer);
        if (isX) {
            int bestMove = getBestMove(x, o);
            board[bestMove] = x;
            printBoard();
            changeTurn();
            system("pause");
        }
        else {
            int bestMove = getBestMove(o, x);
            board[bestMove] = o;
            printBoard();
            changeTurn();
            system("pause");
        }

        if (checkWin(currentPlayer)) {
            printf("The winner is: %c\n", currentPlayer);
            isGameOver = true;
        }
        else if (isBoardFull()) {
            printf("Draw!\n");
            isGameOver = true;
        }

        currentPlayer = isX ? x : o;
    }
}

int main() {
    printf("Choose board size!\n");
    printf("Press 1: 3 by 3\n");
    printf("Press 2: 4 by 4\n");
    printf("Press 3: 5 by 5\n");

    int input;
    std::cin >> input;

    system("cls");

    switch (input)
    {
    case 1:
        boardSize = size3x3;
        globalDepth = 5;
        break;
    case 2:
        boardSize = size4x4;
        globalDepth = 4;
        break;
    case 3:
        boardSize = size5x5;
        globalDepth = 3;
        break;
    default:
        printf("Invalid Input. You'll be playing default 3 by 3.\n");
        boardSize = size3x3;
        globalDepth = 5;
        break;
    }

    printf("Choose game type!\n");
    printf("Press 1: PVP\n");
    printf("Press 2: PVE\n");
    printf("Press 3: EVE\n");

    fillVector(boardSize);

    std::cin >> input;

    system("cls");

    switch (input)
    {
    case 1:
        PVP();
        break;
    case 2:
        PVE();
        break;
    case 3:
        EVE();
        break;
    default:
        printf("Invalid Input. You'll be playing default PVE.\n");
        PVE();
        break;
    }

    printf("GameOver.");
    system("pause");
}

void fillVector(int length) {
    for (int i = 0; i < length; i++) {
        board.push_back(empty);
    }
}

void printBoard() {
    int length = std::sqrt(board.size());
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            printf(" %c ", board[i * length + j]);
        }
        printf("\n");
    }
}

bool isFree(int position) {
    if (board[position] == empty) {
        return true;
    }
    else {
        return false;
    }
}

bool isBoardFull() {
    for (char cell : board) {
        if (cell == empty)
            return false;
    }
    return true;
}

void makeMove() {
    int position = getInput();
    if (isFree(position)) {
        board[position] = isX ? x : o;
        printBoard();
        changeTurn();
    }
    else {
        printf("This space is taken\n");
        makeMove();
    }
}

void changeTurn() {
    isX = !isX;
}

int getInput() {
    printf("Please, input number from 1 to %d\n", boardSize);
    int input;
    std::cin >> input;
    return input - 1;
}

bool checkWin(char player) {
    //Horizontal
    int length = std::sqrt(board.size());

    for (int i = 0; i < length; i++) {
        bool isWin = true;
        for (int j = 0; j < length; j++) {
            if (board[i * length + j] != player) {
                isWin = false;
                break;
            }
        }
        if (isWin) return true;
    }

    //Vertical
    for (int i = 0; i < length; i++) {
        bool isWin = true;
        for (int j = 0; j < length; j++) {
            if (board[j * length + i] != player) {
                isWin = false;
                break;
            }
        }
        if (isWin) return true;
    }

    //Diagonal
    //Form left to right
    bool isWin = true;
    for (int i = 0; i < length; i++) {
        if (board[i * length + i] != player) {
            isWin = false;
            break;
        }
    }
    if (isWin) return true;

    //From right to left
    isWin = true;
    for (int i = 0; i < length; i++) {
        if (board[i * length + length - 1 - i] != player) {
            isWin = false;
            break;
        }
    }
    if (isWin) return true;

    return false;
}

int minimax(int depth, bool isMaximizingPlayer, int boardSize, char bot, char player) {
    if (checkWin(bot)) {
        return 1;
    }
    else if (checkWin(player)) {
        return -1;
    }
    else if (isBoardFull()) {
        return 0;
    }

    //Controlling depth of the algorithm
    if (depth > globalDepth) return 0;

    if (isMaximizingPlayer) {
        int bestScore = INT_MIN;

        for (int i = 0; i < board.size(); i++) {
            if (board[i] == empty) {
                board[i] = bot;
                int currentScore = minimax(depth + 1, false, boardSize, bot, player);
                bestScore = std::max(bestScore, currentScore);
                board[i] = empty;
            }
        }

        return bestScore;
    }
    else {
        int bestScore = INT_MAX;

        for (int i = 0; i < board.size(); i++) {
            if (board[i] == empty) {
                board[i] = player;
                int currentScore = minimax(depth + 1, true, boardSize, bot, player);
                bestScore = std::min(bestScore, currentScore);
                board[i] = empty;
            }
        }

        return bestScore;
    }
}

int getBestMove(char bot, char player) {
    int bestScore = INT_MIN;
    int bestMove;
    bestMove = -1;

    for (int i = 0; i < board.size(); i++) {
        if (board[i] == empty) {
            board[i] = bot;
            int currentScore = minimax(0, false, boardSize, bot, player);
            board[i] = empty;
            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestMove = i;
            }
        }
    }

    return bestMove;
}