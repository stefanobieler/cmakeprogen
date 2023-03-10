#include "filegen_i.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstring>

using std::ofstream;
using std::string;
using std::stringstream;

static const char* tab = "    ";

static void create_cmake_project_file(const cmakeprogen_settings* const settings) {
    // Create a file
    ofstream file{"CMakeLists.txt"};

    // Write minimum requirement
    file << "cmake_minimum_required(VERSION 3.23)\n\n";

    string language_type = settings->get_language_type_for_cmake_config();
    string language_version = settings->get_language_version_for_cmake_config();

    // Write project information
    file << std::right << std::setfill(' ') 
        << "project(\n"
        << tab << settings->m_project_name << "\n"
        << tab << "VERSION " << settings->m_project_version << "\n"
        << tab << "DESCRIPTION \"" << settings->m_project_description << "\"" << "\n"
        << tab << "LANGUAGES " << language_type << ")\n\n";

    // Add default include directories and include directories
    file << "link_directories(${CMAKE_BINARY_DIR}/linux/lib64)\n"
        << "include_directories(include)\n\n";

    // Enable compile commands for LSP support
    file << "set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n\n";

    // Setup some variables and compile flags
    file << "set(CMAKE_" << language_type << "_STANDARD " << language_version << ")\n"
        << "set(CMAKE_" << language_type << "_FLAGS \"-Wall\")\n"
        << "set(CMAKE_" << language_type << "_STANDARD_REQUIRED ON)\n"
        << "set(CMAKE_" << language_type << "_EXTENSIONS OFF)\n\n";

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

    string file_name;
    string file_path;
    string file_name_build;
    if(settings->get_language_type_for_cmake_config() == "CXX") {
        file_name = "config.hpp.in";
        file_path = "config/config.hpp.in";
        file_name_build = "config.hpp";
    } else {
        file_name = "config.h.in";
        file_path = "config/config.h.in";
        file_name_build = "config.h";
    }

    ofstream file{"config/CMakeLists.txt"};

    file << "configure_file(\n"
        << tab << file_name << "\n"
        << tab << "\"${CMAKE_BINARY_DIR}/linux/include/config/" << file_name_build << "\" ESCAPE_QUOTES)\n";

    file.close();

    file.open(file_path);

    const size_t MAX_NAME_LENGTH = 64; 
    char uppercase_project_name[MAX_NAME_LENGTH] = {0};


    for (size_t i = 0; i < MAX_NAME_LENGTH && settings->m_project_name[i] != '\0'; i++) {
        uppercase_project_name[i] = toupper(settings->m_project_name[i]);
    }

    stringstream include_guard;
    if(settings->get_language_type_for_cmake_config() == "CXX") {
        include_guard << "#ifndef " << uppercase_project_name << "_CONFIG_HPP\n"
                    << "#define " << uppercase_project_name << "_CONFIG_HPP\n\n"
                    << "#include <cstdint>\n\n";
    } else {
        include_guard << "#ifndef " << uppercase_project_name << "_CONFIG_H\n"
                    << "#define " << uppercase_project_name << "_CONFIG_H\n\n"
                    << "#include <stdint.h>\n\n";
    }

    file << include_guard.str()
        << "static const char* project_name = \"@PROJECT_NAME@\";\n"
        << "static const char* project_version = \"@PROJECT_VERSION@\";\n\n"
        << "static const int32_t project_version_major = @PROJECT_VERSION_MAJOR@;\n"
        << "static const int32_t project_version_minor = @PROJECT_VERSION_MINOR@;\n"
        << "static const int32_t project_version_patch = @PROJECT_VERSION_PATCH@;\n\n"
        << "#endif\n";

    file.close();
}

static void create_cmake_app_file(const cmakeprogen_settings* const settings) {
    stringstream file_name;

    if(settings->get_language_type_for_cmake_config() == "CXX") {
        file_name << settings->m_project_name << ".cpp";
    } else {
        file_name << settings->m_project_name << ".c";
    }

    ofstream file{"src/app/CMakeLists.txt"};

    // setup name and source files.
    file << "set(APP_NAME " << settings->m_project_name << ")\n"
        << "set(APP_SRC " << file_name.str() << ")\n\n"
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

static void create_cmake_lib_file(const cmakeprogen_settings* const settings) {

    stringstream file_name;
    if(settings->get_language_type_for_cmake_config() == "CXX") {
        file_name << "version.cpp";
    } else {
        file_name << "version.c";
    }

    ofstream file{"src/lib/CMakeLists.txt"};

    // set name
    file << "set(LIB_NAME lib)\n";
    
    // get sources
    file << "set(LIB_SRC " << file_name.str() << ")\n\n";

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
    create_cmake_lib_file(settings);
}

static void create_cpp_app_file(const cmakeprogen_settings* const settings) {
    stringstream file_name;
    char version_header[]= "version.hpp";

    if(settings->get_language_type_for_cmake_config() == "CXX") {
        file_name << "src/app/" << settings->m_project_name << ".cpp";
    } else {
        file_name << "src/app/" << settings->m_project_name << ".c";
        version_header[9] = '\0';
        version_header[10] = '\0';
    }

    ofstream file{file_name.str()};

    //headers
    file << "#include \"" << version_header << "\"\n"
        << "#include <stdio.h>\n\n";

    //main 
    file << "int main(int argc, char** argv) {\n"
        << tab << "printf(\"%s v%s\\n\", get_project_name(), get_project_version());\n"
        << tab << "printf(\"Hello World!\\n\");"
        << tab << "return 0;\n"
        << "}\n";

    file.close();
}

static void create_cpp_version_file(const cmakeprogen_settings* const settings) {
    stringstream file_name;
    char version_header[] = "version.hpp";
    char config_header[] = "config.hpp";

    if(settings->get_language_type_for_cmake_config() == "CXX") {
        file_name << "src/lib/version.cpp";
    } else {
        file_name << "src/lib/version.c";
        version_header[9] = '\0';
        version_header[10] = '\0';
        config_header[8] = '\0';
        config_header[9] = '\0';
    }
    ofstream file(file_name.str());

    file << "#include \"" << version_header << "\"\n"
        << "#include \"config/" << config_header << "\"\n\n";

    file << "const char* get_project_name() {\n"
        << tab << "return project_name;\n"
        << "}\n\n";

    file << "const char* get_project_version() {\n"
        << tab << "return project_version;\n"
        << "}\n";

    file.close();
}

static void create_hpp_version_file(const cmakeprogen_settings* const settings) {
    stringstream file_name;
    char include_guard[] = "VERSION_HPP";

    if(settings->get_language_type_for_cmake_config() == "CXX") {
        file_name << "include/version.hpp";
    } else {
        file_name << "include/version.h";
        include_guard[9] = '\0';
        include_guard[10] = '\0';
    }

    ofstream file{file_name.str()};

    file << "#ifndef " << include_guard << "\n"
        << "#define " << include_guard << "\n\n";

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
    create_cpp_version_file(settings);
}
void create_hpp_files(const cmakeprogen_settings* const settings) {
    create_hpp_version_file(settings);
}
