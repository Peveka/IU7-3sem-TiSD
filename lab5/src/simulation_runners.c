#include "../inc/simulation_core.h"
#include "../inc/array_queue.h"
#include "../inc/linked_queue.h"

void run_array_based_simulation(const simulation_config_t *config)
{
    printf("\n════════════════ ЗАПУСК СИМУЛЯЦИИ (КОЛЬЦЕВОЙ БУФЕР) ════════════════\n");

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
        if (is_processor_busy == 0)
        {
            if (queue.size > 0)
            {
                current_processing_cycles = *(queue.front);
                dequeue_circular_buffer(&queue);

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

    if (operation_result == QUEUE_OVERFLOW)
    {
        printf("Ошибка: переполнение очереди!\n");
    }

    if (operation_result == OPERATION_SUCCESS)
    {
        display_final_results(&metrics, config);
    }
}

void run_list_based_simulation(const simulation_config_t *config, memory_tracker_t *tracker)
{
    printf("\n════════════════ ЗАПУСК СИМУЛЯЦИИ (СВЯЗНЫЙ СПИСОК) ════════════════\n");

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
        if (is_processor_busy == 0)
        {
            if (queue.size > 0)
            {
                current_processing_cycles = queue.front->data;
                record_deallocation(tracker, (void *)queue.front);
                dequeue_linked(&queue);

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

    if (operation_result == MEMORY_ALLOCATION_FAILED)
    {
        printf("Ошибка: не удалось выделить память!\n");
    }

    if (operation_result == OPERATION_SUCCESS)
    {
        display_final_results(&metrics, config);
    }

    cleanup_linked_queue(&queue);
}