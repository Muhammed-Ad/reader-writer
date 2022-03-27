#ifndef RW_H
#define RW_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct _rwlock_t {
    sem_t writelock; //lock to prevent readers and writer from conflicting
    sem_t lock; //lock to prevent readers conficting

    sem_t writer_waiting; //lock to prevent starvation of writer
    int readers;
} rwlock_t;

void rwlock_init(rwlock_t *lock);
void rwlock_acquire_readlock(rwlock_t *lock);
void rwlock_release_readlock(rwlock_t *lock);
void rwlock_acquire_writelock(rwlock_t *lock);
void rwlock_release_writelock(rwlock_t *lock);
void *reader(void *arg);
void *writer(void *arg);

#endif //RW_H


