#ifndef SIMULATION_CORE_H
#define SIMULATION_CORE_H

#include "common_defines.h"

typedef struct
{
    double min_arrival_time;
    double max_arrival_time;
    double min_processing_time;
    double max_processing_time;
    int total_requests;
    int output_frequency;
    int processing_cycles;
} simulation_config_t;

typedef struct
{
    int incoming_requests;
    int completed_requests;
    int processing_operations;

    double total_simulation_time;
    double total_arrival_time;
    double total_processing_time;
    double idle_time;

    int current_queue_size;
    double average_queue_size;
    double average_waiting_time;
} simulation_metrics_t;

void initialize_simulation_config(simulation_config_t *config);
void reset_simulation_metrics(simulation_metrics_t *metrics);

double generate_random_time(double min_time, double max_time);
int compare_doubles(double a, double b);

void display_intermediate_results(const simulation_metrics_t *metrics, const simulation_config_t *config, int step);
void display_final_results(const simulation_metrics_t *metrics, const simulation_config_t *config);

#endif