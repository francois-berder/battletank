#ifndef __ENTITY_VIEWER_H__
#define __ENTITY_VIEWER_H__

#include <string>
#include <SFML/Graphics.hpp>
#include "Tree.hpp"


class EntityViewer
{
    public :
    
        EntityViewer(sf::RenderWindow &renderWindow, Array &entities);
    
        void drawAll();
    
    private :
    
        void drawObstacle(float x, float y);
        void drawTank(float x, float y, int angle, int cannonAngle, unsigned int health);
        void drawBullet(float x, float y, int angle);
        void drawExplosion(float x, float y, unsigned int timer);

        sf::RenderWindow &m_renderWindow;
        Array &m_entities;
};

#endif /* __ENTITY_VIEWER_H__ */

