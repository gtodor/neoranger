#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>
#include <string>
#include <algorithm>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <ncurses.h>
#include "filesystem.h"
#include "region.h"

fs_node::fs_node(fs_node* parent,
        std::string absolute_path) : parent(parent),
    absolute_path(absolute_path),
    content_initialized(false),
    content()
{
    lstat(absolute_path.c_str(), &stats);
}

fs_node::~fs_node()
{
    // loop through this node content and delete all allocated fs_nodes
    for (unsigned int i = 0; i < content.size(); i++) {
        delete content[i];
    }
}

fs_node_type fs_node::type() {
    switch(stats.st_mode & S_IFMT) {
        case S_IFREG: return fs_node_type::REGULAR_FILE;
        case S_IFSOCK: return fs_node_type::SOCKET;
        case S_IFLNK: return fs_node_type::SYMBOLIC_LINK;
        case S_IFBLK: return fs_node_type::BLOCK_DEVICE;
        case S_IFDIR: return fs_node_type::DIRECTORY;
        case S_IFCHR: return fs_node_type::CHARACTER_DEVICE;
        case S_IFIFO: return fs_node_type::FIFO;
        default: return fs_node_type::UNKNOWN;
    }
}

bool compare_by_absolute_path(const fs_node* f1, const fs_node* f2) {
    return f1->absolute_path < f2->absolute_path; 
}

void fs_node::sort() {
    std::sort(content.begin(), content.end(), &compare_by_absolute_path);
}

void fs_explorer::initialize_folder()
{
    assert(m_current_node != NULL);

    std::string path = m_current_node->absolute_path;

    DIR* dir;
    struct dirent* dir_entry;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((dir_entry = readdir(dir)) != NULL) {
            // construct fs_node struct
            std::string entry_name(dir_entry->d_name);
            if (entry_name != "." && entry_name != "..") {
                std::string abs_path = m_current_node->absolute_path + "/" + entry_name;
                fs_node* node = new fs_node(m_current_node, abs_path);
                (m_current_node->content).push_back(node);
            }
        }
        closedir(dir);
    }
}


fs_explorer::fs_explorer(std::string root_path)
{
    fs_node* node = new fs_node(NULL, root_path);
    m_current_node = node;  
    initialize_folder();
    node->sort();
    current_absolute_path = node->content[0]->absolute_path;

    // TODO: regions should not overlap and only two visible 
    region r(*node, 2, 0, 30, LINES - 1);
    regions.push_back(r);
    current_region = 0;
}

fs_explorer::~fs_explorer()
{
    delete m_current_node;
}

void fs_explorer::move_down()
{
    // move down in the latest region
    if (regions.size() > 0) {
        current_absolute_path = regions[current_region].move_down();
    }
}

void fs_explorer::move_up()
{
    // move up in the latest region
    if (regions.size() > 0) {
        current_absolute_path = regions[current_region].move_up();
    }
}

void fs_explorer::descend() {
	fs_node* node = regions[current_region].selected_node();
	if (node->type() == fs_node_type::DIRECTORY) { 
    		m_current_node = node;  
    		initialize_folder();
    		node->sort();
    		current_absolute_path = node->content[0]->absolute_path;
	
		int posy = regions[current_region].posy();

    		// TODO: regions should not overlap and only two visible 
    		region r(*node, 2, posy + 31, 30, LINES - 1);
   		regions.push_back(r);
    		current_region++;
	}
}

void fs_explorer::ascend()
{

}

bool starts_with(const std::string& full_string, const std::string& sub_string) {
	return full_string.rfind(sub_string, 0) == 0;
}

std::string format_first_line(const std::string& path) {
	struct passwd *tmp = getpwuid(getuid());
	std::string homedir = tmp->pw_dir;
	std::string username = tmp->pw_name;
	char hostname[HOST_NAME_MAX];
	gethostname(hostname, HOST_NAME_MAX);

	if (starts_with(path, homedir)) {
		return username + "@" + hostname + ": ~" + path.substr(homedir.size());
	}
	return username + "@" + hostname + ": " + path;
}

void fs_explorer::draw_regions()
{
    erase();
    
    move(0, 0);
    // print the node absolute path
    // TODO: create a method that takes a string with escape codes and transforms it into a series of ncurses calls to display text with color and bold...
    addstr(format_first_line(current_absolute_path).c_str());

    for (unsigned int r = 0; r < regions.size(); r++) {
        regions[r].draw();
    }
}

void fs_explorer::toggle_borders()
{
    if (!regions.empty()) {
        regions[current_region].toggle_border();
    }
}
