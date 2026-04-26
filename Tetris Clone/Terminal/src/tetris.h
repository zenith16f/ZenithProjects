/*
 * Tetris Game
 * Author: zenith16f
 * Version: 1.5
 * Version Name: Evo to 2.0
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
  int rotation;
  int row;
  int col;
} TetrisPiece;

// Complete Game
typedef struct {
  int board[BOARD_HEIGHT][BOARD_WIDTH];

  TetrisPiece current;
  TetrisPiece next;
  TetrisPiece held;

  bool hasHeld;
  bool canHold;

  int score;
  int level;
  int linesCleared;

  int gravityTimer;
  bool gameOver;
  bool lastWasTetris;
} TetrisGame;

// Public Functions
int TetrisGameGetGhostRow(TetrisGame *game);
void TetrisGameInit(TetrisGame *game);
bool TetrisGameTick(TetrisGame *game, TetrisMove move);
void TetrisGameGetPieceCoords(TetrisGame *game, TetrisPiece *piece,
                              int coord[4][2]);
bool TetrisGameIsValid(TetrisGame *game, TetrisPiece *piece, int row, int col,
                       int rot);

#endif
