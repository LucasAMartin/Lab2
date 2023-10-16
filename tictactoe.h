#include <stdbool.h>

#define SHM_SIZE 1024

void printBoard(char *p, int s);
void validateMove(int *x, int *y, char *str, int n);
bool checkWin(char *board, int n, char player);
bool checkDraw(char *board, int n);
