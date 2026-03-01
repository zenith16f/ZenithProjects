/*
 * Tetris Game
 * Author: zenith16f
 * Version: 1.0
 * Version Name: Functional
 * File: tetris.h
 */
#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdio.h>

// Board
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// Pieces Type
typedef enum {
  PIECE_I = 0,
  PIECE_O,
  PIECE_T,
  PIECE_S,
  PIECE_Z,
  PIECE_J,
  PIECE_L,
  PIECE_COUNT
} PieceType;

// Movement
typedef enum {
  MOVE_NONE,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_DOWN,
  MOVE_ROTATE,
  MOVE_DROP,
  MOVE_HOLD,
  MOVE_QUIT,
} TetrisMove;

// Pieces
typedef struct {
    PieceType type;
    int rotation; // rotacion actual 0 1 2 3
    int row;
    int col;
} TetrisPiece;

// Complete Game
typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH]; // Tablero 0 = vacio, 1-7 = color de pieza aterrizada

    TetrisPiece current; // Pieza cayendo actualmente
    TetrisPiece next;  // Siguiente pieza preview
    TetrisPiece held; // Pieza guardada

    bool hasHeld;  // Existe pieza guardada?
    bool canHold;  // Puede guardar una pieza?

    int score;
    int level;
    int linesCleared;

    int gravityTimer; // Contador, al llegar a 0 -> la pieza baja
    bool gameOver;
} TetrisGame;

// Public Functions
void TetrisGameInit(TetrisGame *game);
bool TetrisGameTick(TetrisGame *game, TetrisMove move);
void TetrisGameGetPieceCoords(TetrisGame *game, TetrisPiece *piece, int coord[4][2]);
bool TetrisGameIsValid(TetrisGame *game, TetrisPiece *piece, int row, int col, int rot);

#endif
