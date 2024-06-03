#ifndef BOT_H
#define BOT_H

#include "main.h"

#define MAX_SIZE_FIFO WIDTH*HEIGHT // Définir la taille maximale de la pile FIFO

typedef struct AStarNode{
    coordinate coord;
    int f; // Coût total estimé (g + h)
    int g; // Coût réel depuis le début
    int h; // Coût estimé jusqu'à la cible
    struct AStarNode* parent;
} AStarNode;


int manhattanDistance(coordinate c1, coordinate c2);
int sameCoordinate(coordinate c1, coordinate c2);
enum Direction performAStarSearch(Snake jeu);

enum Direction giveDirection(Snake jeu);

int indexMax(int tab[]);

int areaSize(coordinate coord, char board[HEIGHT][WIDTH]);
int isCoordinateInArray(coordinate data[], int size, coordinate target);



#endif