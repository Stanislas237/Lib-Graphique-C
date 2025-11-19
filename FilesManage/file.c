#include "file.h"
#include "app.h"

Queue* queue_create(int capacity){
    Queue *q = malloc(sizeof(Queue));
    q->capacity = capacity;
    q->size = 0;
    q->datas = (int*) malloc(capacity * sizeof(int));
    q->shapes = (Shape**) malloc(capacity * sizeof(Shape*));
    return q;
}

void queue_destroy(Queue *q){
    if (q){
        if (q->datas)
            free(q->datas);
        if (q->shapes)
            free(q->shapes);
        free(q);
    }
}

void queue_draw(App *app){
    int startX = 50, startY = 100, boxW = 40, boxH = 30;
    Queue *q = app->main_queue;
    Screen *s = app->screen;

    for(int i = 4; i < s->count; i++)
        if (s->elements[i] && s->elements[i]->destroy)
            s->elements[i]->destroy(s->elements[i]);
    s->count = 4;

    for (int i = 0; i < q->size; i++) {
        char txt[16];
        sprintf(txt, "%d", q->datas[i]);
        Shape *shape = shape_create(startX + i*boxW, startY, boxW, boxH, txt, app->bg,app->font);
        shape->border = isFull(q) ? (SDL_Color){200,50,100,255} : (SDL_Color){50,200,100,255};
        shape->border_width = 1;
        screen_add(app->screen, shape);
        q->shapes[i] = shape;
    }
}

bool isEmpty(Queue *q) { return q->size == 0; }
bool isFull(Queue *q) { return q->size == q->capacity; }

void enqueue(Queue *q, int x) {
    if (isFull(q)) return;
    q->datas[q->size++] = x;
}

int dequeue(Queue *q) {
    if (isEmpty(q)) return -1;
    int item = q->datas[0];
    q->size--;
    for(int i=0; i<q->size; i++)
        q->datas[i] = q->datas[i+1];
    return item;
}

int front(Queue *q) {
    if (isEmpty(q)) return -1;
    return q->datas[0];
}
