#include "../inc/simulation_core.h"
#include "../inc/array_queue.h"
#include "../inc/linked_queue.h"
#include <sys/time.h>
#include <time.h>

// Добавляем глобальные переменные для хранения метрик
static performance_metrics_t array_perf_metrics;
static performance_metrics_t list_perf_metrics;

void reset_performance_metrics(performance_metrics_t *metrics)
{
    metrics->simulation_time = 0.0;
    metrics->real_execution_time = 0.0;
    metrics->peak_memory_usage = 0;
    metrics->total_memory_operations = 0;
    metrics->max_queue_size = 0;
    metrics->total_enqueue_ops = 0;
    metrics->total_dequeue_ops = 0;
}

void print_performance_comparison(const performance_metrics_t *array_metrics,
                                  const performance_metrics_t *list_metrics)
{
    printf("\n════════════════ СРАВНИТЕЛЬНЫЙ АНАЛИЗ ЭФФЕКТИВНОСТИ ════════════════\n");

    // Время выполнения
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                      ВРЕМЯ ВЫПОЛНЕНИЯ                       ║\n");
    printf("╠══════════════════════════════════╦═══════════════════════════╣\n");
    printf("║         Показатель               ║   Массив   │   Список    ║\n");
    printf("╠══════════════════════════════════╬════════════╪═════════════╣\n");
    printf("║ Виртуальное время моделирования  ║ %10.2f │ %10.2f  ║\n",
           array_metrics->simulation_time, list_metrics->simulation_time);
    printf("║ Реальное время выполнения (сек)  ║ %10.4f │ %10.4f  ║\n",
           array_metrics->real_execution_time, list_metrics->real_execution_time);
    printf("╚══════════════════════════════════╩════════════╧═════════════╝\n\n");

    // Использование памяти
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                 ИСПОЛЬЗОВАНИЕ ПАМЯТИ                        ║\n");
    printf("╠══════════════════════════════════╦════════════╪═════════════╣\n");
    printf("║         Показатель               ║   Массив   │   Список    ║\n");
    printf("╠══════════════════════════════════╬════════════╪═════════════╣\n");
    printf("║ Пиковое использование (байт)     ║ %10zu │ %10zu  ║\n",
           array_metrics->peak_memory_usage, list_metrics->peak_memory_usage);
    printf("║ Максимальная длина очереди       ║ %10d │ %10d  ║\n",
           array_metrics->max_queue_size, list_metrics->max_queue_size);
    printf("║ Операций с памятью               ║ %10zu │ %10zu  ║\n",
           array_metrics->total_memory_operations, list_metrics->total_memory_operations);
    printf("╚══════════════════════════════════╩════════════╧═════════════╝\n\n");

    // Операции с очередью
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║               ОПЕРАЦИИ С ОЧЕРЕДЬЮ                           ║\n");
    printf("╠══════════════════════════════════╦════════════╪═════════════╣\n");
    printf("║         Показатель               ║   Массив   │   Список    ║\n");
    printf("╠══════════════════════════════════╬════════════╪═════════════╣\n");
    printf("║ Операций добавления              ║ %10lld │ %10lld  ║\n",
           array_metrics->total_enqueue_ops, list_metrics->total_enqueue_ops);
    printf("║ Операций удаления                ║ %10lld │ %10lld  ║\n",
           array_metrics->total_dequeue_ops, list_metrics->total_dequeue_ops);
    printf("╚══════════════════════════════════╩════════════╧═════════════╝\n");
    printf("══════════════════════════════════════════════════════════════════\n");

    // Анализ эффективности
    printf("\nАНАЛИЗ ЭФФЕКТИВНОСТИ:\n");

    // Время
    if (array_metrics->real_execution_time < list_metrics->real_execution_time)
    {
        double speedup = (list_metrics->real_execution_time / array_metrics->real_execution_time) * 100;
        printf("• Массив быстрее списка на %.1f%%\n", speedup - 100);
    }
    else
    {
        double speedup = (array_metrics->real_execution_time / list_metrics->real_execution_time) * 100;
        printf("• Список быстрее массива на %.1f%%\n", speedup - 100);
    }

    // Память
    if (array_metrics->peak_memory_usage < list_metrics->peak_memory_usage)
    {
        double memory_saving = ((double)list_metrics->peak_memory_usage /
                                    array_metrics->peak_memory_usage -
                                1) *
                               100;
        printf("• Массив использует на %.1f%% меньше памяти\n", memory_saving);
    }
    else
    {
        double memory_saving = ((double)array_metrics->peak_memory_usage /
                                    list_metrics->peak_memory_usage -
                                1) *
                               100;
        printf("• Список использует на %.1f%% меньше памяти\n", memory_saving);
    }

    // Фрагментация памяти
    printf("• Фрагментация памяти:\n");
    printf("  - Массив: минимальная (статическое выделение)\n");
    printf("  - Список: возможна (динамическое выделение/освобождение)\n");
}

