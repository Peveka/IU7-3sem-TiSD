#include "performance_analyzer.h"
#include "array_queue.h"
#include "linked_queue.h"
#include <sys/time.h>

size_t get_circular_buffer_memory_footprint(void)
{
    return sizeof(circular_buffer_queue_t);
}

size_t get_linked_queue_memory_footprint(size_t element_count)
{
    return element_count * sizeof(queue_node_t);
}

void display_memory_comparison_header(void)
{
    printf("\n СРАВНЕНИЕ ИСПОЛЬЗОВАНИЯ ПАМЯТИ (байт) \n");
    printf("==========================================================\n");
    printf("│   Размер очереди │   Кольцевой буфер│   Связный список │\n");
}

void display_memory_comparison_row(size_t count, size_t array_memory, size_t list_memory)
{
    printf("==========================================================\n");
    printf("│ %16zu │ %16zu │ %16zu │\n", count, array_memory, list_memory);
}

void analyze_memory_usage(void) {
    printf("\n--- ТЕСТИРОВАНИЕ ПАМЯТИ ---\n");
    printf("Размер | Массив | Список\n");
    printf("-------------------------\n");
    
    for (int i = 0; i <= 1000; i += 50) {
        size_t array_memory = get_circular_buffer_memory_footprint();
        size_t list_memory = get_linked_queue_memory_footprint(i);
        printf("%6d | %6zu | %6zu\n", i, array_memory, list_memory);
    }
    printf("-------------------------\n");
}

void analyze_time_performance(void) {
    circular_buffer_queue_t array_queue;
    initialize_circular_buffer(&array_queue);
    
    dynamic_linked_queue_t list_queue;
    initialize_linked_queue(&list_queue);
    
    struct timeval start_time, end_time;
    int64_t elapsed_time = 0;
    
    printf("\n--- ТЕСТИРОВАНИЕ СКОРОСТИ ---\n");
    printf("Размер | Массив(доб) | Массив(уд) | Список(доб) | Список(уд)\n");
    printf("-------------------------------------------------------------\n");
    
    int step_multiplier = 1;
    bool use_large_step = false;
    
    for (int queue_size = 0; queue_size <= 10000;) {
        int64_t array_enqueue_total = 0, array_dequeue_total = 0;
        int64_t list_enqueue_total = 0, list_dequeue_total = 0;
        
        for (int iteration = 0; iteration < 100; iteration++) {
            // Измерение времени для массива
            for (int j = 0; j < queue_size; j++) {
                gettimeofday(&start_time, NULL);
                enqueue_circular_buffer(&array_queue, j);
                gettimeofday(&end_time, NULL);
                elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000LL + (end_time.tv_usec - start_time.tv_usec);
                array_enqueue_total += elapsed_time;
            }
            
            for (int j = 0; j < queue_size; j++) {
                gettimeofday(&start_time, NULL);
                dequeue_circular_buffer(&array_queue);
                gettimeofday(&end_time, NULL);
                elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000LL + (end_time.tv_usec - start_time.tv_usec);
                array_dequeue_total += elapsed_time;
            }
            
            // Измерение времени для списка
            for (int j = 0; j < queue_size; j++) {
                gettimeofday(&start_time, NULL);
                enqueue_linked(&list_queue, j);
                gettimeofday(&end_time, NULL);
                elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000LL + (end_time.tv_usec - start_time.tv_usec);
                list_enqueue_total += elapsed_time;
            }
            
            for (int j = 0; j < queue_size; j++) {
                gettimeofday(&start_time, NULL);
                dequeue_linked(&list_queue);
                gettimeofday(&end_time, NULL);
                elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000LL + (end_time.tv_usec - start_time.tv_usec);
                list_dequeue_total += elapsed_time;
            }
            
            cleanup_linked_queue(&list_queue);
        }
        
        if (queue_size > 0) {
            array_enqueue_total /= 98;
            array_dequeue_total /= 98;
            list_enqueue_total /= 98;
            list_dequeue_total /= 98;
        }
        
        printf("%6d | %11" PRId64 " | %10" PRId64 " | %11" PRId64 " | %10" PRId64 "\n", 
               queue_size, array_enqueue_total, array_dequeue_total, list_enqueue_total, list_dequeue_total);
        
        // Изменение шага
        if (queue_size == 0) {
            queue_size = 10;
        } else {
            if (!use_large_step) {
                queue_size *= 2;
                use_large_step = true;
            } else {
                queue_size = (queue_size / 2) * 5;
                use_large_step = false;
            }
        }
    }
    printf("-------------------------------------------------------------\n");
}