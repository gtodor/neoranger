#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>

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
};

// a view of the filesystem node to be rendered
struct fs_node_render_info {
  fs_node_type type;
  std::string node_name;
};


class fs_explorer {
 private:
  fs_node* m_filesystem;

  // pointer that handles the movement in the filesystem
  fs_node* m_current_node;

  // initialize folder
  bool initialize_folder(std::string path);

 public:
  fs_explorer(std::string root_path);
  ~fs_explorer();
  std::vector<fs_node_render_info> current_dir_render_data();
  void move_down_in_current_dir();
  void move_up_in_current_dir();
  void descend();
  void ascend();
};

#endif // __FILESYSTEM_H__
