#include <stdlib.h>
#include "menu.h"
#include "struct.h"
#include "io.h"
#include "date.h"
#include "sort.h"

#define MAX_FILENAME_LEN 50

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
            printf("Input error!\n");
            while (getchar() != '\n');
            continue;
        }

        switch(mode)
        {
            case FILE_INPUT:
            {
                printf("Enter filename: ");
                scanf("%50s", filename_in);
                rc = get_data_from_file(subscribers, filename_in, &len);
                if (rc) printf("Data loaded. Records: %d\n", len);
                break;
            }

            case STDIN_INPUT:
            {
                rc = get_data_from_stdin(subscribers, &len);
                if (rc) printf("Data entered. Records: %d\n", len);
                break;
            }

            case FILE_OUTPUT:
            {
                if (len == 0) {
                    printf("Table is empty!\n");
                    break;
                }
                printf("Enter filename: ");
                scanf("%50s", filename_out);
                rc = write_data_to_file(subscribers, len, filename_out);
                if (rc) printf("Data saved to file: %s\n", filename_out);
                break;
            }

            case STDIN_OUTPUT:
            {
                if (len == 0) {
                    printf("Table is empty!\n");
                    break;
                }
                rc = write_data_to_file(subscribers, len, "stdout");
                break;
            }
            
            case SEARCH_DATA:
            {
                if (len == 0) {
                    printf("Table is empty!\n");
                    break;
                }
                printf("Enter current date (day month year): ");
                if (scanf("%d %d %d", &day, &month, &year) != 3) {
                    printf("Date input error!\n");
                    break;
                }
                days_since_new_year = get_day_since_new_year(day, month, year);
                search_date(subscribers, days_since_new_year, len);
                break;
            }

            case SORT_TABLE_BUBBLE:
            {
                if (len == 0) {
                    printf("Table is empty!\n");
                    break;
                }
                bubble_sort(subscribers, len, sizeof(subscriber_t), compare_surname);
                printf("Table sorted by surname (bubble sort)\n");
                break;
            }

            case SORT_TABLE_QSORT:
            {
                if (len == 0) {
                    printf("Table is empty!\n");
                    break;
                }
                qsort(subscribers, len, sizeof(subscriber_t), compare_surname);
                printf("Table sorted by surname (qsort)\n");
                break;
            }

            case SORT_KEYS_BUBBLE:
            {
                if (len == 0) {
                    printf("Table is empty!\n");
                    break;
                }
		if (!keys_table_exist)
		{
		    keys_table_exist = 1;
                    create_key_table(subscribers, keys, len);
		}
                bubble_sort(keys, len, sizeof(key_table_t), compare_keys);
                print_via_keys(subscribers, keys, len);
                break;
            }

            case SORT_KEYS_QSORT:
            {
                if (len == 0) {
                    printf("Table is empty!\n");
                    break;
                }
		if (!keys_table_exist)
                {
                    keys_table_exist = 1;
                    create_key_table(subscribers, keys, len);
                }
                qsort(keys, len, sizeof(key_table_t), compare_keys);
                print_via_keys(subscribers, keys, len);
                break;
            }

            case COMPARE_SORTS:
            {
                measure_sorts(subscribers, len);
                break;
            }

            case EXIT:
            {
                printf("Exiting program.\n");
                break;
            }
	    
	    case PRINT_KEYS_TABLE:
	    {
		if (!keys_table_exist)
                {
                    keys_table_exist = 1;
                    create_key_table(subscribers, keys, len);
                }
		print_keys_table(keys, len);
		break;
            }

            default:
            {
                printf("Invalid mode. Try again.\n");
                break;
            }
        }
    }

    return rc;
}
