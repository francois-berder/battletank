#include <stdexcept>

#include "StringBuffer.hpp"

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

