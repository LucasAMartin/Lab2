#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <signal.h>
#include "tictactoe.h"

#define SHM_SIZE 1024

int shmid;
char *board;

// Signal handler function, used to handle unplanned exiting of the terminal (like ctrl-c)
void handle_sigint(int sig)
{
    // Detach from shared memory
    shmdt(board);
    // Clear the shared memory
    shmctl(shmid, IPC_RMID, NULL);

    exit(0);
}

int main(int argc, char *argv[])
{
    int size = 3; // Default size is 3
    if (argc > 1)
    {
        size = atoi(argv[1]); // If a command line argument is passed, use it as the size
    }

    // Register signal handlers
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGHUP, handle_sigint);

    // magic functions
    key_t key = ftok("tictactoe.c", 99); // https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-ftok-generate-interprocess-communication-ipc-key
    if (key == -1)
    {
        perror("Error in ftok");
        exit(1);
    }
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); // https://stackoverflow.com/questions/40380327/what-is-the-use-of-ipc-creat-0666-flag-in-shmget-function-in-c#:~:text=0666%20sets%20the%20access%20permissions,permission%20to%20access%20the%20segment.
    if (shmid == -1)
    {
        perror("Error in shmget");
        exit(1);
    }
    board = (char *)shmat(shmid, (void *)0, 0); // https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-shmat-shared-memory-attach-operation
    if (board == (char *)(-1))
    {
        perror("Error in shmat");
        exit(1);
    }
    board[1] = size;                     // Store the size of the board in shared memory
    board[0] = 1;                      // It's user1's turn at the beginning
    memset(board + 2, ' ', size * size); // Initialize the board with spaces, start 2 spaces in because 0 and 1 are used to share info
    printf("Start user2 to begin game\n");

    while (1)
    {
        if (board[0] == 1)
        {
            printBoard(board + 2, size); // Print the board before making a move
            if (checkWin(board + 2, size, 'X'))
            {
                printf("Player 1 wins!\n");
                board[0] = 2;
                break;
            }
            if (checkWin(board + 2, size, 'O'))
            {
                printf("Player 1 loses\n");
                board[0] = 2;
                break;
            }
            if (checkDraw(board + 2, size))
            {
                printf("It's a draw!\n");
                board[0] = 2;
                break;
            }
            int row, column;
            validateMove(&row, &column, board, size); // Get a valid move
            board[row * size + column + 2] = 'X';     // User1 is X
            board[0] = 2;                           // Now it's user2's turn
        }
        if (board[0] == 2)
        {
            printf("Waiting for player 2... \n");
        }
        while (board[0] == 2)
        {
            sleep(1); // Wait for the other user to make a move
        }
    }
    shmctl(shmid, IPC_RMID, NULL); // Clear the shared memory before exiting
    shmdt(board);                  // Detach from the memory

    return 0;
}
