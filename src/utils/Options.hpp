#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <list>
#include <string>

class Argument
{
    public :
    
        Argument(const std::string &name = std::string(),
                 const std::string &value = std::string());
        
        std::string getName() const;
        std::string getValue() const;
        
        bool operator==(const std::string &name);
        
    private :
        
        std::string m_name;
        std::string m_value;
};


class Options
{
    public :
        
        Options();
        
        void parse(int argc, char **argv);
        
        std::list<Argument> getArgs() const;
        
    private :
    
        void parseArg(char *arg);
    
        std::list<Argument> m_args;
};

#endif /* __OPTIONS_H__ */

