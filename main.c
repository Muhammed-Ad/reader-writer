#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include "readerwriter.h"

int read_loops;
int write_loops;
volatile int counter = 0;
struct _rwlock_t *lock;
rwlock_t mutex;

int main(){
    //variables
    lock = malloc(sizeof(struct _rwlock_t));
    char rw;
    FILE *file;
    int error;

    //open inputfile
    file = fopen("scenarios.txt", "r");

    //initialize lock
    rwlock_init(lock);
    rwlock_init(&mutex);

    //for threads
    pthread_t thread;

    if (file){
        //scan input file
        
        while (fscanf(file, "%c", &rw) != EOF){
            if (rw == 'r'){
                //if read create a thread to run the readThread
                printf("Create reader.\n");
                error = pthread_create(&thread, NULL, (void *)reader, (void *)&lock);
                if (error != 0){
                    printf("Can't create thread.\n");
                    return 1;
                }
            }

            else if (rw == 'w'){
                //if write create a thread to run the writeThread
                printf("Create writer.\n");
                error = pthread_create(&thread, NULL, (void *)writer, (void *)&lock);
                if (error != 0){
                    printf("Can't create thread.\n");
                    return 1;
                }
            }

            else if (rw == '\n'){
                pthread_join(thread, NULL);
                counter = 0;
                printf("\n\n\nNEW SCENARIO: \n\n\n\n");
                printf("Scenario Starts:\n");
            }
        }
    }

    //close file
    fclose(file);

    //exit threads
    pthread_exit(NULL);

    return 0;
}

void *reader(void *arg) {
    rwlock_acquire_readlock(lock);

    printf("Reader's is in... reading\n");
    int reader_num = counter;
    printf("Finished Reading: %d\n\n", reader_num);

    rwlock_release_readlock(lock);
    
    return NULL;
}

void *writer(void *arg) {
    rwlock_acquire_writelock(lock);

    printf("Writer in... writing: counter from %d to %d\n", counter, counter + 1);
    counter++;
    printf("Finished Writing\n\n");

    rwlock_release_writelock(lock);
    
    return NULL;
}