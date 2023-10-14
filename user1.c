#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include "tictactoe.h"


#define SHM_SIZE 1024


int main(int argc, char *argv[]) {
    int size = 3; // Default size is 3
    if(argc > 1) {
        size = atoi(argv[1]); // If a command line argument is passed, use it as the size
    }

    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, SHM_SIZE, 0666|IPC_CREAT);
    char *board = (char*) shmat(shmid, (void*)0, 0);

    board[1] = size; // Store the size of the board in shared memory
    board[0] = '1'; // It's user1's turn at the beginning
    memset(board+2, ' ', size*size); // Initialize the board with spaces

    while(1) {
    if(board[0] == '1') {
        printBoard(board+2, size); // Print the board before making a move
            if(checkWin(board+2, size, 'X')) {
        printf("Player 1 wins!\n");
        board[0] = '2';
        break;
    }
    if(checkWin(board+2, size, 'O')) {
        printf("Player 1 loses\n");
        board[0] = '2';
        break;
    }
    if(checkDraw(board+2, size)) {
        printf("It's a draw!\n");
        board[0] = '2';
        break;
    }
        int row, column;
        getMove(&row, &column, board, size); // Get a valid move
        board[row*size+column+2] = 'X'; // User1 is X
        board[0] = '2'; // Now it's user2's turn
    }
    if(board[0] == '2') {
        printf("Waiting for player 2... \n");
        }
        while(board[0] == '2') {
            sleep(1); // Wait for the other user to make a move
        }
}
    shmdt(board);

    return 0;
}
