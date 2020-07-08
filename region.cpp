#include <ncurses.h>
#include <string>
#include "region.h"


void rectangle(int y1, int x1, int y2, int x2) {
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

region::region(fs_node& node,
        int x,
        int y,
        int w,
        int h): m_node(node),
    m_index(0),
    has_border(false),
    top_left_x(x),
    top_left_y(y),
    width(w),
    height(h),
    draw_corner_x(x),
    draw_corner_y(y)
{

}


void region::draw()
{
    int cur_x = draw_corner_x;
    // before printing init cursor position at first row
    move(cur_x, draw_corner_y);

    // print the node absolute path
    addstr(m_node.absolute_path.c_str());
    cur_x += 2;
    move(cur_x, draw_corner_y);
    m_node.sort();

    // next print files on screen
    for (unsigned int i = 0; i < m_node.content.size(); i++) {
        std::string str = m_node.content[i]->absolute_path;
        std::size_t found = str.find_last_of("/");
        std::string filename = str.substr(found + 1);
        addstr(filename.c_str());
        cur_x++;
        move(cur_x, draw_corner_y);
    }

    // draw border
    if (has_border) {
        rectangle(top_left_y, top_left_x, top_left_y + height, top_left_y + width);
    }

    // move cursor back to last index
    move(m_index + draw_corner_x + 2, draw_corner_y);

    // init line highlight to current line
    chgat(width - 1, A_REVERSE, 1, NULL);
}

void region::toggle_border()
{
    has_border = !has_border;
    if (has_border) {
        draw_corner_x = top_left_x + 1;
        draw_corner_y = top_left_y + 1;
    } else {
        draw_corner_x = top_left_x;
        draw_corner_y = top_left_y;
    }
}

void region::move_up()
{  
    if (m_index > 0) {
        m_index--;
    }
}

void region::move_down()
{ 
    if ((unsigned int) m_index < m_node.content.size() - 1) {
        m_index++;
    }
}
