#include <ncurses.h>

int main() {
  // init ncurses
  initscr();

  // disable tty character buffering
  cbreak();

  // suppress auto echoing of typed characters
  noecho();

  // capture special keystrokes Backspace, Delete...
  keypad(stdscr, TRUE);

  // enable color support
  start_color();

  // define color pairs
  init_pair(1, COLOR_RED, COLOR_BLUE);
  init_pair(2, COLOR_BLACK, COLOR_RED);

  // apply color pair 1
  wbkgd(stdscr, COLOR_PAIR(1));

  bool isRunning = true;
  int ch;
  int cursor_x = 0;
  int cursor_y = 0;
  
  while(isRunning) {
    ch = getch();
    switch (ch) {
    case 'q': isRunning = false; break;
    case 'j': cursor_x++; break;
    case 'k': cursor_x--; break;
    case 'h': cursor_y--; break;
    case 'l': cursor_y++; break;
    }

    move(cursor_x, cursor_y);
    refresh();
  }

  // restore terminal settings
  endwin();
  return 0;
}
