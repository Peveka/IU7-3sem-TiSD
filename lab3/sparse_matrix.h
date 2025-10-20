#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "alloc.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_menu();
int get_choice(int min, int max);
void clear_input_buffer();

void input_matrix_dimensions(int* rows, int* cols, const char* matrix_name);
error_t input_matrix_elements(dense_matrix_t* matrix);
void print_dense_matrix(const dense_matrix_t* matrix);
void dense_matrix_input_test();

#endif