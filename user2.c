#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>
#include "tictactoe.h"

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {

    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, SHM_SIZE, 0666|IPC_CREAT);
    char *str = (char*) shmat(shmid, (void*)0, 0);

    int n = str[1]; // Get the size of the board from shared memory

    while(1) {
        if(str[0] == '2') {
            printBoard(str+2, n); // Print the board before making a move
            if(checkWin(str+2, n, 'O')) {
            printf("Player 2 wins!\n");
            str[0] = '1';
            break;
        }
        if(checkWin(str+2, n, 'X')) {
            printf("Player 2 loses\n");
            str[0] = '1';
            break;
        }
        if(checkDraw(str+2, n)) {
            printf("It's a draw!\n");
            str[0] = '1';
            break;
        }
            int x, y;
            getMove(&x, &y, str, n); // Get a valid move
            str[x*n+y+2] = 'O'; // User2 is O
            str[0] = '1'; // Now it's user1's turn
        }
        if(str[0] == '1') {
            printf("Waiting for player 1... \n");
        }
        while(str[0] == '1') {
            sleep(1); // Wait for the other user to make a move
        }
    }

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
