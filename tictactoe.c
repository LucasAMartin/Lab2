#include "tictactoe.h"
#include <stdio.h>

void printBoard(char *board, int size) {
    // Print column header numbers
    printf("\n\t");
    for (int j = 0; j < size; j++) {
        printf("%2d ", j); // print the number with a min width of 2
    }
    printf("\n");

    // Print rows
    for (int i = 0; i < size; i++) {
        // Print horizontal dividers
        printf("\t");
        for (int j = 0; j < size; j++) {
            printf("---");
        }
        printf("-\n");

        // Print row number and values
        printf("%d\t", i);
        for (int j = 0; j < size; j++) {
            printf("|%c ", *(board + i * size + j)); //  access the element at row i and column j of the game board
        }
        printf("|\n");
    }

    // Print final horizontal divider
    printf("\t");
    for (int j = 0; j < size; j++) {
        printf("---");
    }
    printf("-\n\n");
}


// Function to get the player's move
void getMove(int *row, int *column, char *board, int size) {
    // Declare a character array to store the player's input
    char line[100];

    // Start an infinite loop
    while (1) {
        // Prompt the player to enter their move
        printf("Enter your move (format: row column): ");

        // Read a line of input from the player
        // If an error occurs or EOF is hit, print an error message and continue to the next iteration of the loop
        if (!fgets(line, sizeof line, stdin)) {
            printf("Invalid input\n");
            continue;
        }

        // Try to parse two integers from the input line
        // If two integers can't be found, print an error message and continue to the next iteration of the loop
        if (sscanf(line, "%d %d", row, column) != 2) {
            printf("Invalid input\n");
            continue;
        }

        // Check if the move is within the bounds of the board and if the chosen space is empty
        // If not, print an error message and continue to the next iteration of the loop
        if (*row < 0 || *row >= size || *column < 0 || *column >= size || board[*row*size+*column+2] != ' ') {
            printf("Invalid move\n");
            continue;
        }

        // If all checks pass, break out of the loop
        break;
    }
}

// Function to check if a player has won
bool checkWin(char *board, int size, char player) {
    // Check each row for a win
    for (int i = 0; i < size; i++) {
        bool win = true;
        for (int j = 0; j < size; j++) {
            if (board[i*size+j] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;  // If a win is found, return true
    }

    // Check each column for a win
    for (int i = 0; i < size; i++) {
        bool win = true;
        for (int j = 0; j < size; j++) {
            if (board[j*size+i] != player) {
                win = false;
                break;
            }
        }
        if (win) return true;  // If a win is found, return true
    }

    // Check diagonals for a win
    bool win = true;
    for (int i = 0; i < size; i++) {
        if (board[i*size+i] != player) {
            win = false;
            break;
        }
    }
    if (win) return true;  // If a win is found, return true

    win = true;
    for (int i = 0; i < size; i++) {
        if (board[i*size+(size-i-1)] != player) {
            win = false;
            break;
        }
    }
    
    // Return whether a win was found
    return win;
}

// Function to check if the game is a draw
bool checkDraw(char *board, int size) {
    // Check each space on the board
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // If there's an empty space, it's not a draw, so return false
            if (board[i*size+j] == ' ') {
                return false;
            }
        }
    }

    // If all spaces are filled, it's a draw, so return true
    return true;
}

