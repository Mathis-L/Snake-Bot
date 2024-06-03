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