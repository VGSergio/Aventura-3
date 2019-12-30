/**
 * This program includes methods to work with my_string and my_stack.
 * 
 * Authors:
 * Korn, Andreas Manuel     (X4890193W)
 * Márquez Cunill, Javier   (46396202G)
 * Vega García, Sergio      (43480752B)
 **/
#include "my_lib.h"

#define INT_SIZE    4

/**
 * Counts the length of the string received by parameter.
 * @param   - char *str:    the string which to count
 * @return  - the length in size_t
 **/
size_t my_strlen(const char *str){
    int i = 0;
    while(str[i]){
        i++; 
    }
    return i;
}

/**
 * Compares two strings.
 * @param   - char *str1:   the first string to compare
 * @param   - char *str2:   the second string to compare
 * @return  - the difference between the two first different characters, 
 *            if str1 is bigger, it will be a number lower than 0, and viceversa.
 **/
int my_strcmp(const char *str1, const char *str2){
    int i = 0;
    while( (str1[i] == str2[i]) && str1[i] && str2[i]){ // Goes through the string while they're the same. 
        i++;
    }
    return str1[i]-str2[i];                             // The difference between the first different letter is returned.
}

/**
 * Copies the content of the source string into the destiny string. This method does not manage 
 * information bleeding in the case the source is longer than the destiny.
 * @param   - char *dest:   the string in which to copy.
 * @param   - char *src:    the string that has to be copied.
 * @return  - the address of dest
 **/
char *my_strcpy(char *dest, const char *src){
    int i = 0;
    while(src[i]){                                      // Goes through source, copying its contents into destiny.
        dest[i] = src[i]; 
        i++;
    }
    dest[i] = 0;                                        // After copying, we add a 0 to mark the end of the string.
    return dest;
}

/**
 * Copies the content of the source string into the destiny string up to the nth letter.
 * This method does not manage information bleeding in case the length of dest is shorter than n, nor does it add 
 * the '\0' terminator on dest.
 * @param   - char *dest:   the string in which to copy.
 * @param   - char *src:    the string that has to be copied up to the nth letter.
 * @param   - size_t n:     the number of letters from src to copy into dest.
 * @return  - the address of dest.
 **/
char *my_strncpy(char *dest, const char *src, size_t n){
    for(int i = 0; i < n; i++){                         // While we're not at n, we copy the source's contents into destiny.
        dest[i] = src[i];
    }
    return dest;
}

/**
 * Concatenates two strings, and adds the '\0' terminator.
 * @param   - char *dest:   the string in which to copy. Its contents will be the first on the resulting string.
 * @param   - char *src:    the string that includes the contents which will be concatenated after the contents of dest.
 * @return  - the address of dest.
 **/
char *my_strcat(char *dest, const char *src){
    int i = 0;
    while(dest[i]){                                     // We travel through the destiny string until we arrive at the end.
        i++;
    }
    my_strcpy(&dest[i],src);                            // When we're at the end, we send the address of the point in which to start copying src's contents
    return dest;
}

/**
 * Initiates the struct my_stack which will contain data from the size passed by parameter.
 * @param   - int size:     the size of the data that will be contained on the stack.
 * @return  - the pointer to the stack.
 **/
struct my_stack *my_stack_init (int size){
    struct my_stack *stack = malloc(sizeof(struct my_stack)); // We allocate memory for the stack.
    if(!stack){
        return NULL;                                          // In case the malloc was not possible, we return NULL.
    }
    stack->first = NULL;                                      // First is initialized to NULL, as there are yet no nodes in the stack.
    stack->size = size;
    return stack;
}

/**
 * Pushes data into the stack and marks it as the stack's first position.
 * @param   - struct my_stack *stack:   the pointer to the stack.
 * @param   - void *data:   the pointer to the data to push.
 * @return  - 0 if all went correctly, -1 otherwise.
 **/
