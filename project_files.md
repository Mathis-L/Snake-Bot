# Project Files

## main.c
### Header File: .\main.h
```c
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
```

### Source File: .\main.c
```c
#include <stdio.h>
#include <conio.h> // Incluez le fichier d'en-tête conio.h pour utiliser kbhit() keyboard input
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "snakeBot.h"
#include "calculAire.h"

int main() {
    int replay;
    do{
        Snake jeu;
        initBoard(&jeu);

        while (!jeu.dead){
            int i = 0;
            while(i !=jeu.speed){
                i++;
            }

            fruit(&jeu);
            //input(&jeu);
            
            //jeu.direction = performAStarSearch(jeu);
            enum Direction player_dir;
            player_dir = giveDirection(jeu);

            if (player_dir == NORTH || player_dir == SOUTH || player_dir == EAST || player_dir == WEST){ //test if the direction given by the player is valid
                jeu.direction = player_dir;
            }

            logic(&jeu);
            
            
            draw(&jeu);
        }

        replay = deadScreen(&jeu);
    }while(replay);
    
    return 0;
}

int increaseSpeed(speed){
    //static int progress = 0;

    if (speed > MAX_SPEED){
        speed = speed-(100000);
    }
    return speed;
}

void initBoard(Snake *jeu){
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // Initialiser les bords
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                jeu->board[i][j] = '#';
            }
            else{
                jeu->board[i][j] = ' ';
            }
        }
    }

    jeu->fruitExist = 0; //fruit à créer

    jeu->speed = MIN_SPEED;

    jeu->direction = NORTH;

    jeu->points = 0;

    jeu->dead = 0; //player alive

    jeu->nodePlayer = NULL;
    coordinate newCoord = {WIDTH / 2, HEIGHT / 2};
    appendNode(&jeu->nodePlayer, newCoord);

}

void draw(Snake *jeu){
    system("cls");
    //board print
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // Initialiser les bords
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                jeu->board[i][j] = '#';
            }
            else{
                jeu->board[i][j] = ' ';
            }
        }
    }
   

    //print snake player
    Node* current = jeu->nodePlayer;
    while (current!= NULL){
        jeu->board[current->coordinate.x][current->coordinate.y] = SNAKE_BODY;
        current = current->next;
    }

     //print fruit
    jeu->board[jeu->fruit.x][jeu->fruit.y] = FRUIT;

    jeu->board[jeu->nodePlayer->coordinate.x][jeu->nodePlayer->coordinate.y] = SNAKE_HEAD;
    
    // display current board
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c ", jeu->board[i][j]);
        }
        printf("\n");
    }
    printf("SCORE : %d\n", jeu->points);
    printf("Quitter le jeu : APPUYER SUR 'X'");
}

int validPosFruit(Snake jeu, int x, int y){
    if (jeu.board[x][y] == ' ')
        return 1;
    else
        return 0;
}

void fruit(Snake *jeu){
    srand(time(NULL));
    if (!jeu->fruitExist){
        int x;
        int y;
        do{
            x = rand() % (HEIGHT - 2) + 1; // Exclure les bords
            y = rand() % (WIDTH - 2) + 1;
        }while (!validPosFruit(*jeu, x, y));
        jeu->fruit.x = x;
        jeu->fruit.y = y;
        jeu->fruitExist = 1;
    }
}


void input(Snake *jeu){
    char ch;
    if (kbhit()) {
        ch = getch(); // Utilisez getch() pour obtenir la touche enfoncée
        //printf("La touche '%c' a été enfoncée.\n", ch);

        if(ch == 'q'){
            if (jeu->direction != EAST)
                jeu->direction=WEST;
        }
        else if (ch == 's'){
            if (jeu->direction != NORTH)
                jeu->direction=SOUTH;
        }
        else if (ch == 'd'){
            if (jeu->direction != WEST)
                jeu->direction=EAST;
        }
        else if (ch == 'z'){
            if (jeu->direction != SOUTH)
                jeu->direction=NORTH;
        }
        else if (ch == 'x'){
            exit(0);
        }
    } 
}


void logic(Snake *jeu){
    coordinate headCoordinate = jeu->nodePlayer->coordinate;
    switch(jeu->direction) {
        case NORTH:
            headCoordinate.x--;
            printf("NORTH\n");
            break;
        case EAST:
            headCoordinate.y++;
            printf("EAST\n");
            break;
        case SOUTH:
            headCoordinate.x++;
            printf("SOUTH\n");
            break;
        case WEST:
            headCoordinate.y--;
            printf("WEST\n");
            break;
        default:
            printf("prout\n");
    }
    //printf("L'aire devant le serpent: %d", areaSize(headCoordinate, jeu->board));

    if (jeu->board[headCoordinate.x][headCoordinate.y] == '#' || jeu->board[headCoordinate.x][headCoordinate.y] == '0'){
        jeu->dead = 1;
    } //player hits a wall 
    else if (sameCoordinate(headCoordinate, jeu->fruit)){
        jeu->points++;
        jeu->fruitExist = 0;
        coordinate tailCoordinate = {-1, -1};
        appendNode(&jeu->nodePlayer, tailCoordinate);
        jeu->speed = increaseSpeed(jeu->speed);
    } //player eats the fruit

    shiftValues(&jeu->nodePlayer, headCoordinate);
        
}

Node* createNode(coordinate coordinate) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Allocation de mémoire échouée\n");
        exit(EXIT_FAILURE);
    }
    newNode->coordinate = coordinate;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node** headRef, coordinate coordinate) {
    Node* newNode = createNode(coordinate);
    if (*headRef == NULL) {
        *headRef = newNode;
    } else {
        Node* current = *headRef;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// void displayList(Node* head) {
//     Node* current = head;
//     while (current != NULL) {
//         printf("(%d, %d) -> ", current->coordinate.x, current->coordinate.y);
//         current = current->next;
//     }
//     printf("NULL\n");
// }

void shiftValues(Node** headRef, coordinate newCoordinate) {
    if (*headRef == NULL)
        return;

    Node* current = *headRef;
    coordinate previousCoordinate = current->coordinate;
    coordinate currentCoordinate;
    current->coordinate = newCoordinate;

    current = current->next;

    while (current != NULL){
        currentCoordinate = current->coordinate;
        current->coordinate = previousCoordinate;
        previousCoordinate = currentCoordinate;
        current = current->next;
    }
}

int deadScreen(Snake *jeu){
    // system("cls");
    printf("      .-\"\"\"\"\"-.\n");
    printf("    .'          '.\n");
    printf("   /   O      O   \\\n");
    printf("  :           `    :\n");
    printf("  |                |\n");
    printf("  :    .------.    :\n");
    printf("   \\  '        '  /\n");
    printf("    '.          .'\n");
    printf("      '-......-'\n");
    printf(" DDDD    EEEEE    AAAA   DDDD  \n");
    printf(" D    D  E       A    A  D    D \n");
    printf(" D    D  EEEE    AAAAAA  D    D \n");
    printf(" D    D  E       A    A  D    D \n");
    printf(" DDDDD   EEEEE   A    A  DDDDD  \n");
    printf("\nSCORE : %d\n", jeu->points);
    
    char inputLetter;
    printf("\nREPLAY ? (y/n)");
    while(1){
        if (kbhit()) {
            inputLetter = getch();
    
            if (inputLetter == 'y'){
                return 1;
            }
            else{
                return 0;
            }
        }
    }
}




