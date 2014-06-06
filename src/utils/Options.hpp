#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <map>
#include <string>

class Options
{
    public :
        
        Options();
        
        void parse(int argc, char **argv);
        
        std::string getValue(const std::string &name);
        
    private :
    
        std::map<std::string, std::string> m_values;
};

#endif /* __OPTIONS_H__ */

