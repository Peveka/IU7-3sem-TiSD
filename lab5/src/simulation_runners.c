#include "../inc/simulation_runners.h"
#include <sys/time.h>
#include <time.h>

performance_metrics_t array_perf_metrics;
performance_metrics_t list_perf_metrics;

static void run_array_simulation_silent(const simulation_config_t *config, performance_metrics_t *metrics);
static void run_list_simulation_silent(const simulation_config_t *config, performance_metrics_t *metrics);

static void run_array_simulation_silent(const simulation_config_t *config, performance_metrics_t *metrics) {
    reset_performance_metrics(metrics);
    
    double next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
    double remaining_processing_time = 0.0;
    int current_processing_cycles = 0, new_request_cycles;
    int is_processor_busy = 0;
    int operation_result = OPERATION_SUCCESS;

    simulation_metrics_t sim_metrics;
    circular_buffer_queue_t queue;
    reset_simulation_metrics(&sim_metrics);
    initialize_circular_buffer(&queue);

    sim_metrics.total_arrival_time += next_arrival_time;

    while (sim_metrics.completed_requests < config->total_requests && operation_result == OPERATION_SUCCESS) {
        if (queue.size > metrics->max_queue_size) {
            metrics->max_queue_size = queue.size;
        }

        size_t current_memory = sizeof(circular_buffer_queue_t);
        if (current_memory > metrics->peak_memory_usage) {
            metrics->peak_memory_usage = current_memory;
        }

        if (is_processor_busy == 0) {
            if (queue.size > 0) {
                current_processing_cycles = *(queue.front);
                dequeue_circular_buffer(&queue);
                metrics->total_dequeue_ops++;

                remaining_processing_time = generate_random_time(config->min_processing_time, config->max_processing_time);
                sim_metrics.total_processing_time += remaining_processing_time;
                is_processor_busy = 1;
                sim_metrics.processing_operations++;
            } else {
                sim_metrics.total_simulation_time += next_arrival_time;
                sim_metrics.idle_time += next_arrival_time;

                current_processing_cycles = 0;
                operation_result = enqueue_circular_buffer(&queue, current_processing_cycles);
                metrics->total_enqueue_ops++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                sim_metrics.total_arrival_time += next_arrival_time;
                sim_metrics.incoming_requests++;
            }
        } else {
            if (compare_doubles(next_arrival_time, remaining_processing_time) > 0) {
                next_arrival_time -= remaining_processing_time;
                sim_metrics.total_simulation_time += remaining_processing_time;
                remaining_processing_time = 0.0;
                is_processor_busy = 0;
                current_processing_cycles++;

                if (current_processing_cycles < config->processing_cycles) {
                    operation_result = enqueue_circular_buffer(&queue, current_processing_cycles);
                    metrics->total_enqueue_ops++;
                } else {
                    sim_metrics.completed_requests++;
                    if (sim_metrics.completed_requests > 0) {
                        sim_metrics.average_queue_size += queue.size;
                    }
                }
            } else {
                if (compare_doubles(remaining_processing_time, next_arrival_time) > 0) {
                    sim_metrics.average_waiting_time += (remaining_processing_time - next_arrival_time) * queue.size;
                }

                remaining_processing_time -= next_arrival_time;
                sim_metrics.total_simulation_time += next_arrival_time;

                new_request_cycles = 0;
                operation_result = enqueue_circular_buffer(&queue, new_request_cycles);
                metrics->total_enqueue_ops++;
                sim_metrics.incoming_requests++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                sim_metrics.total_arrival_time += next_arrival_time;
            }
        }
    }

    metrics->simulation_time = sim_metrics.total_simulation_time;
    metrics->real_execution_time = 0;
    metrics->peak_memory_usage += sizeof(simulation_metrics_t) + sizeof(circular_buffer_queue_t);
}

