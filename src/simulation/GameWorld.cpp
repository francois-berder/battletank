#include <sstream>

#include "GameWorld.hpp"


GameWorld::GameWorld():
m_currentStep(0),
m_entities()
{
}
        
void GameWorld::step()
{
    ++m_currentStep;
    
    for(auto it = m_entities.begin();
        it != m_entities.end();
        ++it)
        (*it)->update();
}

bool GameWorld::isFinished()
{
    return false;
}

std::string GameWorld::print()
{
    std::stringstream ss;
    ss << "step " << m_currentStep << ":\n";
    ss << '[';
    for(auto it = m_entities.begin();
        it != m_entities.end();
        ++it)
        ss << (*it)->print();
    ss << ']';
    return ss.str();
}   
