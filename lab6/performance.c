#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

static void remove_duplicates_from_string(char *str)
{
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = i + 1; j < len; j++)
        {
            if (str[i] == str[j])
            {
                for (size_t k = j; k < len; k++)
                {
                    str[k] = str[k + 1];
                }
                len--;
                j--;
            }
        }
    }
}

void compare_efficiency(const char *input_string)
{
    clock_t start_time, end_time;
    double tree_time_total = 0, string_time_total = 0;
    const int ITERATIONS = 100;

    size_t input_len = strlen(input_string);

    printf("\n=== COMPARING DUPLICATE REMOVAL ===\n");
    printf("Input string: %s\n", input_string);
    printf("Input length: %zu chars\n\n", input_len);

    tree_node *initial_tree = build_tree_from_string(input_string);
    printf("1. Initial tree:\n");
    if (initial_tree)
    {
        print_tree_visual(initial_tree);
        printf("In-order with duplicates: ");
        print_with_duplicates(initial_tree);
        printf("\n");
    }
    else
    {
        printf("(empty)\n");
    }

    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        tree_node *tree_copy = build_tree_from_string(input_string);

        start_time = clock();
        tree_node *result = remove_duplicates_from_tree(tree_copy);
        end_time = clock();

        tree_time_total += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        if (result)
            free_tree(result);
    }
    double tree_time_avg = tree_time_total / ITERATIONS;

    size_t result_len = 0;
    char *final_string = NULL;

    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        char *string_copy = (char *)malloc(input_len + 1);
        strcpy(string_copy, input_string);

        start_time = clock();
        remove_duplicates_from_string(string_copy);
        end_time = clock();

        string_time_total += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        if (iter == ITERATIONS - 1)
        {
            result_len = strlen(string_copy);
            final_string = (char *)malloc(result_len + 1);
            strcpy(final_string, string_copy);
        }

        free(string_copy);
    }
    double string_time_avg = string_time_total / ITERATIONS;

    tree_node *final_tree = build_tree_from_string(input_string);
    tree_node *result_tree = remove_duplicates_from_tree(final_tree);

    printf("\n2. Tree after removing duplicates:\n");
    if (result_tree)
    {
        print_tree_visual(result_tree);
        printf("Post-order traversal: ");
        postorder_traversal(result_tree);
        printf("\n");
    }
    else
    {
        printf("(empty tree)\n");
    }

    printf("\n3. String after removing duplicates:\n");
    if (final_string)
    {
        printf("%s\n", final_string);
        free(final_string);
    }

    int unique_count = initial_tree ? get_node_count(initial_tree) : 0;
    int tree_height = initial_tree ? get_tree_height(initial_tree) : 0;
    int final_count = result_tree ? get_node_count(result_tree) : 0;

    printf("\n=== STATISTICS ===\n");
    printf("Initial length: %zu chars\n", input_len);
    printf("Final length: %zu chars\n", result_len);
    printf("Removed: %zu chars\n", input_len - result_len);
    printf("Unique elements in tree: %d\n", unique_count);
    printf("Elements in final tree: %d\n", final_count);
    printf("Tree height: %d\n", tree_height);

    if (initial_tree)
    {
        int left_h = initial_tree->left ? get_tree_height(initial_tree->left) : 0;
        int right_h = initial_tree->right ? get_tree_height(initial_tree->right) : 0;
        printf("Balanced: %s (diff: %d)\n",
               abs(left_h - right_h) <= 1 ? "YES" : "NO",
               abs(left_h - right_h));
    }
    else
    {
        printf("Balanced: N/A\n");
    }

    printf("\n=== PERFORMANCE (avg of %d runs) ===\n", ITERATIONS);
    printf("Tree time:   %.8f sec\n", tree_time_avg);
    printf("String time: %.8f sec\n", string_time_avg);

    if (tree_time_avg < string_time_avg)
    {
        printf("Tree is %.2fx faster\n", string_time_avg / tree_time_avg);
    }
    else if (string_time_avg < tree_time_avg)
    {
        printf("String is %.2fx faster\n", tree_time_avg / string_time_avg);
    }
    else
    {
        printf("Same speed\n");
    }

    printf("====================================\n");

    if (initial_tree)
        free_tree(initial_tree);
    if (result_tree)
        free_tree(result_tree);
}

