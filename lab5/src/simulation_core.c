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
    double theoretical_processing_time = config->processing_cycles *
                                         (config->max_processing_time - config->min_processing_time) / 2.0 * config->total_requests;
    double theoretical_arrival_time = (config->max_arrival_time - config->min_arrival_time) / 2.0 * config->total_requests;
    double theoretical_idle_time = theoretical_arrival_time - theoretical_processing_time;
    int theoretical_operations = config->processing_cycles * config->total_requests;

    printf("\n=== ИТОГОВЫЕ РЕЗУЛЬТАТЫ ===\n");
    printf("Практические значения:\n");
    printf("  Общее время: %.6f\n", metrics->total_simulation_time);
    printf("  Время поступления: %.6f\n", metrics->total_arrival_time);
    printf("  Время обработки: %.6f\n", metrics->total_processing_time);
    printf("  Время простоя: %.6f\n", metrics->idle_time);
    printf("  Операций выполнено: %d\n", metrics->processing_operations);

    printf("\nТеоретические значения:\n");
    printf("  Общее время: %.0f\n", theoretical_arrival_time);
    printf("  Время поступления: %.0f\n", theoretical_arrival_time);
    printf("  Время обработки: %.0f\n", theoretical_processing_time);
    printf("  Время простоя: %.0f\n", theoretical_idle_time);
    printf("  Операций: %d\n", theoretical_operations);

    // Расчет погрешностей
    double t_amount_application = metrics->total_simulation_time / ((config->max_arrival_time - config->min_arrival_time) / 2.0);
    double t_measurement_input = 100.0 * fabs(metrics->completed_requests - t_amount_application) / t_amount_application;
    double t_model_time = metrics->total_processing_time + metrics->idle_time;
    double t_measurement_output = 100.0 * fabs(metrics->total_simulation_time - t_model_time) / t_model_time;
    double t_measurement_model = 100.0 * fabs(metrics->total_simulation_time - theoretical_arrival_time) / theoretical_arrival_time;

    printf("\nАнализ точности:\n");
    printf("  Погрешность по входу: %.6f%%\n", t_measurement_input);
    printf("  Погрешность по выходу: %.6f%%\n", t_measurement_output);
    printf("  Погрешность модели: %.6f%%\n", t_measurement_model);
    printf("================================\n");
}