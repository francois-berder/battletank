#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <list>
#include <string>

class Argument
{
    public :
    
        Argument(const std::string &name = std::string(),
                 const std::string &value = std::string());
        
        std::string getName();
        std::string getValue();
        
        bool operator==(const std::string &name)
        {
            return m_name == name;
        }
        
    private :
        
        std::string m_name;
        std::string m_value;
};


class Options
{
    public :
        
        Options();
        
        void parse(int argc, char **argv);
        
        std::list<Argument> getArgs();
        
    private :
    
        void parseArg(char *arg);
    
        std::list<Argument> m_args;
};

#endif /* __OPTIONS_H__ */

