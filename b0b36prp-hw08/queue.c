#include "queue.h"

queue_t* create_queue(int capacity) {
    if (capacity == 0) {
        return NULL;
    }

    queue_t *queue = (queue_t *) malloc(sizeof(queue_t));
    if (!queue) {
        return NULL;
    }
 
    queue->queue = (void **) malloc(sizeof(void *) * capacity);
    if (!queue->queue) {
        free(queue);
        return NULL;
    }

    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
    queue->capacity = capacity;

    return queue;
}

void delete_queue(queue_t *queue) {
    if (queue != NULL) {
        if (queue->queue != NULL) {
            free(queue->queue);
        }

        free(queue);
    }
}

bool push_to_queue(queue_t *queue, void *data) {
    if (queue->size == queue->capacity) {
        int old_capacity = queue->capacity;

        void **old_queue = queue->queue;
        void **new_queue = (void **) malloc(sizeof(void *) * 2 * old_capacity);

        int head = queue->head;
        for (int i = 0; i < old_capacity; ++i) {
            new_queue[i] = old_queue[head];
            head = (head + 1) % old_capacity;
        }

        free(queue->queue);
        queue->queue = new_queue;
        queue->head = 0;
        queue->tail = old_capacity;
        queue->capacity *= 2;
    }

    queue->queue[queue->tail] = data;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size += 1;

    return true;
}

void* pop_from_queue(queue_t *queue) {
    if (queue->size == 0) {
        return NULL;
    }

    void *data = queue->queue[queue->head];
    queue->queue[queue->head] = NULL;
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size -= 1;

    if (queue->size != 0 && queue->size < queue->capacity / 2) {
        int old_capacity = queue->capacity;
        int amount_to_remove = queue->capacity / 3;
        int new_capacity = old_capacity - amount_to_remove;

        void **old_queue = queue->queue;
        void **new_queue = (void **) malloc(sizeof(void *) * new_capacity);

        int head = queue->head;
        int curr_size =queue->size;
        for (int i = 0; i < curr_size; ++i) {
            new_queue[i] = old_queue[head];
            head = (head + 1) % old_capacity;
        }

        free(queue->queue);
        queue->queue = new_queue;
        queue->head = 0;
        queue->tail = queue->size;
        queue->capacity = new_capacity;
    }

    return data;
}

void* get_from_queue(queue_t *queue, int idx) {
    if (queue->size == 0 || idx < 0 || idx >= queue->size) {
        return NULL;
    }
 
    return queue->queue[(queue->head + idx) % queue->capacity];
}

int get_queue_size(queue_t *queue) {
    return queue->size;
}
