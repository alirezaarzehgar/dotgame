CC = cc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $< -o $(TARGET)

$(OBJ): $(SRC)
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)