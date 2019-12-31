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
#include <limits.h>

char *filename;

int main(int argc, char *argv[]){
    if(!argv[1]){
        fprintf(stderr, "USAGE: %s filename\n", argv[0]);
        return EXIT_FAILURE;
    } else if(argv[2]){
        fprintf(stderr, "Error: Demasiados argumentos\n");
        return EXIT_FAILURE;
    } else {
        filename = argv[1];
        int fd;
        if(fd = open(filename, O_RDONLY) >= 0){
            struct my_stack *stack = my_stack_read(filename);
            int value;
            
            int i = 0;
            int max = 0;
            int min = INT_MAX;
            int sum = 0;

            printf("Stack length: %d\n", my_stack_len(stack));
            while(value = my_stack_pop(stack) && i < 10){
                printf("%d\n", value);
                if(value < min){
                    min = value;
                }
                if(value > max){
                    max = value;
                }
                sum += value;
                i++;
            }
            printf("Items: %d\t", i);
            printf("Sum: %d\t", sum);
            printf("Min: %d\t", min);
            printf("Max: %d\t", max);
            printf("Average: %d\n", sum/i);
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }
}