```

## calculAire
### Header File: .\calculAire.h
```c
#ifndef AIRE_H
#define AIRE_H

#include "main.h"
#include "snakeBot.h"


#define MAX_SIZE_FIFO WIDTH*HEIGHT // Définir la taille maximale de la pile FIFO
int areaSize(coordinate coord, char board[HEIGHT][WIDTH]);
int isCoordinateInArray(coordinate data[], int size, coordinate target);

#endif
```

### Source File: .\calculAire.c
```c
#include <stdio.h>
#include <conio.h> // Incluez le fichier d'en-tête conio.h pour utiliser kbhit()
#include <stdlib.h>
#include <time.h>
#include "calculAire.h"

int isCoordinateInArray(coordinate data[], int size, coordinate target) {
    for (int i = 0; i < size; i++) {
        if (data[i].x == target.x && data[i].y == target.y) {
            return 1; // Found the coordinate
        }
    }
    return 0; // Coordinate not found
}

int areaSize(coordinate coord, char board[HEIGHT][WIDTH]){

    if (board[coord.x][coord.y] == '#' || board[coord.x][coord.y] == '0'){
        return 0;
    }

    coordinate data[MAX_SIZE_FIFO];
    data[0] = coord;
    
    int area = 0;
    int size_data = 1;

     coordinate pop_coord;

    while(area != size_data && size_data < MAX_SIZE_FIFO){
        
        pop_coord = data[area];
        //printf("coord: (%d,%d) et char:%c\n", pop_coord.x, pop_coord.y, board[pop_coord.x][pop_coord.y]);
        area+=1;
        //top
        coordinate coord_top = {pop_coord.x-1,pop_coord.y};
        if (board[coord_top.x][coord_top.y] != '#' && board[coord_top.x][coord_top.y] != '0' && board[coord_top.x][coord_top.y] != 'X' && !isCoordinateInArray(data, size_data, coord_top)){
            data[size_data] = coord_top;
            size_data+=1;
        }

        //botttom
        coordinate coord_bottom = {pop_coord.x+1,pop_coord.y};
        if (board[coord_bottom.x][coord_bottom.y] != '#' && board[coord_bottom.x][coord_bottom.y] != '0' && board[coord_bottom.x][coord_bottom.y] != 'X' && !isCoordinateInArray(data, size_data, coord_bottom)){
            data[size_data] = coord_bottom;
            size_data+=1;
        }

        //right
        coordinate coord_right = {pop_coord.x,pop_coord.y+1};
        if (board[coord_right.x][coord_right.y] != '#' && board[coord_right.x][coord_right.y] != '0' && board[coord_right.x][coord_right.y] != 'X' && !isCoordinateInArray(data, size_data, coord_right)){
            data[size_data] = coord_right;
            size_data+=1;
        }

        //left
        coordinate coord_left = {pop_coord.x,pop_coord.y-1};
        if (board[coord_left.x][coord_left.y] != '#' && board[coord_left.x][coord_left.y] != '0' && board[coord_left.x][coord_left.y] != 'X' && !isCoordinateInArray(data, size_data, coord_left)){
            data[size_data] = coord_left;
            size_data+=1;
        }
        //printf("size_data: %d \n", size_data);
    }

    return area;

}
```

## snakeBot
### Header File: .\snakeBot.h
```c
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
```


## snakeBot2
### Source File: .\snakeBot2.c
```c
#include <stdio.h>
#include <conio.h> // Incluez le fichier d'en-tête conio.h pour utiliser kbhit()
#include <stdlib.h>
#include <time.h>
#include "snakeBot.h"
#include "calculAire.h"



