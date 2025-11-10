#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITERATIONS 10
#define TEST_SIZES 6

int test_sizes[TEST_SIZES] = {1000, 2000, 5000, 10000, 20000, 25000};

// ================== СТАТИЧЕСКИЙ МАССИВ ==================
#define MAX_STATIC_SIZE 30000

typedef struct 
{
    char data[MAX_STATIC_SIZE];
    int size;
} static_stack_t;

void static_init(static_stack_t *stack) 
{
    stack->size = 0;
}

void static_push(static_stack_t *stack, char data) 
{
    if (stack->size < MAX_STATIC_SIZE) 
    {
        for (int i = stack->size; i > 0; --i)
            stack->data[i] = stack->data[i-1];
        stack->data[0] = data;
        stack->size++;
    }
}

char static_pop(static_stack_t *stack) 
{
    if (stack->size > 0) 
    {
        char data = stack->data[0];
        for (int i = 1; i < stack->size; ++i)
            stack->data[i-1] = stack->data[i];
        stack->size--;
        return data;
    }
    return '\0';
}

int static_check_palindrome(static_stack_t *stack, const char *word) 
{
    int len = strlen(word);
    
    // Очищаем стек
    stack->size = 0;
    
    // Заполняем стек символами слова
    for (int i = 0; i < len; ++i) 
        static_push(stack, word[i]);
    
    // Проверяем палиндром
    for (int i = 0; i < len; ++i) 
    {
        char popped = static_pop(stack);
        if (word[i] != popped) 
            return 0;
    }
    return 1;
}

// ================== ДИНАМИЧЕСКИЙ МАССИВ ==================
typedef struct 
{
    char *data;
    int capacity;
    int size;
} dynamic_stack_t;

void dynamic_init(dynamic_stack_t *stack) 
{
    stack->capacity = 10;
    stack->data = malloc(stack->capacity);
    stack->size = 0;
}

void dynamic_free(dynamic_stack_t *stack) 
{
    free(stack->data);
}

void dynamic_push(dynamic_stack_t *stack, char data) 
{
    if (stack->size >= stack->capacity) 
    {
        stack->capacity *= 2;
        stack->data = realloc(stack->data, stack->capacity);
    }
    
    for (int i = stack->size; i > 0; --i)
        stack->data[i] = stack->data[i-1];
    stack->data[0] = data;
    stack->size++;
}

char dynamic_pop(dynamic_stack_t *stack) 
{
    if (stack->size > 0) 
    {
        char data = stack->data[0];
        for (int i = 1; i < stack->size; ++i)
            stack->data[i-1] = stack->data[i];
        stack->size--;
        return data;
    }
    return '\0';
}

int dynamic_check_palindrome(dynamic_stack_t *stack, const char *word) 
{
    int len = strlen(word);
    
    // Очищаем стек
    stack->size = 0;
    
    // Заполняем стек символами слова
    for (int i = 0; i < len; ++i) 
        dynamic_push(stack, word[i]);
    
    // Проверяем палиндром
    for (int i = 0; i < len; ++i) 
    {
        char popped = dynamic_pop(stack);
        if (word[i] != popped) 
            return 0;
    }
    return 1;
}

// ================== СВЯЗНЫЙ СПИСОК ==================
typedef struct list_node 
{
    void *data_ptr;
    char data;
    struct list_node *next;
} list_node_t;

typedef struct 
{
    list_node_t *head;
    int size;
} list_stack_t;

void list_init(list_stack_t *stack) 
{
    stack->head = NULL;
    stack->size = 0;
}

void list_free(list_stack_t *stack) 
{
    list_node_t *cur = stack->head;
    while (cur != NULL) 
    {
        list_node_t *next = cur->next;
        free(cur);
        cur = next;
    }
}

void list_push(list_stack_t *stack, char data) 
{
    list_node_t *new_node = malloc(sizeof(list_node_t));
    new_node->data = data;
    new_node->next = stack->head;
    stack->head = new_node;
    stack->size++;
}

