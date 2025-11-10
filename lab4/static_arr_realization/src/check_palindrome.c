#include <stdio.h>
#include "errors.h"
#include <string.h>
#include <ctype.h>
#include "data.h"
#include "base_operations.h"
#include "menu.h"

static void scan_word(char *word)
{
    printf("Type word, which you want to check on palindrome (max_word_size = %d): ", MAX_ARR_LEN);
    clear_input();
    if (fgets(word, MAX_ARR_LEN, stdin) != NULL) 
    {
        size_t len = strlen(word);
        if (len > 0 && word[len-1] == '\n') 
            word[len-1] = '\0';
    }
}

void check_palindrome(void)
{
    int palin_flag = 1;
    char word[MAX_ARR_LEN+1];
    scan_word(word);
        
    stack_t temp_stack = {.size = 0, .max_size = 128};

    for (int i = 0; i < strlen(word); ++i)
    {
        add_elem_to_stack(&temp_stack, word[i]);
    }

    for (int i = 0; i < strlen(word); ++i)
    {
        stack_elem_t popped_elem;
        pop_elem_from_stack(&temp_stack, &popped_elem);
        if (toupper(word[i]) != toupper(popped_elem.letter))
        {
            palin_flag = 0;
            break;
        }
    }

    if (palin_flag == 1) 
        printf("Word: %s is palindrome\n", word);
    else 
        printf("Word: %s isn't palindrome\n", word);
}