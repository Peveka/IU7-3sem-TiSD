#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define SIZE 1000000
#define REPS 1

typedef struct stack_elem_t
{
    void *data;
    char letter;
} stack_elem_t;

typedef struct stack_t
{
    stack_elem_t data[SIZE];
    int max_size;                         
    int size;                
} stack_t;

static void elem_cpy(stack_elem_t *dst, stack_elem_t *src)
{
    dst->data = src->data;
    dst->letter = src->letter;
}

void pop_elem_from_stack(stack_t *stack, stack_elem_t *pop_elem)
{
    if (stack->size != 0)
    {
        elem_cpy(pop_elem, &(stack->data)[0]);
        for(int i = 1; i < stack->size; ++i)
            (stack->data)[i-1] = (stack->data)[i];
        (stack->size)--;
    }
}

void clear_all_stack(stack_t *stack)
{
    stack->size = 0; 
}

void add_elem_to_stack(stack_t *stack, char data)
{
    if (stack->size < stack->max_size)
    {
        for (int i = stack->size; i >= 0; --i)
            (stack->data)[i+1] = (stack->data)[i];

        stack->data[0].data = NULL;
        stack->data[0].letter = data;
        (stack->size)++;
    }
}

void check_palindrome(stack_t *stack, char *word)
{
    int word_len = strlen(word);
    
    for (int i = 0; i < word_len; ++i)
    {
        add_elem_to_stack(stack, word[i]);
    }
    
    // Затем проверяем
    for (int i = 0; i < word_len; ++i)
    {
        stack_elem_t popped_elem;
        pop_elem_from_stack(stack, &popped_elem);
        if (toupper(word[i]) != toupper(popped_elem.letter))
        {
            break;
        }
    }

    clear_all_stack(stack);
}

double check_pop(stack_t *stack)
{
    clock_t start = clock();
    stack_elem_t elem;
    for (int i = 0; i < SIZE; ++i)
        pop_elem_from_stack(stack, &elem);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000;
}

double check_add(stack_t *stack)
{
    clock_t start = clock();
    for(int i = 0; i < SIZE; ++i)
        add_elem_to_stack(stack, 'a');
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000; 
}

double check_palin(stack_t *stack)
{
    char word[SIZE+1];
    for(int i = 0; i < SIZE; ++i)
        word[i] = 'a';
    word[SIZE] = '\0';
    
    clock_t start = clock();
    check_palindrome(stack, word);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000;
}

stack_t stack = {.max_size = SIZE, .size = 0};

int main(void)
{
    
    double time_add = 0;
    double time_pop = 0;
    double time_palin = 0;
    
    for (int i = 0; i < REPS; ++i)
    {
        time_add += check_add(&stack);
        clear_all_stack(&stack);
    }
    printf("Average time to add function: %.3f ms (for %d elems, %d reps)\n", 
           time_add / REPS, SIZE, REPS);

    for(int i = 0; i < SIZE; ++i)
        add_elem_to_stack(&stack, 'a');
        
    for (int i = 0; i < REPS; ++i)
    {
        time_pop += check_pop(&stack);
        for(int j = stack.size; j < SIZE; ++j)
            add_elem_to_stack(&stack, 'a');
    }
    printf("Average time to pop function: %.3f ms (for %d elems, %d reps)\n", 
           time_pop / REPS, SIZE, REPS);

    clear_all_stack(&stack);
    
    for (int i = 0; i < REPS; ++i)
        time_palin += check_palin(&stack);
    printf("Average time to palin function: %.3f ms (for %d elems, %d reps)\n", 
           time_palin / REPS, SIZE, REPS);

    return 0;
}