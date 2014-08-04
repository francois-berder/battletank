#include <stdexcept>

#include "StringBuffer.hpp"
#include "Utils.hpp"

StringBuffer::StringBuffer(const std::string &str):
m_str(),
m_index(0)
{
    m_str.reserve(str.size());
    for(unsigned int i = 0; i < str.size(); ++i)
        m_str.push_back(str[i]);
}

char StringBuffer::peek()
{
    if(m_index >= m_str.size())
        throw std::runtime_error("Could not peek char");    

    return m_str[m_index];
}

void StringBuffer::get()
{
    if(m_index >= m_str.size())
        throw std::runtime_error("Could not get char");    
        
    ++m_index;
}

void StringBuffer::get(char c)
{
    char a = peek();
    if(a != c)
        throw std::runtime_error("Invalid char");
        
    ++m_index;
}

std::string StringBuffer::extractWordOrNumber()
{
    char c = peek();
    if(isLetter(c))
        return extractWord();
    else if(isNumber(c) || c == '.' || c == '-')
        return extractNumber();
    else
        throw std::runtime_error("Invalid char in extractWordOrNumber");
}

std::string StringBuffer::extractWord()
{
    std::string str;
    while(m_index < m_str.size() && isLetter(peek()))
        str += m_str[m_index++];

    return str;
}

std::string StringBuffer::extractNumber()
{
    std::string str;
    while(m_index < m_str.size() && (isNumber(peek()) || peek() == '.' || peek() == '-'))
        str += m_str[m_index++];

    return str;
}