char list_pop(list_stack_t *stack) 
{
    if (stack->head != NULL) 
    {
        list_node_t *temp = stack->head;
        char data = temp->data;
        stack->head = temp->next;
        stack->size--;
        free(temp);
        return data;
    }
    return '\0';
}

int list_check_palindrome(list_stack_t *stack, const char *word) 
{
    int len = strlen(word);
    
    // Очищаем стек
    list_free(stack);
    stack->head = NULL;
    stack->size = 0;
    
    // Заполняем стек символами слова
    for (int i = 0; i < len; ++i) 
        list_push(stack, word[i]);
    
    // Проверяем палиндром
    for (int i = 0; i < len; ++i) 
    {
        char popped = list_pop(stack);
        if (word[i] != popped) 
            return 0;
    }
    return 1;
}

// ================== ТЕСТИРОВАНИЕ ==================
void run_push_tests(void) 
{
    printf("PUSH OPERATIONS (ms)\n");
    printf("Size    | Static     | Dynamic    | List      \n");
    printf("--------|------------|------------|-----------\n");
    
    for (int s = 0; s < TEST_SIZES; ++s) 
    {
        int size = test_sizes[s];
        
        double static_time = 0;
        double dynamic_time = 0;
        double list_time = 0;
        
        for (int iter = 0; iter < ITERATIONS; ++iter) 
        {
            // Static push
            static_stack_t sstack;
            static_init(&sstack);
            clock_t start = clock();
            for (int i = 0; i < size; ++i) 
                static_push(&sstack, 'A');
            clock_t end = clock();
            static_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            
            // Dynamic push
            dynamic_stack_t dstack;
            dynamic_init(&dstack);
            start = clock();
            for (int i = 0; i < size; ++i) 
                dynamic_push(&dstack, 'A');
            end = clock();
            dynamic_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            dynamic_free(&dstack);
            
            // List push
            list_stack_t lstack;
            list_init(&lstack);
            start = clock();
            for (int i = 0; i < size; ++i) 
                list_push(&lstack, 'A');
            end = clock();
            list_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            list_free(&lstack);
        }
        
        printf("%-7d | %10.5f | %10.5f | %9.5f\n", 
               size, 
               static_time / ITERATIONS,
               dynamic_time / ITERATIONS, 
               list_time / ITERATIONS);
    }
}

void run_pop_tests(void) 
{
    printf("\nPOP OPERATIONS (ms)\n");
    printf("Size    | Static     | Dynamic    | List      \n");
    printf("--------|------------|------------|-----------\n");
    
    for (int s = 0; s < TEST_SIZES; ++s) 
    {
        int size = test_sizes[s];
        
        double static_time = 0;
        double dynamic_time = 0;
        double list_time = 0;
        
        for (int iter = 0; iter < ITERATIONS; ++iter) 
        {
            // Static pop
            static_stack_t sstack;
            static_init(&sstack);
            for (int i = 0; i < size; ++i) 
                static_push(&sstack, 'A');
            clock_t start = clock();
            for (int i = 0; i < size; ++i) 
                static_pop(&sstack);
            clock_t end = clock();
            static_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            
            // Dynamic pop
            dynamic_stack_t dstack;
            dynamic_init(&dstack);
            for (int i = 0; i < size; ++i) 
                dynamic_push(&dstack, 'A');
            start = clock();
            for (int i = 0; i < size; ++i) 
                dynamic_pop(&dstack);
            end = clock();
            dynamic_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            dynamic_free(&dstack);
            
            // List pop
            list_stack_t lstack;
            list_init(&lstack);
            for (int i = 0; i < size; ++i) 
                list_push(&lstack, 'A');
            start = clock();
            for (int i = 0; i < size; ++i) 
                list_pop(&lstack);
            end = clock();
            list_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            list_free(&lstack);
        }
        
        printf("%-7d | %10.5f | %10.5f | %9.5f\n", 
               size, 
               static_time / ITERATIONS,
               dynamic_time / ITERATIONS, 
               list_time / ITERATIONS);
    }
}

