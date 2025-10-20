#include "menu.h"
#include "dense_matrix.h"
#include "sparse_operations.h"
#include "performance.h"
#include <stdio.h>

int main()
{
    int choice;

    do
    {
        print_menu();
        choice = get_choice(1, 4);

        switch (choice)
        {
        case 1:
            dense_matrix_multiplication();
            break;

        case 2:
            sparse_matrix_multiplication();
            break;

        case 3:
            compare_performance();
            break;

        case 4:
            printf("Выход из программы.\n");
            break;
        }
    } while (choice != 4);

    return 0;
}