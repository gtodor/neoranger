#include <ncurses.h>
#include <string>
#include "filesystem.h"

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
  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  // apply color pair 1
  wbkgd(stdscr, COLOR_PAIR(1));

  bool isRunning = true;
  int ch;

  const int PATH_LENGTH = 40;

  char* path = new char[PATH_LENGTH];
  getcwd(path, PATH_LENGTH);
  std::string home_dir(path);
  delete[] path;
  
  fs_explorer explorer(home_dir);

  // initial draw
  explorer.draw_regions();
  
  while(isRunning) {
    // listen to key events
    ch = getch();
    switch (ch) {
    case 'q': isRunning = false; break;
    case 'b': explorer.toggle_borders(); break;
    case 'j': explorer.move_down(); break;
    case 'k': explorer.move_up(); break;
    case 'h': break;
    case 'l': break;
    case ERR: continue;
    }

    explorer.draw_regions();
    
    refresh();
  }

  // restore terminal settings
  endwin();
  return 0;
}
