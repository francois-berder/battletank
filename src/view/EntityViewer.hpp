#ifndef __ENTITY_VIEWER_H__
#define __ENTITY_VIEWER_H__

#include <string>
#include <SFML/Graphics.hpp>


class EntityViewer
{
    public :
    
        EntityViewer(sf::RenderWindow &renderWindow, const std::string &entities);
    
        void drawAll();
    
    private :
    
        void drawObstacle(float x, float y);
        void drawTank(float x, float y, float angle, unsigned int health);
        
        sf::RenderWindow &m_renderWindow;
        std::string m_entities;
};

#endif /* __ENTITY_VIEWER_H__ */

