#include <stdlib.h>
#include "menu.h"
#include "struct.h"
#include "io.h"
#include "date.h"
#include "sort.h"

#define MAX_FILENAME_LEN 50

int get_confirmation(const char* message) {
    char choice;
    printf("%s (y/n): ", message);
    scanf(" %c", &choice);
    return (choice == 'y' || choice == 'Y');
}

int menu_handler(subscriber_t *subscribers)
{
    int rc = 1;
    int len = 0;
    char filename_in[MAX_FILENAME_LEN];
    char filename_out[MAX_FILENAME_LEN];
    int mode = NO_MODE;
    int day = 0, month = 0, year = 0;
    int days_since_new_year = 0;
    key_table_t keys[MAX_FILE_LEN];
    int keys_table_exist = 0;
    
    while (mode != EXIT && rc)
    {
        printf("\n=================================================\n");
        printf("                 MAIN MENU\n");
        printf("\n=================================================\n");
        printf("1. Input from file\n");
        printf("2. Input from keyboard\n");
        printf("3. Output to file\n");
        printf("4. Output to screen\n");
        printf("5. Search birthdays\n");
        printf("6. Sort table (bubble sort)\n");
        printf("7. Sort table (qsort)\n");
        printf("8. Sort via keys (bubble sort)\n");
        printf("9. Sort via keys (qsort)\n");
        printf("10. Compare sort performance\n");
        printf("11. Exit\n");
        printf("12. Print keys table\n");
        printf("=================================================\n");
        printf("Select mode: ");
        
        if (scanf("%d", &mode) != 1) {
            printf("Input error! Please enter a number between 1 and 12.\n");
            continue;
        }

        switch(mode)
        {
            case FILE_INPUT:
            {
                int success = 0;
                do {
                    printf("Enter filename: ");
                    if (scanf("%50s", filename_in) != 1) {
                        printf("Filename input error!\n");
                        if (!get_confirmation("Try again?")) {
                            success = 1;
                            rc = 1;
                        }
                        continue;
                    }
                    int start_len = len;
                    rc = get_data_from_file(subscribers, filename_in, &len);
                    if (rc && len == start_len)
                    {
                        printf("File is empty\n");
                    }
                    if (rc && len != start_len)
                    {


                        printf("Data loaded successfully. Records: %d\n", len);
                        success = 1;
                    } else {
                        printf("Failed to load data from file: %s\n", filename_in);
                        if (!get_confirmation("Try again with different file?")) {
                            success = 1;
                            rc = 1;
                        }
                    }
                } while (!success);

                

                break;
            }

            case STDIN_INPUT:
            {
                int success = 0;
                do {
                    rc = get_data_from_stdin(subscribers, &len);
                    if (rc) {
                        printf("Data entered successfully. Records: %d\n", len);
                        success = 1;
                    } else {
                        printf("Error during manual input!\n");
                        if (!get_confirmation("Try again?")) {
                            success = 1;
                            rc = 1;
                        }
                    }
                } while (!success);
                break;
            }

            case FILE_OUTPUT:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to save.\n");
                    break;
                }
                
                int success = 0;
                do {
                    printf("Enter filename: ");
                    if (scanf("%50s", filename_out) != 1) {
                        printf("Filename input error!\n");
                        if (!get_confirmation("Try again?")) {
                            success = 1; // Выход в меню при выборе 'n'
                            rc = 1; // Гарантируем продолжение работы
                        }
                        continue;
                    }
                    
                    rc = write_data_to_file(subscribers, len, filename_out);
                    if (rc) {
                        printf("Data successfully saved to file: %s\n", filename_out);
                        success = 1;
                    } else {
                        printf("Failed to save data to file: %s\n", filename_out);
                        if (!get_confirmation("Try again with different filename?")) {
                            success = 1;
                            rc = 1;
                        }
                    }
                } while (!success);
                break;
            }

            case STDIN_OUTPUT:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to display.\n");
                    break;
                }
                
                rc = write_data_to_file(subscribers, len, "stdout");
                if (!rc) {
                    printf("Error during screen output!\n");
                }
                break;
            }
            
            case SEARCH_DATA:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to search.\n");
                    break;
                }
                
                int success = 0;
                do {
                    printf("Enter current date (day month year): ");
                    if (scanf("%d %d %d", &day, &month, &year) != 3) {
                        printf("Date input error! Please enter three numbers.\n");
                        if (!get_confirmation("Try again?")) {
                            success = 1;
                            rc = 1;
                        }
                        continue;
                    }
                    
                    // Простая проверка корректности даты
                    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
                        printf("Invalid date! Day (1-31), Month (1-12), Year (1900-2100)\n");
                        if (!get_confirmation("Try again?")) {
                            success = 1;
                            rc = 1;
                        }
                        continue;
                    }
                    
                    days_since_new_year = get_day_since_new_year(day, month, year);
                    search_date(subscribers, days_since_new_year, len);
                    success = 1;
                    
                } while (!success);
                break;
            }

            case SORT_TABLE_BUBBLE:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to sort.\n");
                    break;
                }
                
                bubble_sort(subscribers, len, sizeof(subscriber_t), compare_surname);
                printf("Table successfully sorted by surname (bubble sort)\n");
                
                if (get_confirmation("View sorted table on screen")) {
                    write_data_to_file(subscribers, len, "stdout");
                }
                break;
            }

            case SORT_TABLE_QSORT:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to sort.\n");
                    break;
                }
                
                qsort(subscribers, len, sizeof(subscriber_t), compare_surname);
                printf("Table successfully sorted by surname (qsort)\n");
                
                if (get_confirmation("View sorted table on screen")) {
                    write_data_to_file(subscribers, len, "stdout");
                }
                break;
            }

            case SORT_KEYS_BUBBLE:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to sort.\n");
                    break;
                }
                
                if (!keys_table_exist) {
                    keys_table_exist = 1;
                    create_key_table(subscribers, keys, len);
                    printf("Keys table created successfully.\n");
                }
                
                bubble_sort(keys, len, sizeof(key_table_t), compare_keys);
                printf("Keys table sorted by surname (bubble sort)\n");
                print_via_keys(subscribers, keys, len);
                break;
            }

            case SORT_KEYS_QSORT:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to sort.\n");
                    break;
                }
                
                if (!keys_table_exist) {
                    keys_table_exist = 1;
                    create_key_table(subscribers, keys, len);
                    printf("Keys table created successfully.\n");
                }
                
                qsort(keys, len, sizeof(key_table_t), compare_keys);
                printf("Keys table sorted by surname (qsort)\n");
                print_via_keys(subscribers, keys, len);
                break;
            }

            case COMPARE_SORTS:
            {
                if (len == 0) {
                    printf("Table is empty! Nothing to compare.\n");
                    break;
                }
                measure_sorts(subscribers, len);
                break;
            }

            case PRINT_KEYS_TABLE:
            {
                if (len == 0) {
                    printf("Table is empty! No keys to display.\n");
                    break;
                }
                
                if (!keys_table_exist) {
                    keys_table_exist = 1;
                    create_key_table(subscribers, keys, len);
                    printf("Keys table created successfully.\n");
                }
                
                print_keys_table(keys, len);
                break;
            }

            case EXIT:
            {
                if (len > 0) {
                    if (get_confirmation("Save data before exiting")) {
                        int saved = 0;
                        do {
                            printf("Enter filename to save: ");
                            if (scanf("%50s", filename_out) != 1) {
                                printf("Filename input error!\n");
                                if (!get_confirmation("Try again?")) {
                                    saved = 1;
                                    rc = 1;
                                }
                                continue;
                            }
                            
                            rc = write_data_to_file(subscribers, len, filename_out);
                            if (rc) {
                                printf("Data successfully saved to: %s\n", filename_out);
                                saved = 1;
                            } else {
                                printf("Failed to save data!\n");
                                if (!get_confirmation("Try again with different filename?")) {
                                    saved = 1; 
                                    rc = 1; 
                                }
                            }
                        } while (!saved);
                    }
                }
                
                printf("Exiting program\n");
                break;
            }

            default:
            {
                printf("Invalid mode (%d). Please choose a number between 1 and 12.\n", mode);
                printf("Available options:\n");
                printf("1-Input from file, 2-Input from keyboard, 3-Output to file, 4-Output to screen\n");
                printf("5-Search birthdays, 6-Sort table (bubble), 7-Sort table (qsort)\n");
                printf("8-Sort via keys (bubble), 9-Sort via keys (qsort), 10-Compare sorts\n");
                printf("11-Exit, 12-Print keys table\n");
                break;
            }
        }
    }

    return rc;
}