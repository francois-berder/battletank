#include <sstream>
#include <stdexcept>

#include "Tree.hpp"

Data::Data(const std::string &name, const std::string &value):
m_name(name),
m_value(value)
{
}

std::string Data::getName() const
{
    return m_name;
}

std::string Data::getValue() const
{
    return m_value;
}

Array::Array():
m_nodes()
{
}

void Array::addNode(NodePtr node)
{
    m_nodes.push_back(node);
}

NodePtr Array::operator[](const unsigned int index)
{
    if(index >= m_nodes.size())
        throw std::runtime_error("Out of bound index");
        
    return m_nodes[index];
}

long unsigned int Array::size() const
{
    return m_nodes.size();
}

Map::Map():
m_nodes()
{
}

void Map::addNode(const std::string &name, NodePtr node)
{
    m_nodes[name] = node;
}

NodePtr Map::operator[](const std::string &name)
{
    auto it = m_nodes.find(name);
    if(it == m_nodes.end())
    {
        std::stringstream msg;
        msg << "Could not find node ";
        msg << name;
        throw std::runtime_error(msg.str());
    }
    
    return it->second;
}

