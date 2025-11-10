#include <stdio.h>
#include "data.h"
#include "errors.h"
#include "menu.h"
#include "base_operations.h"
#include "check_palindrome.h"

static void print_menu(void)
{
    printf("----------MENU----------\n");
    printf("1. Add element to stack\n");
    printf("2. Pop element to stack\n");
    printf("3. Peek stack elem\n");
    printf("4. Check stack status\n");
    printf("5. Check if letter is palindrome\n");
    printf("6. Clear stack\n");
    printf("7. Exit from prog\n");
    printf("------------------------\n");
}

void clear_input(void)
{
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n');
}

static char scan_elem(void)
{
    char char_elem;
    printf("Adding elem to stack. Please, type char to add: ");
    clear_input();
    while (scanf("%c", &char_elem) != 1)
    {
        printf("Error scanning element. Please type it again (1 char): ");
        clear_input();
    }

    return char_elem;
}

static user_mode_t scan_answer(void)
{
    print_menu();
    user_mode_t choosen_mode = no_mode;
    int input;
    
    while (1)
    {
        printf("Choose mode: ");
        if (scanf("%d", &input) == 1)
        {
            choosen_mode = (user_mode_t)input;
            if (choosen_mode >= add_mode && choosen_mode <= exit_mode)
            {
                return choosen_mode;
            }
        }
        else
            clear_input(); 

        printf("Invalid input! Please enter number from %d to %d\n", add_mode, exit_mode);
    }
}

void menu_handler(stack_t *stack)
{
    stack_elem_t pop_elem;
    //error_t rc = OK;
    user_mode_t mode = scan_answer();
    while (1)
    {
        switch(mode)
        {
            case no_mode:
                break;
            case add_mode:
            {
                char data = scan_elem();
                add_elem_to_stack(stack, data);
                break;
            }
            case pop_mode:
            {   
                if (stack->size != 0)
                {
                    printf("Pop elem successfully!\n");
                    printf("Popped up elem: ");
                    pop_elem_from_stack(stack, &pop_elem);
                    print_elem(pop_elem);
                }
                
                else
                    printf("Stack is empty!\n");
                break;
            }
            case peek_mode:
            {
                printf("Top value of stack: ");
                if (stack->size != 0)
                    print_elem((stack->data)[0]);
                else 
                    printf("not exist, because stack is clean\n");
                break;
            }
            case stack_status_mode:
                stack_status(stack);
                break;
            case palindrome_mode:
                check_palindrome();
                break;
            case clear_mode:
            {
                if (stack->size == 0)
                    printf("Stack is already clean\n");
                else
                {   
                    clear_all_stack(stack);
                    printf("Stack cleared succesfully\n");
                }
                break;
            }
            case exit_mode:
                return;
        } 

        mode = scan_answer();
    }
}