#include "Zombie.h"
#include "TextureHolder.h"
#include<cstdlib>
#include<ctime>

using namespace std;

bool Zombie::hit()
{
    m_Health--;

    if (m_Health < 0)
    {
        // dead
        m_Alive = false;
        m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));

        return true;
    }

    // injured but not dead yet
    return false;

}

bool Zombie::isAlive()
{
    return m_Alive;
}

void Zombie::spawn(float startX, float startY, int type, int seed)
{
    switch (type)
    {
    case 0:
        // bloater
        m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/bloater.png"));

        m_Speed = BLOATER_SPEED;
        m_Health = BLOATER_HEALTH;
        break;
    case 1:
        // chaser
        m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/chaser.png"));

        m_Speed = CHASER_SPEED;
        m_Health = CHASER_HEALTH;
        break;
    case 2:
        // crawler
        m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/crawler.png"));

        m_Speed = CRAWLER_SPEED;
        m_Health = CRAWLER_HEALTH;
        break;
    }

    // modify the speed to make the zombie unique
    // every zombie is unique. create a speed modifier
    srand((int)time(0) * seed);

    // somewhere between 80 and 100
    float modifier = (rand() % MAX_VARRIANCE) + OFFSET;

    // express this as a fraction of 1
    modifier /= 100; // now equals between .7 and 1
    m_Speed *= modifier;

    // initialize its location
    m_Position.x = startX;
    m_Position.y = startY;

    m_Sprite.setOrigin(25, 25);

    // set its position
    m_Sprite.setPosition(m_Position);
}

sf::FloatRect Zombie::getPosition()
{
    return m_Sprite.getGlobalBounds();
}

sf::Sprite Zombie::getSprite()
{
    return m_Sprite;
}

void Zombie::update(float elapsedTime, sf::Vector2f playerLocation)
{
    float playerX = playerLocation.x;
    float playerY = playerLocation.y;

    // update the zombie position variables
    if (playerX > m_Position.x)
    {
        m_Position.x = m_Position.x + m_Speed * elapsedTime;
    }
    if (playerY > m_Position.y)
    {
        m_Position.y = m_Position.y + m_Speed * elapsedTime;
    }
    if (playerX < m_Position.x)
    {
        m_Position.x = m_Position.x -
            m_Speed * elapsedTime;
    }
    if (playerY < m_Position.y)
    {
        m_Position.y = m_Position.y -
            m_Speed * elapsedTime;
    }
    // Move the sprite
    m_Sprite.setPosition(m_Position);

    // face the sprite in the correct direction
    float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180) / 3.141;
    
    m_Sprite.setRotation(angle);

    
}
