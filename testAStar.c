#include <stdio.h>
#include <stdlib.h>

#define WIDTH 5
#define HEIGHT 5

typedef struct Node {
    int x, y;
    int g, h;
    struct Node *parent;
} Node;

int manhattan_distance(Node *a, Node *b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

Node* astar(Node *start, Node *goal, int width, int height) {
    Node *grid = (Node*)malloc(sizeof(Node) * width * height);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Node *node = &grid[y * width + x];
            node->x = x;
            node->y = y;
            node->g = node->h = 0;
            node->parent = NULL;
        }
    }

    Node **open_list = (Node**)malloc(sizeof(Node*) * width * height);
    int open_list_count = 0;

    open_list[open_list_count++] = start;

    while (open_list_count > 0) {
        int best_index = 0;
        for (int i = 1; i < open_list_count; ++i) {
            if ((open_list[i]->g + open_list[i]->h) < (open_list[best_index]->g + open_list[best_index]->h))
                best_index = i;
        }

        Node *current = open_list[best_index];
        open_list[best_index] = open_list[--open_list_count];

        if (current == goal)
            return current;

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if ((dx == 0 && dy == 0) || current->x + dx < 0 || current->x + dx >= width || current->y + dy < 0 || current->y + dy >= height)
                    continue;

                Node *neighbor = &grid[(current->y + dy) * width + (current->x + dx)];

                int new_g = current->g + 1;

                if (new_g < neighbor->g || neighbor->g == 0) {
                    neighbor->g = new_g;
                    neighbor->h = manhattan_distance(neighbor, goal);
                    neighbor->parent = current;

                    int found = 0;
                    for (int i = 0; i < open_list_count; ++i) {
                        if (open_list[i] == neighbor) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found)
                        open_list[open_list_count++] = neighbor;
                }
            }
        }
    }

    return NULL;
}

void print_path(Node *goal) {
    if (goal == NULL) {
        printf("Chemin non trouvé.\n");
        return;
    }

    int length = 0;
    Node *current = goal;
    while (current != NULL) {
        length++;
        current = current->parent;
    }

    Node **path = (Node**)malloc(sizeof(Node*) * length);
    current = goal;
    int index = length - 1;
    while (current != NULL) {
        path[index--] = current;
        current = current->parent;
    }

    printf("Chemin trouvé (longueur = %d) :\n", length);
    for (int i = 0; i < length; ++i) {
        printf("(%d, %d) ", path[i]->x, path[i]->y);
    }
    printf("\n");

    free(path);
}

int main() {
    Node start = {0, 0, 0, 0, NULL};
    Node goal = {4, 4, 0, 0, NULL};

    Node *result = astar(&start, &goal, WIDTH, HEIGHT);

    print_path(result);

    return 0;
}