static void run_list_simulation_silent(const simulation_config_t *config, performance_metrics_t *metrics) {
    reset_performance_metrics(metrics);
    
    double next_arrival_time, remaining_processing_time = 0.0;
    int current_processing_cycles = 0, new_request_cycles;
    simulation_metrics_t sim_metrics;
    reset_simulation_metrics(&sim_metrics);

    int is_processor_busy = 0;
    int operation_result = OPERATION_SUCCESS;

    dynamic_linked_queue_t queue;
    initialize_linked_queue(&queue);

    next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
    sim_metrics.total_arrival_time += next_arrival_time;

    while (sim_metrics.completed_requests < config->total_requests && operation_result == OPERATION_SUCCESS) {
        if (queue.size > metrics->max_queue_size) {
            metrics->max_queue_size = queue.size;
        }

        size_t node_size = sizeof(queue_node_t);
        size_t current_memory = queue.size * node_size + sizeof(dynamic_linked_queue_t);
        current_memory = (size_t)(current_memory * 1.1);

        if (current_memory > metrics->peak_memory_usage) {
            metrics->peak_memory_usage = current_memory;
        }

        metrics->total_memory_operations++;

        if (is_processor_busy == 0) {
            if (queue.size > 0) {
                current_processing_cycles = queue.front->data;
                dequeue_linked(&queue);
                metrics->total_dequeue_ops++;

                remaining_processing_time = generate_random_time(config->min_processing_time, config->max_processing_time);
                sim_metrics.total_processing_time += remaining_processing_time;
                is_processor_busy = 1;
                sim_metrics.processing_operations++;
            } else {
                sim_metrics.total_simulation_time += next_arrival_time;
                sim_metrics.idle_time += next_arrival_time;

                current_processing_cycles = 0;
                operation_result = enqueue_linked(&queue, current_processing_cycles);
                metrics->total_enqueue_ops++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                sim_metrics.total_arrival_time += next_arrival_time;
                sim_metrics.incoming_requests++;
            }
        } else {
            if (compare_doubles(next_arrival_time, remaining_processing_time) > 0) {
                next_arrival_time -= remaining_processing_time;
                sim_metrics.total_simulation_time += remaining_processing_time;
                remaining_processing_time = 0.0;
                is_processor_busy = 0;
                current_processing_cycles++;

                if (current_processing_cycles < config->processing_cycles) {
                    operation_result = enqueue_linked(&queue, current_processing_cycles);
                    metrics->total_enqueue_ops++;
                } else {
                    sim_metrics.completed_requests++;
                    if (sim_metrics.completed_requests > 0) {
                        sim_metrics.average_queue_size += queue.size;
                    }
                }
            } else {
                if (compare_doubles(remaining_processing_time, next_arrival_time) > 0) {
                    sim_metrics.average_waiting_time += (remaining_processing_time - next_arrival_time) * queue.size;
                }

                remaining_processing_time -= next_arrival_time;
                sim_metrics.total_simulation_time += next_arrival_time;

                new_request_cycles = 0;
                operation_result = enqueue_linked(&queue, new_request_cycles);
                metrics->total_enqueue_ops++;
                sim_metrics.incoming_requests++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                sim_metrics.total_arrival_time += next_arrival_time;
            }
        }
    }

    metrics->simulation_time = sim_metrics.total_simulation_time;
    metrics->real_execution_time = 0;
    metrics->peak_memory_usage += sizeof(simulation_metrics_t) + sizeof(dynamic_linked_queue_t);
    
    cleanup_linked_queue(&queue);
}

