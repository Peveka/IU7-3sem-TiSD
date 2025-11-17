#ifndef SMOL_SIMULATOR_H
#define SMOL_SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>

extern double g_t1_min; 
extern double g_t1_max; 
extern double g_t2_min;
extern double g_t2_max; 

#define MAX_CYCLES 5        
#define MAX_SERVED_TOTAL 1000 

void change_parameters();
void simulate_smol(int type);

#endif