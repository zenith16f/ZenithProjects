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
#define TICK_MS 50 // 50 Milisegundos

// Colors
#define COLOR_PAIR_I 1 /* Pieza I — cyan    */
#define COLOR_PAIR_O 2 /* Pieza O — amarillo */
#define COLOR_PAIR_T 3 /* Pieza T — magenta  */
#define COLOR_PAIR_S 4 /* Pieza S — verde    */
#define COLOR_PAIR_Z 5 /* Pieza Z — rojo     */
#define COLOR_PAIR_J 6 /* Pieza J — azul     */
#define COLOR_PAIR_L 7 /* Pieza L — naranja (blanco en terminales básicos) */
#define COLOR_PAIR_BORDER 8 /* Bordes del tablero — blanco sobre negro */
#define COLOR_PAIR_TEXT 9   /* Texto del panel    — blanco sobre negro */
#define COLOR_PAIR_EMPTY 10 /* Celda vacía        — negro sobre negro  */

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
}

static void InitCurses(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // Habilitar teclas especiales (flechas)
  timeout(TICK_MS);           // Control de velocidad
  curs_set(0);

  if (!has_colors()) {
    endwin();
    fprintf(stderr, "ERROR: El terminal no soporta colores.\n");
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
  attron(COLOR_PAIR(colorPair)); // Activar color
  mvprintw(termRow, termCol, "  ");
  attroff(COLOR_PAIR(colorPair)); // Desactivar color
}

static void DrawBorder(void) {
  attron(COLOR_PAIR(COLOR_PAIR_BORDER));

  for (int c = 0; c <= BOARD_WIDTH * BLOCK_WIDTH + 1; c++) {
    mvprintw(BOARD_ORIGIN_ROW - 1, BOARD_ORIGIN_COL - 1 + c, "-");
    mvprintw(BOARD_ORIGIN_ROW + BOARD_HEIGHT, BOARD_ORIGIN_COL - 1 + c, "-");
  }

  for (int r = 0; r < BOARD_HEIGHT; r++) {
    mvprintw(BOARD_ORIGIN_ROW + r, BOARD_ORIGIN_COL - 1, "|");
    mvprintw(BOARD_ORIGIN_ROW + r, + BOARD_ORIGIN_COL + BOARD_WIDTH * BLOCK_WIDTH,
             "|");
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

    if (r < 0 || r >= BOARD_HEIGHT || c < 0 || c >= BOARD_WIDTH)
      continue;

    int termRow = 5 + r;
    int termCol = PANEL_COL + c * BLOCK_WIDTH;
    DrawBlock(termRow, termCol, color);
  }
}

static void DrawPanel(TetrisGame *game) {
  attron(COLOR_PAIR(COLOR_PAIR_TEXT));

  /* Título */
  mvprintw(1, PANEL_COL, "T E T R I S");

  /* Score */
  mvprintw(10, PANEL_COL, "Puntuacion:");
  mvprintw(11, PANEL_COL, "%d", game->score);

  /* Nivel */
  mvprintw(13, PANEL_COL, "Nivel:");
  mvprintw(14, PANEL_COL, "%d", game->level);

  /* Líneas */
  mvprintw(16, PANEL_COL, "Lineas:");
  mvprintw(17, PANEL_COL, "%d", game->linesCleared);

  /* Controles */
  mvprintw(20, PANEL_COL, "Controles:");
  mvprintw(21, PANEL_COL, "<- -> : mover");
  mvprintw(22, PANEL_COL, "^    : rotar");
  mvprintw(23, PANEL_COL, "v    : bajar");
  mvprintw(24, PANEL_COL, "SPC  : drop");
  mvprintw(25, PANEL_COL, "c    : hold");
  mvprintw(26, PANEL_COL, "q    : salir");

  attroff(COLOR_PAIR(COLOR_PAIR_TEXT));

  DrawNextPiece(game);
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
    return MOVE_LEFT;
  case KEY_RIGHT:
    return MOVE_RIGHT;
  case KEY_DOWN:
    return MOVE_DOWN;
  case KEY_UP:
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
}}

static void DrawFrame(TetrisGame *game){
    clear();
    DrawBorder();
    DrawBoard(game);
    DrawPiece(game, &game->current, BOARD_ORIGIN_ROW, BOARD_ORIGIN_COL);
    DrawPanel(game);
    refresh();
}

// Main Function
int main(void){
    TetrisGame game;

    InitCurses();

    TetrisGameInit(&game);

    // Game Loop
    bool running = true;

    while (running) {
        TetrisMove move = ReadInput();

        if(move==MOVE_QUIT)break;

        running= TetrisGameTick(&game, move);

        DrawFrame(&game);
    }

    if(game.gameOver){
        DrawFrame(&game);
        GameOver(&game);
    }

    endwin();

    return 0;
}