int manhattanDistance(coordinate c1, coordinate c2) {
    return abs(c1.x - c2.x) + abs(c1.y - c2.y);
}

int sameCoordinate(coordinate c1, coordinate c2){
    if (c1.x == c2.x && c1.y == c2.y)
        return 1;
    else
        return 0;
}

int indexMax(int tab[]){
    int max_value = tab[0];
    int max_index = 0;

     // Iterate through the array starting from index 1
    for (int i = 0; i < 4; i++) {
        //printf("i=%d, value = %d\n", i ,values_directions[i]);
        // If current element is greater than current maximum, update max_value and max_index
        if (tab[i] > max_value) {
            max_value = tab[i];
            max_index = i;
        }
    }

    return max_index;
}

int foodInFirstRow(char board[HEIGHT][WIDTH]){
    for (int j = 0; j < WIDTH; j++) {
        
        if (board[1][j] == FRUIT) {
            // printf("first row\n");
            return 1; // '*' is present in the second row
        }
    }
    
    return 0; // '*' is not present in the second row
}

int foodInLastRow(char board[HEIGHT][WIDTH]){
    for (int j = 0; j < WIDTH; j++) {
        if (board[HEIGHT-2][j] == FRUIT) {
            //printf("last row\n");
            return 1; // '*' is present in the second row
        }
    }
    return 0; // '*' is not present in the second row
}

