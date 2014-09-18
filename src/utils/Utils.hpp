#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <sstream>
#include <string>
#include <list>

bool isEmpty(const std::string &str);
bool isInteger(const std::string &str);
bool isLetter(const char c);
bool isNumber(const char c);
bool isAlphaNumeric(const char c);

unsigned int toUInteger(const std::string &str);
int toInteger(const std::string &str);
float toFloat(const std::string &str);

template<typename T>
std::string toString(T t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

std::list<std::string> split(const std::string &str, const char sep);
std::string concat(std::list<std::string> &elems, const char sep);

float convertRadToDeg(float angle);
float convertDegToRad(float angle);

#endif /* __UTILS_HPP__ */

