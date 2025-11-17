#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "smol_simulator.h" 
#include "ui_utils.h"       

int main() 
{
    srand(time(NULL)); 

    int choice;
    
    do {
        print_menu();
        choice = scan_choice();

        switch (choice) {
            case 1:
                simulate_smol(1);
                break;
            case 2:
                simulate_smol(2);
                break;
            case 3:
                change_parameters();
                break;
            case 0:
                printf("Завершение программы.\n");
                break;
        }
    } while (choice != 0);

    return 0;
}