int foodInFirstCollumn(char board[HEIGHT][WIDTH]){
    for (int j = 0; j < HEIGHT; j++) {
        if (board[j][1] == FRUIT) {
            //printf("first COLLUMN\n");
            return 1; // '*' is present in the second row
        }
    }
    return 0; // '*' is not present in the second row
}

int foodInLastCollumn(char board[HEIGHT][WIDTH]){
    for (int j = 0; j < HEIGHT; j++) {
        if (board[j][WIDTH-2] == FRUIT) {
            //printf("LAST COLLUMN\n");
            return 1; // '*' is present in the second row
        }
    }
    return 0; // '*' is not present in the second row
}


enum Direction giveDirection(Snake jeu){
    enum Direction AstarResult;
    enum Direction final_dir;


    AstarResult = performAStarSearch(jeu);
    printf("AstarResult= %d\n", AstarResult);

    coordinate player_pos = jeu.nodePlayer->coordinate;

    int values_directions[4];

    //top 
    coordinate player_pos_north = {player_pos.x-1, player_pos.y};
    values_directions[0] = areaSize(player_pos_north, jeu.board);

    //bottom
    coordinate player_pos_south = {player_pos.x+1, player_pos.y};
    values_directions[1] = areaSize(player_pos_south, jeu.board);

    //left
    coordinate player_pos_west = {player_pos.x, player_pos.y-1};
    values_directions[2] = areaSize(player_pos_west, jeu.board);

    //right
    coordinate player_pos_east = {player_pos.x, player_pos.y+1};
    values_directions[3] = areaSize(player_pos_east, jeu.board);

    if (AstarResult == NORTH || AstarResult == SOUTH || AstarResult == EAST || AstarResult == WEST){
        //printf("TESSSSST\n\n");
        values_directions[AstarResult]+=1; //put priority on this value
    }

    int max_index = indexMax(values_directions);
    //printf("max_value = %d\n", max_index);

    if (AstarResult == NORTH || AstarResult == SOUTH || AstarResult == EAST || AstarResult == WEST){
        if (values_directions[AstarResult] < (4*(jeu.points-2)) && AstarResult != max_index){
            //printf("CHANGEMENT\n");
            final_dir = max_index;
        }
        else{
            final_dir = AstarResult;
        }
    }

    else{
        final_dir = max_index;
    }
    // printf("FRUIT: %c\n ", jeu.board[jeu.fruit.x][jeu.fruit.y]);
    // printf("FirstRow : %d\n", foodInFirstRow(jeu.board));
    // printf("LastRow : %d\n", foodInLastRow(jeu.board));
    // printf("first collumn : %d\n", foodInFirstCollumn(jeu.board));
    // printf("last collumn : %d\n", foodInLastCollumn(jeu.board));

    //Try to avoid blocking itself and letting a one space gap 
    if ((final_dir == NORTH && player_pos.x - 2 >= 0 && jeu.board[player_pos.x - 2][player_pos.y] == '#' && !foodInFirstRow(jeu.board)) ||
    (final_dir == SOUTH && player_pos.x + 2 < HEIGHT && jeu.board[player_pos.x + 2][player_pos.y] == '#' && !foodInLastRow(jeu.board)) ||
    (final_dir == WEST && player_pos.y - 2 >= 0 && jeu.board[player_pos.x][player_pos.y - 2] == '#' && !foodInFirstCollumn(jeu.board)) ||
    (final_dir == EAST && player_pos.y + 2 < WIDTH && jeu.board[player_pos.x][player_pos.y + 2] == '#' && !foodInLastCollumn(jeu.board))) {
        values_directions[final_dir] = values_directions[final_dir] - 1;
        max_index = indexMax(values_directions);
        final_dir = max_index;
    }


