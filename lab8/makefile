CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
TARGET = graph_program
SOURCES = main.c graph.c ford_bellman.c graphviz.c errors.c err_alloc.c menu_handlers.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) graph.dot graph.png

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run