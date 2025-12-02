#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "common_defines.h"
#include "simulation_core.h"

void display_main_menu(const simulation_config_t *config);
int get_user_choice(int min_option, int max_option);
void modify_arrival_times(simulation_config_t *config);
void modify_processing_times(simulation_config_t *config);

#endif