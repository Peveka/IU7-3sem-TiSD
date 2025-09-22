#ifndef DATE_H__
#define DATE_H__

int is_leap(int year);
int get_day_since_new_year(int day, int month, int year);
void search_date(subscriber_t *subs, int date, int len);

#endif
