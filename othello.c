#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void printBoard(char board[][26], int n);

bool positionInBounds(int n, int row, int col);

bool checkLegalInDirection(char board[26][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol);

void flipPosition(char board[26][26], int row, int col, char colour,
                  int deltaRow, int deltaCol);

bool endGame(char board[][26], int n, bool humanMoveCheck);

bool checkAvailableMoves(char board[][26], int n, char turn);

int numberOfFlips(char board[][26], int n, int row, int col, char turn,
                  int deltaRow, int deltaCol);

void makeMoveComputer(char board[26][26], int n, char turn);

int main(void) {
  int n = 0;
  char computerMove;

  printf("Enter the board dimension: ");
  scanf("%d", &n);
  char board[26][26];
  for (int row = 0; row < 26; row++) {
    for (int col = 0; col < 26; col++) {
      board[row][col] = 'U';
    }
  }
  board[(n / 2) - 1][(n / 2) - 1] = 'W';
  board[(n / 2)][(n / 2)] = 'W';
  board[(n / 2)][(n / 2) - 1] = 'B';
  board[(n / 2) - 1][(n / 2)] = 'B';

  printf("Computer plays (B/W): ");
  scanf(" %c", &computerMove);
  printBoard(board, n);

  char humanMove = (computerMove == 'B') ? 'W' : 'B';
  char currentTurn = 'B';  // Black always moves first
  bool gameEnded = false;

  while (!gameEnded) {
    if (currentTurn == computerMove) {
      // Computer's turn
      if (checkAvailableMoves(board, n, computerMove)) {
        makeMoveComputer(board, n, computerMove);
      } else {
        printf("%c player has no valid move.\n", computerMove);
      }
    } else {
      // Human's turn
      if (checkAvailableMoves(board, n, humanMove)) {
        char rowC, colC;
        int rowHuman, colHuman;
        bool isValid = false;

        do {
          printf("Enter move for colour %c (RowCol): ", humanMove);
          scanf(" %c%c", &rowC, &colC);
          rowHuman = rowC - 97;
          colHuman = colC - 97;

          if (positionInBounds(n, rowHuman, colHuman) &&
              board[rowHuman][colHuman] == 'U') {
            for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
              for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                if (checkLegalInDirection(board, n, rowHuman, colHuman, humanMove,
                                          deltaRow, deltaCol)) {
                  isValid = true;
                  flipPosition(board, rowHuman, colHuman, humanMove, deltaRow, deltaCol);
                }
              }
            }
          }

          if (!isValid) {
            printf("Invalid move.\n");
          }
        } while (!isValid);

        board[rowHuman][colHuman] = humanMove;
        printBoard(board, n);
      } else {
        printf("%c player has no valid move.\n", humanMove);
      }
    }

    // Check if the game has ended
    if (!checkAvailableMoves(board, n, 'B') && !checkAvailableMoves(board, n, 'W')) {
      gameEnded = true;
    } else {
      // Switch turns
      currentTurn = (currentTurn == 'B') ? 'W' : 'B';
    }
  }

  // Game has ended, calculate scores
  int scoreComputer = 0;
  int scoreHuman = 0;
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < n; c++) {
      if (board[r][c] == computerMove) {
        scoreComputer++;
      } else if (board[r][c] == humanMove) {
        scoreHuman++;
      }
    }
  }

  // Print final board and results
  printBoard(board, n);
  if (scoreComputer > scoreHuman)
    printf("%c player wins.\n", computerMove);
  else if (scoreHuman > scoreComputer)
    printf("%c player wins.\n", humanMove);
  else
    printf("Draw.\n");

  return 0;
}

void printBoard(char board[][26], int n) {
  char letterRow = 'a';
  char letterCol = 'a';

  for (int c = -2; c < n; c++) {
    if (c == -2 || c == -1) {
      printf(" ");
    } else {
      printf("%c", letterRow);
      letterRow++;
    }
  }
  printf("\n");
  for (int row = 0; row < n; row++) {
    for (int col = -2; col < n; col++) {
      if (col == -2) {
        printf("%c", letterCol);
        letterCol++;
      } else if (col == -1) {
        printf(" ");
      } else if (col >= 0) {
        printf("%c", board[row][col]);
      }
    }
    printf("\n");
  }
}

bool positionInBounds(int n, int row, int col) {
  if (row > n - 1 || col > n - 1 || row < 0 || col < 0)
    return false;
  else
    return true;
}

