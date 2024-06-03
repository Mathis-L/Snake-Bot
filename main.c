#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // for sleep function
#include "main.h"
#include "humanInput.h"
#include "snakeBot.h"

int main() {
    int replay;
    printf("Bienvenue dans le jeu Snake !\n");

    do{
        Snake jeu;
        initBoard(&jeu);

        enum PlayerType choice;
        userChoice(&choice);
        sleep(2); //wait one seconds

        while (!jeu.dead){
            int i = 0;
            while(i !=jeu.speed){
                i++;
            }

            fruit(&jeu);
            
            //jeu.direction = performAStarSearch(jeu);
            enum Direction player_dir;

            if (choice == HUMAN){
                player_dir = input(jeu);
            }
            else{
                player_dir = giveDirection(jeu);
            }

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

void userChoice(enum PlayerType *playerType){
    int choice;
    printf("Choisissez une option :\n");
    printf("1. Joueur Humain\n");
    printf("2. Intelligence Artificielle\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Vous avez choisi de jouer en tant que Joueur Humain.\n");
            *playerType = HUMAN;
            break;
        case 2:
            printf("Vous avez choisi de regarder l'Intelligence Artificielle.\n");
            *playerType = AI;
            break;
        default:
            printf("You are too dumb to play by yourself \n");
            *playerType = AI;
            break;
    }
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
    printf("Quitter le jeu : APPUYER SUR 'X'\n");
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

    if (jeu->board[headCoordinate.x][headCoordinate.y] == '#' || jeu->board[headCoordinate.x][headCoordinate.y] == '0'){
        jeu->dead = 1;
    } //player hits a wall 
    else if (sameCoordinate(headCoordinate, jeu->fruit)){
        jeu->points++;
        jeu->fruitExist = 0;
        coordinate tailCoordinate = {-1, -1};
        appendNode(&jeu->nodePlayer, tailCoordinate);
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



