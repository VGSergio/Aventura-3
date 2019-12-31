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
#include <errno.h>      
#include <string.h>     

#define NUM_THREADS         10
#define N                   1000000

char *filename;
pthread_t thread[NUM_THREADS];
struct my_stack *stack;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int getFileName(char *args[]);
static void *thread_start(void* ptr);

typedef struct str_thdata {
    int thread_no;
    char message[100];
} thdata;

void main(int argc, char *argv[]){
    if(getFileName(argv)==1){
        printf("Threads: %d, Iterations: %d\n", NUM_THREADS, N);
        int fd = open(filename, O_RDONLY);
        if(fd == -1){
            open(filename, O_CREAT);
        } else {
            stack = my_stack_read(filename);
        }
        stack = my_stack_init(NUM_THREADS);
        int length = my_stack_len(stack);
        printf("stack->size: %d\n", stack->size);
        printf("initial stack lenght: %d\n", length);
        while(length < NUM_THREADS){
            int *data=0;
            my_stack_push(stack, data);
            length++;
        }
        printf("final stack length: %d\n", my_stack_len(stack));
        
        for(int i=0; i<NUM_THREADS; i++){
            thdata data; strcpy(data.message, "Soy el hilo "); data.thread_no=i;
            pthread_create(&thread[i], NULL, (void *)thread_start, (void *)&data);
        }
        for(int i=0; i<NUM_THREADS; i++){
            pthread_join(thread[i], NULL);
        }

        printf("Written elements from stack to file: %d\n", my_stack_len(stack));   fflush(stdout);
        printf("Released bytes: %d\n", my_stack_write(stack, filename));        fflush(stdout);
        printf("Bye from main\n");
        close(fd);
        pthread_exit(&mutex);
    }
}

static void *thread_start(void* ptr){
    thdata *data;
    data = (thdata *) ptr;
    printf("%d) Thread %ld created\n", data->thread_no, pthread_self());     fflush(stdout);
    for(int i=0; i<N; i++){
        pthread_mutex_lock(&mutex);
        //printf("%s%ld ejecutando pop\n", data->message, pthread_self());     fflush(stdout);
        int* value = (int)my_stack_pop(stack);
        value++;
        
        //sleep(0.001);
        
        //printf("%s%ld ejecutando push\n", data->message, pthread_self());    fflush(stdout);
        my_stack_push(stack, value);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(thread);
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