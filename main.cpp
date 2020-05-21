#include <ncurses.h>
#include <cassert>

void move_cursor(int cursor_x,
		 int cursor_y,
		 int screen_width,
		 int screen_height)
{
  assert(cursor_x >= 0 && cursor_x < screen_height);
  assert(cursor_y >= 0 && cursor_y < screen_width);
  move(cursor_x, cursor_y);
}

void increment_cursor(int& cursor, int max)
{
  if (cursor < max - 1) cursor++;
}

void decrement_cursor(int& cursor)
{
  if (cursor > 0) cursor--;
}

int main()
{
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
  int screen_width;
  int screen_height;
  int cursor_x = 0;
  int cursor_y = 0;
  
  
  while(isRunning) {
    // the screen size could have changed so update the values
    getmaxyx(stdscr, screen_height, screen_width);

    // listen to key events
    ch = getch();
    switch (ch) {
    case 'q': isRunning = false; break;
    case 'j': increment_cursor(cursor_x, screen_height); break;
    case 'k': decrement_cursor(cursor_x); break;
    case 'h': decrement_cursor(cursor_y); break;
    case 'l': increment_cursor(cursor_y, screen_width); break;
    }

    // update those events
    move_cursor(cursor_x, cursor_y, screen_width, screen_height);
    refresh();
  }

  // restore terminal settings
  endwin();
  return 0;
}
