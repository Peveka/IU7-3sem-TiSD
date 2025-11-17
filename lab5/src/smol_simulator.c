#include <math.h>
#include <float.h>
#include "smol_simulator.h"
#include "queue_list.h"
#include "queue_array.h"

double g_t1_min = 0.0; 
double g_t1_max = 6.0; 
double g_t2_min = 0.0;
double g_t2_max = 1.0; 

// Структура, объединяющая все переменные состояния и статистики
typedef struct 
{
    // 1. Переменные состояния СМО
    double current_time;          // Текущее время моделирования
    void *queue_ptr;             // Указатель на очередь (list_t* или array_queue_t*)
    int oa_is_busy;              // Флаг, занят ли ОА (1 - да, 0 - нет)
    double time_arrival;          // Время прихода следующей заявки
    double time_service_finish;   // Время окончания обслуживания
    request_t req_serviced;      // Заявка, которая сейчас обслуживается
    int queue_type;              // Тип очереди (1 - список, 2 - массив)

    // 2. Статистические счетчики
    int entered_count;           // Всего вошло в систему
    int served_count;            // Всего вышло из системы (цель: MAX_SERVED_TOTAL)
    int served_in_oa_count;      // Количество срабатываний ОА
    double idle_time;            // Общее время простоя ОА
    double total_queue_len;       // Накопленная длина очереди * время (для среднего)
    double last_time;             // Время последнего изменения длины очереди
    int max_queue_len;           // Максимальная длина очереди
} smol_t;

// Генерация равномерно распределенного времени T
static double get_random_time(double t_min, double t_max)
{
    return t_min + (t_max - t_min) * ((double)rand() / RAND_MAX);
}

// Получить текущую длину очереди
static int get_queue_len(smol_t *s) 
{
    if (s->queue_type == 1) 
        return ((queue_t*)s->queue_ptr)->count; 
    else 
        return ((array_queue_t*)s->queue_ptr)->count;
}

// Включить элемент в очередь
static int do_enqueue(smol_t *s, request_t req) 
{
    if (s->queue_type == 1) 
        return enqueue((queue_t*)s->queue_ptr, req);
    else 
        return enqueue_array((array_queue_t*)s->queue_ptr, req);
}

// Исключить элемент из очереди
static int do_dequeue(smol_t *s, request_t *req) 
{
    if (s->queue_type == 1) 
        return dequeue((queue_t*)s->queue_ptr, req);
    else 
        return dequeue_array((array_queue_t*)s->queue_ptr, req);
}

// Обновление статистики по длине очереди
static void update_queue_stats(smol_t *s, double time)
{
    int len = get_queue_len(s); 
    // Накопление (длина * время, в течение которого она держалась)
    s->total_queue_len += len * (time - s->last_time);
    s->last_time = time;
    if (len > s->max_queue_len)
        s->max_queue_len = len;
}

// Обработка события: Приход новой заявки
static void handle_arrival(smol_t *s)
{
    // 1. Создание новой заявки
    request_t new_req = {
        .id = ++s->entered_count,
        .cycles = MAX_CYCLES,
        .time_in = s->current_time 
    };

    // 2. Добавление в очередь
    if (do_enqueue(s, new_req) != 0)
    {
        printf("Критическая ошибка: Очередь переполнена. Завершение симуляции.\n");
        s->served_count = MAX_SERVED_TOTAL;
        return;
    }

    // 3. Планирование следующего прихода
    s->time_arrival = s->current_time + get_random_time(g_t1_min, g_t1_max);
}

// Обработка события: Окончание обслуживания
static void handle_service_finish(smol_t *s)
{
    s->served_in_oa_count++;

    // 1. Проверка циклов (уменьшаем оставшиеся)
    s->req_serviced.cycles--;

    if (s->req_serviced.cycles > 0) 
    {
        // 2a. Рециркуляция: Добавление обратно в хвост очереди
        if (do_enqueue(s, s->req_serviced) != 0)
        {
            printf("Критическая ошибка: Очередь переполнена при рециркуляции. Завершение симуляции.\n");
            s->served_count = MAX_SERVED_TOTAL; // Принудительный выход из цикла
            return;
        }
    } 
    else 
    {
        // 2b. Выход: Заявка полностью завершила все циклы
        s->served_count++;
        
        // Промежуточный вывод статистики (после каждых 100 вышедших)
        if (s->served_count % 100 == 0)
        {
            printf("\n--- Промежуточный вывод (%d вышедших) ---\n", s->served_count);
            printf("Текущая длина очереди: %d\n", get_queue_len(s));
            printf("Средняя длина очереди: %.2f\n", s->total_queue_len / s->current_time);
            
            if (s->queue_type == 1) print_free_list();
            else print_queue_array(s->queue_ptr);

            printf("---------------------------------------\n");
        }
    }

    // 3. ОА освободился
    s->oa_is_busy = 0;
    s->time_service_finish = DBL_MAX; // Установка в бесконечность
}

