#include <stdexcept>

#include "EntityParser.hpp"
#include "Utils.hpp"

enum : char
{
    // Array tokens
    ARRAY_BEGIN = '[',
    ARRAY_END = ']',
    ARRAY_SEP = ';',
    
    // Map tokens
    MAP_BEGIN = '{',
    MAP_END = '}',
    MAP_SEP = ',',
    MAP_NAME_SEP = ':'
};

EntityParser::EntityParser(const std::string &str):
m_buffer(str)
{
}
        
Array EntityParser::parse()
{
    return parseArray();
}

NodePtr EntityParser::parseNode()
{
    char c = m_buffer.peek();
    switch(c)
    {
        case ARRAY_BEGIN:
            return NodePtr(new Array(parseArray()));
        case MAP_BEGIN:
            return NodePtr(new Map(parseMap()));
        default:
            if(isAlphaNumeric(c) || c == '.')
                return NodePtr(new Data(parseData()));
            else
                throw std::runtime_error("Invalid token");
    }
}

Data EntityParser::parseData()
{
    return Data(m_buffer.extractWordOrNumber());
}

Array EntityParser::parseArray()
{
    m_buffer.get(ARRAY_BEGIN);

    Array a;
    
    while(m_buffer.peek() != ARRAY_END)
    {
        if(!a.empty())
            m_buffer.get(ARRAY_SEP);

         a.addNode(parseNode());   
    }

    m_buffer.get(ARRAY_END);
    
    return a;
}

Map EntityParser::parseMap()
{
    m_buffer.get(MAP_BEGIN);
    
    Map m;
    
    while(m_buffer.peek() != MAP_END)
    {
        if(!m.empty())
            m_buffer.get(MAP_SEP);
        
        std::string name = m_buffer.extractWord();
        m_buffer.get(MAP_NAME_SEP);
        m.addNode(name, parseNode());   
    }
    
    m_buffer.get(MAP_END);
 
    return m;
}

