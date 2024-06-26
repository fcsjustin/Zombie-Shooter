#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
private:
    // where is the bullet?
    sf::Vector2f m_Position;
    
    // what each bullet looks like
    sf::RectangleShape m_BulletShape;

    // is the bullet currently whizzing through the air?
    bool m_InFlight = false;

    // how fast does a bullet travel
    float m_BulletSpeed = 1000;

    // what fraction of 1 pixel does that bullet travel,
    // horizontally and vertically each frame?
    // these values will be derived from m_BulletSpeed
    float m_BulletDistanceX;
    float m_BulletDistanceY;

    // some boundaries so the bullet doesn't fly forever
    float m_MaxX;
    float m_MinX;
    float m_MaxY;
    float m_MinY;

public:

    // the constructor
    Bullet();

    // stop the bullet
    void stop();

    // returns the value of m_InFlight
    bool isInFlight();

    // launch a new bullet
    void shoot(float startX, float startY, float xTarget, float yTarget);

    // tell the calling code where the bullet is in the world
    sf::FloatRect getPosition();

    // return the actual shape (for drawing)
    sf::RectangleShape getShape();

    // update the bullet each frame
    void update(float elapsedTime);

};