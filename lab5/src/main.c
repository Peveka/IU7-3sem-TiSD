#include "../inc/simulation_core.h"
#include "../inc/array_queue.h"
#include "../inc/linked_queue.h"
#include "../inc/performance_analyzer.h"
#include "../inc/user_interface.h"

void run_array_based_simulation(const simulation_config_t *config);
void run_list_based_simulation(const simulation_config_t *config, memory_tracker_t *tracker);

int main(void)
{
    simulation_config_t simulation_config;
    initialize_simulation_config(&simulation_config);

    memory_tracker_t memory_tracker;
    initialize_memory_tracker(&memory_tracker);

    int user_choice = -1;

    while (user_choice != 0)
    {
        display_main_menu(&simulation_config);
        user_choice = get_user_choice(0, 7);

        switch (user_choice)
        {
        case 1:
            run_array_based_simulation(&simulation_config);
            break;
        case 2:
            run_list_based_simulation(&simulation_config, &memory_tracker);
            break;
        case 3:
            display_memory_operations(&memory_tracker);
            break;
        case 4:
            analyze_time_performance();
            break;
        case 5:
            analyze_memory_usage();
            break;
        case 6:
            modify_arrival_times(&simulation_config);
            break;
        case 7:
            modify_processing_times(&simulation_config);
            break;
        case 0:
            printf("Программа завершена.\n");
            break;
        }
    }

    return OPERATION_SUCCESS;
}