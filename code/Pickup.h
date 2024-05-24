#pragma once
#include <SFML/Graphics.hpp>

class Pickup
{
private:
    // start value for health pickups
    const int HEALTH_START_VALUE = 50;
    const int AMMO_START_VALUE = 36;
    const int START_WAIT_TIME = 10;
    const int START_SECONDS_TO_LIVE = 5;

    // the sprite that represents this pickup
    sf::Sprite m_Sprite;

    // the arena it exists in
    sf::IntRect m_Arena;

    // how much is this pickup worth?
    int m_Value;

    // what type of pickup is this?
    // 1 = health, 2 = ammo
    int m_Type;

    // handle spawning and disappearing
    bool m_Spawned;
    float m_SecondsSinceSpawn;
    float m_SecondsSinceDeSpawn;
    float m_SecondsToLive;
    float m_SecondsToWait;

public:
    Pickup(int type);

    // prepare a new pickup
    void setArena(sf::IntRect arena);

    void spawn();

    // check the position of a pickup
    sf::FloatRect getPosition();

    // get the sprite for drawing
    sf::Sprite getSprite();

    // let the pickup update itself each frame
    void update(float elapsedTime);

    // is this pickup currently spawned?
    bool isSpawned();

    // get the goodness from the pickup
    int gotIt();

    // upgrade the value of each pickup
    void upgrade();

};