CC = cc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = dotgame

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $< -o $(TARGET)

$(OBJ): $(SRC)
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)