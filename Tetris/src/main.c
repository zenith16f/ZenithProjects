/*
 * Tetris Game
 * Author: zenith16f
 * Version: 1.5
 * Version Name: Evo to 2.0
 * File: main.c
 */

// Includes
#include "tetris.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Defines
#define BLOCK_WIDTH 2
#define BOARD_ORIGIN_ROW 1
#define BOARD_ORIGIN_COL 2
#define PANEL_COL (BOARD_ORIGIN_COL + BOARD_WIDTH * BLOCK_WIDTH + 2)
#define TICK_MS 50

// Colors
#define COLOR_PAIR_I 1
#define COLOR_PAIR_O 2
#define COLOR_PAIR_T 3
#define COLOR_PAIR_S 4
#define COLOR_PAIR_Z 5
#define COLOR_PAIR_J 6
#define COLOR_PAIR_L 7
#define COLOR_PAIR_BORDER 8
#define COLOR_PAIR_TEXT 9
#define COLOR_PAIR_EMPTY 10
#define COLOR_PAIR_GHOST 11 /* Ghost piece     — gris oscuro   */
#define COLOR_PAIR_DIM 12   /* Hold deshabilitado — texto grisaceo   */

// Functions
static void InitColors(void) {
  start_color();

  init_pair(COLOR_PAIR_I, COLOR_BLACK,
            COLOR_CYAN); // ID, color texto, color fondo
  init_pair(COLOR_PAIR_O, COLOR_BLACK, COLOR_YELLOW);
  init_pair(COLOR_PAIR_T, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(COLOR_PAIR_S, COLOR_BLACK, COLOR_GREEN);
  init_pair(COLOR_PAIR_Z, COLOR_BLACK, COLOR_RED);
  init_pair(COLOR_PAIR_J, COLOR_BLACK, COLOR_BLUE);
  init_pair(COLOR_PAIR_L, COLOR_BLACK, COLOR_WHITE);
  init_pair(COLOR_PAIR_BORDER, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_PAIR_TEXT, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_PAIR_EMPTY, COLOR_BLACK, COLOR_BLACK);

  init_pair(COLOR_PAIR_GHOST, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_PAIR_DIM, COLOR_WHITE, COLOR_BLACK);
}

static void InitCurses(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  timeout(TICK_MS);
  mousemask(0, NULL);
  curs_set(0);

  if (!has_colors()) {
    endwin();
    fprintf(stderr, "ERROR: La terminal no soporta colores.\n");
    exit(1);
  }
  InitColors();
}

static int PieceColorPair(int typePlusOne) {
  switch (typePlusOne) {
  case 1:
    return COLOR_PAIR_I;
  case 2:
    return COLOR_PAIR_O;
  case 3:
    return COLOR_PAIR_T;
  case 4:
    return COLOR_PAIR_S;
  case 5:
    return COLOR_PAIR_Z;
  case 6:
    return COLOR_PAIR_J;
  case 7:
    return COLOR_PAIR_L;
  default:
    return COLOR_PAIR_EMPTY;
  }
}

static void DrawBlock(int termRow, int termCol, int colorPair) {
  attron(COLOR_PAIR(colorPair));
  mvprintw(termRow, termCol, "  ");
  attroff(COLOR_PAIR(colorPair));
}

static void DrawBorder(void) {
  attron(COLOR_PAIR(COLOR_PAIR_BORDER));

  for (int c = 0; c <= BOARD_WIDTH * BLOCK_WIDTH + 1; c++) {
    mvprintw(BOARD_ORIGIN_ROW - 1, BOARD_ORIGIN_COL - 1 + c, "-");
    mvprintw(BOARD_ORIGIN_ROW + BOARD_HEIGHT, BOARD_ORIGIN_COL - 1 + c, "-");
  }

  for (int r = 0; r < BOARD_HEIGHT; r++) {
    mvprintw(BOARD_ORIGIN_ROW + r, BOARD_ORIGIN_COL - 1, "|");
    mvprintw(BOARD_ORIGIN_ROW + r,
             +BOARD_ORIGIN_COL + BOARD_WIDTH * BLOCK_WIDTH, "|");
  }
  attroff(COLOR_PAIR(COLOR_PAIR_BORDER));
}

static void DrawBoard(TetrisGame *game) {
  for (int r = 0; r < BOARD_HEIGHT; r++) {
    for (int c = 0; c < BOARD_WIDTH; c++) {
      int termRow = BOARD_ORIGIN_ROW + r;
      int termCol = BOARD_ORIGIN_COL + c * BLOCK_WIDTH;

      if (game->board[r][c] == 0) {
        DrawBlock(termRow, termCol, COLOR_PAIR_EMPTY);
      } else {
        DrawBlock(termRow, termCol, PieceColorPair(game->board[r][c]));
      }
    }
  }
}

static void DrawPiece(TetrisGame *game, TetrisPiece *piece, int originRow,
                      int originCol) {
  int coords[4][2];
  TetrisGameGetPieceCoords(game, piece, coords);

  int color = PieceColorPair(piece->type + 1);

  for (int i = 0; i < 4; i++) {
    int boardR = coords[i][0];
    int boardC = coords[i][1];

    if (boardR < 0 || boardR >= BOARD_HEIGHT)
      continue;
    if (boardC < 0 || boardC >= BOARD_WIDTH)
      continue;

    int termRow = originRow + boardR;
    int termCol = originCol + boardC * BLOCK_WIDTH;

    DrawBlock(termRow, termCol, color);
  }
}

static void DrawNextPiece(TetrisGame *game) {
  attron(COLOR_PAIR(COLOR_PAIR_TEXT));
  mvprintw(3, PANEL_COL, "Siguiente:");
  attroff(COLOR_PAIR(COLOR_PAIR_TEXT));

  TetrisPiece preview = game->next;
  preview.row = 0;
  preview.col = 0;

  int coords[4][2];
  TetrisGameGetPieceCoords(game, &preview, coords);

  int color = PieceColorPair(game->next.type + 1);

  for (int i = 0; i < 4; i++) {
    int r = coords[i][0];
    int c = coords[i][1];

    if (r < 0 || r >= 4 || c < 0 || c >= 4)
      continue;

    int termRow = 5 + r;
    int termCol = PANEL_COL + c * BLOCK_WIDTH;
    DrawBlock(termRow, termCol, color);
  }
}

static void DrawHoldPiece(TetrisGame *game) {
  attron(COLOR_PAIR(COLOR_PAIR_TEXT));
  mvprintw(10, PANEL_COL, "Guardada:");
  if (!game->canHold && game->hasHeld) {
    mvprintw(10, PANEL_COL + 10, "[--]");
  } else
    mvprintw(10, PANEL_COL + 10, "    ");
  attroff(COLOR_PAIR(COLOR_PAIR_TEXT));

  if (!game->hasHeld)
    return;

  TetrisPiece preview = game->held;
  preview.row = 0;
  preview.col = 0;

  int coords[4][2];
  TetrisGameGetPieceCoords(game, &preview, coords);

  int color =
      game->canHold ? PieceColorPair(game->held.type + 1) : COLOR_PAIR_DIM;

  for (int i = 0; i < 4; i++) {
    int r = coords[i][0];
    int c = coords[i][1];

    if (r < 0 || r >= 4 || c < 0 || c >= 4)
      continue;

    int termRow = 12 + r;
    int termCol = PANEL_COL + c * BLOCK_WIDTH;

    if (!game->canHold) {
      /* Atenuado: activar A_DIM antes de dibujar */
      attron(A_DIM);
      DrawBlock(termRow, termCol, color);
      attroff(A_DIM);
    } else {
      DrawBlock(termRow, termCol, color);
    }
  }
}

static void DrawPanel(TetrisGame *game) {
  attron(COLOR_PAIR(COLOR_PAIR_TEXT));

  /* Título */
  mvprintw(1, PANEL_COL, "T E T R I S");

  /* Separador entre piezas y stats */
  mvprintw(17, PANEL_COL, "------------");

  /* Score — %07d: siempre 7 dígitos con ceros a la izquierda */
  mvprintw(18, PANEL_COL, "Puntuacion:");
  mvprintw(19, PANEL_COL, "%07d", game->score);

  /* Nivel */
  mvprintw(21, PANEL_COL, "Nivel: %d", game->level);

  /* Líneas con progreso al siguiente nivel */
  int progress = game->linesCleared % 10; /* líneas en el nivel actual */
  mvprintw(23, PANEL_COL, "Lineas: %d", game->linesCleared);
  mvprintw(24, PANEL_COL, "[");
  for (int i = 0; i < 10; i++)
    mvprintw(24, PANEL_COL + 1 + i, i < progress ? "=" : "-");
  mvprintw(24, PANEL_COL + 11, "]");

  /* Separador */
  mvprintw(26, PANEL_COL, "------------");

  /* Controles */
  mvprintw(27, PANEL_COL, "Controles:");
  mvprintw(28, PANEL_COL, "<-/a ->/ d");
  mvprintw(29, PANEL_COL, "^/w  v/s");
  mvprintw(30, PANEL_COL, "SPC:drop c:hold");
  mvprintw(31, PANEL_COL, "q:salir");

  attroff(COLOR_PAIR(COLOR_PAIR_TEXT));

  DrawNextPiece(game);
  DrawHoldPiece(game);
}

static void GameOver(TetrisGame *game) {
  int rows, cols;
  getmaxyx(stdscr, rows, cols); /* Obtener tamaño actual del terminal */

  int center_row = rows / 2;
  int center_col = cols / 2 - 8;

  attron(COLOR_PAIR(COLOR_PAIR_BORDER) |
         A_BOLD); /* A_BOLD = texto en negrita */

  mvprintw(center_row - 1, center_col, "================");
  mvprintw(center_row, center_col, "  GAME  OVER    ");
  mvprintw(center_row + 1, center_col, "================");
  mvprintw(center_row + 3, center_col, "Score: %d", game->score);
  mvprintw(center_row + 4, center_col, "Presiona 'q'");

  attroff(COLOR_PAIR(COLOR_PAIR_BORDER) | A_BOLD);

  refresh();   /* Aplicar cambios a la pantalla */
  timeout(-1); /* Bloquear hasta que el usuario presione una tecla */

  int ch;
  while ((ch = getch()) != 'q' && ch != '\n' && ch != KEY_ENTER)
    ;
}

static TetrisMove ReadInput(void) {
  int ch = getch();

  switch (ch) {
  case KEY_LEFT:
  case 'a':
  case 'A':
    return MOVE_LEFT;
  case KEY_RIGHT:
  case 'd':
  case 'D':
    return MOVE_RIGHT;
  case KEY_DOWN:
  case 's':
  case 'S':
    return MOVE_DOWN;
  case KEY_UP:
  case 'w':
  case 'W':
    return MOVE_ROTATE;
  case ' ':
    return MOVE_DROP;
  case 'c':
  case 'C':
    return MOVE_HOLD;
  case 'q':
  case 'Q':
    return MOVE_QUIT;
  default:
    return MOVE_NONE;
  }
}

static void DrawFrame(TetrisGame *game) {
  clear();
  DrawBorder();
  DrawBoard(game);
  DrawPiece(game, &game->current, BOARD_ORIGIN_ROW, BOARD_ORIGIN_COL);
  DrawPanel(game);
  refresh();
}

// Main Function
int main(void) {
  TetrisGame game;

  InitCurses();

  TetrisGameInit(&game);

  // Game Loop
  bool running = true;

  while (running) {
    TetrisMove move = ReadInput();

    if (move == MOVE_QUIT)
      break;

    running = TetrisGameTick(&game, move);

    DrawFrame(&game);
  }

  if (game.gameOver) {
    DrawFrame(&game);
    GameOver(&game);
  }

  endwin();

  return 0;
}
