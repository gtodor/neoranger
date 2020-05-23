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
};


class fs_explorer {
 private:
  fs_node* m_filesystem;

  // pointer that handles the movement in the filesystem
  fs_node* m_current_node;

  std::vector<region> regions;

  // initialize folder
  bool initialize_folder(std::string path);

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
