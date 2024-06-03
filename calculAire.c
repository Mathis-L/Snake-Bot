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