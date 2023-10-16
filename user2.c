#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include "tictactoe.h"

#define SHM_SIZE 1024

int shmid;
char *board;

// Signal handler function
void handle_sigint(int sig)
{
    // Detach from shared memory
    shmdt(board);
    exit(0);
}

int main(int argc, char *argv[])
{

    // Register signal handlers
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGHUP, handle_sigint);

    // Comment explanations of these magic functions in user1.c file
    key_t key = ftok("tictactoe.c", 99);
    if (key == -1)
    {
        perror("Error in ftok");
        exit(1);
    }

    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1)
    {
        printf("Please start user1.c first.\n");
        exit(1);
    }

    board = (char *)shmat(shmid, (void *)0, 0);
    if (board == (char *)(-1))
    {
        perror("Error in shmat");
        exit(1);
    }

    int size = board[1]; // Get the size of the board from shared memory

    while (1)
    {
        if (board[0] == 2)
        {
            printBoard(board + 2, size); // Print the board before making a move
            if (checkWin(board + 2, size, 'O'))
            {
                printf("Player 2 wins!\n");
                board[0] = 1;
                break;
            }
            if (checkWin(board + 2, size, 'X'))
            {
                printf("Player 2 loses\n");
                board[0] = 1;
                break;
            }
            if (checkDraw(board + 2, size))
            {
                printf("It's a draw!\n");
                board[0] = 1;
                break;
            }
            int row, column;
            validateMove(&row, &column, board, size); // Get a valid move
            board[row * size + column + 2] = 'O';     // User2 is O
            board[0] = 1;                           // Now it's user1's turn
        }
        if (board[0] == 1)
        {
            printf("Waiting for player 1... \n");
        }
        while (board[0] == 1)
        {
            sleep(1); // Wait for the other user to make a move
        }
    }

    shmdt(board); // Detach from the memory

    return 0;
}
