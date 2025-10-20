#include "sparse_matrix.h"

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
                printf("\n--- Умножение обычных матриц ---\n");
                dense_matrix_input_test();
                break;
                
            case 2:
                printf("\n--- Умножение CSR на CSC ---\n");
                printf("Функционал в разработке...\n");
                // sparse_matrix_multiplication();
                break;
                
            case 3:
                printf("\n--- Сравнение производительности ---\n");
                printf("Функционал в разработке...\n");
                // compare_performance();
                break;
                
            case 4:
                printf("Выход из программы.\n");
                break;
        }
    }
    while (choice != 4);
    
    return 0;
}