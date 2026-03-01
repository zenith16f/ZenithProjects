# Makefile para Tetris
# Uso:
#   make        → compilar
#   make clean  → eliminar binarios
#   make run    → compilar y ejecutar

CC      = gcc
CFLAGS  = -Wall -Wextra -g        # -Wall: todos los warnings, -g: debug symbols
LIBS    = -lncurses                # Enlazar con la biblioteca ncurses
SRC     = src/tetris.c src/main.c  # Archivos fuente
TARGET  = tetris                   # Nombre del ejecutable

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
