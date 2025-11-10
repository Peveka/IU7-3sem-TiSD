CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -Iinc
OBJS := out/menu.c out/check_palindrome.c out/base_operations.c out/main.o
all : app.exe

app.exe : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

out/main.o : src/main.c inc/data.h inc/menu.h
	$(CC) $(CFLAGS) -c $^ -o $@

out/menu.o : src/check_palindrome.c inc/data.h inc/menu.h inc/menu.h
	$(CC) $(CFLAGS) -c $< -o $@

out/menu.o : src/base_operations.c inc/data.h inc/errors.h
	$(CC) $(CFLAGS) -c $< -o $@

out/menu.o : src/menu.c inc/menu.h inc/data.h inc/errors.h
	$(CC) $(CFLAGS) -c $< -o $@
clean :
	rm -f out/*.o *.exe

.PHONY : all clean unit