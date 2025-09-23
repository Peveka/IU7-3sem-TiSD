#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "io.h"
#include "menu.h"
#include "sort.h"

int main(void) {
    int rc = 0;
    
    subscriber_t *subscribers = malloc(MAX_FILE_LEN * sizeof(subscriber_t));
    if (!subscribers) {
        printf("Memory allocation failed! Need %zu bytes\n", 
               MAX_FILE_LEN * sizeof(subscriber_t));
        return 1;
    }
    
    printf("Program started. Maximum records: %d\n", MAX_FILE_LEN);
    printf("Available memory: %zu bytes\n", MAX_FILE_LEN * sizeof(subscriber_t));
    
    rc = menu_handler(subscribers);
    
    free(subscribers);
    return rc;
}