#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include "readerwriter.h"

int read_loops;
int write_loops;
volatile int counter;
struct _rwlock_t *lock;

int main(){
    //variables
    counter = 0;
    lock = malloc(sizeof(struct _rwlock_t));
    char rw;
    FILE *file;
    int error;

    //open inputfile
    file = fopen("scenarios.txt", "r");

    //initialize lock
    rwlock_init(lock);

    //for threads
    pthread_t thread;
    int thread_count = 0;
    if (file){
        //scan input file
        
        while (fscanf(file, "%c", &rw) != EOF){
            if (rw == 'r'){
                //if read create a thread to run the readThread
                printf("Create reader.\n");
                thread_count++;
                error = pthread_create(&thread, NULL, (void *)readThread, (void *)&lock);
                if (error != 0){
                    printf("Can't create thread.\n");
                    return 1;
                }
            }

            else if (rw == 'w'){
                //if write create a thread to run the writeThread
                printf("Create writer.\n");
                thread_count++;
                error = pthread_create(&thread, NULL, (void *)writeThread, (void *)&lock);
                if (error != 0){
                    printf("Can't create thread.\n");
                    return 1;
                }
            }

            // else if (rw == '\n'){
            //     while(thread_count > 0){
            //         pthread_join(thread, NULL);
            //         thread_count--;
            //     }
            //     counter = 0;
            //     printf("\n\n\nNEW SCENARIO: \n\n\n\n");
            //     printf("Scenario Starts:\n");
            // }
        }
    }

    //close file
    fclose(file);

    //exit threads
    pthread_exit(NULL);

    return 0;
}

void* readThread(void *arg) {
    rwlock_acquire_readlock(lock);

    printf("Reader's is in... reading\n");
    int reader_num = counter;
    timewaster();
    printf("Finished Reading: %d\n", reader_num);

    rwlock_release_readlock(lock);
    
    return NULL;
}

void* writeThread(void *arg) {
    rwlock_acquire_writelock(lock);

    printf("Writer's in... writing\n");
    counter++;
    timewaster();
    printf("Finished Writing: from %d to %d\n", counter - 1, counter);

    rwlock_release_writelock(lock);
    
    return NULL;
}

void timewaster(){
    int T = rand() % 10000;  

    for(int i = 0; i < T; i++)  
        for(int j = 0; j < T; j++){
            int x = i * j;
            x *= x;
        }   
           

}