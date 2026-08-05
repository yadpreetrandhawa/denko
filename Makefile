CC = clang
CFLAGS = -Wall -Wextra -Iinclude -pthread
SRC = src/main.c src/net.c src/http.c
TARGET = bin/denko

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf bin/

run: all
	./$(TARGET)

.PHONY: all clean run