#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <list>

bool isEmpty(const std::string &str);
bool isInteger(const std::string &str);
bool isLetter(const char c);
bool isNumber(const char c);
bool isAlphaNumeric(const char c);

int toInteger(const std::string &str);
float toFloat(const std::string &str);

std::list<std::string> split(const std::string &str, const char sep);
std::string concat(std::list<std::string> &elems, const char sep);

#endif /* __UTILS_HPP__ */