void run_array_based_simulation(const simulation_config_t *config)
{
    printf("\n════════════════ ЗАПУСК СИМУЛЯЦИИ (КОЛЬЦЕВОЙ БУФЕР) ════════════════\n");

    // Начало замера времени
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    clock_t cpu_start = clock();

    reset_performance_metrics(&array_perf_metrics);

    double next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
    double remaining_processing_time = 0.0;
    int current_processing_cycles = 0, new_request_cycles;
    int is_processor_busy = 0;
    int operation_result = OPERATION_SUCCESS;

    simulation_metrics_t metrics;
    circular_buffer_queue_t queue;
    reset_simulation_metrics(&metrics);
    initialize_circular_buffer(&queue);

    metrics.total_arrival_time += next_arrival_time;

    int next_output_step = config->output_frequency;

    while (metrics.completed_requests < config->total_requests && operation_result == OPERATION_SUCCESS)
    {
        // Обновляем максимальный размер очереди
        if (queue.size > array_perf_metrics.max_queue_size)
        {
            array_perf_metrics.max_queue_size = queue.size;
        }

        // Рассчитываем использование памяти для массива
        // Для статического массива: размер массива + служебные данные
        size_t current_memory = sizeof(circular_buffer_queue_t);
        if (current_memory > array_perf_metrics.peak_memory_usage)
        {
            array_perf_metrics.peak_memory_usage = current_memory;
        }

        if (is_processor_busy == 0)
        {
            if (queue.size > 0)
            {
                current_processing_cycles = *(queue.front);
                dequeue_circular_buffer(&queue);
                array_perf_metrics.total_dequeue_ops++;

                remaining_processing_time = generate_random_time(config->min_processing_time, config->max_processing_time);
                metrics.total_processing_time += remaining_processing_time;
                is_processor_busy = 1;
                metrics.processing_operations++;
            }
            else
            {
                metrics.total_simulation_time += next_arrival_time;
                metrics.idle_time += next_arrival_time;

                current_processing_cycles = 0;
                operation_result = enqueue_circular_buffer(&queue, current_processing_cycles);
                array_perf_metrics.total_enqueue_ops++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                metrics.total_arrival_time += next_arrival_time;
                metrics.incoming_requests++;
            }
        }
        else
        {
            if (compare_doubles(next_arrival_time, remaining_processing_time) > 0)
            {
                next_arrival_time -= remaining_processing_time;
                metrics.total_simulation_time += remaining_processing_time;
                remaining_processing_time = 0.0;
                is_processor_busy = 0;
                current_processing_cycles++;

                if (current_processing_cycles < config->processing_cycles)
                {
                    operation_result = enqueue_circular_buffer(&queue, current_processing_cycles);
                    array_perf_metrics.total_enqueue_ops++;
                }
                else
                {
                    metrics.completed_requests++;
                    if (metrics.completed_requests > 0)
                    {
                        metrics.average_queue_size += queue.size;
                    }
                }
            }
            else
            {
                if (compare_doubles(remaining_processing_time, next_arrival_time) > 0)
                {
                    metrics.average_waiting_time += (remaining_processing_time - next_arrival_time) * queue.size;
                }

                remaining_processing_time -= next_arrival_time;
                metrics.total_simulation_time += next_arrival_time;

                new_request_cycles = 0;
                operation_result = enqueue_circular_buffer(&queue, new_request_cycles);
                array_perf_metrics.total_enqueue_ops++;
                metrics.incoming_requests++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                metrics.total_arrival_time += next_arrival_time;
            }

            if (metrics.completed_requests == next_output_step && metrics.completed_requests > 0)
            {
                metrics.current_queue_size = queue.size;
                display_intermediate_results(&metrics, config, next_output_step);
                next_output_step += config->output_frequency;
            }
        }
    }

    // Конец замера времени
    gettimeofday(&end_time, NULL);
    clock_t cpu_end = clock();

    double real_time_elapsed = (end_time.tv_sec - start_time.tv_sec) +
                               (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    double cpu_time_used = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    array_perf_metrics.simulation_time = metrics.total_simulation_time;
    array_perf_metrics.real_execution_time = real_time_elapsed;

    // Увеличиваем пиковое использование памяти для учета служебных данных
    array_perf_metrics.peak_memory_usage += sizeof(simulation_metrics_t) + sizeof(circular_buffer_queue_t);

    if (operation_result == QUEUE_OVERFLOW)
    {
        printf("Ошибка: переполнение очереди!\n");
    }

    if (operation_result == OPERATION_SUCCESS)
    {
        display_final_results(&metrics, config);

        printf("\n════════════════ ЭФФЕКТИВНОСТЬ (МАССИВ) ════════════════\n");
        printf("Реальное время выполнения: %.6f сек\n", real_time_elapsed);
        printf("Процессорное время: %.6f сек\n", cpu_time_used);
        printf("Пиковое использование памяти: %zu байт\n", array_perf_metrics.peak_memory_usage);
        printf("Максимальная длина очереди: %d\n", array_perf_metrics.max_queue_size);
        printf("Операций добавления: %lld\n", array_perf_metrics.total_enqueue_ops);
        printf("Операций удаления: %lld\n", array_perf_metrics.total_dequeue_ops);
        printf("Фрагментация памяти: минимальная (статический массив)\n");
        printf("═══════════════════════════════════════════════════════\n");
    }
}

void run_list_based_simulation(const simulation_config_t *config, memory_tracker_t *tracker)
{
    printf("\n════════════════ ЗАПУСК СИМУЛЯЦИИ (СВЯЗНЫЙ СПИСОК) ════════════════\n");

    // Начало замера времени
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    clock_t cpu_start = clock();

    reset_performance_metrics(&list_perf_metrics);

    double next_arrival_time, remaining_processing_time = 0.0;
    int current_processing_cycles = 0, new_request_cycles;
    simulation_metrics_t metrics;
    reset_simulation_metrics(&metrics);

    int is_processor_busy = 0;
    int operation_result = OPERATION_SUCCESS;

    dynamic_linked_queue_t queue;
    initialize_linked_queue(&queue);

    next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
    metrics.total_arrival_time += next_arrival_time;

    int next_output_step = config->output_frequency;

    while (metrics.completed_requests < config->total_requests && operation_result == OPERATION_SUCCESS)
    {
        // Обновляем максимальный размер очереди
        if (queue.size > list_perf_metrics.max_queue_size)
        {
            list_perf_metrics.max_queue_size = queue.size;
        }

        // Рассчитываем использование памяти для списка
        // Каждый узел: данные (int) + указатель (queue_node_t*)
        size_t node_size = sizeof(queue_node_t);
        size_t current_memory = queue.size * node_size + sizeof(dynamic_linked_queue_t);

        // Учитываем фрагментацию (грубая оценка: добавляем 10% на накладные расходы malloc)
        current_memory = (size_t)(current_memory * 1.1);

        if (current_memory > list_perf_metrics.peak_memory_usage)
        {
            list_perf_metrics.peak_memory_usage = current_memory;
        }

        // Увеличиваем счетчик операций с памятью
        list_perf_metrics.total_memory_operations++;

        if (is_processor_busy == 0)
        {
            if (queue.size > 0)
            {
                current_processing_cycles = queue.front->data;
                record_deallocation(tracker, (void *)queue.front);
                dequeue_linked(&queue);
                list_perf_metrics.total_dequeue_ops++;

                remaining_processing_time = generate_random_time(config->min_processing_time, config->max_processing_time);
                metrics.total_processing_time += remaining_processing_time;
                is_processor_busy = 1;
                metrics.processing_operations++;
            }
            else
            {
                metrics.total_simulation_time += next_arrival_time;
                metrics.idle_time += next_arrival_time;

                current_processing_cycles = 0;
                operation_result = enqueue_linked(&queue, current_processing_cycles);
                list_perf_metrics.total_enqueue_ops++;

                if (operation_result == OPERATION_SUCCESS)
                {
                    record_allocation(tracker, (void *)queue.rear);
                }

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                metrics.total_arrival_time += next_arrival_time;
                metrics.incoming_requests++;
            }
        }
        else
        {
            if (compare_doubles(next_arrival_time, remaining_processing_time) > 0)
            {
                next_arrival_time -= remaining_processing_time;
                metrics.total_simulation_time += remaining_processing_time;
                remaining_processing_time = 0.0;
                is_processor_busy = 0;
                current_processing_cycles++;

                if (current_processing_cycles < config->processing_cycles)
                {
                    operation_result = enqueue_linked(&queue, current_processing_cycles);
                    list_perf_metrics.total_enqueue_ops++;

                    if (operation_result == OPERATION_SUCCESS)
                    {
                        record_allocation(tracker, (void *)queue.rear);
                    }
                }
                else
                {
                    metrics.completed_requests++;
                    if (metrics.completed_requests > 0)
                    {
                        metrics.average_queue_size += queue.size;
                    }
                }
            }
            else
            {
                if (compare_doubles(remaining_processing_time, next_arrival_time) > 0)
                {
                    metrics.average_waiting_time += (remaining_processing_time - next_arrival_time) * queue.size;
                }

                remaining_processing_time -= next_arrival_time;
                metrics.total_simulation_time += next_arrival_time;

                new_request_cycles = 0;
                operation_result = enqueue_linked(&queue, new_request_cycles);
                list_perf_metrics.total_enqueue_ops++;

                if (operation_result == OPERATION_SUCCESS)
                {
                    record_allocation(tracker, (void *)queue.rear);
                }
                metrics.incoming_requests++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                metrics.total_arrival_time += next_arrival_time;
            }

            if (metrics.completed_requests == next_output_step && metrics.completed_requests > 0)
            {
                metrics.current_queue_size = queue.size;
                display_intermediate_results(&metrics, config, next_output_step);
                next_output_step += config->output_frequency;
            }
        }
    }

    // Конец замера времени
    gettimeofday(&end_time, NULL);
    clock_t cpu_end = clock();

    double real_time_elapsed = (end_time.tv_sec - start_time.tv_sec) +
                               (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    double cpu_time_used = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    list_perf_metrics.simulation_time = metrics.total_simulation_time;
    list_perf_metrics.real_execution_time = real_time_elapsed;

    // Добавляем память под служебные структуры
    list_perf_metrics.peak_memory_usage += sizeof(simulation_metrics_t) + sizeof(dynamic_linked_queue_t);

    if (operation_result == MEMORY_ALLOCATION_FAILED)
    {
        printf("Ошибка: не удалось выделить память!\n");
    }

    if (operation_result == OPERATION_SUCCESS)
    {
        display_final_results(&metrics, config);

        printf("\n════════════════ ЭФФЕКТИВНОСТЬ (СПИСОК) ════════════════\n");
        printf("Реальное время выполнения: %.6f сек\n", real_time_elapsed);
        printf("Процессорное время: %.6f сек\n", cpu_time_used);
        printf("Пиковое использование памяти: %zu байт\n", list_perf_metrics.peak_memory_usage);
        printf("Максимальная длина очереди: %d\n", list_perf_metrics.max_queue_size);
        printf("Операций с памятью: %zu (malloc/free)\n", list_perf_metrics.total_memory_operations);
        printf("Операций добавления: %lld\n", list_perf_metrics.total_enqueue_ops);
        printf("Операций удаления: %lld\n", list_perf_metrics.total_dequeue_ops);

        // Анализ фрагментации
        if (list_perf_metrics.total_memory_operations > 1000)
        {
            printf("Фрагментация памяти: возможна (много динамических операций)\n");
        }
        else
        {
            printf("Фрагментация памяти: минимальная\n");
        }
        printf("═══════════════════════════════════════════════════════\n");

        // Если метрики для массива уже собраны, показываем сравнение
        if (array_perf_metrics.real_execution_time > 0)
        {
            printf("\nНажмите Enter для просмотра сравнительного анализа...");
            getchar(); // Ожидание нажатия Enter
            getchar(); // Для учета символа новой строки
            print_performance_comparison(&array_perf_metrics, &list_perf_metrics);
        }
    }

    cleanup_linked_queue(&queue);
}