bool checkLegalInDirection(char board[26][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  if (board[row][col] != 'U') {
    return false;
  }
  if (deltaRow == 0 && deltaCol == 0) {
    return false;
  }
  if (positionInBounds(n, row + deltaRow, col + deltaCol) == false) {
    return false;
  }
  if (board[row + deltaRow][col + deltaCol] == colour) {
    return false;
  }
  if (board[row + deltaRow][col + deltaCol] == 'U') {
    return false;
  }

  if (!positionInBounds(n, row, col)) {
    return false;
  }
  if (board[row][col] == 'W' || board[row][col] == 'B') {
    return false;
  }
  while (positionInBounds(n, row + deltaRow, col + deltaCol) &&
         board[row + deltaRow][col + deltaCol] != colour) {
    if (deltaRow < 0) {
      deltaRow--;
    } else if (deltaRow > 0)
      deltaRow++;
    else
      deltaRow = 0;

    if (deltaCol < 0) {
      deltaCol--;
    } else if (deltaCol > 0) {
      deltaCol++;
    } else
      deltaCol = 0;

    if (!positionInBounds(n, row + deltaRow, col + deltaCol) ||
        board[row + deltaRow][col + deltaCol] == 'U') {
      return false;
    }

    if (board[row + deltaRow][col + deltaCol] == colour) {
      return true;
    }
  }
  return false;
}

void flipPosition(char board[26][26], int row, int col, char colour,
                  int deltaRow, int deltaCol) {
  while (board[row + deltaRow][col + deltaCol] != colour) {
    board[row + deltaRow][col + deltaCol] = colour;
    if (deltaRow < 0) {
      deltaRow--;
    } else if (deltaRow > 0)
      deltaRow++;
    else
      deltaRow = 0;

    if (deltaCol < 0) {
      deltaCol--;
    } else if (deltaCol > 0) {
      deltaCol++;
    } else
      deltaCol = 0;
  }
}

int numberOfFlips(char board[][26], int n, int row, int col, char turn,
                  int deltaRow, int deltaCol) {
  int maxFlip = 0;
  int i = 1;
  while ((positionInBounds(n, row + (deltaRow * i), col + (deltaCol * i))) &&
         board[row + (deltaRow * i)][col + (deltaCol * i)] != turn) {
    maxFlip++;
    i++;
  }
  return maxFlip;
}

void makeMoveComputer(char board[26][26], int n, char turn) {
  if (checkAvailableMoves(board, n, turn)) {
    int maxFlip = 0;
    char maxRow = '\0';
    char maxCol = '\0';

    for (int r = 0; r < n; r++) {  // Order of printing moves is not right
      for (int c = 0; c < n; c++) {
        int currentFlip = 0;
        for (int dr = -1; dr <= 1; dr++) {
          for (int dc = -1; dc <= 1; dc++) {
            if (checkLegalInDirection(board, n, r, c, turn, dr, dc)) {
              currentFlip += numberOfFlips(board, n, r, c, turn, dr, dc);
            }
          }
        }
        if (currentFlip > maxFlip) {
          maxFlip = currentFlip;
          maxRow = (char)(r + 97);
          maxCol = (char)(c + 97);
          break;
        }
      }
    }
    printf("Computer places %c at %c%c.\n", turn, maxRow, maxCol);
    for (int dr = -1; dr <= 1; dr++) {
      for (int dc = -1; dc <= 1; dc++) {
        if (checkLegalInDirection(board, n, maxRow - 97, maxCol - 97, turn, dr,
                                  dc)) {
          flipPosition(board, maxRow - 97, maxCol - 97, turn, dr, dc);
        }
      }
    }
    board[maxRow - 97][maxCol - 97] = turn;
    printBoard(board, n);
  } else {
    printf("%c player has no valid move.\n", turn);
    return;
  }
}

bool endGame(char board[][26], int n, bool humanMoveCheck) {
  if (!humanMoveCheck) return true;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        return false;
      }
    }
  }
  return true;
}

bool checkAvailableMoves(char board[][26], int n, char turn) {
  bool moveAvailable = false;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (checkLegalInDirection(board, n, i, j, turn, 1, 0)) {
        return true;
      } else if (checkLegalInDirection(board, n, i, j, turn, -1, 0)) {
        return true;
      } else if (checkLegalInDirection(board, n, i, j, turn, 0, 1)) {
        return true;
      } else if (checkLegalInDirection(board, n, i, j, turn, 0, -1)) {
        return true;
      } else if (checkLegalInDirection(board, n, i, j, turn, -1, -1)) {
        return true;
      } else if (checkLegalInDirection(board, n, i, j, turn, 1, 1)) {
        return true;
      } else if (checkLegalInDirection(board, n, i, j, turn, 1, -1)) {
        return true;
      } else if (checkLegalInDirection(board, n, i, j, turn, -1, 1)) {
        return true;
      }
    }
  }
  return moveAvailable;
}