void run_simulation_comparison(const simulation_config_t *config) {
    printf("\n════════════════ СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ РЕАЛИЗАЦИЙ ════════════════\n");
    printf("Запуск симуляций с текущими настройками T1 и T2...\n");
    printf("Каждая симуляция будет выполнена 1000 раз для получения точных средних значений.\n\n");
    
    printf("Текущие настройки:\n");
    printf("• T1 (время между заявками): %.1f - %.1f\n", 
           config->min_arrival_time, config->max_arrival_time);
    printf("• T2 (время обработки): %.1f - %.1f\n", 
           config->min_processing_time, config->max_processing_time);
    printf("• Количество заявок: %d\n", config->total_requests);
    printf("• Циклов обработки: %d\n\n", config->processing_cycles);
    
    simulation_config_t current_config = *config;
    
    const int ITERATIONS = 1000;
    
    performance_metrics_t *array_metrics_runs = malloc(ITERATIONS * sizeof(performance_metrics_t));
    performance_metrics_t *list_metrics_runs = malloc(ITERATIONS * sizeof(performance_metrics_t));
    
    performance_metrics_t array_metrics_sum;
    performance_metrics_t list_metrics_sum;
    
    reset_performance_metrics(&array_metrics_sum);
    reset_performance_metrics(&list_metrics_sum);
    
    struct timeval start_time, end_time;
    double elapsed_time;
    
    printf("=== СИМУЛЯЦИЯ НА МАССИВЕ (КОЛЬЦЕВОЙ БУФЕР) ===\n");
    printf("Прогоны: ");
    fflush(stdout);
    
    int progress_step = ITERATIONS / 50;
    if (progress_step < 1) progress_step = 1;
    
    for (int run = 0; run < ITERATIONS; run++) {
        if (run % progress_step == 0) {
            printf(".");
            fflush(stdout);
        }
        
        gettimeofday(&start_time, NULL);
        
        run_array_simulation_silent(&current_config, &array_metrics_runs[run]);
        
        gettimeofday(&end_time, NULL);
        
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
        array_metrics_runs[run].real_execution_time = elapsed_time;
        
        array_metrics_sum.simulation_time += array_metrics_runs[run].simulation_time;
        array_metrics_sum.real_execution_time += array_metrics_runs[run].real_execution_time;
        array_metrics_sum.peak_memory_usage += array_metrics_runs[run].peak_memory_usage;
        array_metrics_sum.total_memory_operations += array_metrics_runs[run].total_memory_operations;
        array_metrics_sum.max_queue_size += array_metrics_runs[run].max_queue_size;
        array_metrics_sum.total_enqueue_ops += array_metrics_runs[run].total_enqueue_ops;
        array_metrics_sum.total_dequeue_ops += array_metrics_runs[run].total_dequeue_ops;
    }
    printf(" ✓\n");
    
    printf("=== СИМУЛЯЦИЯ НА СПИСКЕ (СВЯЗНЫЙ СПИСОК) ===\n");
    printf("Прогоны: ");
    fflush(stdout);
    
    for (int run = 0; run < ITERATIONS; run++) {
        if (run % progress_step == 0) {
            printf(".");
            fflush(stdout);
        }
        
        gettimeofday(&start_time, NULL);
        
        run_list_simulation_silent(&current_config, &list_metrics_runs[run]);
        
        gettimeofday(&end_time, NULL);
        
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
        list_metrics_runs[run].real_execution_time = elapsed_time;
        
        list_metrics_sum.simulation_time += list_metrics_runs[run].simulation_time;
        list_metrics_sum.real_execution_time += list_metrics_runs[run].real_execution_time;
        list_metrics_sum.peak_memory_usage += list_metrics_runs[run].peak_memory_usage;
        list_metrics_sum.total_memory_operations += list_metrics_runs[run].total_memory_operations;
        list_metrics_sum.max_queue_size += list_metrics_runs[run].max_queue_size;
        list_metrics_sum.total_enqueue_ops += list_metrics_runs[run].total_enqueue_ops;
        list_metrics_sum.total_dequeue_ops += list_metrics_runs[run].total_dequeue_ops;
    }
    printf(" ✓\n\n");
    
    performance_metrics_t array_metrics_avg;
    performance_metrics_t list_metrics_avg;
    
    reset_performance_metrics(&array_metrics_avg);
    reset_performance_metrics(&list_metrics_avg);
    
    array_metrics_avg.simulation_time = array_metrics_sum.simulation_time / ITERATIONS;
    array_metrics_avg.real_execution_time = array_metrics_sum.real_execution_time / ITERATIONS;
    array_metrics_avg.peak_memory_usage = array_metrics_sum.peak_memory_usage / ITERATIONS;
    array_metrics_avg.total_memory_operations = array_metrics_sum.total_memory_operations / ITERATIONS;
    array_metrics_avg.max_queue_size = array_metrics_sum.max_queue_size / ITERATIONS;
    array_metrics_avg.total_enqueue_ops = array_metrics_sum.total_enqueue_ops / ITERATIONS;
    array_metrics_avg.total_dequeue_ops = array_metrics_sum.total_dequeue_ops / ITERATIONS;
    
    list_metrics_avg.simulation_time = list_metrics_sum.simulation_time / ITERATIONS;
    list_metrics_avg.real_execution_time = list_metrics_sum.real_execution_time / ITERATIONS;
    list_metrics_avg.peak_memory_usage = list_metrics_sum.peak_memory_usage / ITERATIONS;
    list_metrics_avg.total_memory_operations = list_metrics_sum.total_memory_operations / ITERATIONS;
    list_metrics_avg.max_queue_size = list_metrics_sum.max_queue_size / ITERATIONS;
    list_metrics_avg.total_enqueue_ops = list_metrics_sum.total_enqueue_ops / ITERATIONS;
    list_metrics_avg.total_dequeue_ops = list_metrics_sum.total_dequeue_ops / ITERATIONS;
    
    double array_time_std = 0, list_time_std = 0;
    double array_memory_std = 0, list_memory_std = 0;
    
    for (int i = 0; i < ITERATIONS; i++) {
        array_time_std += pow(array_metrics_runs[i].real_execution_time - array_metrics_avg.real_execution_time, 2);
        list_time_std += pow(list_metrics_runs[i].real_execution_time - list_metrics_avg.real_execution_time, 2);
        
        array_memory_std += pow(array_metrics_runs[i].peak_memory_usage - array_metrics_avg.peak_memory_usage, 2);
        list_memory_std += pow(list_metrics_runs[i].peak_memory_usage - list_metrics_avg.peak_memory_usage, 2);
    }
    
    array_time_std = sqrt(array_time_std / ITERATIONS);
    list_time_std = sqrt(list_time_std / ITERATIONS);
    array_memory_std = sqrt(array_memory_std / ITERATIONS);
    list_memory_std = sqrt(list_memory_std / ITERATIONS);
    
    printf("════════════════ РЕЗУЛЬТАТЫ (СРЕДНЕЕ ЗА %d ПРОГОНОВ) ════════════════\n", ITERATIONS);
    
    print_performance_comparison(&array_metrics_avg, &list_metrics_avg);
    
    printf("\n--- СТАТИСТИКА ТОЧНОСТИ ИЗМЕРЕНИЙ ---\n");
    printf("Время выполнения (стандартное отклонение):\n");
    printf("• Массив: %.6f ± %.6f сек (относительная погрешность: %.1f%%)\n",
           array_metrics_avg.real_execution_time, array_time_std,
           (array_time_std / array_metrics_avg.real_execution_time) * 100);
    printf("• Список: %.6f ± %.6f сек (относительная погрешность: %.1f%%)\n",
           list_metrics_avg.real_execution_time, list_time_std,
           (list_time_std / list_metrics_avg.real_execution_time) * 100);
    
    printf("\nИспользование памяти (стандартное отклонение):\n");
    printf("• Массив: %zu ± %.0f байт (относительная погрешность: %.1f%%)\n",
           array_metrics_avg.peak_memory_usage, array_memory_std,
           (array_memory_std / array_metrics_avg.peak_memory_usage) * 100);
    printf("• Список: %zu ± %.0f байт (относительная погрешность: %.1f%%)\n",
           list_metrics_avg.peak_memory_usage, list_memory_std,
           (list_memory_std / list_metrics_avg.peak_memory_usage) * 100);
    
    double array_time_margin = 1.96 * array_time_std / sqrt(ITERATIONS);
    double list_time_margin = 1.96 * list_time_std / sqrt(ITERATIONS);
    
    double array_time_min = array_metrics_avg.real_execution_time - array_time_margin;
    double array_time_max = array_metrics_avg.real_execution_time + array_time_margin;
    double list_time_min = list_metrics_avg.real_execution_time - list_time_margin;
    double list_time_max = list_metrics_avg.real_execution_time + list_time_margin;
    
    printf("\n--- СТАТИСТИЧЕСКИЙ АНАЛИЗ ---\n");
    printf("Доверительный интервал 95%% для времени выполнения:\n");
    printf("• Массив: [%.6f, %.6f] сек\n", array_time_min, array_time_max);
    printf("• Список: [%.6f, %.6f] сек\n", list_time_min, list_time_max);
    
    printf("\n--- СТАТИСТИЧЕСКАЯ ЗНАЧИМОСТЬ ---\n");
    if (array_time_max < list_time_min) {
        printf("✓ Различие СТАТИСТИЧЕСКИ ЗНАЧИМОЕ\n");
        printf("  Массив НАДЕЖНО быстрее списка\n");
    } else if (list_time_max < array_time_min) {
        printf("✓ Различие СТАТИСТИЧЕСКИ ЗНАЧИМОЕ\n");
        printf("  Список НАДЕЖНО быстрее массива\n");
    } else {
        printf("⚠ Различие СТАТИСТИЧЕСКИ НЕЗНАЧИМОЕ\n");
        printf("  Нет надежных доказательств преимущества одной реализации\n");
    }
    
    printf("\n--- АНАЛИЗ ЭФФЕКТИВНОСТИ ---\n");
    
    double avg_T1 = (config->min_arrival_time + config->max_arrival_time) / 2.0;
    double avg_T2 = (config->min_processing_time + config->max_processing_time) / 2.0;
    double load_factor = avg_T2 / avg_T1;
    
    printf("Коэффициент загрузки системы: %.2f\n", load_factor);
    
    if (load_factor < 0.5) {
        printf("• Система слабо нагружена\n");
        printf("• Обе реализации будут работать хорошо\n");
        printf("• Массив может быть немного быстрее из-за кэширования\n");
    } else if (load_factor < 0.9) {
        printf("• Система умеренно нагружена\n");
        if (array_metrics_avg.max_queue_size < MAX_QUEUE_CAPACITY * 0.7) {
            printf("• Массив эффективен (очередь не превышает 70%% от макс. размера)\n");
        } else {
            printf("• Список предпочтительнее (риск переполнения массива)\n");
        }
    } else {
        printf("• Система сильно нагружена\n");
        printf("• Список более надежен (нет ограничения по размеру)\n");
        printf("• Массив может переполниться при пиковых нагрузках\n");
    }
    
    printf("\n--- СРАВНЕНИЕ ИСПОЛЬЗОВАНИЯ ПАМЯТИ ---\n");
    printf("• Массив: фиксированные %zu байт (в среднем за %d прогонов)\n", array_metrics_avg.peak_memory_usage, ITERATIONS);
    printf("• Список: динамическая память, ~%zu байт на элемент\n", sizeof(queue_node_t));
    
    printf("\n--- РЕКОМЕНДАЦИИ ---\n");
    printf("1. Для систем с ограниченной памятью: используйте массив\n");
    printf("2. Для систем с непредсказуемой нагрузкой: используйте список\n");
    printf("3. Для систем с реальным временем: используйте массив (предсказуемое время доступа)\n");
    printf("4. Для систем с большим количеством вставок/удалений: список может быть эффективнее\n");
    
    printf("\n════════════════════════════════════════════════════════════════════════════\n");
    
    free(array_metrics_runs);
    free(list_metrics_runs);
}

