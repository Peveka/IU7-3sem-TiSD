#include "../inc/simulation_core.h"
#include "../inc/array_queue.h"
#include "../inc/linked_queue.h"
#include "../inc/performance_analyzer.h"
#include "../inc/user_interface.h"
#include <time.h>

void run_array_based_simulation(const simulation_config_t *config);
void run_list_based_simulation(const simulation_config_t *config, memory_tracker_t *tracker);
void run_comparative_analysis(void);
void run_performance_comparison_test(void);

int main(void)
{
    srand(time(NULL));

    simulation_config_t simulation_config;
    initialize_simulation_config(&simulation_config);

    memory_tracker_t memory_tracker;
    initialize_memory_tracker(&memory_tracker);

    int user_choice = -1;

    while (user_choice != 0)
    {
        display_main_menu(&simulation_config);
        user_choice = get_user_choice(0, 9);

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
        case 8:
            run_comparative_analysis();
            break;
        case 9: 
            run_performance_comparison_test();
            break;
        case 0:
            printf("Программа завершена.\n");
            break;
        }
    }

    return OPERATION_SUCCESS;
}

void run_comparative_analysis(void)
{
    printf("\n════════════════ СРАВНИТЕЛЬНЫЙ АНАЛИЗ ════════════════\n");
    printf("Для точного сравнения необходимо запустить обе симуляции.\n");
    printf("1. Сначала запустите симуляцию на массиве (пункт 1)\n");
    printf("2. Затем запустите симуляцию на списке (пункт 2)\n");
    printf("После этого будет доступно автоматическое сравнение.\n");
    printf("═══════════════════════════════════════════════════════\n");
}

// Новая функция для сравнения производительности
void run_performance_comparison_test(void)
{
    printf("\n════════════════ СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ════════════════\n");
    printf("Выполняется комплексное тестирование производительности...\n");
    printf("Тестирование включает:\n");
    printf("1. Использование памяти при разных размерах очереди\n");
    printf("2. Время операций добавления/удаления\n");
    printf("══════════════════════════════════════════════════════════════\n");
    
    analyze_memory_usage();
    analyze_time_performance();
    
    printf("\n════════════════ РЕЗЮМЕ СРАВНЕНИЯ ════════════════\n");
    
    simulation_config_t config;
    initialize_simulation_config(&config);
    
    printf("Текущие настройки времени:\n");
    printf("• Время между заявками (T1): %.1f - %.1f\n", config.min_arrival_time, config.max_arrival_time);
    printf("• Время обработки (T2): %.1f - %.1f\n", config.min_processing_time, config.max_processing_time);
    printf("\n");
    
    printf("АНАЛИТИЧЕСКИЕ ВЫВОДЫ:\n");
    printf("1. Кольцевой буфер (массив):\n");
    printf("   • Фиксированное использование памяти: %zu байт\n", get_circular_buffer_memory_footprint());
    printf("   • Быстрые операции добавления/удаления O(1)\n");
    printf("   • Ограничен максимальным размером: %d элементов\n", MAX_QUEUE_CAPACITY);
    printf("   • Минимальная фрагментация памяти\n");
    printf("\n");
    
    printf("2. Связный список:\n");
    printf("   • Динамическое использование памяти\n");
    printf("   • Память: ~%zu байт на элемент\n", sizeof(queue_node_t));
    printf("   • Гибкость - нет ограничений по размеру\n");
    printf("   • Возможная фрагментация памяти\n");
    printf("\n");
    
    printf("РЕКОМЕНДАЦИИ:\n");
    printf("• Для предсказуемых нагрузок (до %d заявок) - используйте массив\n", MAX_QUEUE_CAPACITY);
    printf("• Для непредсказуемых/больших нагрузок - используйте список\n");
    printf("• Для систем с ограниченной памятью - массив более эффективен\n");
    printf("• Для систем с частыми вставками/удалениями - список может быть быстрее\n");
    printf("═══════════════════════════════════════════════════════\n");
}