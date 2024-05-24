#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <iostream>

int main()
{
    // here is the instance of TextureHolder
    TextureHolder holder;
    // the game will always be in our of our four states
    enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

    // start with the GAME_OVER state
    State state = State::GAME_OVER;
    
    sf::Vector2f resolution;
    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Arena", sf::Style::Fullscreen);

    sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    // our clock for timing everything
    sf::Clock clock;

    // how long has the PLAYING state been active
    sf::Time gameTimeTotal;

    // where is the mouse in relation to world coordinates
    sf::Vector2f mouseWorldPosition;

    // where is the mouse in relation to screen coordinates
    sf::Vector2i mouseScreenPosition;

    // create an instance of the player class
    Player player;

    // the boundaries of the arena
    sf::IntRect arena;

    // create the background
    sf::VertexArray background;
    // load the texture for our background vertex array
    sf::Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

    // prepare for a horde of zombies
    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    // 100 bullets
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    // when was the fire button last pressed?
    sf::Time lastPressed;

    // hide the mouse pointer and replace it with crosshair
    window.setMouseCursorVisible(false);
    sf::Sprite spriteCrosshair;
    sf::Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);

    spriteCrosshair.setOrigin(25, 25);

    Pickup healthPickup(1);
    Pickup ammoPickup(2);

    // about the game
    int score = 0;
    int hiScore = 0;

    // for the home/game over screen
    sf::Sprite spriteGameOver;
    sf::Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
    spriteGameOver.setTexture(textureGameOver);
    spriteGameOver.setPosition(0, 0);

    // create a view for the HUD
    sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    // create a sprite for the ammo icon
    sf::Sprite spriteAmmoIcon;
    sf::Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");

    spriteAmmoIcon.setTexture(textureAmmoIcon);
    spriteAmmoIcon.setPosition(20, 980);

    sf::Font font;
    font.loadFromFile("fonts/zombiecontrol.ttf");

    // paused
    sf::Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(155);
    pausedText.setFillColor(sf::Color::White);
    pausedText.setPosition(400, 400);
    pausedText.setString("Press Enter \nto continue");

    // Game Over
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(125);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(250, 850);
    gameOverText.setString("Press Enter to play");

    // LEVELING up
    sf::Text levelUpText;
    levelUpText.setFont(font);
    levelUpText.setCharacterSize(80);
    levelUpText.setFillColor(sf::Color::White);
    levelUpText.setPosition(150, 250);
    std::stringstream levelUpStream;
    levelUpStream <<
        "1- Increased rate of fire" <<
        "\n2- Increased clip size(next reload)" <<
        "\n3- Increased max health" <<
        "\n4- Increased run speed" <<
        "\n5- More and better health pickups" <<
        "\n6- More and better ammo pickups";
    levelUpText.setString(levelUpStream.str());
    // Ammo
    sf::Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(55);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(200, 980);
    // Score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(55);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 0);

    // load the high score from a text file
    ifstream inputFile("scores.txt");
    if (inputFile.is_open())
    {
        // >> reads the data
        inputFile >> hiScore;
        inputFile.close();
    }

    // Hi Score
    sf::Text hiScoreText;
    hiScoreText.setFont(font);
    hiScoreText.setCharacterSize(55);
    hiScoreText.setFillColor(sf::Color::White);
    hiScoreText.setPosition(1400, 0);
    std::stringstream s;
    s << "Hi Score:" << hiScore;
    hiScoreText.setString(s.str());
    
    // Zombies remaining
    sf::Text zombiesRemainingText;
    zombiesRemainingText.setFont(font);
    zombiesRemainingText.setCharacterSize(55);
    zombiesRemainingText.setFillColor(sf::Color::White);
    zombiesRemainingText.setPosition(1500, 980);
    zombiesRemainingText.setString("Zombies: 100");
    // Wave number
    int wave = 0;
    sf::Text waveNumberText;
    waveNumberText.setFont(font);
    waveNumberText.setCharacterSize(55);
    waveNumberText.setFillColor(sf::Color::White);
    waveNumberText.setPosition(1250, 980);
    waveNumberText.setString("Wave: 0");
    
    // Health bar
    sf::RectangleShape healthBar;
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(450, 980);

    //health bar under part
    sf::RectangleShape healthBar2;
    healthBar2.setFillColor(sf::Color(255, 0, 0, 140));
    healthBar2.setPosition(450, 980);
    healthBar2.setSize(sf::Vector2f(300, 50));

    // Debug HUD
    sf::Text debugText;
    debugText.setFont(font);
    debugText.setCharacterSize(25);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(20, 220);
    std::ostringstream ss;
    // When did we last update the HUD?
    int framesSinceLastHUDUpdate = 0;

    // How often (in frames) should we update the HUD
    int fpsMeasurementFrameInterval = 1000;

    // prepare the hit sound
    sf::SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("sound/hit.wav");
    sf::Sound hit;
    hit.setBuffer(hitBuffer);

    hit.setVolume(10);

    // Prepare the splat sound
    sf::SoundBuffer splatBuffer;
    splatBuffer.loadFromFile("sound/splat.wav");
    sf::Sound splat;
    splat.setBuffer(splatBuffer);

    splat.setVolume(10);

    // Prepare the shoot sound
    sf::SoundBuffer shootBuffer;
    shootBuffer.loadFromFile("sound/shoot.wav");
    sf::Sound shoot;
    shoot.setBuffer(shootBuffer);

    shoot.setVolume(10);

    // Prepare the reload sound
    sf::SoundBuffer reloadBuffer;
    reloadBuffer.loadFromFile("sound/reload.wav");
    sf::Sound reload;
    reload.setBuffer(reloadBuffer);

    reload.setVolume(10);

    // Prepare the failed sound
    sf::SoundBuffer reloadFailedBuffer;
    reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
    sf::Sound reloadFailed;
    reloadFailed.setBuffer(reloadFailedBuffer);

    reloadFailed.setVolume(10);

    // Prepare the powerup sound
    sf::SoundBuffer powerupBuffer;
    powerupBuffer.loadFromFile("sound/powerup.wav");
    sf::Sound powerup;
    powerup.setBuffer(powerupBuffer);

    powerup.setVolume(10);

    // Prepare the pickup sound
    sf::SoundBuffer pickupBuffer;
    pickupBuffer.loadFromFile("sound/pickup.wav");
    sf::Sound pickup;
    pickup.setBuffer(pickupBuffer);

    pickup.setVolume(10);

    while (window.isOpen())
    {
        /*
        ************
         Handle Input
        ************
        */

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                // pause the game
                if (event.key.code == sf::Keyboard::Return && state == State::PLAYING)
                {
                    state = State::PAUSED;
                }

                // restart while paused
                else if (event.key.code == sf::Keyboard::Return && state == State::PAUSED)
                {
                    state = State::PLAYING;
                    // reset the clock so there isn't a frame jump
                    clock.restart();
                }
                else if (event.key.code == sf::Keyboard::Return && state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                    wave = 0;
                    score = 0;

                    // prepare the gun and ammo for next game
                    currentBullet = 0;
                    bulletsSpare = 24;
                    bulletsInClip = 6;
                    clipSize = 6;
                    fireRate = 1;

                    // reset the player's stats
                    player.resetPlayerStats();
                }
                if (state == State::PLAYING)
                {
                    // reloading
                    if (event.key.code == sf::Keyboard::R)
                    {
                        if (bulletsSpare >= clipSize)
                        {
                            // plenty of bullets. reload.
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                            reload.play();
                        }
                        else if (bulletsSpare > 0)
                        {
                            // only a few bullets left
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                            reload.play();
                        }
                        else
                        {
                            // more here soon?!
                            reloadFailed.play();
                        }
                    }
                }


            }
        }// end event polling

        // handle the player quitting
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        // Handle WASD while playing
        if (state == State::PLAYING)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopDown();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }

            // fire a bullet
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
                {
                    // pass the centre of the player
                    // and the centre of the cross-hair
                    // to the shoot function
                    bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

                    currentBullet++;
                    if (currentBullet > 99)
                    {
                        currentBullet = 0;
                    }
                    lastPressed = gameTimeTotal;
                    
                    bulletsInClip--;

                    shoot.play();
                }
            }
        } // end WASD while playing

            // Handle the LEVELING up state
        if (state == State::LEVELING_UP)
        {
            // Handle the player LEVELING up
            if (event.key.code == sf::Keyboard::Num1)
            {
                // increase the fire rate
                fireRate++;
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num2)
            {
                // increase clip size
                clipSize += clipSize;
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num3)
            {
                // increase health
                player.upgradeHealth();
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num4)
            {
                // increase speed
                player.upgradeSpeed();
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num5)
            {
                // upgrade pickup
                healthPickup.upgrade();
                state = State::PLAYING;
            }
            if (event.key.code == sf::Keyboard::Num6)
            {
                // upgrade pickup
                ammoPickup.upgrade();
                state = State::PLAYING;
            }

            if (state == State::PLAYING)
            {
                // increase the wave number
                wave++;

                arena.width = 500 * wave;
                arena.height = 500 * wave;
                arena.left = 0;
                arena.top = 0;

                // pass the vertex array by reference
                // to the createBackground function
                int tileSize = createBackground(background, arena);

                // spawn the player in the middle of the arena
                player.spawn(arena, resolution, tileSize);

                // configure the pick ups
                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);

                // create a horde of zombies
                numZombies = 5 * wave;

                // delete the previously allocated memory (if it exists)
                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;

                // play the powerup sound
                powerup.play();

                // reset the clock so there isn't a frame jump
                clock.restart();
            }
        } // end leveling up
            
              /*
                UPDATE THE FRAME
            
            */
       if (state == State::PLAYING)
       {
           // update the delta time
           sf::Time dt = clock.restart();
       
           // update the total game time
           gameTimeTotal += dt;
       
           // make a decimal fraction of 1 from the delta time
           float dtAsSeconds = dt.asSeconds();
       
           // where is the mouse pointer
           mouseScreenPosition = sf::Mouse::getPosition();
       
           // convert mouse position to world coordinates of mainView
           mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);
       
           // set the crosshair to the mouse world location
           spriteCrosshair.setPosition(mouseWorldPosition);
       
           // update the player
           player.update(dtAsSeconds, sf::Mouse::getPosition());
       
           // make a note of the players new position
           sf::Vector2f playerPosition(player.getCenter());
       
           // make the view center around the player
           mainView.setCenter(player.getCenter());
       
           // loop through each zombie and update them
           for (int i = 0; i < numZombies; i++)
           {
               if (zombies[i].isAlive())
               {
                   zombies[i].update(dt.asSeconds(), playerPosition);
               }
           }
       
           // update any bullets that are in flight
           for (int i = 0; i < 100; i++)
           {
               if (bullets[i].isInFlight())
               {
                   bullets[i].update(dtAsSeconds);
               }
           }
       
           // update the pickups
           healthPickup.update(dtAsSeconds);
           ammoPickup.update(dtAsSeconds);
       
           // collision detection
           // have any zombies been shot?7
           for (int i = 0; i < 100; i++)
           {
               for (int j = 0; j < numZombies; j++)
               {
                   if (bullets[i].isInFlight() && zombies[j].isAlive())
                   {
                       if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
                       {
                           bullets[i].stop();
       
                           if (zombies[j].hit())
                           {
                               // not just a hit but a kill too
                               score += 10;
                               if (score >= hiScore)
                               {
                                   hiScore = score;
                               }
       
                               numZombiesAlive--;
       
                               // when all the zombies are dead(again)
                               if (numZombiesAlive == 0)
                               {
                                   state = State::LEVELING_UP;
                               }
                           }
                           splat.play();
       
                       }
                   }
               }
           } // end zombie being shot
       
           // have any zombies touched the player?
           for (int i = 0; i < numZombies; i++)
           {
               if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
               {
                   if (player.hit(gameTimeTotal))
                   {
                       // more here later
                       hit.play();
                   }
       
                   if (player.getHealth() <= 0)
                   {
                       state = State::GAME_OVER;
       
                       ofstream outputFile("scores.txt");
                       // << writes the data
                       outputFile << hiScore;
                       outputFile.close();
                   }
               }
           } // end player touched
       
           // has the player touched health pickup
           if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
           {
               player.increaseHealthLevel(healthPickup.gotIt());
               pickup.play();
           }
       
           // has the player touched ammo pickup
           if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
           {
               bulletsSpare += ammoPickup.gotIt();
               pickup.play();
           }
       
           // size up the health bar
           healthBar.setSize(sf::Vector2f(player.getHealth() * 3, 50));
       
           // increment the number of frames since the previous update
           framesSinceLastHUDUpdate++;
       
           // re-calculate every fpsMeasurementFrameInterval frames
           if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
           {
               // Update game HUD text
               std::stringstream ssAmmo;
               std::stringstream ssScore;
               std::stringstream ssHiScore;
               std::stringstream ssWave;
               std::stringstream ssZombiesAlive;
               // Update the ammo text
               ssAmmo << bulletsInClip << "/" << bulletsSpare;
               ammoText.setString(ssAmmo.str());
               
               // Update the score text
               ssScore << "Score:" << score;
               scoreText.setString(ssScore.str());
               // Update the high score text
               ssHiScore << "Hi Score:" << hiScore;
               hiScoreText.setString(ssHiScore.str());
               // Update the wave
               ssWave << "Wave:" << wave;
               waveNumberText.setString(ssWave.str());
               // Update the high score text
               ssZombiesAlive << "Zombies:" << numZombiesAlive;
               zombiesRemainingText.setString(ssZombiesAlive.str());
               framesSinceLastHUDUpdate = 0;
           } // end HUD update
       
       
       } // end updating the scene

            /*
                DRAW THE SCENE
            */
            window.clear();

            if (state == State::PLAYING)
            {

                
                // set the mainView to be displayed in the window
                // and draw everything related to it
                window.setView(mainView);

                // draw the background
                window.draw(background, &textureBackground);

                // Draw the zombies
                for (int i = 0; i < numZombies; i++)
                {
                    window.draw(zombies[i].getSprite());
                }


                for (int i = 0; i < 100; i++)
                {
                    if (bullets[i].isInFlight())
                    {
                        window.draw(bullets[i].getShape());
                    }
                }
                //draw the player
                window.draw(player.getSprite());

                // draw the pick ups. if currently spawned
                if (ammoPickup.isSpawned())
                {
                    window.draw(ammoPickup.getSprite());
                }
                if (healthPickup.isSpawned())
                {
                    window.draw(healthPickup.getSprite());
                }

                // draw the crosshair
                window.draw(spriteCrosshair);

                // Switch to the HUD view
                window.setView(hudView);
                // Draw all the HUD elements
                window.draw(spriteAmmoIcon);
                window.draw(ammoText);
                window.draw(scoreText);
                window.draw(hiScoreText);
                window.draw(healthBar);
                window.draw(healthBar2);
                window.draw(waveNumberText);
                window.draw(zombiesRemainingText);
            }

            if (state == State::LEVELING_UP)
            {
                window.setView(hudView);
                window.draw(spriteGameOver);
                window.draw(levelUpText);
            }
            if (state == State::PAUSED)
            {
                window.setView(hudView);
                window.draw(pausedText);

            }
            if (state == State::GAME_OVER)
            {
                window.setView(hudView);
                window.draw(spriteGameOver);
                window.draw(gameOverText);
                window.draw(scoreText);
                window.draw(hiScoreText);
            }
            window.display();
        }

        // delete the previously allocated memory
        delete[] zombies;

        return 0;
    } // end game loop
