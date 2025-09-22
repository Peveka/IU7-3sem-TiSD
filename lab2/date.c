#include <stdio.h>
#include "struct.h"

int is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_day_since_new_year(int day, int month, int year) {
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dayCount = 0;

    // if the year is a leap year, February has 29 days
    if (is_leap(year)) {
        daysInMonth[2] = 29;
    }

    for (int i = 1; i < month; i++)
    {
        dayCount += daysInMonth[i];
    }

    dayCount += day;
    return dayCount;
}

void search_date(subscriber_t *subs, int current_date, int len)
{
    int search_flag = 0;
    int days_in_year = 365;
    
    // Можно добавить проверку на високосный год для более точного расчета
    // int current_year = ...; // нужно передавать год или вычислять его
    
    for (int i = 0; i < len; ++i) 
    {
        if (subs[i].status == FRIEND)
        {
            int birth_day = subs[i].subscriber.friend.birth_date.date_since_new_year;
            int diff = birth_day - current_date;
            
            if (diff < 0) {
                diff = birth_day + (days_in_year - current_date);
            }
            
            if (diff >= 0 && diff <= 7)
            {
                printf("%s %s - %d.%d.%d (in %d days)\n", 
                       subs[i].name, 
                       subs[i].surname,
                       subs[i].subscriber.friend.birth_date.date,
                       subs[i].subscriber.friend.birth_date.month,
                       subs[i].subscriber.friend.birth_date.year,
                       diff);
                search_flag++;
            }
        }
    }

    if (!search_flag)
        printf("No birthdays found in the next 7 days\n");
}
