CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = bst_program
OBJS = main.o menu.o bst.o tree_visualizer.o performance.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

main.o: main.c bst.h
	$(CC) $(CFLAGS) -c main.c

menu.o: menu.c bst.h
	$(CC) $(CFLAGS) -c menu.c

bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c bst.c

tree_visualizer.o: tree_visualizer.c bst.h
	$(CC) $(CFLAGS) -c tree_visualizer.c

performance.o: performance.c bst.h
	$(CC) $(CFLAGS) -c performance.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean