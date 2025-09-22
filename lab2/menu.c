#include "menu.h"
#include "struct.h"
#include "io.h"

#define MAX_FILENAME_LEN 50

int menu_handler(subscriber_t *subscribers)
{
    int rc = 1;
    int len = 0;
    char filename_in[MAX_FILENAME_LEN];
    char filename_out[MAX_FILENAME_LEN];
    int mode = NO_MODE;

    while (mode != EXIT && rc)
    {
        printf("Select a mode of operation:\n");
        printf("1. Input from file\n");
        printf("2. Input from console\n");
        printf("3. Output to file\n");
        printf("4. Output to console\n");
        printf("9. Exit\n");
        printf("---------------------\n");
        printf("Enter a number: ");
        scanf("%d", &mode);

        switch(mode)
        {
            case FILE_INPUT:
            {
                printf("Enter source filename: ");
                scanf("%50s", filename_in);
                rc = get_data_from_file(subscribers, filename_in, &len);
                break;
            }

            case STDIN_INPUT:
            {
                rc = get_data_from_stdin(subscribers, &len);
                break;
            }

            case FILE_OUTPUT:
            {
                printf("Enter destination filename: ");
                scanf("%50s", filename_out);
                rc = write_data_to_file(subscribers, len, filename_out);
                break;
            }

            case STDIN_OUTPUT:
            {
                rc = write_data_to_file(subscribers, len, "stdout");
                break;
            }
            
            case EXIT:
            {
                // Exit
                break;
            }

            default:
            {
                printf("Invalid mode. Please try again.\n");
                break;
            }
        }
    }

    return rc;
}