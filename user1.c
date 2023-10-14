#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include "tictactoe.h"


#define SHM_SIZE 1024


int main(int argc, char *argv[]) {
    int n = 3; // Default size is 3
    if(argc > 1) {
        n = atoi(argv[1]); // If a command line argument is passed, use it as the size
    }

    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, SHM_SIZE, 0666|IPC_CREAT);
    char *str = (char*) shmat(shmid, (void*)0, 0);

    str[1] = n; // Store the size of the board in shared memory
    str[0] = '1'; // It's user1's turn at the beginning
    memset(str+2, ' ', n*n); // Initialize the board with spaces

    while(1) {
    if(str[0] == '1') {
        printBoard(str+2, n); // Print the board before making a move
            if(checkWin(str+2, n, 'X')) {
        printf("Player 1 wins!\n");
        str[0] = '2';
        break;
    }
    if(checkWin(str+2, n, 'O')) {
        printf("Player 1 loses\n");
        str[0] = '2';
        break;
    }
    if(checkDraw(str+2, n)) {
        printf("It's a draw!\n");
        str[0] = '2';
        break;
    }
        int x, y;
        getMove(&x, &y, str, n); // Get a valid move
        str[x*n+y+2] = 'X'; // User1 is X
        str[0] = '2'; // Now it's user2's turn
    }
    if(str[0] == '2') {
        printf("Waiting for player 2... \n");
        }
        while(str[0] == '2') {
            sleep(1); // Wait for the other user to make a move
        }
}
    shmdt(str);

    return 0;
}
