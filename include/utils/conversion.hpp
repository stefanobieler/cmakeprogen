#ifndef CMAKEPROGEN_CONVERTION_HPP
#define CMAKEPROGEN_CONVERTION_HPP

#include <string>
#include "settings/settings.hpp"


/*
*   Converts the int value into a string
*   with a hex value representation
*
*   @param value : the int that will be represented by the string hex returned.
*   @return string : returns the string of hexidecimals representing the int value
*
*/
std::string int_to_hex(const int& value);
language_type string_to_language_type(const std::string& input);


#endif
