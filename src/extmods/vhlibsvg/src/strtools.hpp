#pragma once

#include "vhplatform.hpp"

const char * szhex ();
bool check_str_digit(std::string str);
bool check_str_ishex(std::string str);
std::vector<std::string> split(std::string str, char sym);
std::string join(const std::vector<std::string>& elements, const std::string& delimiter);
