#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct stack_elem_t
{
    void *data;
    char letter;
} stack_elem_t;

typedef struct stack_t
{
    stack_elem_t *data;
    int capacity;
    int size;
} stack_t;

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

void compare_growth_strategies();

int stack_init(stack_t *stack)
{
    stack->data = (stack_elem_t*)malloc(INITIAL_CAPACITY * sizeof(stack_elem_t));
    if (stack->data == NULL)
        return 0;
    
    stack->capacity = INITIAL_CAPACITY;
    stack->size = 0;
    return 1;
}

void stack_free(stack_t *stack)
{
    if (stack->data != NULL)
    {
        free(stack->data);
        stack->data = NULL;
    }
    stack->capacity = 0;
    stack->size = 0;
}

int stack_resize(stack_t *stack, int new_capacity)
{
    stack_elem_t *new_data = (stack_elem_t*)realloc(stack->data, new_capacity * sizeof(stack_elem_t));
    if (new_data == NULL)
        return 0;
    
    stack->data = new_data;
    stack->capacity = new_capacity;
    return 1;
}

void add_elem_to_stack(stack_t *stack, char data)
{
    if (stack->size >= stack->capacity)
    {
        stack_resize(stack, stack->capacity * GROWTH_FACTOR);
    }
    
    for (int i = stack->size; i > 0; --i)
        stack->data[i] = stack->data[i-1];

    stack->data[0].data = NULL;
    stack->data[0].letter = data;
    stack->size++;
}

void pop_elem_from_stack(stack_t *stack, stack_elem_t *pop_elem)
{
    if (stack->size == 0)
        return;
    
    *pop_elem = stack->data[0];
    
    for (int i = 1; i < stack->size; ++i)
        stack->data[i-1] = stack->data[i];
    
    stack->size--;
    
    if (stack->size > 0 && stack->size < stack->capacity / (GROWTH_FACTOR * 2))
    {
        stack_resize(stack, stack->capacity / GROWTH_FACTOR);
    }
}

void clear_all_stack(stack_t *stack)
{
    stack->size = 0;
    stack_resize(stack, INITIAL_CAPACITY);
}

