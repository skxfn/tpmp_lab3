CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude
LDFLAGS=-lsqlite3
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
TARGET=project5_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) data/project5_people.db data/exported_photo.bin
