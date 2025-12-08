#ifndef PERFORMANCE_ANALYZER_H
#define PERFORMANCE_ANALYZER_H

#include "common_defines.h"

size_t get_circular_buffer_memory_footprint(void);
size_t get_linked_queue_memory_footprint(size_t element_count);

void display_memory_comparison_header(void);
void display_memory_comparison_row(size_t count, size_t array_memory, size_t list_memory);
void analyze_memory_usage(void);

void display_time_comparison_header(void);
void display_time_comparison_row(size_t count, int64_t array_enqueue_time, int64_t array_dequeue_time,
                                 int64_t list_enqueue_time, int64_t list_dequeue_time);
void analyze_time_performance(void);

#endif