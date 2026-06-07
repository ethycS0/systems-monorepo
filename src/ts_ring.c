#include <stdatomic.h>
#include <stdbool.h>

#define BUFFER_SIZE 128

typedef struct ring_buffer_t {
        unsigned char buffer[BUFFER_SIZE];
        atomic_int head;
        atomic_int tail;
} ring_buffer_t;

bool pop(ring_buffer_t *r, unsigned char *out) {
        int head = atomic_load_explicit(&r->head, memory_order_acquire);
        int tail = atomic_load_explicit(&r->tail, memory_order_relaxed);

        if (head == tail) {
                return false;
        }

        *out = r->buffer[tail];
        atomic_store_explicit(&r->tail, (tail + 1) % BUFFER_SIZE, memory_order_release);
        return true;
}

int push(ring_buffer_t *r, unsigned char c) {
        int head = atomic_load_explicit(&r->head, memory_order_relaxed);
        int tail = atomic_load_explicit(&r->tail, memory_order_acquire);

        if ((head + 1) % BUFFER_SIZE == tail) {
                return -1;
        }

        r->buffer[head] = c;
        atomic_store_explicit(&r->head, (head + 1) % BUFFER_SIZE, memory_order_release);

        return 0;
}
