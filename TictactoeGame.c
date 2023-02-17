#include <stdio.h>

char board[3][3]; // Currently using a global variable, will change that later probably
const char PLAYER = 'X';
const char COMPUTER = 'O';
const char EMPTY_SPACE = '_';

void resetBoard();
void printBoard();
void playerMove();
int evaluateBoard(char board[][3]);
int moveSearch(char board[][3], int, char);
void computerMove();
int emptySquares();
char checkWin(char);

int main()
{
    resetBoard(); // Makes sure to initialize the board with EMPTY_SPACE
    printBoard();
    do {
        playerMove();
        if (checkWin(PLAYER) == PLAYER || checkWin(COMPUTER) == COMPUTER)
        {
            printf("\n///// COMPUTER WINS! /////");
            break; // If one of the players wins, the game ends, otherwise it goes until the board is filled
        }
        computerMove();
        if (checkWin(PLAYER) == PLAYER || checkWin(COMPUTER) == COMPUTER)
        {
            printf("\n///// PLAYER WINS! /////");
            break; // If one of the players wins, the game ends, otherwise it goes until the board is filled
        }
        printBoard();
    } while(emptySquares() != 0);
    printf("\n");
    return 0;
}

void resetBoard() {
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            board[row][column] = EMPTY_SPACE;
        }
    }
}

void printBoard() {
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            printf("%c ", board[row][column]);
        }
        printf("\n");
    }
}

void playerMove() {
    int row = 0;
    int column = 0;
    int validChoice = 0;
    do
    { // Validates the choice (1 <= row <= 3, 1 <= column <= 3, and unfilled square)
        printf("\nRow: ");
        scanf("%d", &row);
        printf("\nColumn: ");
        scanf("%d", &column);
        printf("\n");
        validChoice = 1;

        if (row < 1 || row > 3)
        {
            validChoice = 0;
        }
        if (column < 1 || column > 3)
        {
            validChoice = 0;
        }
        if (board[row - 1][column - 1] != '_')
        {
            printf("Square already filled. Choose another one.\n");
            validChoice = 0;
        }
    }   while(validChoice == 0);
    board[row - 1][column - 1] = PLAYER;
}

int evaluateBoard(char board[][3])
{
    // I still have to find a better way to sum up the values
    // However, this works for now, and it works decently quick.
    int score = 0;
    if (board[0][0] == COMPUTER) score -= 3;
    if (board[0][1] == COMPUTER) score -= 2;
    if (board[0][2] == COMPUTER) score -= 3;
    if (board[1][0] == COMPUTER) score -= 2;
    if (board[1][1] == COMPUTER) score -= 4;
    if (board[1][2] == COMPUTER) score -= 2;
    if (board[2][0] == COMPUTER) score -= 3;
    if (board[2][1] == COMPUTER) score -= 2;
    if (board[2][2] == COMPUTER) score -= 3;
    if (board[0][0] == PLAYER) score += 3;
    if (board[0][1] == PLAYER) score += 2;
    if (board[0][2] == PLAYER) score += 3;
    if (board[1][0] == PLAYER) score += 2;
    if (board[1][1] == PLAYER) score += 4;
    if (board[1][2] == PLAYER) score += 2;
    if (board[2][0] == PLAYER) score += 3;
    if (board[2][1] == PLAYER) score += 2;
    if (board[2][2] == PLAYER) score += 3;
    return score;
}

int moveSearch(char board[][3], int depth, char lastPlayer)
{
    // Works 90% of the time. There is a bug regarding some circumstances
    // such as when the player is close to getting a three in a row, sometimes
    // the bot can win but chooses instead to stop the said three in a row.

    // Function currently has no pruning whatsoever, still a WIP
    int score, WinMultiplier;
    WinMultiplier = (lastPlayer == COMPUTER) ? -1 : 1;
    int scoreMinMax = WinMultiplier * 100;

    if (checkWin(lastPlayer) == lastPlayer) return (WinMultiplier * 42);
    if (depth == 0) return evaluateBoard(board);

    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            if (board[row][column] != EMPTY_SPACE) continue;
            if (lastPlayer == PLAYER)
            {
                board[row][column] = COMPUTER;
                score = moveSearch(board, depth - 1, COMPUTER);
                if (scoreMinMax > score) scoreMinMax = score;
                board[row][column] = EMPTY_SPACE;
            }
            else
            {
                board[row][column] = PLAYER;
                score = moveSearch(board, depth - 1, PLAYER);
                if (scoreMinMax < score) scoreMinMax = score;
                board[row][column] = EMPTY_SPACE;
            }
        }
    }
    return scoreMinMax;
}

void computerMove()
{
    int score, scoreMinMax = 100, play[2];
    int numEmptySquares = emptySquares();

    if (numEmptySquares == 8 || numEmptySquares == 9)
    { // Either plays on the center or the top left corner as a first move
        if (board[1][1] == EMPTY_SPACE)
        {
            board[1][1] = COMPUTER;
        }
        else
        {
            board[0][0] = COMPUTER;
        }
        return;
    }

    for (int row = 0; row < 3; row++)
    { // Assigns a value to each play using a minimax algorithm set up in the previous function
        for (int column = 0; column < 3; column++)
        {
            if (board[row][column] != EMPTY_SPACE) continue;
            board[row][column] = COMPUTER;
            score = moveSearch(board, numEmptySquares - 1, COMPUTER);
            if (scoreMinMax > score) scoreMinMax = score, play[0] = row, play[1] = column;
            board[row][column] = EMPTY_SPACE;
        }
    }
    board[play[0]][play[1]] = COMPUTER;
    return;
}

int emptySquares()
{
    int emptySquares = 0;
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            if (board[row][column] == EMPTY_SPACE)
            {
                emptySquares++;
            }
        }
    }
    return emptySquares;
}

char checkWin(char winner)
{
    for (int row = 0; row < 3; row++)
    { // Checks for a win in every row and column "at the same time" so to speak
        int row_counter = 0;
        for (int column = 0; column < 3; column++)
        {
            if (board[row][column] != winner)
            {
                break;
            }
            row_counter++;
        }
        if (board[0][row] == winner && board[1][row] == winner && board[2][row] == winner)
        {
            return winner;
        }
        if (row_counter == 3)
        {
            return winner;
        }
    }
    // Checks for a win in each diagonal
    if (board[0][0] == winner && board[1][1] == winner && board[2][2] == winner)
    {
        return winner;
    }
    if (board[0][2] == winner && board[1][1] == winner && board[2][0] == winner)
    {
        return winner;
    }

    return EMPTY_SPACE;
}