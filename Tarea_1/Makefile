CC = gcc
CFLAGS = -Iinclude -std=c99 -Wall -Wextra -g

SRC = src/main.c src/cpu.c src/memory.c
OBJ = $(SRC:.c=.o)
EXEC = system_monitor

.PHONY: all clean run

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)