// Функция для смены параметров
void change_parameters()
{
    printf("\n--- Смена Параметров Моделирования ---\n");

    printf("Текущий интервал прихода T1 (min: %.2lf, max: %.2lf)\n", g_t1_min, g_t1_max);
    printf("Введите новый T1_max (текущий %.2lf): ", g_t1_max);
    scanf("%lf", &g_t1_max);
    
    printf("Текущее время обслуживания T2 (min: %.2lf, max: %.2lf)\n", g_t2_min, g_t2_max);
    printf("Введите новый T2_max (текущий %.2lf): ", g_t2_max);
    scanf("%lf", &g_t2_max);
    
    printf("Новые параметры: T1 [0.0..%.2lf], T2 [0.0..%.2lf]\n", g_t1_max, g_t2_max);
}

void simulate_smol(int type) 
{
    queue_t q_list;
    array_queue_t q_array;
    
    // Инициализация структуры состояния СМО
    smol_t s = {
        .current_time = 0.0,
        .queue_ptr = NULL,
        .oa_is_busy = 0,
        .time_arrival = get_random_time(g_t1_min, g_t1_max),
        .time_service_finish = DBL_MAX,
        .req_serviced = {0},
        .queue_type = type,
        
        .entered_count = 0,
        .served_count = 0,
        .served_in_oa_count = 0,
        .idle_time = 0.0,
        .total_queue_len = 0.0,
        .last_time = 0.0,
        .max_queue_len = 0
    };

    // Инициализация очереди 
    if (type == 1) // Список
    {
        create_queue(&q_list);
        s.queue_ptr = &q_list;
        printf("\n--- Начинаем моделирование с ОЧЕРЕДЬЮ НА СПИСКЕ ---\n");
    } 
    else // Массив
    {
        create_queue_array(&q_array);
        s.queue_ptr = &q_array;
        printf("\n--- Начинаем моделирование с ОЧЕРЕДЬЮ НА МАССИВЕ (кольцевая) ---\n");
    }

    while (s.served_count < MAX_SERVED_TOTAL)
    {
        // 1. Определение ближайшего события
        double next_event_time = fmin(s.time_arrival, s.time_service_finish);
        
        // 2. Сдвиг времени и обновление статистики длины очереди
        double dt = next_event_time - s.current_time;
        if (dt < 0) 
            dt = 0; 
        s.current_time = next_event_time;
        update_queue_stats(&s, s.current_time);

        // 3. Обработка событий
        if (s.current_time == s.time_arrival) 
            handle_arrival(&s);
        
        if (s.current_time == s.time_service_finish) 
            handle_service_finish(&s);

        // 4. Проверка и Запуск Обслуживания (если ОА свободен И очередь не пуста)
        if (!s.oa_is_busy && get_queue_len(&s) > 0)
        {
            // Извлечение заявки из головы
            if (do_dequeue(&s, &s.req_serviced) == 0)
            {
                s.oa_is_busy = 1;
                // Планирование окончания обслуживания
                double service_time = get_random_time(g_t2_min, g_t2_max);
                s.time_service_finish = s.current_time + service_time;
            } 
        }

        // 5. Расчет простоя ОА
        if (!s.oa_is_busy && get_queue_len(&s) == 0 && s.current_time < s.time_arrival)
        {
             // ОА простаивает до прихода следующей заявки
             s.idle_time += s.time_arrival - s.current_time;
             s.current_time = s.time_arrival;
             update_queue_stats(&s, s.current_time);
        }
    }
    
    printf("\n======================================================\n");
    printf("           КОНЕЧНЫЕ РЕЗУЛЬТАТЫ МОДЕЛИРОВАНИЯ          \n");
    printf("======================================================\n");
    printf("Использованная структура: %s\n", (type == 1) ? "Список" : "Массив (кольцевой)");
    printf("Общее время моделирования: %.4lf е.в.\n", s.current_time);
    printf("------------------------------------------------------\n");
    printf("Количество вошедших в систему заявок: %d\n", s.entered_count);
    printf("Количество вышедших из системы заявок: %d (Цель: %d)\n", s.served_count, MAX_SERVED_TOTAL);
    printf("Количество срабатываний ОА: %d\n", s.served_in_oa_count);
    printf("Время простоя аппарата: %.4lf е.в.\n", s.idle_time);
    printf("------------------------------------------------------\n");
    printf("Средняя длина очереди: %.4f\n", s.total_queue_len / s.current_time);
    printf("Максимальная длина очереди: %d\n", s.max_queue_len);
    printf("------------------------------------------------------\n");

    if (type == 1) {
        printf("\n--- Анализ Памяти (Список) ---\n");
        print_free_list();
    }
}