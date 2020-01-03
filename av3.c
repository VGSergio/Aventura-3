/**
 * Authors:
 * Korn, Andreas Manuel     (X4890193W)
 * Márquez Cunill, Javier   (46396202G)
 * Vega García, Sergio      (43480752B)
 **/

#include "my_lib.c"
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
void increment_stack();

typedef struct str_thdata {
    int thread_no;
    char message[100];
} thdata;

int main(int argc, char *argv[]){
    if(getFileName(argv) == EXIT_SUCCESS){
        printf("Threads: %d, Iterations: %d\n", NUM_THREADS, N);
        int fd = open(filename, O_RDONLY);
        if(fd == -1){
            open(filename, O_CREAT);
        } else {
            stack = my_stack_read(filename);
        }
        stack = my_stack_init(INT_SIZE);
        int length = my_stack_len(stack);
        printf("stack->size: %d\n", stack->size);
        printf("initial stack lenght: %d\n", length);
        while(length < NUM_THREADS){
            int data = 0;
            my_stack_push(stack, &data);
            length++;
        }
        printf("final stack length: %d\n", my_stack_len(stack));
        
        for(int i=0; i<NUM_THREADS; i++){
            thdata data; strcpy(data.message, "Soy el hilo "); data.thread_no=i;
            pthread_create(&thread[i], NULL, (void *)increment_stack, NULL);
            //pthread_create(&thread[i], NULL, (void *)thread_start, (void *)&data);
        }

        for(int i=0; i<NUM_THREADS; i++){
            pthread_join(thread[i], NULL);
        }

        printf("Written elements from stack to file: %d\n", my_stack_write(stack, filename));   fflush(stdout);
        //printf("Released bytes: %d\n", my_stack_purge(stack));        fflush(stdout);
        printf("Bye from main\n");
        close(fd);
        pthread_exit(&mutex);
    } else {
        return EXIT_FAILURE;
    }
}

void increment_stack(){
    int *number;
    for(int i = 0; i < 5 && stack->first; i++){
        pthread_mutex_lock(&mutex);
        printf("Soy el hilo %ld ejecutando pop \n", pthread_self());
        number = my_stack_pop(stack);
        pthread_mutex_unlock(&mutex);
        if(number == NULL){
            pthread_exit(NULL);
        }

        fprintf(stdout, "numero = %d\n", *number);    fflush(stdout);
        *number++;
        fprintf(stdout, "numero = %d\n", *number);    fflush(stdout);

        pthread_mutex_lock(&mutex);
        printf("Soy el hilo %ld ejecutando push \n", pthread_self());
        my_stack_push(stack, number);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

static void *thread_start(void* ptr){
    thdata *data;
    data = (thdata *) ptr;
    printf("%d) Thread %ld created\n", data->thread_no, pthread_self());     fflush(stdout);
    for(int i=0; i<N; i++){
        pthread_mutex_lock(&mutex);
        //printf("%s%ld ejecutando pop\n", data->message, pthread_self());     fflush(stdout);
        int* value = my_stack_pop(stack);
        value++;
        
        //sleep(0.001);
        
        //printf("%s%ld ejecutando push\n", data->message, pthread_self());    fflush(stdout);
        my_stack_push(stack, value);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(thread);
}

int getFileName(char *argv[]){

    if(!argv[1]){
        fprintf(stderr, "USAGE: %s filename\n", argv[0]);
        return EXIT_FAILURE;
    } else if(argv[2]){
        fprintf(stderr, "Error: Demasiados argumentos\n");
        return EXIT_FAILURE;
    } else {
        filename = argv[1];
        return EXIT_SUCCESS;
    }
}