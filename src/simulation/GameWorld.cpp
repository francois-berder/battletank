#include "GameWorld.hpp"


GameWorld::GameWorld():
m_currentStep(0),
m_entities()
{
}
        
void GameWorld::step()
{
    for(auto it = m_entities.begin();
        it != m_entities.end();
        ++it)
        (*it)->update();

    ++m_currentStep;
}

bool GameWorld::isFinished()
{
    return false;
}
