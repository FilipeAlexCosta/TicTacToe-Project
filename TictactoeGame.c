#include <stdio.h>

char board[3][3]; // Currently using a global variable, will change that later probably
const char PLAYER = 'X';
const char COMPUTER = 'O';
const char EMPTY_SPACE = '_';
const int SCORE_BOARD[3][3] = {{3, 2, 3}, {2, 4, 2}, {3, 2, 3}};

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
            break; // If one of the players wins, the game ends, otherwise it goes until the board is filled
        }
        computerMove();
        if (checkWin(PLAYER) == PLAYER || checkWin(COMPUTER) == COMPUTER)
        {
            break;
        }
        printBoard();
    } while(emptySquares() != 0);
    printf("\n");
    return 0;
}

void resetBoard()
{
    /* Returns a 3x3 matrix representative of a tictactoe board to its default state
    which is considered to be when it is fully filled with whatever char the constant
    EMPTY_SPACE represents (in this case its: "_") */
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            board[row][column] = EMPTY_SPACE;
        }
    }
}

void printBoard()
{
    /* Prints a 3x3 matrix that is used to represent the board of a
    tictactoe game */
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            printf("%c ", board[row][column]);
        }
        printf("\n");
    }
}

void playerMove()
{
    /* Takes the row and column chosen by the player and, if they are valid,
    sets the value of the board at that position to the value of the const PLAYER */
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
    /* Evaluates the score of the game board, subtracting points if
    a square is filled by the computer and adding if its filled by the player.
    Each square has its own value proportional to the number of threes in a row
    passing through it. */
    int score = 0;
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        { // The score_board defined at the start is used to quickly sum the values
            if (board[row][column] == PLAYER) score += SCORE_BOARD[row][column];
            if (board[row][column] == COMPUTER) score -= SCORE_BOARD[row][column];
        }
    }
    return score;
}

int moveSearch(char board[][3], int depth, char lastPlayer)
{
    /* Minimax algorithm in which the bot tries to lower its score as much
    as possible while the "player" tries to maximize it.*/
    int score, WinMultiplier;
    WinMultiplier = (lastPlayer == COMPUTER) ? 1 : -1;
    int scoreMinMax = -(WinMultiplier * 100); // Initializes the score in a way it will always be changed
                                              // to the first score calculated
    
    if (checkWin(lastPlayer) == lastPlayer) return -(WinMultiplier * 42);
    if (depth == 0) return evaluateBoard(board);

    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            if (board[row][column] != EMPTY_SPACE) continue;
            if (lastPlayer == PLAYER)
            { // If the Player last played, then it must be the computer's turn
                board[row][column] = COMPUTER;
                score = moveSearch(board, depth - 1, COMPUTER);
                if (scoreMinMax > score) scoreMinMax = score; // Finds the minimum score
                board[row][column] = EMPTY_SPACE;
            }
            else
            { // Opposite case
                board[row][column] = PLAYER;
                score = moveSearch(board, depth - 1, PLAYER);
                if (scoreMinMax < score) scoreMinMax = score; // Finds the maximum score
                board[row][column] = EMPTY_SPACE;
            }
        }
    }
    return scoreMinMax;
}

void computerMove()
{
    /* Finds out the best possible move the computer can play to either secure its
    win or at the very least a draw */
    int score, scoreMinMax = 100, play[2];
    int numEmptySquares = emptySquares();

    if (numEmptySquares == 8 || numEmptySquares == 9)
    { // Either plays on the center or the top left corner as a first move
      // Much faster this way and it doesn't really matter where it first plays
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
    { // Assigns a value to each possible play and takes the one
      // with the lowest score (see evaluateBoard score meaning)
        for (int column = 0; column < 3; column++)
        {
            if (board[row][column] != EMPTY_SPACE) continue;
            board[row][column] = COMPUTER;
            if (checkWin(COMPUTER) == COMPUTER)
            { // Serves as a way to correct for human unpredictability, so that it takes any
              // chance to win it gets
                printBoard();
                return;
            }
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
    /* Gets how many empty squares there are currently on the board.
    Very useful as a stopping condition or for finding out the depth for moveSearch */
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
    /* Checks if there are any three in a rows associated to the winner given by the
    parameter. It will either return the winner if he indeed wins or the const EMPTY_SPACE */
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