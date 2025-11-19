#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "screen.h"
typedef struct App App;

typedef struct Queue {
    int size, capacity;
    int *datas;
    Shape **shapes;
} Queue;

Queue* queue_create(int capacity);
void queue_destroy(Queue *q);
void queue_draw(App *app);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
void enqueue(Queue *q, int x);
int dequeue(Queue *q);
int front(Queue *q);

#endif