// Упрощенная версия симуляции на списке (без вывода промежуточных результатов)
static void run_list_simulation_silent(const simulation_config_t *config, performance_metrics_t *metrics) {
    reset_performance_metrics(metrics);
    
    double next_arrival_time, remaining_processing_time = 0.0;
    int current_processing_cycles = 0, new_request_cycles;
    simulation_metrics_t sim_metrics;
    reset_simulation_metrics(&sim_metrics);

    int is_processor_busy = 0;
    int operation_result = OPERATION_SUCCESS;

    dynamic_linked_queue_t queue;
    initialize_linked_queue(&queue);

    next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
    sim_metrics.total_arrival_time += next_arrival_time;

    while (sim_metrics.completed_requests < config->total_requests && operation_result == OPERATION_SUCCESS) {
        if (queue.size > metrics->max_queue_size) {
            metrics->max_queue_size = queue.size;
        }

        size_t node_size = sizeof(queue_node_t);
        size_t current_memory = queue.size * node_size + sizeof(dynamic_linked_queue_t);
        current_memory = (size_t)(current_memory * 1.1);

        if (current_memory > metrics->peak_memory_usage) {
            metrics->peak_memory_usage = current_memory;
        }

        metrics->total_memory_operations++;

        if (is_processor_busy == 0) {
            if (queue.size > 0) {
                current_processing_cycles = queue.front->data;
                dequeue_linked(&queue);
                metrics->total_dequeue_ops++;

                remaining_processing_time = generate_random_time(config->min_processing_time, config->max_processing_time);
                sim_metrics.total_processing_time += remaining_processing_time;
                is_processor_busy = 1;
                sim_metrics.processing_operations++;
            } else {
                sim_metrics.total_simulation_time += next_arrival_time;
                sim_metrics.idle_time += next_arrival_time;

                current_processing_cycles = 0;
                operation_result = enqueue_linked(&queue, current_processing_cycles);
                metrics->total_enqueue_ops++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                sim_metrics.total_arrival_time += next_arrival_time;
                sim_metrics.incoming_requests++;
            }
        } else {
            if (compare_doubles(next_arrival_time, remaining_processing_time) > 0) {
                next_arrival_time -= remaining_processing_time;
                sim_metrics.total_simulation_time += remaining_processing_time;
                remaining_processing_time = 0.0;
                is_processor_busy = 0;
                current_processing_cycles++;

                if (current_processing_cycles < config->processing_cycles) {
                    operation_result = enqueue_linked(&queue, current_processing_cycles);
                    metrics->total_enqueue_ops++;
                } else {
                    sim_metrics.completed_requests++;
                    if (sim_metrics.completed_requests > 0) {
                        sim_metrics.average_queue_size += queue.size;
                    }
                }
            } else {
                if (compare_doubles(remaining_processing_time, next_arrival_time) > 0) {
                    sim_metrics.average_waiting_time += (remaining_processing_time - next_arrival_time) * queue.size;
                }

                remaining_processing_time -= next_arrival_time;
                sim_metrics.total_simulation_time += next_arrival_time;

                new_request_cycles = 0;
                operation_result = enqueue_linked(&queue, new_request_cycles);
                metrics->total_enqueue_ops++;
                sim_metrics.incoming_requests++;

                next_arrival_time = generate_random_time(config->min_arrival_time, config->max_arrival_time);
                sim_metrics.total_arrival_time += next_arrival_time;
            }
        }
    }

    metrics->simulation_time = sim_metrics.total_simulation_time;
    metrics->real_execution_time = 0; // Будет измеряться во внешнем цикле
    metrics->peak_memory_usage += sizeof(simulation_metrics_t) + sizeof(dynamic_linked_queue_t);
    
    cleanup_linked_queue(&queue);
}

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
    printf("\nАНАЛИЗ ЭФФЕКТИВНОСТИ:\n");
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
    printf("• Фрагментация памяти:\n");
    printf("  - Массив: минимальная (статическое выделение)\n");
    printf("  - Список: возможна (динамическое выделение/освобождение)\n");
}

