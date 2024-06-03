#ifndef AIRE_H
#define AIRE_H

#include "main.h"
#include "snakeBot.h"


#define MAX_SIZE_FIFO WIDTH*HEIGHT // Définir la taille maximale de la pile FIFO
int areaSize(coordinate coord, char board[HEIGHT][WIDTH]);
int isCoordinateInArray(coordinate data[], int size, coordinate target);

#endif