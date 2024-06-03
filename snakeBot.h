#ifndef BOT_H
#define BOT_H

#include "main.h"

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



#endif