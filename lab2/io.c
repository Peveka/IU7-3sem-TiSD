#include <stdio.h>
#include <string.h>
#include "io.h"
#include "struct.h"
#include "date.h"

int get_single_subscriber_from_stdin(subscriber_t *sub)
{
    printf("Enter surname: ");
    if (scanf("%20s", sub->surname) != 1)
        return 0;

    printf("Enter name: ");
    if (scanf("%20s", sub->name) != 1) 
        return 0;

    printf("Enter phone: ");
    if (scanf("%11s", sub->phone) != 1)
        return 0;

    printf("Enter address: ");
    if (scanf("%25s", sub->address) != 1)
        return 0;

    printf("Enter status (1 - friend, 2 - colleague): ");
    int temp_status;
    if (scanf("%d", &temp_status) != 1)
        return 0;
    sub->status = (type_status_t)temp_status;

    if (sub->status == FRIEND)
    {
        printf("Enter date of birth (day month year): ");
        if (scanf("%d %d %d", &sub->subscriber.friend.birth_date.date,
                             &sub->subscriber.friend.birth_date.month,
                             &sub->subscriber.friend.birth_date.year) != 3)
        {
            return 0;
        }
	sub->subscriber.friend.birth_date.date_since_new_year =
        get_day_since_new_year(sub->subscriber.friend.birth_date.date,
                              sub->subscriber.friend.birth_date.month,
                              sub->subscriber.friend.birth_date.year);
    }
    else if (sub->status == COLLEAGUE)
    {
        printf("Enter job title: ");
        if (scanf("%25s", sub->subscriber.colleague.job) != 1) 
            return 0;
        printf("Enter organization: ");
        if (scanf("%25s", sub->subscriber.colleague.org) != 1)
            return 0;
    }
    else
    {
        printf("Invalid status.\n");
        return 0;
    }

    return 1;
}

int get_data_from_stdin(subscriber_t *data, int *len)
{
    char choice = 'y';
    *len = 0;

    while (*len < MAX_FILE_LEN && (choice == 'y' || choice == 'Y'))
    {
        if (get_single_subscriber_from_stdin(data + *len))
        {
            (*len)++;
            printf("Continue input? (y/n): ");
            scanf(" %c", &choice);
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int get_based_info(subscriber_t *sub, FILE* file)
{
    if (fscanf(file, "%20s", sub->surname) != 1 ||
        fscanf(file, "%20s", sub->name) != 1 ||
        fscanf(file, "%11s", sub->phone) != 1 ||
        fscanf(file, "%25s", sub->address) != 1) {
        return 0;
    }
    return 1;
}

int get_status_info(subscriber_t *sub, FILE* file)
{
    int temp_status;
    if (fscanf(file, "%d", &temp_status) != 1)
        return 0;
    sub->status = (type_status_t)temp_status;
    
    switch (sub->status){
        case FRIEND:
        {
            if (fscanf(file, "%d %d %d", &sub->subscriber.friend.birth_date.date,
                                       &sub->subscriber.friend.birth_date.month,
                                       &sub->subscriber.friend.birth_date.year) != 3) {
                return 0;
            }

	    sub->subscriber.friend.birth_date.date_since_new_year = 
            get_day_since_new_year(sub->subscriber.friend.birth_date.date,
                                  sub->subscriber.friend.birth_date.month,
                                  sub->subscriber.friend.birth_date.year);

            break;
        }
        case COLLEAGUE:
        {
            if (fscanf(file, "%25s %25s", sub->subscriber.colleague.job,
                                        sub->subscriber.colleague.org) != 2) {
                return 0;
            }
            break;
        }
        default:
        {
            printf("Unknown status!\n");
            return 0;
        }
    }
    
    return 1;
}

int get_data_from_file(subscriber_t *data, const char *filename, int *len)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error while opening file\n");
        return 0;
    }
    *len = 0;
    
    while (*len < MAX_FILE_LEN && get_based_info(data + *len, file) && get_status_info(data + *len, file))
    {
        (*len)++;
    }
    
    if (!feof(file))
    {
        printf("File reading error or too many records.\n");
        fclose(file);
        return 0;
    }
    
    fclose(file);
    return 1;
}

int write_data_to_file(subscriber_t *data, int count, const char *filename)
{
    FILE *file;
    if (strcmp(filename, "stdout") == 0)
    {
        file = stdout;
    }
    else
    {
        file = fopen(filename, "w");
    }

    if (file == NULL)
    {
        printf("File open error!\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %s %s %s ", data[i].surname, data[i].name, data[i].phone, data[i].address);
        fprintf(file, "%d ", data[i].status);

        if (data[i].status == FRIEND)
        {
            fprintf(file, "%d %d %d\n", data[i].subscriber.friend.birth_date.date,
                                       data[i].subscriber.friend.birth_date.month,
                                       data[i].subscriber.friend.birth_date.year);
        }
        else if (data[i].status == COLLEAGUE)
        {
            fprintf(file, "%s %s\n", data[i].subscriber.colleague.job,
                                     data[i].subscriber.colleague.org);
        }
    }
    if (file != stdout)
    {
        fclose(file);
    }
    return 1;
}
