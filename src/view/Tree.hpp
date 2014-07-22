#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <vector>
#include <map>
#include <string>
#include <memory>

class Node
{
    public :
    
        virtual ~Node() = default;
};

typedef std::shared_ptr<Node> NodePtr;

class Data : public Node
{
    public :

        Data(const std::string &value);
        
        std::string getValue() const;
        
    private :
    
        std::string m_value;
};

class Array : public Node
{
    public :
    
        Array();
        
        void addNode(NodePtr node);
        NodePtr operator[](const unsigned int index);
        
        long unsigned int size() const;
        bool empty() const;
        
    private :
    
        std::vector<NodePtr> m_nodes; 
};

class Map : public Node
{
    public :

        Map();

        void addNode(const std::string &name, NodePtr node);
        NodePtr operator[](const std::string &name);

        long unsigned int size() const;
        bool empty() const;
        
    private :
    
        std::map<std::string, NodePtr> m_nodes; 
};


#endif /* __TREE_HPP__ */

