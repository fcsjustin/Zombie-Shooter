#pragma once
#include <SFML/Graphics.hpp>

class Zombie
{
private:
    // how fast is each zombie type?
    const float BLOATER_SPEED = 40;
    const float CHASER_SPEED = 80;
    const float CRAWLER_SPEED = 20;

    // how tough is each zombie type
    const float BLOATER_HEALTH = 5;
    const float CHASER_HEALTH = 1;
    const float CRAWLER_HEALTH = 3;

    // make each zombie vary its speed slightly
    const int MAX_VARRIANCE = 30;
    const int OFFSET = 101 - MAX_VARRIANCE;

    // where is the zombie?
    sf::Vector2f m_Position;
    
    // A sprite for the zombie
    sf::Sprite m_Sprite;

    // how fast can this one run/crawl
    float m_Speed;

    // how much health has it got
    float m_Health;
    
    // is it still alive?
    bool m_Alive;

public:

    // handle when a bullet hits a zombie
    bool hit();

    // find out if the zombie is alive
    bool isAlive();

    // spawn a new zombie
    void spawn(float startX, float startY, int type, int seed);

    // return a rectangle that is the position in the world
    sf::FloatRect getPosition();

    // get a copy of the sprite to draw
    sf::Sprite getSprite();

    // update the zombie each frame
    void update(float elapsedTime, sf::Vector2f playerLocation);

};