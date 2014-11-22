#ifndef __ENTITY_PARSER_H__
#define __ENTITY_PARSER_H__

#include <string>
#include "Tree.hpp"
#include "StringBuffer.hpp"


class EntityParser
{
    public :
    
        EntityParser(const std::string &str);
    
        Array parse();
        
    private :
    
        NodePtr parseNode();
        Data parseData();
        Array parseArray();
        Map parseMap();
        
        StringBuffer m_buffer;
};

#endif /* __ENTITY_PARSER_H__ */

