#ifndef __REGION_H__
#define __REGION_H__

#include <string>
#include "filesystem.h"

class region {
    private:
        // useful to hold info about the data
        fs_node& m_node;
        int m_index;

        // useful for rendering
        bool has_border;
        int top_left_x;
        int top_left_y;
        int width;
        int height;
        int draw_corner_x;
        int draw_corner_y;
    public:
        region(fs_node& node, int x, int y, int w, int h);
        void draw();
        void toggle_border();
        std::string move_up();
        std::string move_down();
	fs_node* selected_node();
	int posy();
};

#endif // __REGION_H__
