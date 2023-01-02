#include "utils/conversion.hpp"
#include <iomanip>
#include <iostream>

using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;

string int_to_hex(const int& value) {
    stringstream stream;
    stream << "0x" << setfill('0') << setw(sizeof(int) * 2) << hex << value;
    return stream.str();
}