double measure_delete_time(tree_node *tree, char element, int iterations)
{
    if (tree == NULL)
        return 0.0;

    clock_t start_time, end_time;
    double total_time = 0;

    for (int i = 0; i < iterations; i++)
    {
        tree_node *copy = NULL;
        start_time = clock();
        delete_node(tree, element);
        end_time = clock();
        total_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    return total_time / iterations;
}

double measure_inorder_time(tree_node *tree, int iterations)
{
    if (tree == NULL)
        return 0.0;

    clock_t start_time, end_time;
    double total_time = 0;

    FILE *null_output = fopen("/dev/null", "w");
    FILE *original_stdout = stdout;

    for (int i = 0; i < iterations; i++)
    {
        stdout = null_output;

        start_time = clock();
        print_with_duplicates(tree);
        end_time = clock();

        stdout = original_stdout;

        total_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    if (null_output)
        fclose(null_output);
    stdout = original_stdout;

    return total_time / iterations;
}

void benchmark_sort_and_search(const char *input_string)
{
    printf("\n=== BENCHMARK SORT AND SEARCH ===\n");
    printf("Input string length: %zu\n", strlen(input_string));

    const int ITERATIONS = 10000;

    tree_node *tree = build_tree_from_string(input_string);
    if (tree == NULL)
    {
        printf("Cannot build tree\n");
        return;
    }

    clock_t start_time, end_time;

    // 1. СОБИРАЕМ ВСЕ УНИКАЛЬНЫЕ ЭЛЕМЕНТЫ ИЗ ДЕРЕВА
    int unique_count = get_node_count(tree);
    printf("Unique elements in tree: %d\n", unique_count);

    // Собираем все уникальные элементы в массив
    char *unique_elements = (char *)malloc(unique_count);
    if (!unique_elements)
    {
        printf("Memory allocation failed for unique elements\n");
        free_tree(tree);
        return;
    }

    // Обход in-order для сбора всех уникальных элементов
    int idx = 0;
    tree_node *stack[256];
    int top = -1;
    tree_node *current = tree;

    while (current != NULL || top >= 0)
    {
        while (current != NULL)
        {
            if (top < 255)
                stack[++top] = current;
            current = current->left;
        }

        if (top >= 0)
        {
            current = stack[top--];
            unique_elements[idx++] = current->data;
            current = current->right;
        }
    }

    // 2. ТЕСТИРУЕМ ПОИСК ДЛЯ КАЖДОГО УНИКАЛЬНОГО ЭЛЕМЕНТА
    printf("\nTesting search for ALL %d unique elements...\n", unique_count);

    double total_search_time = 0;
    int search_tests = 0;

    for (int i = 0; i < unique_count; i++)
    {
        char element = unique_elements[i];
        double search_time_sum = 0;

        // Многократный поиск одного элемента
        for (int j = 0; j < ITERATIONS; j++)
        {
            start_time = clock();
            tree_node *found = find_node(tree, element);
            end_time = clock();
            search_time_sum += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

            // Проверяем, что элемент найден (на первой итерации)
            if (j == 0 && found == NULL)
            {
                printf("WARNING: Element %d not found in tree!\n", (int)element);
            }
        }

        double avg_search_time_for_element = search_time_sum / ITERATIONS;
        total_search_time += avg_search_time_for_element;
        search_tests++;

        // Прогресс для больших деревьев
        if (unique_count > 50 && i % (unique_count / 10) == 0)
        {
            printf("  Progress: %d/%d elements tested\n", i, unique_count);
        }
    }

    double avg_search_time = total_search_time / search_tests;

    // 3. ТЕСТИРУЕМ ОБХОД ВСЕГО ДЕРЕВА
    printf("\nTesting in-order traversal of entire tree...\n");

    double total_inorder_time = 0;

    for (int i = 0; i < ITERATIONS; i++)
    {
        start_time = clock();

        // Итеративный обход in-order (без вывода)
        if (tree != NULL)
        {
            tree_node *stack[256];
            int top = -1;
            tree_node *current = tree;
            int visited = 0;

            while (current != NULL || top >= 0)
            {
                while (current != NULL)
                {
                    if (top < 255)
                        stack[++top] = current;
                    current = current->left;
                }

                if (top >= 0)
                {
                    current = stack[top--];
                    visited++; // "посещаем" узел
                    current = current->right;
                }
            }
        }

        end_time = clock();
        total_inorder_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    double avg_inorder_time = total_inorder_time / ITERATIONS;

    // 4. ВЫВОД РЕЗУЛЬТАТОВ
    printf("\n=== RESULTS (avg of %d runs) ===\n", ITERATIONS);
    printf("In-order traversal time: %.10f sec\n", avg_inorder_time);
    printf("Average search time:     %.10f sec (averaged over %d unique elements)\n",
           avg_search_time, search_tests);
    printf("Total search operations: %d × %d = %d\n",
           search_tests, ITERATIONS, search_tests * ITERATIONS);

    printf("\n=== TREE INFO ===\n");
    printf("Total nodes (unique): %d\n", unique_count);
    printf("Tree height: %d\n", get_tree_height(tree));

    // Расчет теоретической эффективности
    double optimal_height = log2(unique_count);
    printf("Optimal height (log₂ %d): %.1f\n", unique_count, optimal_height);

    int actual_height = get_tree_height(tree);
    double efficiency_ratio = (double)actual_height / optimal_height;

    printf("Height efficiency ratio: %.2f\n", efficiency_ratio);

    if (efficiency_ratio < 2.0)
    {
        printf("Tree balance: GOOD (near optimal)\n");
    }
    else if (efficiency_ratio < 4.0)
    {
        printf("Tree balance: FAIR\n");
    }
    else
    {
        printf("Tree balance: POOR (unbalanced)\n");
    }

    // Очистка
    free(unique_elements);
    free_tree(tree);
    printf("==============================\n");
}

void benchmark_different_configs()
{
    printf("\n=== BENCHMARK DIFFERENT CONFIGURATIONS ===\n");

    const char *tests[] = {
        "ABCDEFGHIJ",
        "FACEBDGIHJK",
        "AAABBBCCCDDD",
        "ZYXWVUTSRQ",
        "MJNKBVCFXZAQWE",
        "1234567890"};

    // Выводим тесты из заданного списка
    for (int i = 0; i < 6; i++)
    {
        printf("\nTest %d: %s\n", i + 1, tests[i]);
        benchmark_sort_and_search(tests[i]);
    }

    // Добавляем тесты сгенерированных деревьев БЕЗ ПОВТОРОВ
    printf("\n=== GENERATED TREES BENCHMARK (SAME DATA, DIFFERENT INSERTION ORDER) ===\n");

    // Размеры деревьев для тестирования
    int sizes[] = {100, 150, 200, 250};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++)
    {
        int size = sizes[s];

        printf("\n\n=== TESTING SIZE: %d UNIQUE VALUES ===\n", size);

        // 1. СОЗДАЕМ ОДИН НАБОР УНИКАЛЬНЫХ ЗНАЧЕНИЙ
        char *unique_values = (char *)malloc(size);
        if (!unique_values)
        {
            printf("Memory allocation failed for size %d\n", size);
            continue;
        }
        int base_value = -128;
        // Заполняем уникальными значениями (0..size-1)
        for (int i = 0; i < size; i++)
        {
            base_value++;
            unique_values[i] = (char) base_value; // Уникальные значения от 0 до size-1
        }

        printf("Generated %d unique values (range: 0 to %d)\n", size, size - 1);

        // 2. ТЕСТ 1: СБАЛАНСИРОВАННОЕ ДЕРЕВО (случайный порядок вставки)
        printf("\n--- TEST 1: BALANCED TREE (random insertion order) ---\n");

        // Создаем копию для перемешивания
        char *shuffled_values = (char *)malloc(size);
        memcpy(shuffled_values, unique_values, size);

        // Перемешиваем (случайный порядок)
        srand(time(NULL) + s);
        for (int i = size - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            char temp = shuffled_values[i];
            shuffled_values[i] = shuffled_values[j];
            shuffled_values[j] = temp;
        }

        // Строим сбалансированное дерево
        tree_node *balanced_tree = NULL;
        for (int i = 0; i < size; i++)
        {
            balanced_tree = insert_node(balanced_tree, shuffled_values[i]);
        }

        //printf("Balanced tree built\n");
        printf("Insertion order (first 10): ");
        for (int i = 0; i < 10 && i < size; i++)
        {
            printf("%d ", (int)shuffled_values[i]);
        }
        printf("...\n");

        // Запускаем тест
        benchmark_tree_direct(balanced_tree, size, "BALANCED");

        // 3. ТЕСТ 2: ВЫРОЖДЕННОЕ ДЕРЕВО (отсортированный порядок вставки)
        printf("\n--- TEST 2: DEGENERATED TREE (sorted insertion order) ---\n");

        // Используем ТЕ ЖЕ САМЫЕ значения, но в отсортированном порядке
        // (у нас unique_values уже отсортированы по возрастанию)

        tree_node *degenerated_tree = NULL;
        for (int i = 0; i < size; i++)
        {
            degenerated_tree = insert_node(degenerated_tree, unique_values[i]); // 0, 1, 2, 3...
        }

        printf("Degenerated tree built (inserted in sorted order: 0, 1, 2, ...)\n");

        // Запускаем тест
        benchmark_tree_direct(degenerated_tree, size, "DEGENERATED");

        // 4. СРАВНЕНИЕ РЕЗУЛЬТАТОВ
        printf("\n=== COMPARISON FOR SIZE %d ===\n", size);

        int balanced_height = get_tree_height(balanced_tree);
        int degenerated_height = get_tree_height(degenerated_tree);
        int balanced_nodes = get_node_count(balanced_tree);
        int degenerated_nodes = get_node_count(degenerated_tree);

        printf("Balanced tree:   %d nodes, height = %d\n",
               balanced_nodes, balanced_height);
        printf("Degenerated tree: %d nodes, height = %d\n",
               degenerated_nodes, degenerated_height);

        if (balanced_nodes == degenerated_nodes && degenerated_nodes == size)
        {
            printf("✓ Both trees contain all %d unique values\n", size);
        }
        else
        {
            printf("✗ Tree size mismatch! Check for duplicates.\n");
            printf("  Balanced: %d, Degenerated: %d, Expected: %d\n",
                   balanced_nodes, degenerated_nodes, size);
        }

        printf("\nHeight ratio (degenerated/balanced): %.2f\n",
               (double)degenerated_height / balanced_height);

        if (degenerated_height == size)
        {
            printf("✓ Degenerated tree is a linked list (worst case)\n");
        }

        // if (balanced_height <= (int)(log2(size) + 1))
        // {
        //     printf("✓ Balanced tree is near optimal height\n");
        // }

        // 5. ОЧИСТКА
        free(unique_values);
        free(shuffled_values);
        free_tree(balanced_tree);
        free_tree(degenerated_tree);

        printf("==============================================\n");
    }

    printf("\n=== BENCHMARK COMPLETED ===\n");
    printf("\nSUMMARY:\n");
    printf("- Same data used for both trees\n");
    printf("- Balanced: Random insertion order → O(log n) operations\n");
    printf("- Degenerated: Sorted insertion order → O(n) operations\n");
    printf("- All trees contain unique values without duplicates\n");
}

// Улучшенная функция для тестирования дерева
void benchmark_tree_direct(tree_node *tree, int expected_size, const char *type)
{
    if (tree == NULL)
    {
        printf("Error: Tree is NULL\n");
        return;
    }

    // Уменьшаем итерации, но делаем тест более честным
    const int ITERATIONS_PER_ELEMENT = 100;
    clock_t start_time, end_time;

    // Получаем статистику дерева
    int actual_size = get_node_count(tree);
    int height = get_tree_height(tree);

    printf("\n[%s TREE] Nodes: %d, Height: %d\n", type, actual_size, height);

    if (actual_size != expected_size)
    {
        printf("WARNING: Expected %d nodes, got %d\n", expected_size, actual_size);
    }

    // 1. СОБИРАЕМ ВСЕ ЭЛЕМЕНТЫ ДЕРЕВА
    printf("Collecting all %d elements from tree...\n", actual_size);

    char *all_values = (char *)malloc(actual_size);
    if (!all_values)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    // Обход in-order для сбора всех значений
    int idx = 0;
    tree_node *stack[512];
    int top = -1;
    tree_node *current = tree;

    while (current != NULL || top >= 0)
    {
        while (current != NULL)
        {
            if (top < 511)
                stack[++top] = current;
            current = current->left;
        }

        if (top >= 0)
        {
            current = stack[top--];
            all_values[idx++] = current->data;
            current = current->right;
        }
    }

    printf("Collected %d unique values\n", idx);

    // 2. ТЕСТИРУЕМ ПОИСК КАЖДОГО ЭЛЕМЕНТА (БЕЗ КЭШИРОВАНИЯ ПУТИ)
    printf("\nTesting search for ALL %d elements (avoiding cache)...\n", actual_size);

    double total_search_time_all_elements = 0.0;
    int total_searches = 0;

    // Перемешиваем массив значений, чтобы избежать последовательного доступа
    srand(time(NULL));
    for (int i = actual_size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        char temp = all_values[i];
        all_values[i] = all_values[j];
        all_values[j] = temp;
    }

    // Тестируем поиск в случайном порядке
    for (int elem_idx = 0; elem_idx < actual_size; elem_idx++)
    {
        char value = all_values[elem_idx];

        // Измеряем время одного поиска (без многократных итераций)
        start_time = clock();

        // Делаем несколько поисков, но сбрасываем кэш через поиск других значений
        for (int iter = 0; iter < ITERATIONS_PER_ELEMENT; iter++)
        {
            // Ищем текущее значение
            tree_node *found = find_node(tree, value);

            // Чтобы сбросить кэш, ищем еще 10 случайных значений
            for (int k = 0; k < 10; k++)
            {
                int random_idx = rand() % actual_size;
                find_node(tree, all_values[random_idx]);
            }

            if (found == NULL)
            {
                printf("ERROR: Element %d not found!\n", (int)value);
                break;
            }
        }

        end_time = clock();

        double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        double avg_search_time = total_time / ITERATIONS_PER_ELEMENT;

        total_search_time_all_elements += avg_search_time;
        total_searches++;
    }

    // Среднее время поиска одного элемента
    double avg_search_per_element = total_search_time_all_elements / total_searches;

    printf("Average search time per element: %.10f sec (%.2f µs)\n",
           avg_search_per_element, avg_search_per_element * 1000000);

    // 3. ТЕОРЕТИЧЕСКИЕ РАСЧЕТЫ
    printf("\n=== THEORETICAL ANALYSIS ===\n");

    double optimal_height = log2(actual_size);
    printf("Optimal height (log₂ %d): %.1f\n", actual_size, optimal_height);

    if (strcmp(type, "BALANCED") == 0)
    {
        printf("Expected comparisons per search: ~%.0f\n", optimal_height);
        printf("Actual height: %d (%.1fx optimal)\n",
               height, (double)height / optimal_height);
    }
    else
    {
        printf("Expected comparisons per search: ~%d (average)\n", actual_size / 2);
        printf("Actual height: %d (%.1fx optimal)\n",
               height, (double)height / optimal_height);
        printf("Theoretical speed difference: %.1fx slower\n",
               ((double)actual_size / 2) / optimal_height);
    }

    printf("\nTesting in-order traversal...\n");

    double total_traversal_time = 0.0;
    const int TRAVERSAL_ITERATIONS = 100;

    for (int iter = 0; iter < TRAVERSAL_ITERATIONS; iter++)
    {
        start_time = clock();

        // Итеративный обход
        if (tree != NULL)
        {
            tree_node *stack[512];
            int top = -1;
            tree_node *current = tree;

            while (current != NULL || top >= 0)
            {
                while (current != NULL)
                {
                    if (top < 511)
                        stack[++top] = current;
                    current = current->left;
                }

                if (top >= 0)
                {
                    current = stack[top--];
                    current = current->right;
                }
            }
        }

        end_time = clock();
        total_traversal_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    double avg_traversal_time = total_traversal_time / TRAVERSAL_ITERATIONS;
    printf("Average traversal time: %.10f sec (%.2f µs)\n",
           avg_traversal_time, avg_traversal_time * 1000000);

    printf("\n====================================\n");

    free(all_values);
}

// Прямой benchmark без преобразования в строку
void benchmark_tree_operations_direct(tree_node *tree, int expected_size, const char *type)
{
    if (tree == NULL)
    {
        printf("Error: Tree is NULL\n");
        return;
    }

    printf("\n=== DIRECT BENCHMARK FOR %s TREE (size: %d) ===\n",
           type, expected_size);

    const int ITERATIONS = 10000;
    clock_t start_time, end_time;

    // Тест обхода in-order
    double total_inorder_time = 0;

    for (int i = 0; i < ITERATIONS; i++)
    {
        start_time = clock();

        // Итеративный обход in-order
        tree_node *stack[256];
        int top = -1;
        tree_node *current = tree;

        while (current != NULL || top >= 0)
        {
            while (current != NULL)
            {
                stack[++top] = current;
                current = current->left;
            }

            current = stack[top--];
            current = current->right;
        }

        end_time = clock();
        total_inorder_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    double avg_inorder_time = total_inorder_time / ITERATIONS;

    // Тест поиска
    double total_search_time = 0;
    int search_tests = 0;

    // Тестируем поиск некоторых элементов
    int test_indices[] = {0, expected_size / 4, expected_size / 2, 3 * expected_size / 4, expected_size - 1};
    int num_tests = sizeof(test_indices) / sizeof(test_indices[0]);

    // Сначала собираем значения из дерева для тестирования
    char *test_values = (char *)malloc(num_tests);
    if (test_values)
    {
        // Получаем значения для тестирования через обход
        tree_node *stack[256];
        int top = -1;
        tree_node *current = tree;
        int index = 0;

        while (current != NULL || top >= 0)
        {
            while (current != NULL)
            {
                stack[++top] = current;
                current = current->left;
            }

            current = stack[top--];

            // Сохраняем значения по выбранным индексам
            for (int i = 0; i < num_tests; i++)
            {
                if (index == test_indices[i])
                {
                    test_values[i] = current->data;
                }
            }

            index++;
            current = current->right;
        }

        // Тестируем поиск
        for (int i = 0; i < num_tests; i++)
        {
            double search_time_sum = 0;

            for (int j = 0; j < ITERATIONS; j++)
            {
                start_time = clock();
                tree_node *found = find_node(tree, test_values[i]);
                end_time = clock();
                search_time_sum += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

                // Небольшая проверка
                if (found == NULL && j == 0)
                {
                    printf("WARNING: Element %d not found in tree!\n", (int)test_values[i]);
                }
            }

            total_search_time += search_time_sum / ITERATIONS;
            search_tests++;
        }

        free(test_values);
    }

    double avg_search_time = search_tests > 0 ? total_search_time / search_tests : 0;

    // Выводим статистику дерева
    int actual_size = get_node_count(tree);
    int height = get_tree_height(tree);

    printf("\n=== RESULTS (avg of %d runs) ===\n", ITERATIONS);
    printf("In-order traversal time: %.10f sec\n", avg_inorder_time);
    printf("Average search time:     %.10f sec\n", avg_search_time);
    printf("Search tests performed:  %d\n", search_tests);

    printf("\n=== TREE STATISTICS ===\n");
    printf("Expected size:    %d\n", expected_size);
    printf("Actual tree size: %d\n", actual_size);
    printf("Tree height:      %d\n", height);
    printf("Balance factor:   %d\n", get_tree_balance_factor(tree));

    printf("===================================\n");
}

// Новая вспомогательная функция для работы с char массивами
void benchmark_tree_operations_array(const char *input, int size, const char *type)
{
    printf("\nBuilding %s tree from %d unique chars...\n", type, size);

    // Создаем дерево
    tree_node *tree = NULL;
    for (int i = 0; i < size; i++)
    {
        tree = insert_node(tree, input[i]);
    }

    if (tree == NULL)
    {
        printf("Error: Failed to build tree\n");
        return;
    }

    // Проверяем размер дерева
    int tree_nodes = get_node_count(tree);
    printf("Tree built successfully. Nodes in tree: %d\n", tree_nodes);

    if (tree_nodes != size)
    {
        printf("WARNING: Tree nodes (%d) != input size (%d)\n", tree_nodes, size);
        printf("This indicates duplicates in input\n");
    }

    // Запускаем benchmark
    printf("\nRunning benchmark_sort_and_search...\n");

    // Создаем временную строку для benchmark_sort_and_search
    char *temp_string = (char *)malloc(size + 1);
    if (temp_string)
    {
        for (int i = 0; i < size; i++)
        {
            temp_string[i] = input[i];
        }
        temp_string[size] = '\0';

        benchmark_sort_and_search(temp_string);
        free(temp_string);
    }
    else
    {
        printf("Failed to allocate temp string\n");
    }

    // Освобождаем память
    free_tree(tree);
}

void compare_different_trees(void)
{
    printf("\n=== COMPARING DIFFERENT TREE CONFIGURATIONS ===\n");

    const char *test_cases[] =
        {
            "ABCDEFGHIJ",
            "FACEBDGIHJK",
            "AAABBBCCCDDD",
            "ZYXWVUTSRQ",
            "MJNKBVCFXZAQWE",
            "ikissthegirlilikeitandstillrememberthisdaymyfriendsoitshardtobreath",
            "88888888asdadsa",
            "1234567890",
            "a1b2c3d4e5f6g7h8i9j0",
            "111222333444555666777888999000"};

    for (int i = 0; i < 10; i++)
    {
        printf("\nTest %d: %s\n", i + 1, test_cases[i]);

        tree_node *tree = build_tree_from_string(test_cases[i]);
        if (tree)
        {
            char filename_dot[50];
            char filename_png[50];

            snprintf(filename_dot, sizeof(filename_dot), "tree_test_%d.dot", i + 1);
            snprintf(filename_png, sizeof(filename_png), "tree_test_%d.png", i + 1);

            visualize_tree_graphviz(tree, filename_dot);

            char command[100];
            snprintf(command, sizeof(command), "dot -Tpng %s -o %s 2>/dev/null", filename_dot, filename_png);
            system(command);

            printf("Tree saved as: %s\n", filename_png);

            free_tree(tree);
        }

        compare_efficiency(test_cases[i]);
    }
}