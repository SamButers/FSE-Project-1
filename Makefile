PROJECT_NAME=project1

C_SRC=$(wildcard src/*.c)

H_SRC=$(wildcard inc/*.h)

OBJ=$(subst .c,.o,$(subst src,obj,$(C_SRC)))

CC=gcc

CC_FLAGS = -Wall -c -I inc

all: $(PROJECT_NAME)

clean:
	rm -rf obj/*.o *.bin

$(PROJECT_NAME): $(OBJ)
	$(CC) -o $@.bin $^

obj/%.o: src/%.c inc/%.h
	$(CC) -o $@ $< $(CC_FLAGS)

obj/main.o: src/main.c $(H_SRC)
	$(CC) -o $@ $< $(CC_FLAGS)

