#include <stdio.h>
#include "struct.h"
#include "io.h"
#include "menu.h"


int main(void)
{
    int rc = 0;
    subscriber_t subscribers[MAX_FILE_LEN];
    rc = menu_handler(subscribers);
    return rc;
}
