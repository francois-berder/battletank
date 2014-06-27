#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <list>
#include <string>

class Argument : public std::string
{
    public :
    
        Argument() = default;
        Argument(const std::string &str);
        static std::list<Argument> parse(const std::string &arg);    
        static std::string concat(std::list<Argument> &args);

        bool isEmpty() const;
        bool isInteger() const;
        
        std::string toString() const;
        float toFloat() const;
};

#endif /* __ARGUMENT_H__ */

