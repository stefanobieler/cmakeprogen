#include "settings/settings.hpp"
#include "progen.hpp"
#include <iostream>


int main(int argc, char** argv) {

    cmakeprogen_settings* settings = parse_arguments(argc, argv);
    generate_project(settings);
    return EXIT_SUCCESS;
}
