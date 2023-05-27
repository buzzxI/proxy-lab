#include <csapp.h>
#include "sbuf.h"

void subf_init(sbuf_t* sp, int n) {
    sp->n = n;
    sp->buf = (int*)Malloc(n * sizeof(int));
    sp->front = -1;
    sp->rear = -1;
    Sem_init(&sp->mutex, 0, 1);
    Sem_init(&sp->slots, 0, n);
    Sem_init(&sp->items, 0, 0);
}

void sbuf_offer_first(sbuf_t* sp, int item) {
    P(&sp->slots);
    P(&sp->mutex);
    
    if (sp->front < 0) {
        sp->front = 0;
        sp->rear = 0;
    } else sp->front = (sp->front - 1 + sp->n) % sp->n;
    
    sp->buf[sp->front] = item;

    V(&sp->mutex);
    V(&sp->items);
}

void sbuf_offer_last(sbuf_t* sp, int item) {
    P(&sp->slots);
    P(&sp->mutex);

    if (sp->front < 0) {
        sp->front = 0;
        sp->rear = 0;
    } else sp->rear = (sp->rear + 1) % sp->n;
    
    sp->buf[sp->rear] = item;

    V(&sp->mutex);
    V(&sp->items);
}

int sbuf_poll_first(sbuf_t* sp) {
    P(&sp->items);
    P(&sp->mutex);

    int rst = sp->buf[sp->front];
    sp->front = (sp->front + 1) % sp->n;

    V(&sp->mutex);
    V(&sp->slots);
    return rst;
}


int sbuf_poll_last(sbuf_t* sp) {
    P(&sp->items);
    P(&sp->mutex);

    int rst = sp->buf[sp->rear];
    sp->rear = (sp->rear - 1 + sp->n) % sp->n;

    V(&sp->mutex);
    V(&sp->slots);
    return rst;
}

void sbuf_deinit(sbuf_t* sp) {
    Free(sp->buf);
}