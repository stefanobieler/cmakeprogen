#include "settings/settings.hpp"
#include "utils/conversion.hpp"
#include "utils/version.hpp"
#include <getopt.h>
#include <iostream>
#include <iomanip>

using std::cout;
using std::ostream;
using std::string;

struct cmakeprogen_options {
    static const std::string version;
    static const std::string help;
};

// initializing constants in header
const string cmakeprogen_options::version = "version";
const string cmakeprogen_options::help = "help";

// static variables and functions
static cmakeprogen_settings sg_settings{"hello_world", "0.1.0",
                                        "Hello world application"};

// full worded options.
static option long_options[] = {
    option{cmakeprogen_options::version.c_str(), no_argument, nullptr, 0},
    option{cmakeprogen_options::help.c_str(), no_argument, nullptr, 0}

};

// print the version of the software.
// TODO: find a better place for this function.
static void print_version() {
    cout << get_project_name() << " v" << get_project_version() << "\n";
}

static void print_help() {
    std::ios_base::fmtflags orig_format_flags(cout.flags());
    print_version();
    int width = 15;
    cout << "\n";
    cout << std::left << std::setfill(' ')
        << std::setw(width) << " -n" << "Name of the project\n"
        << std::setw(width) << " -v" << "Version of the project\n"
        << std::setw(width) << " -d" << "Project description\n\n"
        << std::setw(width) << " -l" << "Language C/C++ (C++ 20 by default)"
        << std::setw(width) << " --version" << "Print out the version of this software\n"
        << std::setw(width) << " --help" << "Prints out the help menu\n";

    cout.flags(orig_format_flags);
}

// cmakeprogen_settings function definitions
cmakeprogen_settings::cmakeprogen_settings(string project_name,
                                           string project_version,
                                           string project_description,
                                           language_type project_language)
    : m_project_name(std::move(project_name)),
      m_project_version(std::move(project_version)),
      m_project_description(std::move(project_description)),
      m_project_language(project_language){}

cmakeprogen_settings::cmakeprogen_settings(cmakeprogen_settings&& other)
    : m_project_name(std::move(other.m_project_name)),
      m_project_version(std::move(other.m_project_version)),
      m_project_description(std::move(other.m_project_description)),
      m_project_language(other.m_project_language){}

cmakeprogen_settings&
cmakeprogen_settings::operator=(cmakeprogen_settings&& other) {
    m_project_name = std::move(other.m_project_name);
    m_project_version = std::move(other.m_project_version);
    m_project_description = std::move(other.m_project_description);
    m_project_language = other.m_project_language;
    return *this;
}

string cmakeprogen_settings::get_language_type_for_cmake_config() const {
    switch(m_project_language) {
        case language_type::c11: { 
            return string{"C"};
        }
        case language_type::c17: {
            return string{"C"};
        }
        case language_type::cpp11: {
            return string{"CXX"};
        }
        case language_type::cpp14: {
            return string{"CXX"};
        }
        case language_type::cpp17: {
            return string{"CXX"};
        }
        case language_type::cpp20: {
            return string{"CXX"};
        }
        default: {
            return string{"CXX"};
        }
    }
}

string cmakeprogen_settings::get_language_version_for_cmake_config() const {
    switch(m_project_language) {
        case language_type::c11: {
            return string{"11"};
        }
        case language_type::c17: {
            return string{"17"};
        }
        case language_type::cpp11: {
            return string{"11"};
        }
        case language_type::cpp14: {
            return string{"14"};
        }
        case language_type::cpp17: {
            return string{"17"};
        }
        case language_type::cpp20: {
            return string{"20"};
        }
        default: {
            return string{"20"};
        }
    }
}

static ostream& operator<<(ostream& os, const language_type& project_language) {
    switch(project_language)
    {
        case language_type::c11: {
            os << "C11";
            return os;
        }
        case language_type::c17: {
            os << "C17";
            return os;
        }
        case language_type::cpp11: {
            os << "C++11";
            return os;
        }
        case language_type::cpp17: {
            os << "C++17";
            return os;
        }
        case language_type::cpp20: {
            os << "C++20";
            return os;
        }
        default: {
            os << "Uknown";
            return os;
        }
    }
}

ostream& operator<<(ostream& os, const cmakeprogen_settings& settings) {
    os << settings.m_project_name << "\n"
       << settings.m_project_version << "\n"
       << settings.m_project_description << "\n"
       << settings.m_project_language << "\n";
    return os;
}

// function definitions
cmakeprogen_settings* parse_arguments(const int argc, char** argv) {

    int option_index{};
    int option{};

    while ((option = getopt_long(argc, argv, "n:v:d:l:", long_options,
                                 &option_index)) != -1) {
        switch (option) {
            case 0: {
                const string string_option =
                    string(long_options[option_index].name);

                if (string_option == cmakeprogen_options::version) {
                    print_version();
                    exit(EXIT_SUCCESS);
                } else if (string_option == cmakeprogen_options::help) {
                    print_help();
                    exit(EXIT_SUCCESS);
                }
                break;
            }
            case 'n': {
                sg_settings.m_project_name = optarg;
                break;
            }
            case 'v': {
                sg_settings.m_project_version = optarg;
                break;
            }
            case 'd': {
                sg_settings.m_project_description = optarg;
                break;
            }
            case 'l': {
                language_type lang_type = string_to_language_type(optarg);
                if(lang_type == language_type::unknown) {
                    cout << "Unkown language type " << optarg << "\n";
                    exit(EXIT_FAILURE);
                }
                sg_settings.m_project_language = lang_type;
                break;
            }
            case '?': {
                string option_hex = int_to_hex(option);
                cout << "Unknown option " << option_hex << "\n";
                exit(EXIT_FAILURE);
                break;
            }
            case ':': {
                string option_hex = int_to_hex(option);
                cout << "Missing option for " << option_hex << "\n";
                exit(EXIT_FAILURE);
                break;
            }
            default: {
                exit(EXIT_FAILURE);
            }
        }
    }

    return &sg_settings;
}
