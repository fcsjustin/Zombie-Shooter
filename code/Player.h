/*
void spawn(IntRect arena, Vector2f resolution, int tileSize):
This function does what its name suggests. It will prepare the object ready
for use, which includes putting it in its starting location (that is, spawning
it). Notice that it doesn't return any data, but it does have three arguments.
It receives an IntRect instance called arena, which will be the size and
location of the current level; a Vector2f instance, which will contain the
screen resolution; and an int, which will hold the size of a background tile.

• void resetPlayerStats: Once we give the player the ability to level up
between waves, we will need to be able to take away/reset those abilities at
the start of a new game.

• Time getLastHitTime(): This function does just one thing – it returns the
time when the player was last hit by a zombie. We will use this function
when detecting collisions, and it will allow us to make sure the player isn't
punished too frequently for making contact with a zombie.

• FloatRect getPosition(): This function returns a FloatRect instance
that describes the horizontal and vertical floating-point coordinates of the
rectangle, which contains the player graphic. This is also useful for collision
detection.

• Vector2f getCenter(): This is slightly different to getPosition because it
is a Vector2f type and contains just the x and y locations of the very center
of the player graphic.

• float getRotation(): The code in the main function will sometimes need
to know, in degrees, which way the player is currently facing. 3 o'clock is 0
degrees and increases clockwise.

• Sprite getSprite(): As we discussed previously, this function returns a
copy of the sprite that represents the player.

• void moveLeft(), ..Right(), ..Up(), ..Down(): These four functions have
no return type or parameters. They will be called from the main function and
the Player class will then be able to act when one or more of the WASD keys
have been pressed.

• void stopLeft(), ..Right(), ..Up(), ..Down(): These four functions have
no return type or parameters. They will be called from the main function, and
the Player class will then be able to act when one or more of the WASD keys
have been released.

void update(float elapsedTime, Vector2i mousePosition): This will
be the only long function of the entire class. It will be called once per frame
from main. It will do everything necessary to make sure the player object's
data is updated so that it's ready for collision detection and drawing. Notice
that it returns no data but receives the amount of elapsed time since the last
frame, along with a Vector2i instance, which will hold the horizontal and
vertical screen location of the mouse pointer/crosshair

void upgradeSpeed(): A function that can be called from the leveling up
screen when the player chooses to make the player faster.

• void upgradeHealth(): Another function that can be called from the
leveling up screen when the player chooses to make the player stronger
(that is, have more health).

• void increaseHealthLevel(int amount): A subtle but important
difference regarding the previous function in that this one will increase the
amount of health the player has, up to the maximum that's currently set. This
function will be used when the player picks up a health pick-up.

• int getHealth(): With the level of health being as dynamic as it is, we
need to be able to determine how much health the player has at any given
moment. This function returns an int, which holds that value.

*/

#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
    const float START_SPEED = 200;
    const float START_HEALTH = 100;

    // where is the player ?
    sf::Vector2f m_Position;
   
    sf::Sprite m_Sprite;
    sf::Texture m_Texture;

    sf::Vector2f m_Resolution;

    // what is the size of the current arena
    sf::IntRect m_Arena;
    int m_TileSize;

    // which direction is the player currently moving in
    bool m_UpPressed;
    bool m_DownPressed;
    bool m_LeftPressed;
    bool m_RightPressed;

    int m_Health;
    int m_MaxHealth;

    // when was the player last hit
    sf::Time m_LastHit;

    // speed in pixels per second
    float m_Speed;

public:
    Player();

    void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);

    // call this at the end of every game
    void resetPlayerStats();

    // handle the player getting hit by a zombie
    bool hit(sf::Time timeHit);
    
    // How long ago was the player last hit
    sf::Time getLastHitTime();
    
    // Where is the player
    sf::FloatRect getPosition();
    
    // Where is the center of the player
    sf::Vector2f getCenter();
    
    // What angle is the player facing
    float getRotation();
    
    // Send a copy of the sprite to the main function
    sf::Sprite getSprite();
    
    // The next four functions move the player
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    // Stop the player moving in a specific direction
    void stopLeft();
    void stopRight();
    void stopUp();
    void stopDown();
    
    // We will call this function once every frame
    void update(float elapsedTime, sf::Vector2i mousePosition);
    
    // Give the player a speed boost
    void upgradeSpeed();
    
    // Give the player some health
    void upgradeHealth();
    
    // Increase the maximum amount of health the player can have
    void increaseHealthLevel(int amount);
    
    // How much health has the player currently got?
    int getHealth();

};