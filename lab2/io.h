#ifndef IO_H__
#define IO_H__
#include <stdio.h>
#include "struct.h"

int get_single_subscriber_from_stdin(subscriber_t *sub);
int get_data_from_stdin(subscriber_t *data, int *len);
int get_based_info(subscriber_t *sub, FILE* file);
int get_status_info(subscriber_t *sub, FILE* file);
int get_data_from_file(subscriber_t *data, const char *filename, int *len);
int write_data_to_file(subscriber_t *data, int count, const char *filename);

#endif