    return final_dir;
}

// Fonction pour effectuer une recherche A* et retourner la prochaine direction à suivre
enum Direction performAStarSearch(Snake jeu) {
    static int mallocCount = 0;
    static int freeCount = 0;
    // Déclarer un tableau pour stocker les nœuds à explorer
    AStarNode *openList[HEIGHT * WIDTH * 100];
    int openListCount = 0;

    // Déclarer un tableau pour suivre les nœuds déjà explorés
    AStarNode *closedList[HEIGHT * WIDTH * 100];
    int closedListCount = 0;

    // Ajouter le nœud de départ à la liste ouverte
    AStarNode *startNode = malloc(sizeof(AStarNode));
    if (startNode == NULL) {
        printf("Allocation de mémoire échouée\n");
        exit(EXIT_FAILURE);
    }
    mallocCount++;
    startNode->coord = jeu.nodePlayer->coordinate;
    printf("PLAYER: (%d, %d)\n", startNode->coord.x, startNode->coord.y);
    startNode->f = 0;
    startNode->g = 0;
    startNode->h = 0;
    startNode->parent = NULL;

    openList[openListCount] = startNode;
    openListCount++;

    // Déclarer les coordonnées de la nourriture
    coordinate foodCoord = jeu.fruit;
    printf("FOOD: (%d, %d)\n", foodCoord.x, foodCoord.y);
    
