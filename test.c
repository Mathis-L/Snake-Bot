#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Allocation de mémoire échouée\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void append(Node** headRef, int data) {
    Node* newNode = createNode(data);
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

void displayList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void shiftValues(Node** headRef, int newCoordinate) {
    if (*headRef == NULL)
        return;
    
    Node* current = *headRef;
    int previousCoordinate = current->data;
    int currentCoordinate;
    current->data = newCoordinate;

    current = current->next;

    while (current != NULL){
        currentCoordinate = current->data;
        current->data = previousCoordinate;
        previousCoordinate = currentCoordinate;
        current = current->next;
    }

}

int main() {
    Node* head = NULL;

    append(&head, 1);

    printf("Liste initiale : ");
    displayList(head);

    shiftValues(&head, 8);

    printf("Liste après décalage : ");
    displayList(head);

    return 0;
}
