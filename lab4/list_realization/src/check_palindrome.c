#include <stdio.h>
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "base_operations.h"
#include "menu.h"
#include "check_palindrome.h"

static void scan_word(char *word)
{
    printf("Type word, which you want to check on palindrome (max_word_size = %d): ", MAX_WORD_LEN);
    clear_input();
    if (fgets(word, MAX_WORD_LEN, stdin) != NULL) 
    {
        size_t len = strlen(word);
        if (len > 0 && word[len-1] == '\n') 
            word[len-1] = '\0';
    }
}

void check_palindrome(void)
{
    int palin_flag = 1;
    char word[MAX_WORD_LEN+1];
    scan_word(word);
    
    stack_list_t temp_stack;
    init_stack(&temp_stack);

    for (int i = 0; i < strlen(word); ++i)
        add_elem_to_stack(&temp_stack, word[i]);

    for (int i = 0; i < strlen(word) && palin_flag; ++i)
    {
        list_node_t *popped_elem = pop_elem_from_stack(&temp_stack);
        if (toupper(word[i]) != toupper(popped_elem->data))
            palin_flag = 0;
        free(popped_elem);
    }

    if (palin_flag == 1) 
        printf("Word: %s is palindrome\n", word);
    else 
        printf("Word: %s isn't palindrome\n", word);

    free_stack(&temp_stack);
    
}