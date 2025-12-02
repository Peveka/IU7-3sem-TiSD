CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
INCLUDE = -Iinc
SOURCES = src/main.c src/simulation_core.c src/array_queue.c src/linked_queue.c src/performance_analyzer.c src/user_interface.c src/simulation_runners.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = simulation_system

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lm

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)