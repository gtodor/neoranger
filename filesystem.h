#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>

class region;

enum fs_node_type {
    DIRECTORY,
    REGULAR_FILE,
    SYMBOLIC_LINK,
    SOCKET,
    BLOCK_DEVICE,
    CHARACTER_DEVICE,
    FIFO,
    UNKNOWN
};

// filesystem node info
struct fs_node {
    fs_node* parent;
    std::string absolute_path;
    struct stat stats;
    bool content_initialized;
    std::vector<fs_node*> content;

    fs_node(fs_node* parent, std::string absolute_path);
    ~fs_node();
    fs_node_type type();
    void sort();
};


class fs_explorer {
    private:
        // pointer that handles the movement in the filesystem
        fs_node* m_current_node;
	int current_region;

        std::vector<region> regions;

        std::string current_absolute_path;

        // initialize folder
        void initialize_folder();

    public:
        fs_explorer(std::string root_path);
        ~fs_explorer();
        void move_down();
        void move_up();
        void descend();
        void ascend();
        void draw_regions();
        void toggle_borders();
};

#endif // __FILESYSTEM_H__
