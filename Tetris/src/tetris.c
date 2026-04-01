/*
 * Tetris Game
 * Author: zenith16f
 * Version: 1.5
 * Version Name: Evo to 2.0
 * File: tetris.c
 */

// Includes
#include "tetris.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Defines
#define GRAVITY_TABLE_SIZE 15

#define SCORE_1_LINE 100
#define SCORE_2_LINE 300
#define SCORE_3_LINE 500
#define SCORE_4_LINE 800
#define SCORE_BACK_TO_BACK_BONUS 1200
#define SCORE_SOFT_DROP 1
#define SCORE_HARD_DROP 2

#define LINES_PER_LEVEL 10

// Arrays
// Gravity
static const int GRAVITY_TABLE[] = {
    /*  nv1  nv2  nv3  nv4  nv5  nv6  nv7  nv8  nv9 */
    48, 43, 38, 33, 28, 23, 18, 13, 8,
    /*  nv10 nv11 nv12 nv13 nv14 nv15 */
    6, 5, 5, 4, 3, 2};
// Tetrominos
static const int PIECES[PIECE_COUNT][4][4][2] =
    {
        [PIECE_I] =
            {
                {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
                {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
                {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
                {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
            },

        [PIECE_O] =
            {
                {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
                {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
                {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
                {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
            },

        [PIECE_T] =
            {
                {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
                {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
                {{0, 1}, {1, 0}, {1, 1}, {2, 1}},
            },

        [PIECE_S] =
            {
                {{0, 2}, {1, 2}, {1, 1}, {2, 1}},
                {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
                {{0, 2}, {1, 2}, {1, 1}, {2, 1}},
                {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
            },

        [PIECE_Z] =
            {
                {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                {{1, 0}, {1, 1}, {0, 1}, {0, 2}},
                {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                {{1, 0}, {1, 1}, {0, 1}, {0, 2}},
            },

        [PIECE_L] =
            {
                {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
                {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
            },

        [PIECE_J] =
            {
                {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                {{0, 1}, {1, 1}, {2, 0}, {2, 1}},
            },
};

// Functions
static int GetGravity(int level) {
  int index = level - 1;
  if (index < 0)
    index = 0;

  if (index >= GRAVITY_TABLE_SIZE)
    index = GRAVITY_TABLE_SIZE - 1;
  return GRAVITY_TABLE[index];
}

static TetrisPiece RandomPiece() {
  TetrisPiece piece;
  piece.type = rand() % PIECE_COUNT;
  piece.rotation = 0;
  piece.row = 0;
  piece.col = BOARD_WIDTH / 2 - 1;
  return piece;
}

void TetrisGameGetPieceCoords(TetrisGame *game __attribute__((unused)),
                              TetrisPiece *piece, int coord[4][2]) {
  const int (*offset)[2] = PIECES[piece->type][piece->rotation];
  for (int i = 0; i < 4; i++) {
    coord[i][0] = piece->row + offset[i][0];
    coord[i][1] = piece->col + offset[i][1];
  }
}

bool TetrisGameIsValid(TetrisGame *game, TetrisPiece *piece, int row, int col,
                       int rot) {
  const int (*offsets)[2] = PIECES[piece->type][rot];

  for (int i = 0; i < 4; i++) {
    int r = row + offsets[i][0];
    int c = col + offsets[i][1];

    if (r < 0)
      continue;

    if (r >= BOARD_HEIGHT)
      return false;

    if (c < 0 || c >= BOARD_WIDTH)
      return false;

    if (game->board[r][c] != 0)
      return false;
  }

  return true;
}

static void LandPiece(TetrisGame *game) {
  int coords[4][2];
  TetrisGameGetPieceCoords(game, &game->current, coords);
  for (int i = 0; i < 4; i++) {
    int r = coords[i][0];
    int c = coords[i][1];

    if (r >= 0 && r < BOARD_HEIGHT && c >= 0 && c < BOARD_WIDTH)
      game->board[r][c] = game->current.type + 1;
  }
}

static int CheckLines(TetrisGame *game) {
  int cleared = 0;

  for (int r = BOARD_HEIGHT - 1; r >= 0; r--) {
    bool full = true;
    for (int c = 0; c < BOARD_WIDTH; c++) {
      if (game->board[r][c] == 0) {
        full = false;
        break;
      }
    }

    if (full) {
      cleared++;

      for (int row = r; row > 0; row--) {
        for (int c = 0; c < BOARD_WIDTH; c++) {
          game->board[row][c] = game->board[row - 1][c];
        }
      }
      for (int c = 0; c < BOARD_WIDTH; c++) {
        game->board[0][c] = 0;
      }
      r++;
    }
  }
  return cleared;
}

static void AdjustScore(TetrisGame *game, int lines) {
  int points[] = {0, SCORE_1_LINE, SCORE_2_LINE, SCORE_3_LINE, SCORE_4_LINE};

  if (lines > 0 && lines <= 4) {
    int base = points[lines];

    if (lines == 4 && game->lastWasTetris) {
      base = SCORE_BACK_TO_BACK_BONUS;
    }

    game->score += base * game->level;
    game->lastWasTetris = (lines == 4);
  }

  game->linesCleared += lines;
  game->level = (game->linesCleared / LINES_PER_LEVEL) + 1;
}

static bool SpawnNext(TetrisGame *game) {
  game->current = game->next;
  game->next = RandomPiece();
  game->canHold = true;

  return TetrisGameIsValid(game, &game->current, game->current.row,
                           game->current.col, game->current.rotation);
}

static void HandleRotate(TetrisGame *game) {
  int newRotation = (game->current.rotation + 1) % 4;
  int kicks[] = {0, 1, -1, 2, -2};
  int numKicks = 5;

  for (int i = 0; i < numKicks; i++) {
    int newCol = game->current.col + kicks[i];
    if (TetrisGameIsValid(game, &game->current, game->current.row, newCol,
                          newRotation)) {
      game->current.rotation = newRotation;
      game->current.col = newCol;
      return;
    }
  }
}

static void HardDrop(TetrisGame *game) {
  int cellsDropped = 0;
  while (TetrisGameIsValid(game, &game->current, game->current.row + 1,
                           game->current.col, game->current.rotation)) {
    game->current.row++;
    cellsDropped++;
  }
  game->score += cellsDropped * SCORE_HARD_DROP;

  LandPiece(game);
  int lines = CheckLines(game);
  AdjustScore(game, lines);

  if (!SpawnNext(game)) {
    game->gameOver = true;
  }

  game->gravityTimer = GetGravity(game->level);
}

static void HandleHold(TetrisGame *game) {
  if (!game->canHold)
    return;

  if (!game->hasHeld) {
    game->held = game->current;
    game->held.row = 0;
    game->held.col = 0;
    game->held.rotation = 0;
    game->hasHeld = true;

    if (!SpawnNext(game))
      game->gameOver = true;
  } else {

    TetrisPiece temporal = game->held;
    game->held = game->current;
    game->held.row = 0;
    game->held.col = 0;
    game->held.rotation = 0;

    game->current = temporal;
    game->current.row = 0;
    game->current.col = BOARD_WIDTH / 2 - 1;
    game->current.rotation = 0;
  }

  game->canHold = false;
}

void TetrisGameInit(TetrisGame *game) {
  memset(game, 0, sizeof(TetrisGame));

  srand(time(NULL));

  game->level = 1;
  game->score = 0;
  game->gameOver = false;
  game->hasHeld = false;
  game->canHold = true;

  game->current = RandomPiece();
  game->next = RandomPiece();

  game->gravityTimer = GetGravity(game->level);
}

bool TetrisGameTick(TetrisGame *game, TetrisMove move) {
  if (game->gameOver)
    return false;

  // User Input
  switch (move) {
  case MOVE_LEFT:
    if (TetrisGameIsValid(game, &game->current, game->current.row,
                          game->current.col - 1, game->current.rotation)) {
      game->current.col--;
    };
    break;
  case MOVE_RIGHT:
    if (TetrisGameIsValid(game, &game->current, game->current.row,
                          game->current.col + 1, game->current.rotation)) {
      game->current.col++;
    };
    break;
  case MOVE_DOWN:
    if (TetrisGameIsValid(game, &game->current, game->current.row + 1,
                          game->current.col, game->current.rotation)) {
      game->current.row++;
      game->score += SCORE_SOFT_DROP;
    };
    break;
  case MOVE_ROTATE:
    HandleRotate(game);
    break;
  case MOVE_HOLD:
    HandleHold(game);
    break;
  case MOVE_DROP:
    HardDrop(game);
    return !game->gameOver;
    break;
  case MOVE_QUIT:
    game->gameOver = true;
    break;
  case MOVE_NONE:
  default:
    break;
  }

  // Gravity
  game->gravityTimer--;
  if (game->gravityTimer <= 0) {
    game->gravityTimer = GetGravity(game->level);

    if (TetrisGameIsValid(game, &game->current, game->current.row + 1,
                          game->current.col, game->current.rotation)) {
      game->current.row++;
    } else {
      LandPiece(game);

      int lines = CheckLines(game);
      AdjustScore(game, lines);

      // Spawn new piece
      if (!SpawnNext(game)) {
        game->gameOver = true;
        return false;
      }
    }
  }

  return true;
}

// Get Ghost Row
