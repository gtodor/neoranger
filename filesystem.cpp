#include <dirent.h>
#include <string>
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
  stat(absolute_path.c_str(), &stats);
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


void fs_explorer::initialize_folder()
{
  assert(m_current_node != NULL && m_filesystem != NULL);

  std::string path = m_current_node->absolute_path;
  
  DIR* dir;
  struct dirent* dir_entry;
  if ((dir = opendir(path.c_str())) != NULL) {
    while ((dir_entry = readdir(dir)) != NULL) {
      // construct fs_node struct
      std::string entry_name(dir_entry->d_name);
      std::string abs_path = m_current_node->absolute_path + "/" + entry_name;
      fs_node* node = new fs_node(m_current_node, abs_path);
      (m_current_node->content).push_back(node);
    }
    closedir(dir);
  }
}


fs_explorer::fs_explorer(std::string root_path)
{
  fs_node* node = new fs_node(NULL, root_path);
  m_filesystem = node;
  m_current_node = node;

  // TODO: regions should not overlap and only two visible 
  region r(*node, 0, 0, 30, 23);
  regions.push_back(r);
	
  initialize_folder();
}

fs_explorer::~fs_explorer()
{
  delete m_filesystem;
}

void fs_explorer::move_down()
{
  // move down in the latest region
  if (regions.size() > 0) {
    regions.back().move_down();
  }
}

void fs_explorer::move_up()
{
  // move up in the latest region
  if (regions.size() > 0) {
    regions.back().move_up();
  }
}

void fs_explorer::descend()
{

}

void fs_explorer::ascend()
{

}

void fs_explorer::draw_regions()
{
  erase();
  if (!regions.empty()) {
    regions[0].draw();
  }
}

void fs_explorer::toggle_borders()
{
  if (!regions.empty()) {
    regions[0].toggle_border();
  }
}
