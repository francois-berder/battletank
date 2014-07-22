#ifndef __STRING_BUFFER_HPP__
#define __STRING_BUFFER_HPP__

#include <string>
#include <vector>

class StringBuffer
{
    public :
    
        StringBuffer(const std::string &str);
    
        char peek();
        void get();
        void get(char c);
        
    private :
    
        std::vector<char> m_str;
        unsigned int m_index;
};

#endif /* __STRING_BUFFER_HPP__ */