double test_add_operations(stack_t *stack, int num_operations)
{
    clock_t start = clock();
    for(int i = 0; i < num_operations; ++i)
        add_elem_to_stack(stack, 'a' + (i % 26));
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

double test_pop_operations(stack_t *stack, int num_operations)
{
    clock_t start = clock();
    stack_elem_t elem;
    for(int i = 0; i < num_operations; ++i)
        pop_elem_from_stack(stack, &elem);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

double test_mixed_operations(stack_t *stack, int num_operations)
{
    clock_t start = clock();
    stack_elem_t elem;
    
    for(int i = 0; i < num_operations; ++i)
    {
        if (i % 2 == 0)
            add_elem_to_stack(stack, 'a' + (i % 26));
        else
            pop_elem_from_stack(stack, &elem);
    }
    
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

double test_palindrome_check(stack_t *stack, char *word)
{
    int word_len = strlen(word);
    
    for (int i = 0; i < word_len; ++i)
        add_elem_to_stack(stack, word[i]);
    
    clock_t start = clock();
    
    stack_elem_t popped_elem;
    for (int i = 0; i < word_len; ++i)
        pop_elem_from_stack(stack, &popped_elem);
    
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
}

void test_memory_usage()
{
    printf("\n=== MEMORY USAGE ANALYSIS ===\n");
    
    stack_t stack;
    stack_init(&stack);
    
    printf("Initial state: capacity=%d, size=%d, memory=%.2f KB\n", 
           stack.capacity, stack.size, 
           (double)(stack.capacity * sizeof(stack_elem_t)) / 1024.0);
    
    int test_sizes[] = {10, 50, 100, 500, 1000, 5000, 10000};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    for (int t = 0; t < num_tests; t++)
    {
        int target_size = test_sizes[t];
        
        for (int i = stack.size; i < target_size; i++)
            add_elem_to_stack(&stack, 'a');
        
        printf("Size: %5d, Capacity: %5d, Memory: %6.2f KB, Utilization: %.1f%%\n",
               stack.size, stack.capacity,
               (double)(stack.capacity * sizeof(stack_elem_t)) / 1024.0,
               (double)stack.size / stack.capacity * 100.0);
    }
    
    printf("\n--- Testing memory reduction ---\n");
    while (stack.size > 0)
    {
        int prev_capacity = stack.capacity;
        stack_elem_t elem;
        pop_elem_from_stack(&stack, &elem);
        
        if (prev_capacity != stack.capacity)
        {
            printf("After pop: Size: %5d, Capacity: %5d, Memory: %6.2f KB\n",
                   stack.size, stack.capacity,
                   (double)(stack.capacity * sizeof(stack_elem_t)) / 1024.0);
        }
    }
    
    stack_free(&stack);
}

int main(void)
{
    const int REPS = 5;
    const int SIZES[] = {100, 1000, 5000, 10000, 50000};
    const int NUM_SIZES = sizeof(SIZES) / sizeof(SIZES[0]);
    
    printf("=== DYNAMIC STACK PERFORMANCE TESTS ===\n");
    printf("Testing with %d repetitions for each size\n\n", REPS);
    
    for (int s = 0; s < NUM_SIZES; s++)
    {
        int size = SIZES[s];
        printf("\n--- Testing with %d elements ---\n", size);
        
        stack_t stack;
        if (!stack_init(&stack))
        {
            printf("Memory allocation failed for size %d\n", size);
            continue;
        }
        
        double total_add_time = 0;
        for (int r = 0; r < REPS; r++)
        {
            total_add_time += test_add_operations(&stack, size);
            clear_all_stack(&stack);
        }
        printf("Add operations:    %8.3f ms (avg), Capacity: %d\n", 
               total_add_time / REPS, stack.capacity);

        double total_pop_time = 0;
        for (int r = 0; r < REPS; r++)
        {
            for (int i = 0; i < size; i++)
                add_elem_to_stack(&stack, 'a');
                
            total_pop_time += test_pop_operations(&stack, size);
            clear_all_stack(&stack);
        }
        printf("Pop operations:    %8.3f ms (avg), Capacity: %d\n", 
               total_pop_time / REPS, stack.capacity);
        
        double total_mixed_time = 0;
        for (int r = 0; r < REPS; r++)
        {
            total_mixed_time += test_mixed_operations(&stack, size);
            clear_all_stack(&stack);
        }
        printf("Mixed operations:  %8.3f ms (avg), Capacity: %d\n", 
               total_mixed_time / REPS, stack.capacity);
        
        char *test_word = (char*)malloc(size + 1);
        if (test_word)
        {
            for (int i = 0; i < size; i++)
                test_word[i] = 'a' + (i % 26);
            test_word[size] = '\0';
            
            double total_palin_time = 0;
            for (int r = 0; r < REPS; r++)
            {
                total_palin_time += test_palindrome_check(&stack, test_word);
                clear_all_stack(&stack);
            }
            printf("Palindrome check:  %8.3f ms (avg)\n", total_palin_time / REPS);
            
            free(test_word);
        }
        
        stack_free(&stack);
    }
    
    test_memory_usage();
    
    printf("\n=== GROWTH STRATEGY COMPARISON ===\n");
    compare_growth_strategies();
    
    return 0;
}

void compare_growth_strategies()
{
    const int TEST_SIZE = 10000;
    
    printf("Testing different growth strategies with %d elements:\n", TEST_SIZE);
    
    stack_t stack1;
    stack_init(&stack1);
    
    clock_t start1 = clock();
    for (int i = 0; i < TEST_SIZE; i++)
        add_elem_to_stack(&stack1, 'a');
    clock_t end1 = clock();
    
    printf("Growth factor 2:   %6.2f ms, Final capacity: %d, Reallocations: ~%d\n",
           (double)(end1 - start1) / CLOCKS_PER_SEC * 1000.0,
           stack1.capacity,
           (int)(log(TEST_SIZE / INITIAL_CAPACITY) / log(GROWTH_FACTOR)));
    
    stack_free(&stack1);
    
    stack_t stack2;
    stack2.data = (stack_elem_t*)malloc(INITIAL_CAPACITY * sizeof(stack_elem_t));
    stack2.capacity = INITIAL_CAPACITY;
    stack2.size = 0;
    
    clock_t start2 = clock();
    for (int i = 0; i < TEST_SIZE; i++)
    {
        if (stack2.size >= stack2.capacity)
        {
            stack_resize(&stack2, stack2.capacity + 100);
        }
        
        for (int j = stack2.size; j > 0; --j)
            stack2.data[j] = stack2.data[j-1];
        
        stack2.data[0].data = NULL;
        stack2.data[0].letter = 'a';
        stack2.size++;
    }
    clock_t end2 = clock();
    
    printf("Fixed growth 100:  %6.2f ms, Final capacity: %d, Reallocations: %d\n",
           (double)(end2 - start2) / CLOCKS_PER_SEC * 1000.0,
           stack2.capacity,
           (TEST_SIZE - INITIAL_CAPACITY) / 100);
    
    free(stack2.data);
}