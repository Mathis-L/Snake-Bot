#include "humanInput.h"
#include "main.h"

enum Direction input(Snake jeu){
    enum Direction user_dir;
    char ch;
    if (kbhit()) {
        ch = getch(); // Utilisez getch() pour obtenir la touche enfoncée
        //printf("La touche '%c' a été enfoncée.\n", ch);

        if(ch == 'q'){
            if (jeu.direction != EAST)
                user_dir=WEST;
        }
        else if (ch == 's'){
            if (jeu.direction != NORTH)
                user_dir=SOUTH;
        }
        else if (ch == 'd'){
            if (jeu.direction != WEST)
                user_dir=EAST;
        }
        else if (ch == 'z'){
            if (jeu.direction != SOUTH)
                user_dir=NORTH;
        }
    } 
    return(user_dir);
}
