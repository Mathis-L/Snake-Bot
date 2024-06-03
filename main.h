#ifndef SNAKE_H
#define SNAKE_H

#define HEIGHT 20
#define WIDTH 20
#define SNAKE_BODY '0'
#define SNAKE_HEAD 'X'
#define FRUIT '*'

#define MIN_SPEED 5 //50000000 //100000000
#define MAX_SPEED 10000000 
#define ACCELERATION_SPEED 400000
enum Direction{
    NORTH,
    SOUTH,
    WEST,
    EAST
};

typedef struct{
    int x;
    int y;
} coordinate;

typedef struct Node { //faire le free de la liste chainée
    coordinate coordinate;
    struct Node* next;
} Node;

typedef struct Snake{
    char board[HEIGHT][WIDTH];
    int speed;
    Node * nodePlayer;
    int fruitExist;
    coordinate fruit;
    int points;
    int dead;
    enum Direction direction;
} Snake;



void initBoard(Snake *jeu);
void draw(Snake *jeu);
void fruit(Snake *jeu);
int validPosFruit(Snake jeu, int x, int y);
void input(Snake *jeu);
void logic(Snake *jeu);
int deadScreen(Snake *jeu);
int increaseSpeed(int speed);

//function to genrate and increse the size of the snake
Node* createNode(coordinate);
void appendNode(Node** headRef, coordinate);
// void displayList(Node* head);
void shiftValues(Node** headRef, coordinate);

#endif