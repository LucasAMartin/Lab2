#include "tictactoe.h"
#include <stdio.h>

void printBoard(char *p, int s) {
    int i, j;
    // Print column headers
    printf("\t");
    for (j = 0; j < s; j++) {
        printf("  %d ", j);
    }
    printf("\n");

    for (i = 0; i < s; i++) {
        printf("\n");
        if (i > 0) {
            // Print horizontal dividers
            printf("\t");
            for (j = 0; j < s; j++) {
                if (j > 0) {
                    printf("----");
                } else {
                    printf("---");
                }
            }
        }
        printf("\n %d", i);
        // Print row values
        for (j = 0; j < s; j++) {
            if (j > 0) {
                printf("|");
            } else {
                printf("\t");
            }
            printf(" %c ", *(p + (sizeof(char) * i * s) + (sizeof(char) * j)));
        }
    }
    printf("\n\n");
}

void getMove(int *x, int *y, char *str, int n) {
    char line[100];
    while (1) {
        printf("Enter your move (format: x y): ");
        if (!fgets(line, sizeof line, stdin)) {
            // EOF was hit or an error occurred, handle it
            printf("Invalid input\n");
            continue;
        }
        // Try to read two integers from the line
        if (sscanf(line, "%d %d", x, y) != 2) {
            printf("Invalid input\n");
            continue;
        }
        // Check if the move is within the board and the space is empty
        if (*x < 0 || *x >= n || *y < 0 || *y >= n || str[*x*n+*y+2] != ' ') {
            printf("Invalid move\n");
            continue;
        }
        // If we got here, the move is valid
        break;
    }
}

bool checkWin(char *board, int n, char player) {
    // Check rows
    for (int i = 0; i < n; i++) {
        bool win = true;
        for (int j = 0; j < n; j++) {
            if (board[i*n+j] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    // Check columns
    for (int i = 0; i < n; i++) {
        bool win = true;
        for (int j = 0; j < n; j++) {
            if (board[j*n+i] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    // Check diagonals
    bool win = true;
    for (int i = 0; i < n; i++) {
        if (board[i*n+i] != player) {
            win = false;
            break;
        }
    }
    if (win) return true;

    win = true;
    for (int i = 0; i < n; i++) {
        if (board[i*n+(n-i-1)] != player) {
            win = false;
            break;
        }
    }
    return win;
}

bool checkDraw(char *board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i*n+j] == ' ') {
                return false; // If there's an empty space, it's not a draw
            }
        }
    }
    return true; // If we got here, all spaces are filled, so it's a draw
}