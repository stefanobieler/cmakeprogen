#include "filegen_i.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstring>

using std::ofstream;

static const char* tab = "    ";

static void create_cmake_project_file(const cmakeprogen_settings* const settings) {
    // Create a file
    ofstream file{"CMakeLists.txt"};

    // Write minimum requirement
    file << "cmake_minimum_required(VERSION 3.23)\n\n";

    // Write project information
    file << std::right << std::setfill(' ') 
        << "project(\n"
        << tab << settings->m_project_name << "\n"
        << tab << "VERSION " << settings->m_project_version << "\n"
        << tab << "DESCRIPTION \"" << settings->m_project_description << "\"" << "\n"
        << tab << "LANGUAGES CXX)" << "\n\n";

    // Add default include directories and include directories
    file << "link_directories(${CMAKE_BINARY_DIR}/linux/lib64)\n"
        << "include_directories(include)\n\n";

    // Enable compile commands for LSP support
    file << "set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n\n";

    // Setup some variables and compile flags
    file << "set(CMAKE_CXX_STANDARD 20)\n"
        << "set(CMAKE_CXX_FLAGS \"-Wall\")\n"
        << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
        << "set(CMAKE_CXX_EXTENSIONS OFF)\n\n";

    //output of binary files
    file << "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/bin)\n";
    file << "set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/lib64)\n";
    file << "set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux/lib64)\n\n";

    // Copy include dir to build dir
    file << "file(COPY include DESTINATION ${CMAKE_BINARY_DIR}/linux/include)\n\n";

    // Add config and src
    file << "add_subdirectory(config)\n"
        << "add_subdirectory(src)\n";

    file.close();
}

static void create_cmake_config_files(const cmakeprogen_settings* const settings) {
    ofstream file{"config/CMakeLists.txt"};

    file << "configure_file(\n"
        << tab << "config.hpp.in\n"
        << tab << "\"${CMAKE_BINARY_DIR}/linux/include/config/config.hpp\" ESCAPE_QUOTES)\n";

    file.close();

    file.open("config/config.hpp.in");

    const size_t MAX_NAME_LENGTH = 64; 
    char uppercase_project_name[MAX_NAME_LENGTH] = {0};

    for (size_t i = 0; i < MAX_NAME_LENGTH && settings->m_project_name[i] != '\0'; i++) {
        uppercase_project_name[i] = toupper(settings->m_project_name[i]);
    }

    file << "#ifndef " << uppercase_project_name << "_CONFIG_HPP\n"
        << "#define " << uppercase_project_name << "_CONFIG_HPP\n\n"
        << "#include <cstdint>\n\n"
        << "static const char* project_name = \"@PROJECT_NAME@\";\n"
        << "static const char* project_version = \"@PROJECT_VERSION@\";\n\n"
        << "static constexpr std::int32_t project_version_major{@PROJECT_VERSION_MAJOR@};\n"
        << "static constexpr std::int32_t project_version_minor{@PROJECT_VERSION_MINOR@};\n"
        << "static constexpr std::int32_t project_version_patch{@PROJECT_VERSION_PATCH@};\n\n"
        << "#endif\n";

    file.close();
}

static void create_cmake_app_file(const cmakeprogen_settings* const settings) {
    ofstream file{"src/app/CMakeLists.txt"};

    // setup name and source files.
    file << "set(APP_NAME " << settings->m_project_name << ")\n"
        << "set(APP_SRC " << settings->m_project_name << ".cpp)\n\n"
        << "add_executable(${APP_NAME} ${APP_SRC})\n\n";

    // target include directories
    file << "target_include_directories(\n"
        << tab << "${APP_NAME}\n"
        << tab << "PRIVATE\n"
        << tab << "./)\n\n";

    // target link libraries.
    file << "target_link_libraries(\n"
        << tab << "${APP_NAME}\n"
        << tab << "PRIVATE\n"
        << tab << "lib)\n\n";

    file.close();
}

static void create_cmake_lib_file() {
    ofstream file{"src/lib/CMakeLists.txt"};

    // set name
    file << "set(LIB_NAME lib)\n";
    
    // get sources
    file << "set(LIB_SRC version.cpp)\n\n";

    // create library
    file << "add_library(${LIB_NAME} STATIC ${LIB_SRC})\n\n";

    // add target include directories
    file << "target_include_directories(\n"
        << tab << "${LIB_NAME}\n"
        << tab << "PRIVATE\n"
        << tab << "${CMAKE_BINARY_DIR}/linux/include/\n"
        << tab << "./)\n";

    file.close();
}

static void create_cmake_src_files(const cmakeprogen_settings* const settings) {
    ofstream file{"src/CMakeLists.txt"};
    file << "add_subdirectory(lib)\n"
        << "add_subdirectory(app)\n";
    file.close();

    create_cmake_app_file(settings);
    create_cmake_lib_file();
}

static void create_cpp_app_file(const cmakeprogen_settings* const settings) {
    const size_t MAX_FILE_PATH_LENGTH = 1024;
    char file_path[MAX_FILE_PATH_LENGTH] = {0};
    snprintf(file_path, MAX_FILE_PATH_LENGTH, "src/app/%s.cpp", settings->m_project_name.c_str());

    ofstream file{file_path};

    //headers
    file << "#include \"version.hpp\"\n"
        << "#include <iostream>\n\n";

    file << "using std::cout;\n\n";

    //main 
    file << "int main(int argc, char** argv) {\n"
        << tab << "cout << get_project_name() << \" v\" << get_project_version() << \"\\n\";\n"
        << tab << "cout << \"Hello World!\" << \"\\n\";\n\n"
        << tab << "return 0;\n"
        << "}\n";

    file.close();
}

static void create_cpp_version_file() {
    ofstream file("src/lib/version.cpp");

    file << "#include \"version.hpp\"\n"
        << "#include \"config/config.hpp\"\n\n";

    file << "const char* get_project_name() {\n"
        << tab << "return project_name;\n"
        << "}\n\n";

    file << "const char* get_project_version() {\n"
        << tab << "return project_version;\n"
        << "}\n";

    file.close();
}

static void create_hpp_version_file() {
    ofstream file{"include/version.hpp"};

    file << "#ifndef VERSION_HPP\n"
        << "#define VERSION_HPP\n\n";

    file << "const char* get_project_name();\n"
        << "const char* get_project_version();\n\n";

    file << "#endif\n";
    file.close();
}

void create_cmake_files(const cmakeprogen_settings* const settings) {
    create_cmake_project_file(settings);
    create_cmake_src_files(settings);
    create_cmake_config_files(settings);
}

void create_cpp_files(const cmakeprogen_settings* const settings) {
    create_cpp_app_file(settings);
    create_cpp_version_file();
}
void create_hpp_files(const cmakeprogen_settings* const settings) {
    create_hpp_version_file();
}
