#include "utils/conversion.hpp"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <vector>

using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

string int_to_hex(const int& value) {
    stringstream stream;
    stream << "0x" << setfill('0') << setw(sizeof(int) * 2) << hex << value;
    return stream.str();
}

language_type string_to_language_type(const std::string& input) {
    
    vector<char> input_lower_case_vec;

    for(size_t i = 0; i < input.length(); i++) {
        input_lower_case_vec.push_back(tolower(input[i]));
    }

    string input_lower_case(&input_lower_case_vec[0]);

    if(input_lower_case == "c") {
        return language_type::c17;
    } else if (input_lower_case == "c++") {
        return language_type::cpp20;
    } else if (input_lower_case == "c11") {
        return language_type::c11;
    } else if (input_lower_case == "c17") {
        return language_type::c17;
    } else if (input_lower_case == "cpp11") {
        return language_type::cpp11;
    } else if (input_lower_case == "cpp14") {
        return language_type::cpp14;
    } else if (input_lower_case == "cpp17") {
        return language_type::cpp17;
    } else if (input_lower_case == "cpp20") {
        return language_type::cpp20;
    } else {
        return language_type::unknown;
    }

}
