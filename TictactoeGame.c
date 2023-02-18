#include <stdio.h>
#include <string.h>

const char PLAYER = 'X';
const char COMPUTER = 'O';
const char EMPTY_SPACE = ' ';

short chooseDifficulty();
void resetBoard(char board[][3]);
void printRow(char board[][3], short);
void printBoard(char board[][3]);
void playerMove(char board[][3]);
short evaluateBoard(char board[][3]);
short moveSearch(char board[][3], short, char);
void computerMove(char board[][3], short);
short emptySquares(char board[][3]);
char checkWin(char board[][3], char);
short announceWinner(char board[][3]);

int main()
{
    char board[3][3];
    short difficulty = chooseDifficulty();
    resetBoard(board); // Makes sure to initialize the board with EMPTY_SPACE
    printBoard(board);
    do {
        playerMove(board);
        if (announceWinner(board) != 0) return 0; // checks if the player won
        if (emptySquares(board) == 0) break; // checks if full before letting the computer play
        computerMove(board, difficulty);
        if (announceWinner(board) != 0) return 0; // checks if the computer won
        printBoard(board);
    } while(emptySquares(board) != 0);
    printBoard(board);
    printf("\n///// It's a DRAW! /////\n");
    return 0;
}

short chooseDifficulty()
{
    /* Lets the user choose a difficulty which will decide
    how in-depth the computer will search for moves */
    char choice[6];
    short continueLoop = 1;
    do
    {
        printf("Which difficulty would you like (Easy, Medium, Hard)?\n");
        scanf("%s", choice);
        if (strcmp(choice, "Easy") == 0) return 3; // Each value will be used to
        if (strcmp(choice, "Medium") == 0) return 2; // lower the depth of moveSearch
        if (strcmp(choice, "Hard") == 0) return 1; // making the game easier
    } while (continueLoop == 1);
}

void resetBoard(char board[][3])
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

void printRow(char board[][3], short row)
{
    /* Prints a single row of the board of tictatoe, has the added
    functionality of printing a "breaker" if the row passed is -1 */
    if (row == -1)
    {
        printf("--|---|--");
        return;
    }
    for (short column = 0; column < 3; column++)
    {
        printf("%c", board[row][column]);
        if (column < 2) printf(" | ");
    }
    return;
}

void printBoard(char board[][3])
{
    /* Prints a 3x3 matrix that is used to represent the board of a
    tictactoe game */
    short row = 0;
    for (short i = 0; i < 5; i++)
    {
        (i % 2 == 0) ? (printRow(board, row), row++) : printRow(board, -1); // decides between a breaker and a row
        printf("\n");
    }
}

void playerMove(char board[][3])
{
    /* Takes the row and column chosen by the player and, if they are valid,
    sets the value of the board at that position to the value of the const PLAYER */
    short row, column, validChoice = 0;
    do
    { // Validates the choice (1 <= row <= 3, 1 <= column <= 3, and unfilled square)
        printf("\nRow: ");
        scanf("%hi", &row);
        printf("Column: ");
        scanf("%hi", &column);
        printf("\n");
        validChoice = 1;

        if (row < 1 || row > 3) validChoice = 0;
        if (column < 1 || column > 3) validChoice = 0;
        if (board[row - 1][column - 1] != EMPTY_SPACE)
        {
            printf("Square already filled. Choose another one.\n");
            validChoice = 0;
        }
    }   while(validChoice == 0);
    board[row - 1][column - 1] = PLAYER;
}

short evaluateBoard(char board[][3])
{
    /* Evaluates the score of the game board, subtracting points if
    a square is filled by the computer and adding if its filled by the player.
    Each square has its own value proportional to the number of threes in a row
    passing through it. */
    const int SCORE_BOARD[3][3] = {{3, 2, 3}, {2, 4, 2}, {3, 2, 3}};
    short score = 0;
    for (short row = 0; row < 3; row++)
    {
        for (short column = 0; column < 3; column++)
        { // The score_board defined at the start is used to quickly sum the values
            if (board[row][column] == PLAYER) score += SCORE_BOARD[row][column];
            if (board[row][column] == COMPUTER) score -= SCORE_BOARD[row][column];
        }
    }
    return score;
}

short moveSearch(char board[][3], short depth, char lastPlayer)
{
    /* Minimax algorithm in which the bot tries to lower its score as much
    as possible while the "player" tries to maximize it.*/
    short score, WinMultiplier;
    WinMultiplier = (lastPlayer == COMPUTER) ? 1 : -1;
    short scoreMinMax = -(WinMultiplier * 100); // Initializes the score in a way it will always be changed
                                              // to the first score calculated
    
    if (checkWin(board, lastPlayer) == lastPlayer) return -(WinMultiplier * 42);
    if (depth == 0) return evaluateBoard(board);

    for (short row = 0; row < 3; row++)
    {
        for (short column = 0; column < 3; column++)
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

void computerMove(char board[][3], short difficulty)
{
    /* Finds out the best possible move the computer can play to either secure its
    win or at the very least a draw */
    short score, scoreMinMax = 100, play[2];
    short numEmptySquares = emptySquares(board);

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

    numEmptySquares = (numEmptySquares >= difficulty) ? numEmptySquares / difficulty : numEmptySquares;
    /* Makes sure the depth wont be 0 as soon as there are fewer empty squares than the
    short representing the difficulty chosen at the start */
    for (short row = 0; row < 3; row++)
    { // Assigns a value to each possible play and takes the one
      // with the lowest score (see evaluateBoard score meaning)
        for (short column = 0; column < 3; column++)
        {
            if (board[row][column] != EMPTY_SPACE) continue;
            board[row][column] = COMPUTER;
            if (checkWin(board, COMPUTER) == COMPUTER)
            { // Serves as a way to correct for human unpredictability, so that it takes any
              // chance to win it gets
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

short emptySquares(char board[][3])
{
    /* Gets how many empty squares there are currently on the board.
    Very useful as a stopping condition or for finding out the depth for moveSearch */
    short emptySquares = 0;
    for (short row = 0; row < 3; row++)
    {
        for (short column = 0; column < 3; column++)
        {
            if (board[row][column] == EMPTY_SPACE)
            {
                emptySquares++;
            }
        }
    }
    return emptySquares;
}

char checkWin(char board[][3], char winner)
{
    /* Checks if there are any three in a rows associated to the winner given by the
    parameter. It will either return the winner if he indeed wins or the const EMPTY_SPACE */
    for (short row = 0; row < 3; row++)
    { // Checks for a win in every row and column "at the same time" so to speak
        short row_counter = 0;
        for (short column = 0; column < 3; column++)
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

short announceWinner(char board[][3])
{
    /* Checks if either the PLAYER or the COMPUTER won, if that is the case
    then announces and ends the game, otherwise it keeps going */
    if (checkWin(board, PLAYER) == PLAYER)
    {
        printBoard(board);
        printf("\n///// PLAYER WINS! /////\n");
        return 1;
    }
    if (checkWin(board, COMPUTER) == COMPUTER)
    {
        printBoard(board);
        printf("\n///// COMPUTER WINS! /////\n");
        return -1;
    }
    return 0;
}
