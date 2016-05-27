//
// Created by 刘优 on 16/5/26.
//

#include "common/utils.h"
#include <sys/stat.h>  // for mkdir



bool elon::file_exists(const std::string& path) {
  return (access(path.c_str(), F_OK) != -1);
}

void elon::mkdir_if_not_exists(const std::string& path) {
  if (!file_exists(path))
    mkdir(path.c_str(), 0755);
}

std::string elon::get_current_path() {
  std::string current_absolute_path;
  char current_absolute[256];
  if (NULL == getcwd(current_absolute, 256))
    return "";

  current_absolute_path = current_absolute;
  return (current_absolute_path + "/");
}