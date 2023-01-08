#include "progen.hpp"
#include "filegen_i.hpp"
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
using fs::path;
using std::cout;
using std::ofstream;
using std::string;

inline static void create_directory(const string& dir_name) {
    bool dir_created = fs::create_directories(dir_name);
    if(!dir_created) {
        cout << "Unable to create directory: " << dir_name << "\n";
        exit(EXIT_FAILURE);
    }
}

static void create_directory_structure(const cmakeprogen_settings* const settings) {
    string project_folder_name = settings->m_project_name;

    create_directory(project_folder_name);

    // change path to project folder
    path current_dir = fs::current_path();
    current_dir.concat("/" + project_folder_name);
    fs::current_path(current_dir);

    // build diretories in project folder
    create_directory("src/app");
    create_directory("src/lib");
    create_directory("include");
    create_directory("config");
}

static void create_files(const cmakeprogen_settings* const settings) {
    create_cmake_files(settings);
    create_cpp_files(settings);
    create_hpp_files(settings);
}

static void start_generating_project(const cmakeprogen_settings* const settings) {
    create_directory_structure(settings);
    create_files(settings);
}

void generate_project(const cmakeprogen_settings* const settings) {
    cout << "Generating project\n";
    start_generating_project(settings);
}
