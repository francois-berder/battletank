#include <sstream>
#include <stdexcept>

#include "Tree.hpp"

Data& Node::asData()
{
    return dynamic_cast<Data&>(*this);
}

Array& Node::asArray()
{
    return dynamic_cast<Array&>(*this);
}

Map& Node::asMap()
{
    return dynamic_cast<Map&>(*this);
}

Data::Data(const std::string &value):
m_value(value)
{
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

bool Array::empty() const
{
    return size() == 0;
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

long unsigned int Map::size() const
{
    return m_nodes.size();
}

bool Map::empty() const
{
    return size() == 0;
}