    // printf("Starting A* search...\n");
    while (openListCount > 0 && closedListCount< HEIGHT * WIDTH) {
        // printf("Number of nodes in open list: %d\n", openListCount);
        // Trouver le nœud avec le coût total le plus bas dans la liste ouverte
        int lowestFIndex = 0;
        for (int i = 1; i < openListCount; ++i) {
            if (openList[i]->f < openList[lowestFIndex]->f) {
                lowestFIndex = i;
            }
        }
        
        // Récupérer et supprimer le nœud avec le coût total le plus bas
        AStarNode *current_node = openList[lowestFIndex];
        openListCount--;
        openList[lowestFIndex] = openList[openListCount]; // Récupère le noeud le dernier noeud pour le mettre à la place de la valeur lowestFIndex
        // printf("Current node coordinates: (%d, %d)\n", current_node->coord.x, current_node->coord.y);
        // add to closed list
        closedList[closedListCount] = current_node;
        closedListCount++;
        

        // Vérifier si le nœud actuel est la nourriture
        if (sameCoordinate(current_node->coord, foodCoord)) {
            // printf("Food found!\n");
            // printf("PLAYER: (%d, %d)\n", startNode->coord.x, startNode->coord.y);
            coordinate path[HEIGHT * WIDTH];
            AStarNode *current = current_node;
            int i = 0;
            while (current != NULL){
                path[i]= current->coord;
                current = current->parent;
                // printf("(%d, %d)\n", path[i].x, path[i].y);
                i++;
            }
            // printf("FINIS\n");
            
            // Free allocated memory
            for (int i = 0; i < openListCount; i++) {
                free(openList[i]);
                freeCount++;
            }
            // printf("openlist clean\n");
            // printf("closedlist à clean %d, malloc count %d\n", closedListCount, mallocCount);
            for (int i = 0; i < closedListCount; i++) {
                free(closedList[i]);
                // printf("%d, %d / ", i, freeCount);
                freeCount++;
            }

            // printf("closedlist clean\n");
            // printf("tant d'allocation: %d, tant de free %d \n", mallocCount, freeCount);
            // printf("VOISIIN A VISE (%d, %d)\n", path[i-2].x, path[i-2].y);
            for (int j = 0; j <i; j++){
                // printf("(%d, %d)\n", path[j].x, path[j].y);
            }

            if (path[i-2].x > startNode->coord.x && path[i-2].y == startNode->coord.y){
                if (jeu.direction != NORTH){
                    printf("SOUTH\n");
                    return SOUTH;
                }
            }
            else if (path[i-2].x < startNode->coord.x && path[i-2].y == startNode->coord.y){
                if (jeu.direction != SOUTH){
                    printf("NORTH\n");
                    return NORTH;
                }
            }                
            else if (path[i-2].y > startNode->coord.y && path[i-2].x == startNode->coord.x){
                if (jeu.direction != WEST){
                    printf("EAST\n");
                    return EAST;
                }
            }
            else if (path[i-2].y < startNode->coord.y && path[i-2].x == startNode->coord.x){
                if (jeu.direction != EAST){
                    printf("WEST\n");
                    return WEST;
                }
                
            }                
            else{
                //printf("VOISIIN A VISE (%d, %d)\n", path[i-2].x, path[i-2].y);
                //printf("PLAYER: (%d, %d)\n", startNode->coord.x, startNode->coord.y);
                return -1; // Indication d'une direction invalide
            }
                

            break;
        }


        AStarNode *children[4];
        int nbChildren = 0;
        //printf("Generating neighbors for node: (%d, %d)\n", current_node->coord.x, current_node->coord.y);
        // Générer les voisins du nœud actuel
        for (int i = -1; i <= 1; ++i) { //itère sur -1, 0 et 1 (déplacement verticaux)
            for (int j = -1; j <= 1; ++j) { //déplacement horizontaux
                if ((i == 0 || j == 0) && !(i == 0 && j == 0)) { // remove les diagonales et le point en cours
                    coordinate node_position = {current_node->coord.x + i, current_node->coord.y + j};

                    // make sure it's in the board
                    if (node_position.x > HEIGHT || node_position.x < 0 || node_position.y > WIDTH || node_position.y < 0){
                        continue;
                    }

                    // make sure it's a walkable terrain
                    if (jeu.board[node_position.x][node_position.y] == '#' || jeu.board[node_position.x][node_position.y] == '0'){
                        continue;
                    }
                    
                    AStarNode *new_node = malloc(sizeof(AStarNode));
                    if (startNode == NULL) {
                        printf("Allocation de mémoire échouée\n");
                        exit(EXIT_FAILURE);
                    }
                    mallocCount++;
                    new_node->coord = node_position;
                    new_node->parent = current_node;
                    //printf("Neighbors %d: (%d, %d)\n", nbChildren, new_node->coord.x, new_node->coord.y);
                    children[nbChildren] = new_node; 
                    nbChildren++;    
                }
            }
        }

        // loop through children
        for (int i = 0; i<nbChildren; i++){
            for (int j = 0; j<closedListCount; j++){ //loop through the closed list
                if (sameCoordinate(children[i]->coord, closedList[j]->coord)){
                    // If child is in the closed list, free it and set it to NULL
                    free(children[i]);
                    freeCount++;
                    children[i] = NULL;
                    break;
                }
            }

            if (children[i] != NULL) {
                // Create the f, g, h values
                children[i]->g = current_node->g + 1;
                children[i]->h = manhattanDistance(children[i]->coord, foodCoord);
                children[i]->f = children[i]->g + children[i]->h;

                // Child is already in the open list
                for (int k = 0; k < openListCount; k++) {
                    if (sameCoordinate(children[i]->coord, openList[k]->coord) && children[i]->g > openList[k]->g) {
                        // If child is in the open list and its current path is better, free it and set it to NULL
                        free(children[i]);
                        freeCount++;
                        children[i] = NULL;
                        break;
                    }   
                }
            }
        }

        // Add non-null children to the open list
        for (int i = 0; i < nbChildren; i++) {
            if (children[i] != NULL) {
                openList[openListCount] = children[i];
                openListCount++;
            }
        }
    }

    
    // Free allocated memory
    for (int i = 0; i < openListCount; i++) {
        free(openList[i]);
        freeCount++;
    }
    for (int i = 0; i < closedListCount; i++) {
        free(closedList[i]);
        freeCount++;
    }

    //printf("tant d'allocation: %d, tant de free %d \n", mallocCount, freeCount);

}
```



