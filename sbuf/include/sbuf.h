// #include <csapp.h>

/**
 * buf is originazed as deque
*/
typedef struct {
    int* buf;       /* buffer self */
    int n;          /* buffer size */
    int front;      /* (front % n) points to the first item in buffer */
    int rear;       /* (rear % n) points to the last item in buffer */
    sem_t mutex;    /* mutex for thread safe */
    // slots and items are declared with sem_t, which ensures atomic access
    sem_t slots;    /* counts of available free space in buffer */
    sem_t items;    /* counts of items in buffer */
} sbuf_t;

// this function should be called in main thread
void subf_init(sbuf_t* sp, int n);

/**
 * insert item at front of buf
 * same as `deque.offerFirst(item)`
*/
void sbuf_offer_first(sbuf_t* sp, int item);

/**
 * insert item at last of buf
 * same as `deque.offerLast(item)`
*/
void sbuf_offer_last(sbuf_t* sp, int item);

/**
 * poll front item in buf
 * same as `deque.pollFirst()`
 */ 
int sbuf_poll_first(sbuf_t* sp);

/**
 * poll last item in buf
 * same as `deque.pollLast()`
 */ 
int sbuf_poll_last(sbuf_t* sp);

/**
 * free space
*/
void sbuf_deinit(sbuf_t* sp);
