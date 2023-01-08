#ifndef CMAKEPROGEN_FILEGEN_HPP
#define CMAKEPROGEN_FILEGEN_HPP

#include "settings/settings.hpp"
#include <string>

void create_cmake_files(const cmakeprogen_settings* const settings);
void create_cpp_files(const cmakeprogen_settings* const settings);
void create_hpp_files(const cmakeprogen_settings* const settings);

#endif