int my_stack_push(struct my_stack *stack, void *data){
    if(stack && stack->size > 0){
        struct my_stack_node *node = malloc(sizeof(struct my_stack_node)); // We allocate memory for my_stack_node
        if(!node){
            return -1;
        }
        node->next = stack->first;                                         // The old first (the stack's top) is the new node's next.
        node->data = data;
        stack->first = node;                                               // The new first is the direction of the node we just created.
        return 0;
    }else{
        return -1;
    }
}

/**
 * Pops the last added data out of the stack and returns its pointer.
 * @param   - struct my_stack *stack:   the pointer to the stack to pop.
 * @return  - the address of the data.
 **/
void *my_stack_pop(struct my_stack *stack){
    if(!stack->first) return NULL;                  // When the stack is empty, we return NULL
    struct my_stack_node *aux = stack->first;       // We use an auxiliary pointer which points to the stack's first
    stack->first = aux->next;                       // We update the stack's "first" pointer to point to the new node.
    void *auxData = aux->data;                      // We use auxiliary pointer to data, so we don't lose its address after freeing aux.
    free(aux);
    return auxData;
}

/**
 * Counts the number of nodes of the stack, without modifying anything.
 * @param   - struct my_stack *stack:   the pointer to the stack.
 * @return  - the number of nodes of the stack, in int.
 **/
int my_stack_len(struct my_stack *stack){
    if(!stack->first) return 0;
    struct my_stack_node *node = stack->first;
    int i = 1;
    while(node->next){
        i++;
        node = node->next;                          // We go through the nodes on the stack while counting them. Nothing is modified.
    }
    return i;
}

/**
 * Frees the allocated memory from the stack, all its nodes and all its contents.
 * @param   - struct my_stack *stack:   the stack to purge.
 * @return  - the number of freed bytes.
 **/
int my_stack_purge(struct my_stack *stack){
    int freedSpace = 0;
    while(stack->first){
        freedSpace += sizeof(*stack->first);
        void *data = my_stack_pop(stack);           // As my_stack_pop already frees the space of the nodes, we just use pop to empty the stack.
        freedSpace += stack->size;
        free(data);
    }
    freedSpace += sizeof(*stack);
    free(stack);
    return freedSpace;
}

/**
 * Writes the contents of the stack into the given file. The original stack is mantained as is.
 * @param   - struct my_stack *stack:   the stack to write into the file.
 * @param   - char *filename:           the string containing the name of the file.
 * @return  - The number of nodes written into the file.
 **/
int my_stack_write(struct my_stack *stack, char *filename){
    // We prepare the inverted auxiliary stack which will be written on the file.
    if(!stack) return -1;
    struct my_stack *aux = my_stack_init(stack->size);
    struct my_stack_node *node = stack->first;
    while(node->next){
        my_stack_push(aux, node->data);
        node = node->next;
    }
    my_stack_push(aux, node->data);

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    write(fd, &aux->size, INT_SIZE);                                            // We write the integer which contains the size of the stack's node's data.
    int counter = 0;
    while(aux->first){
        write(fd, my_stack_pop(aux), aux->size);
        counter++;
    }

    my_stack_purge(aux);                                                        // The aux stack is purged, as it is not needed anymore.
    close(fd);
    return counter;
}

/**
 * Reads the file that contains a stack and makes a stack containing its information.
 * @param   - char *filename:   the name of the file to read.
 * @return  - the pointer to the read stack.
 **/
struct my_stack *my_stack_read(char *filename){
    int fd = open(filename, O_RDONLY);
    int size;
    read(fd, &size, INT_SIZE);                                                  // We read the size of the data as it is the first information on the file and initialize the stack with it.
    struct my_stack *stack = my_stack_init(size);
    
    void *data = malloc(size);
    read(fd, data, size);
    while(data){
        my_stack_push(stack, data);
        void *auxdata = malloc(size);
        data = auxdata;
        int a = read(fd, data, size);
        if(!a) break;                                                           // if a is 0, then that marks the end of the file and we break the loop. We couldn't use data
                                                                                // because it is a pointer and it never got the value of NULL.
    }

    close(fd);
    return stack;
}