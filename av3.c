/**
 * Authors:
 * Korn, Andreas Manuel     (X4890193W)
 * Márquez Cunill, Javier   (46396202G)
 * Vega García, Sergio      (43480752B)
 **/

#include "my_lib.c"
#include "reader.c"
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS         3
#define N                   5

char *filename;
pthread_t thread[NUM_THREADS];
struct my_stack *stack;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]){
    int fd = getFileName(argv);
    if(fd==1){
        printf("Threads: %d, Iterations: %d\n", NUM_THREADS, N);
        if(open(filename, O_RDONLY) == -1){
            open(filename, O_CREAT);
        }
        stack = my_stack_read(filename);
        int length = my_stack_len(stack);
        printf("stack->size: %d\n", stack->size);
        printf("initial stack lenght: %d\n", my_stack_len(stack));
        while(length< 10){
            my_stack_push(stack, (void *)0);
            length++;
        }
        printf("final stack length: %d\n", my_stack_len(stack));
        my_stack_write(stack, filename);
        /*for(int i=0; i<NUM_THREADS; i++){
            printf("%d) ", i);
            pthread_create(&thread[i], NULL, thread_start, NULL);
            pthread_join(thread[i], NULL);
        }
        printf("Written elements from stack to file: %d", my_stack_len(stack));
        printf("Released bytes; %d", my_stack_purge(stack));
        my_stack_write(stack, filename);
        printf("Bye from main");
        pthread_exit(thread);*/
        close(fd);
    }
}

static void *thread_start(void *arg){
    printf("%ld created\n", pthread_self());
    for(int i=0; i<N; i++){
        pthread_mutex_lock(&mutex);
        printf("Soy el hilo %ld ejecutando pop\n", pthread_self());
        int value = my_stack_pop(stack);
        value++;
        
        sleep(0.001);
        
        pthread_mutex_unlock(&mutex);
        printf("Soy el hilo %ld ejecutando push\n", pthread_self());
        my_stack_push(stack, (void *)value);
    }
    pthread_exit(&mutex);
}

int getFileName(char *argv[]){
    char* file;
    if(!argv[1]){
        argv[2]=NULL;       // We have to do this because if the basic cd is the first command used, args[2] has no assigned value and may not be NULL, as it is not touched during parse_args
    }
    
    if(argv[2]){
        fprintf(stderr, "Error: Demasiados argumentos\n");
        return -1;
    } else if(!argv[1]){
        fprintf(stderr, "USAGE: ./av3 filename\n");
        return -1;
    } else {
        filename = argv[1];
        return 1;
    }
}