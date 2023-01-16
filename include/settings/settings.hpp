#ifndef CMAKEPROGEN_SETTINGS_HPP
#define CMAKEPROGEN_SETTINGS_HPP

#include <memory>
#include <string>


enum class language_type {
    unknown,
    c11,
    c17,
    cpp11,
    cpp14,
    cpp17,
    cpp20
};

struct cmakeprogen_settings {
    std::string m_project_name;
    std::string m_project_version;
    std::string m_project_description;
    language_type m_project_language;

    cmakeprogen_settings(std::string project_name,
                            std::string project_version,
                            std::string project_description,
                            language_type project_language = language_type::cpp20);

    cmakeprogen_settings(cmakeprogen_settings&& other);
    cmakeprogen_settings& operator=(cmakeprogen_settings&& other);
    cmakeprogen_settings(const cmakeprogen_settings& other) = delete;
    cmakeprogen_settings& operator=(const cmakeprogen_settings& other) = delete;

    friend std::ostream& operator<<(std::ostream& os,
                                    const cmakeprogen_settings& settings);

    std::string get_language_type_for_cmake_config() const;
    std::string get_language_version_for_cmake_config() const;
};

cmakeprogen_settings* parse_arguments(const int argc, char** argv);

#endif
