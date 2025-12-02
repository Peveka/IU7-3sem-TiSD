#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#define MAX_QUEUE_CAPACITY 1000
#define MAX_SIMULATION_CYCLES 5
#define DEFAULT_REQUEST_COUNT 1000
#define OUTPUT_INTERVAL 100
#define TIME_EPSILON 1e-6

#define MIN_ARRIVAL_TIME 0.0
#define MAX_ARRIVAL_TIME 6.0
#define MIN_PROCESSING_TIME 0.0
#define MAX_PROCESSING_TIME 1.0

#define MEMORY_MEASURE_STEP 100
#define MAX_MEMORY_MEASURE 1000
#define TIME_MEASURE_ITERATIONS 100

typedef enum {
    OPERATION_SUCCESS = 0,
    MEMORY_ALLOCATION_FAILED = -1,
    QUEUE_OVERFLOW = -2,
    QUEUE_EMPTY = -3,
    INVALID_INPUT = -4
} error_code_t;

#endif