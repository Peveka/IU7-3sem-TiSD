#ifndef STRUCT_H__
#define STRUCT_H__

#define NAME_LEN 20
#define SURNAME_LEN 20
#define PHONE_LEN 11
#define ADDRESS_LEN 25
#define JOB_LEN 25
#define ORG_LEN 25
#define MAX_FILE_LEN 1000

typedef struct
{
    int date;
    int month;
    int year;
    int date_since_new_year;
} date_t;

typedef enum 
{
    FRIEND=1,
    COLLEGUE=2
} type_status_t;

typedef struct 
{
    date_t birth_date;
} friend_t;

typedef struct 
{
    char job[JOB_LEN+1];
    char org[ORG_LEN+1];
} collegue_t;

typedef union 
{
    collegue_t collegue;
    friend_t friend;
} subs_status_t;

typedef struct 
{
    char name[NAME_LEN+1];
    char surname[SURNAME_LEN+1];
    char phone[PHONE_LEN+1];
    char address[ADDRESS_LEN+1];
    type_status_t status;
    subs_status_t subscriber;
} subscriber_t;

typedef struct {
    int index;          // index in main table
    char key[50];       // row value
} key_table_t;


#endif