void run_array_based_simulation(const simulation_config_t *config)
{
    printf("\n════════════════ ЗАПУСК СИМУЛЯЦИИ (КОЛЬЦЕВОЙ БУФЕР) ════════════════\n");
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
        if (queue.size > array_perf_metrics.max_queue_size)
        {
            array_perf_metrics.max_queue_size = queue.size;
        }

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

    gettimeofday(&end_time, NULL);
    clock_t cpu_end = clock();

    double real_time_elapsed = (end_time.tv_sec - start_time.tv_sec) +
                               (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    //double cpu_time_used = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    array_perf_metrics.simulation_time = metrics.total_simulation_time;
    array_perf_metrics.real_execution_time = real_time_elapsed;
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
        //printf("Процессорное время: %.6f сек\n", cpu_time_used);
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
        if (queue.size > list_perf_metrics.max_queue_size)
        {
            list_perf_metrics.max_queue_size = queue.size;
        }
        size_t node_size = sizeof(queue_node_t);
        size_t current_memory = queue.size * node_size + sizeof(dynamic_linked_queue_t);
        current_memory = (size_t)(current_memory * 1.1);

        if (current_memory > list_perf_metrics.peak_memory_usage)
        {
            list_perf_metrics.peak_memory_usage = current_memory;
        }

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

    gettimeofday(&end_time, NULL);
    clock_t cpu_end = clock();

    double real_time_elapsed = (end_time.tv_sec - start_time.tv_sec) +
                               (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    double cpu_time_used = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    list_perf_metrics.simulation_time = metrics.total_simulation_time;
    list_perf_metrics.real_execution_time = real_time_elapsed;

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
        if (list_perf_metrics.total_memory_operations > 1000)
        {
            printf("Фрагментация памяти: возможна (много динамических операций)\n");
        }
        else
        {
            printf("Фрагментация памяти: минимальная\n");
        }
        printf("═══════════════════════════════════════════════════════\n");
        if (array_perf_metrics.real_execution_time > 0)
        {
            printf("\nНажмите Enter для просмотра сравнительного анализа...");
            getchar();
            getchar();
            print_performance_comparison(&array_perf_metrics, &list_perf_metrics);
        }
    }

    cleanup_linked_queue(&queue);
}

void run_simulation_comparison(const simulation_config_t *config) {
    printf("\n════════════════ СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ РЕАЛИЗАЦИЙ ════════════════\n");
    printf("Запуск симуляций с текущими настройками T1 и T2...\n");
    printf("Каждая симуляция будет выполнена 20 раз для получения точных средних значений.\n\n");
    
    printf("Текущие настройки:\n");
    printf("• T1 (время между заявками): %.1f - %.1f\n", 
           config->min_arrival_time, config->max_arrival_time);
    printf("• T2 (время обработки): %.1f - %.1f\n", 
           config->min_processing_time, config->max_processing_time);
    printf("• Количество заявок: %d\n", config->total_requests);
    printf("• Циклов обработки: %d\n\n", config->processing_cycles);
    
    // Создаем локальную копию конфигурации
    simulation_config_t current_config = *config;
    
    // Массивы для хранения метрик по всем прогонам
    performance_metrics_t array_metrics_runs[20];
    performance_metrics_t list_metrics_runs[20];
    
    // Переменные для суммарных значений
    performance_metrics_t array_metrics_sum;
    performance_metrics_t list_metrics_sum;
    
    // Инициализируем суммарные значения
    reset_performance_metrics(&array_metrics_sum);
    reset_performance_metrics(&list_metrics_sum);
    
    // Переменные для измерения времени
    struct timeval start_time, end_time;
    double elapsed_time;
    
    // ЗАПУСК 20 ПРОГОНОВ ДЛЯ МАССИВА
    printf("=== СИМУЛЯЦИЯ НА МАССИВЕ (КОЛЬЦЕВОЙ БУФЕР) ===\n");
    printf("Прогоны: ");
    fflush(stdout);
    
    for (int run = 0; run < 20; run++) {
        printf(".");
        fflush(stdout);  // Для отображения прогресса
        
        // Начало замера времени
        gettimeofday(&start_time, NULL);
        
        // Запуск упрощенной симуляции на массиве
        run_array_simulation_silent(&current_config, &array_metrics_runs[run]);
        
        // Конец замера времени
        gettimeofday(&end_time, NULL);
        
        // Вычисляем реальное время выполнения
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
        array_metrics_runs[run].real_execution_time = elapsed_time;
        
        // Суммируем метрики
        array_metrics_sum.simulation_time += array_metrics_runs[run].simulation_time;
        array_metrics_sum.real_execution_time += array_metrics_runs[run].real_execution_time;
        array_metrics_sum.peak_memory_usage += array_metrics_runs[run].peak_memory_usage;
        array_metrics_sum.total_memory_operations += array_metrics_runs[run].total_memory_operations;
        array_metrics_sum.max_queue_size += array_metrics_runs[run].max_queue_size;
        array_metrics_sum.total_enqueue_ops += array_metrics_runs[run].total_enqueue_ops;
        array_metrics_sum.total_dequeue_ops += array_metrics_runs[run].total_dequeue_ops;
    }
    printf("=== СИМУЛЯЦИЯ НА СПИСКЕ (СВЯЗНЫЙ СПИСОК) ===\n");
    printf("Прогоны: ");
    fflush(stdout);
    
    for (int run = 0; run < 20; run++) {
        printf(".");
        fflush(stdout);  // Для отображения прогресса
        
        // Начало замера времени
        gettimeofday(&start_time, NULL);
        
        // Запуск упрощенной симуляции на списке
        run_list_simulation_silent(&current_config, &list_metrics_runs[run]);
        
        // Конец замера времени
        gettimeofday(&end_time, NULL);
        
        // Вычисляем реальное время выполнения
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
        list_metrics_runs[run].real_execution_time = elapsed_time;
        
        // Суммируем метрики
        list_metrics_sum.simulation_time += list_metrics_runs[run].simulation_time;
        list_metrics_sum.real_execution_time += list_metrics_runs[run].real_execution_time;
        list_metrics_sum.peak_memory_usage += list_metrics_runs[run].peak_memory_usage;
        list_metrics_sum.total_memory_operations += list_metrics_runs[run].total_memory_operations;
        list_metrics_sum.max_queue_size += list_metrics_runs[run].max_queue_size;
        list_metrics_sum.total_enqueue_ops += list_metrics_runs[run].total_enqueue_ops;
        list_metrics_sum.total_dequeue_ops += list_metrics_runs[run].total_dequeue_ops;
    }
    printf(" ✓\n\n");
    
    // Вычисляем средние значения
    performance_metrics_t array_metrics_avg;
    performance_metrics_t list_metrics_avg;
    
    reset_performance_metrics(&array_metrics_avg);
    reset_performance_metrics(&list_metrics_avg);
    
    // Вычисляем средние для массива
    array_metrics_avg.simulation_time = array_metrics_sum.simulation_time / 20.0;
    array_metrics_avg.real_execution_time = array_metrics_sum.real_execution_time / 20.0;
    array_metrics_avg.peak_memory_usage = array_metrics_sum.peak_memory_usage / 20;
    array_metrics_avg.total_memory_operations = array_metrics_sum.total_memory_operations / 20;
    array_metrics_avg.max_queue_size = array_metrics_sum.max_queue_size / 20;
    array_metrics_avg.total_enqueue_ops = array_metrics_sum.total_enqueue_ops / 20;
    array_metrics_avg.total_dequeue_ops = array_metrics_sum.total_dequeue_ops / 20;
    
    // Вычисляем средние для списка
    list_metrics_avg.simulation_time = list_metrics_sum.simulation_time / 20.0;
    list_metrics_avg.real_execution_time = list_metrics_sum.real_execution_time / 20.0;
    list_metrics_avg.peak_memory_usage = list_metrics_sum.peak_memory_usage / 20;
    list_metrics_avg.total_memory_operations = list_metrics_sum.total_memory_operations / 20;
    list_metrics_avg.max_queue_size = list_metrics_sum.max_queue_size / 20;
    list_metrics_avg.total_enqueue_ops = list_metrics_sum.total_enqueue_ops / 20;
    list_metrics_avg.total_dequeue_ops = list_metrics_sum.total_dequeue_ops / 20;
    
    // Вычисляем стандартные отклонения (для оценки точности)
    double array_time_std = 0, list_time_std = 0;
    double array_memory_std = 0, list_memory_std = 0;
    
    for (int i = 0; i < 20; i++) {
        array_time_std += pow(array_metrics_runs[i].real_execution_time - array_metrics_avg.real_execution_time, 2);
        list_time_std += pow(list_metrics_runs[i].real_execution_time - list_metrics_avg.real_execution_time, 2);
        
        array_memory_std += pow(array_metrics_runs[i].peak_memory_usage - array_metrics_avg.peak_memory_usage, 2);
        list_memory_std += pow(list_metrics_runs[i].peak_memory_usage - list_metrics_avg.peak_memory_usage, 2);
    }
    
    array_time_std = sqrt(array_time_std / 20);
    list_time_std = sqrt(list_time_std / 20);
    array_memory_std = sqrt(array_memory_std / 20);
    list_memory_std = sqrt(list_memory_std / 20);
    
    printf("════════════════ РЕЗУЛЬТАТЫ (СРЕДНЕЕ ЗА 20 ПРОГОНОВ) ════════════════\n");
    
    // Выводим сравнение производительности
    print_performance_comparison(&array_metrics_avg, &list_metrics_avg);
    
    // Выводим статистику точности
    printf("\n--- СТАТИСТИКА ТОЧНОСТИ ИЗМЕРЕНИЙ ---\n");
    printf("Время выполнения (стандартное отклонение):\n");
    printf("• Массив: %.6f ± %.6f сек (относительная погрешность: %.1f%%)\n",
           array_metrics_avg.real_execution_time, array_time_std,
           (array_time_std / array_metrics_avg.real_execution_time) * 100);
    printf("• Список: %.6f ± %.6f сек (относительная погрешность: %.1f%%)\n",
           list_metrics_avg.real_execution_time, list_time_std,
           (list_time_std / list_metrics_avg.real_execution_time) * 100);
    
    // Определяем доверительный интервал для времени (95%)
    double array_time_margin = 1.96 * array_time_std / sqrt(20);
    double list_time_margin = 1.96 * list_time_std / sqrt(20);
    
    double array_time_min = array_metrics_avg.real_execution_time - array_time_margin;
    double array_time_max = array_metrics_avg.real_execution_time + array_time_margin;
    double list_time_min = list_metrics_avg.real_execution_time - list_time_margin;
    double list_time_max = list_metrics_avg.real_execution_time + list_time_margin;
    
    // Статистический анализ
    printf("\n--- СТАТИСТИЧЕСКИЙ АНАЛИЗ ---\n");
    printf("Доверительный интервал 95%% для времени выполнения:\n");
    printf("• Массив: [%.6f, %.6f] сек\n", array_time_min, array_time_max);
    printf("• Список: [%.6f, %.6f] сек\n", list_time_min, list_time_max);
    
    // Проверяем статистическую значимость различий
    printf("\n--- СТАТИСТИЧЕСКАЯ ЗНАЧИМОСТЬ ---\n");
    if (array_time_max < list_time_min) {
        printf("✓ Различие СТАТИСТИЧЕСКИ ЗНАЧИМОЕ\n");
        printf("  Массив НАДЕЖНО быстрее списка\n");
    } else if (list_time_max < array_time_min) {
        printf("✓ Различие СТАТИСТИЧЕСКИ ЗНАЧИМОЕ\n");
        printf("  Список НАДЕЖНО быстрее массива\n");
    } else {
        printf("⚠ Различие СТАТИСТИЧЕСКИ НЕЗНАЧИМОЕ\n");
        printf("  Нет надежных доказательств преимущества одной реализации\n");
    }
    
    // Анализ эффективности для текущих настроек
    printf("\n--- АНАЛИЗ ЭФФЕКТИВНОСТИ ---\n");
    
    double avg_T1 = (config->min_arrival_time + config->max_arrival_time) / 2.0;
    double avg_T2 = (config->min_processing_time + config->max_processing_time) / 2.0;
    double load_factor = avg_T2 / avg_T1;
    
    printf("Коэффициент загрузки системы: %.2f\n", load_factor);
    
    if (load_factor < 0.5) {
        printf("• Система слабо нагружена\n");
        printf("• Обе реализации будут работать хорошо\n");
        printf("• Массив может быть немного быстрее из-за кэширования\n");
    } else if (load_factor < 0.9) {
        printf("• Система умеренно нагружена\n");
        if (array_metrics_avg.max_queue_size < MAX_QUEUE_CAPACITY * 0.7) {
            printf("• Массив эффективен (очередь не превышает 70%% от макс. размера)\n");
        } else {
            printf("• Список предпочтительнее (риск переполнения массива)\n");
        }
    } else {
        printf("• Система сильно нагружена\n");
        printf("• Список более надежен (нет ограничения по размеру)\n");
        printf("• Массив может переполниться при пиковых нагрузках\n");
    }
    
    // Сравнение памяти
    printf("\n--- СРАВНЕНИЕ ИСПОЛЬЗОВАНИЯ ПАМЯТИ ---\n");
    printf("• Массив: фиксированные %zu байт (в среднем за 20 прогонов)\n", array_metrics_avg.peak_memory_usage);
    printf("• Список: динамическая память, ~%zu байт на элемент\n", sizeof(queue_node_t));
    
    // Практические рекомендации
    printf("\n--- РЕКОМЕНДАЦИИ ---\n");
    printf("1. Для систем с ограниченной памятью: используйте массив\n");
    printf("2. Для систем с непредсказуемой нагрузкой: используйте список\n");
    printf("3. Для систем с реальным временем: используйте массив (предсказуемое время доступа)\n");
    printf("4. Для систем с большим количеством вставок/удалений: список может быть эффективнее\n");
    
    printf("\n════════════════════════════════════════════════════════════════════════════\n");
}