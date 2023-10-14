#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>


#define SHM_SIZE 1024

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



int main(int argc, char *argv[]) {

    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, SHM_SIZE, 0666|IPC_CREAT);
    char *str = (char*) shmat(shmid, (void*)0, 0);

    int n = str[1]; // Get the size of the board from shared memory

    while(1) {
        if(checkWin(str+2, n, 'O')) {
            printf("Player 2 wins!\n");
            break;
        }
        if(checkWin(str+2, n, 'X')) {
            printf("Player 2 loses\n");
            break;
        }
        if(str[0] == '2') {
            printBoard(str+2, n); // Print the board before making a move
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
