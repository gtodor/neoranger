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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);

    // apply color pair 1
    wbkgd(stdscr, COLOR_PAIR(1));
    
    curs_set(0);

    bool isRunning = true;
    int ch;

    const int PATH_LENGTH = 40;

    char* path = new char[PATH_LENGTH];
    getcwd(path, PATH_LENGTH);
    std::string current_dir(path);
    delete[] path;

    fs_explorer explorer(current_dir);

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
	    case 'l': explorer.descend(); break;
            case ERR: continue;
        }

        explorer.draw_regions();

        refresh();
    }

    // restore terminal settings
    endwin();
    return 0;
}
