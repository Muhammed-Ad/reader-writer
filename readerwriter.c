#include "readerwriter.h"

void rwlock_init(rwlock_t *lock) {
    lock->readers = 0;

    sem_init(&lock->lock, 0, 1); 
    sem_init(&lock->writelock, 0, 1); 
    sem_init(&lock->writer_waiting, 0, 1); 
}

void rwlock_acquire_readlock(rwlock_t *lock) {
    sem_wait(&lock->writer_waiting);
    sem_wait(&lock->lock);
    lock->readers++;

    if (lock->readers == 1)
	    sem_wait(&lock->writelock); // first reader acquires writelock

    sem_post(&lock->lock);
    sem_post(&lock->writer_waiting);
}

void rwlock_release_readlock(rwlock_t *lock) {
    sem_wait(&lock->lock);
    lock->readers--;

    if (lock->readers == 0)
	    sem_post(&lock->writelock); // last reader releases writelock

    sem_post(&lock->lock);
}

void rwlock_acquire_writelock(rwlock_t *lock) {
    sem_wait(&lock->writer_waiting);
    sem_wait(&lock->writelock);
}

void rwlock_release_writelock(rwlock_t *lock) {
    sem_post(&lock->writelock);
    sem_post(&lock->writer_waiting);
    
}

