#include "simulation_core.h"

void initialize_simulation_config(simulation_config_t *config)
{
    config->min_arrival_time = MIN_ARRIVAL_TIME;
    config->max_arrival_time = MAX_ARRIVAL_TIME;
    config->min_processing_time = MIN_PROCESSING_TIME;
    config->max_processing_time = MAX_PROCESSING_TIME;
    config->total_requests = DEFAULT_REQUEST_COUNT;
    config->output_frequency = OUTPUT_INTERVAL;
    config->processing_cycles = MAX_SIMULATION_CYCLES;
}

void reset_simulation_metrics(simulation_metrics_t *metrics)
{
    metrics->incoming_requests = 0;
    metrics->completed_requests = 0;
    metrics->processing_operations = 0;
    metrics->total_simulation_time = 0.0;
    metrics->total_arrival_time = 0.0;
    metrics->total_processing_time = 0.0;
    metrics->idle_time = 0.0;
    metrics->current_queue_size = 0;
    metrics->average_queue_size = 0.0;
    metrics->average_waiting_time = 0.0;
}

double generate_random_time(double min_time, double max_time)
{
    return min_time + ((double)rand() / RAND_MAX) * (max_time - min_time);
}

int compare_doubles(double a, double b)
{
    if (fabs(a - b) < TIME_EPSILON)
        return 0;
    return (a < b) ? -1 : 1;
}

void display_intermediate_results(const simulation_metrics_t *metrics, const simulation_config_t *config, int step)
{
    printf("\n--- Промежуточные результаты [шаг %d] ---\n", step);
    printf("Заявок получено: %d\n", metrics->incoming_requests);
    printf("Заявок обработано: %d\n", metrics->completed_requests);
    printf("Текущая очередь: %d\n", metrics->current_queue_size);
    printf("Средняя очередь: %.1f\n",
           metrics->average_queue_size / (config->processing_cycles / 2.0) / metrics->completed_requests);
    printf("Среднее время ожидания: %.6f\n",
           metrics->average_waiting_time / metrics->completed_requests);
    printf("----------------------------------------\n");
}

void display_final_results(const simulation_metrics_t *metrics, const simulation_config_t *config)
{
    double average_arrival_time = (config->min_arrival_time + config->max_arrival_time) / 2.0;
    double average_processing_time = (config->min_processing_time + config->max_processing_time) / 2.0;

    double t1_avg = (config->min_arrival_time + config->max_arrival_time) / 2.0;
    double t2_avg = (config->min_processing_time + config->max_processing_time) / 2.0;

    double expected_time;
    if (t1_avg * config->total_requests > t2_avg * metrics->processing_operations)
    {
        expected_time = t1_avg * config->total_requests;
    }
    else
    {
        int machine_activations = metrics->processing_operations;
        expected_time = machine_activations * t2_avg;
    }

    double inaccuracy_percentage = fabs(metrics->total_simulation_time - expected_time) / expected_time * 100;

    printf("\n=== ИТОГОВЫЕ РЕЗУЛЬТАТЫ ===\n");
    printf("Практические значения:\n");
    printf("  Общее время моделирования: %.6f\n", metrics->total_simulation_time);
    printf("  Суммарное время поступления: %.6f\n", metrics->total_arrival_time);
    printf("  Суммарное время обработки: %.6f\n", metrics->total_processing_time);
    printf("  Время простоя аппарата: %.6f\n", metrics->idle_time);
    printf("  Операций выполнено (срабатывания аппарата): %d\n", metrics->processing_operations);

    printf("\nТеоретические оценки:\n");
    printf("  Среднее время между заявками (T1): %.6f\n", average_arrival_time);
    printf("  Среднее время обработки (T2): %.6f\n", average_processing_time);
    printf("  Количество заявок: %d\n", config->total_requests);
    printf("  Циклов обработки на заявку (теоретически): %d\n", config->processing_cycles);
    printf("  Реальное количество срабатываний аппарата: %d\n", metrics->processing_operations);
    printf("  Ожидаемое общее время: %.6f\n", expected_time);
    printf("  Относительная погрешность: %.6f%%\n", inaccuracy_percentage);

    if (inaccuracy_percentage > 3.0)
    {
        printf("\nВнимание: Погрешность превышает допустимые 3%%!\n");
    }
    else
    {
        printf("\nПогрешность в допустимых пределах (<= 3%%)\n");
    }

    printf("\nАнализ загрузки системы:\n");

    double utilization_rate = 0;
    if (metrics->total_simulation_time > 0)
    {
        utilization_rate = metrics->total_processing_time / metrics->total_simulation_time * 100.0;
    }
    printf("  Коэффициент использования аппарата: %.2f%%\n", utilization_rate);

    if (metrics->completed_requests > 0)
    {
        double avg_system_time = metrics->total_simulation_time / metrics->completed_requests;
        printf("  Среднее время пребывания заявки в системе: %.6f\n", avg_system_time);
    }

    if (metrics->average_queue_size > 0 && metrics->completed_requests > 0)
    {
        double avg_queue_length = metrics->average_queue_size / metrics->completed_requests;
        printf("  Средняя длина очереди: %.6f\n", avg_queue_length);
    }

    printf("================================\n");
}