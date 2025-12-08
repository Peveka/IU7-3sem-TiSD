#ifndef SIMULATION_RUNNERS_H
#define SIMULATION_RUNNERS_H

#include "simulation_core.h"
#include "array_queue.h"
#include "linked_queue.h"

// Объявляем глобальные переменные как extern
extern performance_metrics_t array_perf_metrics;
extern performance_metrics_t list_perf_metrics;

// Прототипы функций
void run_array_based_simulation(const simulation_config_t *config);
void run_list_based_simulation(const simulation_config_t *config, memory_tracker_t *tracker);
void print_performance_comparison(const performance_metrics_t *array_metrics,
                                  const performance_metrics_t *list_metrics);
void reset_performance_metrics(performance_metrics_t *metrics);

// Новая функция для сравнения производительности через симуляции
void run_simulation_comparison(const simulation_config_t *config);

#endif