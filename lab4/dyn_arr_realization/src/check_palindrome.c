#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "errors.h"
#include "data.h"
#include "base_operations.h"
#include "menu.h"

static void scan_word(char *word, int max_len)
{
    printf("Type word to check for palindrome (max %d chars): ", max_len - 1);
    clear_input();
    if (fgets(word, max_len, stdin) != NULL) 
    {
        size_t len = strlen(word);
        if (len > 0 && word[len-1] == '\n') 
            word[len-1] = '\0';
    }
}

void check_palindrome(void)
{
    char word[256];
    scan_word(word, sizeof(word));
    int word_len = strlen(word);
    
    if (word_len == 0) {
        printf("Empty word!\n");
        return;
    }

    stack_t temp_stack;
    if (stack_init(&temp_stack) != OK) {
        printf("Memory error!\n");
        return;
    }
    
    for (int i = 0; i < word_len; ++i)
    {
        if (add_elem_to_stack(&temp_stack, word[i]) != OK) {
            printf("Stack error!\n");
            stack_free(&temp_stack);
            return;
        }
    }

    int is_palindrome = 1;
    for (int i = 0; i < word_len; ++i)
    {
        stack_elem_t popped_elem;
        if (pop_elem_from_stack(&temp_stack, &popped_elem) != OK) {
            printf("Stack error!\n");
            break;
        }
        
        if (tolower(word[i]) != tolower(popped_elem.letter)) 
        {
            is_palindrome = 0;
            break;
        }
    }

    if (is_palindrome) 
        printf("Word: '%s' is palindrome\n", word);
    else 
        printf("Word: '%s' is not palindrome\n", word);

    stack_free(&temp_stack);
}