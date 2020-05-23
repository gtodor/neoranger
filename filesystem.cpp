#include <iostream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <cerrno>
#include <cassert>
#include "filesystem.h"


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

bool fs_explorer::initialize_folder(std::string path)
{
  assert((m_current_node != NULL && m_filesystem != NULL) ||
	 (m_current_node == NULL && m_filesystem == NULL));
  
  DIR* dir;
  struct dirent* dir_entry;
  if ((dir = opendir(path.c_str())) != NULL) {
    while ((dir_entry = readdir(dir)) != NULL) {
      // construct fs_node struct
      std::string entry_name(dir_entry->d_name);
      if (m_filesystem == NULL) {
	fs_node* node = new fs_node(NULL, entry_name);
	m_filesystem = node;
	m_current_node = node;
      } else {
	std::string abs_path = m_current_node->absolute_path + "/" + entry_name;
	fs_node* node = new fs_node(m_current_node, abs_path);
	(m_current_node->content).push_back(node);
      }
	
    }
    closedir(dir);
    return true;
  } else {
    std::cerr<<"Error initializing: "<<strerror(errno)<<std::endl;
    return false;
  }
}


fs_explorer::fs_explorer(std::string root_path): m_filesystem(NULL),
						 m_current_node(NULL)
{
  initialize_folder(root_path);
}

fs_explorer::~fs_explorer()
{
  // TODO: maybe it needs to delete all resources recurrsivly
  delete m_filesystem;
}


std::vector<fs_node_render_info> fs_explorer::current_dir_render_data()
{
  std::vector<fs_node_render_info> res;
  for (unsigned int i = 0; i < (m_current_node->content).size(); i++) {
    fs_node_render_info node_render_info;
    node_render_info.type = m_current_node->content[i]->type();
    node_render_info.node_name = m_current_node->content[i]->absolute_path;
    res.push_back(node_render_info);
  }

  return res;
}

void fs_explorer::move_down_in_current_dir()
{

}

void fs_explorer::move_up_in_current_dir()
{

}

void fs_explorer::descend()
{

}

void fs_explorer::ascend()
{

}
