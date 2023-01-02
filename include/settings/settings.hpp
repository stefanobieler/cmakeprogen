#ifndef CMAKEPROGEN_SETTINGS_HPP
#define CMAKEPROGEN_SETTINGS_HPP

#include <memory>
#include <string>


struct cmakeprogen_settings {
    std::string m_project_name;
    std::string m_project_version;
    std::string m_project_description;

    cmakeprogen_settings(std::string project_name, std::string project_version,
                         std::string project_description);

    cmakeprogen_settings(cmakeprogen_settings&& other);
    cmakeprogen_settings& operator=(cmakeprogen_settings&& other);
    cmakeprogen_settings(const cmakeprogen_settings& other) = delete;
    cmakeprogen_settings& operator=(const cmakeprogen_settings& other) = delete;

    friend std::ostream& operator<<(std::ostream& os,
                                    const cmakeprogen_settings& settings);
};

cmakeprogen_settings* parse_arguments(const int argc, char** argv);

#endif
