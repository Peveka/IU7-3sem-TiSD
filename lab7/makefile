CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

SOURCES = main.c menu.c menu_handlers.c tree_visualizer.c \
          bst.c avl.c hash_table.c hash_table_chain.c compare_efficiency.c

OBJECTS = $(SOURCES:.c=.o)

EXEC = prog

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC) *.dot *.png

png: 
	./generate_png.sh

rebuild: clean all

.PHONY: all clean png rebuild