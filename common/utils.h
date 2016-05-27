//
// Created by 刘优 on 16/5/26.
//

#ifndef SNMPCOLLECTOR_UTILS_H
#define SNMPCOLLECTOR_UTILS_H

#include <string>

namespace elon {

bool file_exists(const std::string& path);

void mkdir_if_not_exists(const std::string& path);

std::string get_current_path();

/// simple implements for perfect-forwards make_unique
#if __cplusplus < 201402L
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts &&... params) {
  return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
#endif

}



#endif //SNMPCOLLECTOR_UTILS_H