void run_palindrome_tests(void) 
{
    printf("\nPALINDROME CHECK (ms)\n");
    printf("Size    | Static     | Dynamic    | List      \n");
    printf("--------|------------|------------|-----------\n");
    
    for (int s = 0; s < TEST_SIZES; ++s) 
    {
        int size = test_sizes[s];
        
        // Создаем тестовое слово (не палиндром для худшего случая)
        char *word = malloc(size + 1);
        for (int i = 0; i < size; ++i) 
            word[i] = 'A' + (i % 26);
        word[size] = '\0';
        
        double static_time = 0;
        double dynamic_time = 0;
        double list_time = 0;
        
        for (int iter = 0; iter < ITERATIONS; ++iter) 
        {
            // Static palindrome
            static_stack_t sstack;
            static_init(&sstack);
            clock_t start = clock();
            static_check_palindrome(&sstack, word);
            clock_t end = clock();
            static_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            
            // Dynamic palindrome
            dynamic_stack_t dstack;
            dynamic_init(&dstack);
            start = clock();
            dynamic_check_palindrome(&dstack, word);
            end = clock();
            dynamic_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            dynamic_free(&dstack);
            
            // List palindrome
            list_stack_t lstack;
            list_init(&lstack);
            start = clock();
            list_check_palindrome(&lstack, word);
            end = clock();
            list_time += (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            list_free(&lstack);
        }
        
        printf("%-7d | %10.5f | %10.5f | %9.5f\n", 
               size, 
               static_time / ITERATIONS,
               dynamic_time / ITERATIONS, 
               list_time / ITERATIONS);
        
        free(word);
    }
}

void run_memory_tests(void) 
{
    printf("\nMEMORY USAGE (bytes)\n");
    printf("Size    | Static Used | Dynamic Used | List Used | Static Waste | Dynamic Waste | List Waste\n");
    printf("--------|-------------|--------------|-----------|--------------|---------------|-----------\n");
    
    for (int s = 0; s < TEST_SIZES; ++s) 
    {
        int size = test_sizes[s];
        
        // Static memory
        static_stack_t sstack;
        static_init(&sstack);
        for (int i = 0; i < size; ++i) 
            static_push(&sstack, 'A');
        size_t static_used = MAX_STATIC_SIZE;
        size_t static_waste = MAX_STATIC_SIZE - size;
        
        // Dynamic memory
        dynamic_stack_t dstack;
        dynamic_init(&dstack);
        for (int i = 0; i < size; ++i) 
            dynamic_push(&dstack, 'A');
        size_t dynamic_used = dstack.capacity;
        size_t dynamic_waste = dstack.capacity - size;
        dynamic_free(&dstack);
        
        // List memory
        size_t list_used = size * sizeof(list_node_t);
        size_t list_waste = size * (sizeof(list_node_t) - 1);
        
        printf("%-7d | %11zu | %12zu | %9zu | %12zu | %13zu | %9zu\n", 
               size, static_used, dynamic_used, list_used, 
               static_waste, dynamic_waste, list_waste);
    }
}

int main(void) 
{
    printf("SEPARATED OPERATIONS PERFORMANCE TEST\n");
    printf("=====================================\n");
    printf("Iterations: %d\n\n", ITERATIONS);
    
    run_push_tests();
    run_pop_tests(); 
    run_palindrome_tests();
    run_memory_tests();
    
    printf("\nNode size: %zu bytes\n", sizeof(list_node_t));
    printf("sizeof static_struct: %zu sizeof dynamic_struct: %zu sizeof list_struct: %zu", sizeof(static_stack_t), sizeof(dynamic_stack_t), sizeof(list_stack_t));
    return 0;
}