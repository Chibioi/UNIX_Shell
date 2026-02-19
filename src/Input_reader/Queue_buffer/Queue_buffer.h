#ifndef QUEUE_BUFFER_H
#define QUEUE_BUFFER_H

typedef struct Node Node;

typedef struct Queue Queue;

Node *createNode();

Queue *createQueue();

void *move_left(Queue *q);

void *move_right(Queue *q);

void insert_char(Queue *q, char c);

void backspace(Queue *q);

void freeQueue(Queue *q);

void printBuffer(Queue *q);

void queue_clear(Queue *q);

#endif
