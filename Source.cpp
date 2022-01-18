//SFML C++ Bullet Hell
// Created by Dr. Mo Jan 2022
//dynamicsofanasteroid.com

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "globals.h"
#include "missiles.h"
using namespace std;



int main() {
    //game set up (you'll need these lines in every game)
    sf::RenderWindow screen(sf::VideoMode(SCREEN_W, SCREEN_H), "Bullet Hell"); //set up screen
    sf::Event event; //set up event queue
    sf::Clock clock; //set up the clock (needed for game timing)
    const float FPS = 60.0f; //FPS
    screen.setFramerateLimit(FPS); //set FPS

    //player setup------------------------------------------------
    sf::Texture ship;
    ship.loadFromFile("ship.png");
    sf::IntRect playerImg(0, 0, 30, 30); //select portion of image to draw (our ship is 30x30)
    sf::Sprite player(ship, playerImg);
    // player.setTexture(ship); //use this instead of 2 lines above for static images
    int xpos = SCREEN_W/2 - 15;
    int ypos = SCREEN_H/1.2;
    int vx = 0;
    int vy = 0;
    player.setPosition(xpos, ypos); //top left "corner" of circle (not center!)
    bool keys[] = { false, false, false, false, false };
    
    //animation setup----------------------------------------------
    int ticker = 0;
    int frameNum = 0;

    //missile (player projectiles) setup---------------------------
    int justShot = 0;
    //setup variables for missile image
    sf::Texture MissileImg;
    MissileImg.loadFromFile("missile.png");
    sf::Sprite MissilePic;
    MissilePic.setTexture(MissileImg);

    //vector to hold missiles
    vector<missile*> missiles; //creates vector of class missile POINTERS
    vector<missile*>::iterator iter2; //creates an iterator to walk through our missiles vector
    for (int i = 0; i < 5; i++) {
        missile* newMissile = new missile(xpos, ypos, MissilePic); //dynamically instantiates 5 missiles
        missiles.push_back(newMissile); //pushes each missile into vector
    }

    //################### HOLD ONTO YOUR BUTTS, ITS THE GAME LOOP###############################################################
    while (screen.isOpen()) {//keep window open until user shuts it down

        while (screen.pollEvent(event)) { //look for events-----------------------

            //this checks if the user has clicked the little "x" button in the top right corner
            if (event.type == sf::Event::EventType::Closed)
                screen.close();

            //KEYBOARD INPUT 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                keys[LEFT] = true;
            }
            else keys[LEFT] = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                keys[RIGHT] = true;
            }
            else keys[RIGHT] = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                keys[UP] = true;
            }
            else keys[UP] = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                keys[DOWN] = true;
            }
            else keys[DOWN] = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                keys[SPACE] = true;
            }
            else keys[SPACE] = false;


        }//end event loop---------------------------------------------------------------

         //move ship
        if (keys[LEFT] && keys[RIGHT]==false){
            vx = -5;
            playerImg = sf::IntRect((frameNum + 5) * 32 + 32, 0, -30, 30); //NEGATIVE width flips the image for us 
        }
        else if (keys[RIGHT] && keys[LEFT]==false){
            vx = 5;
            playerImg = sf::IntRect((frameNum + 5) * 32, 0, 30, 30);
        }
        else {
            vx = 0;
            playerImg = sf::IntRect(frameNum * 32, 0, 30, 30);
        }

        if (keys[UP] && keys[DOWN] == false) {
            vy = -5;
        }
        else if (keys[DOWN] && keys[UP] == false) {
            vy = 5;
        }
        else {
            vy = 0;
        }

        justShot++; //increment ticker
        if (keys[SPACE]) {
            for (iter2 = missiles.begin(); iter2 != missiles.end(); iter2++) {//walk though the vector, look at each thing
                if (((*iter2)->isAlive()) == false && justShot > 5) { //only shoot dead missiles, add in pause
                    (*iter2)->shoot(xpos, ypos); //shoot the thing the iter is pointing to
                    justShot = 0; //reset ticker
                }
            }
        }

        //move missiles
        for (iter2 = missiles.begin(); iter2 != missiles.end(); iter2++) {
            if ((*iter2)->isAlive()) //only move live missiles
                (*iter2)->move();
        }
        //cull the missiles
        for (iter2 = missiles.begin(); iter2 != missiles.end(); iter2++) {
            if ((*iter2)->offScreen()) //if we move above the edge of the screen
                (*iter2)->kill(); //set as dead
        }

        sf::Sprite player(ship, playerImg);
        xpos += vx;
        ypos += vy;
        
        if (xpos < 0) {
            xpos = 0;
        }
        if (xpos > SCREEN_W - 30) {
            xpos = SCREEN_W - 30;
        }
        if (ypos < 0) {
            ypos = 0;
        }
        if (ypos > SCREEN_H - 30) {
            ypos = SCREEN_H - 30;
        }

        player.setPosition(xpos, ypos);

        //animation section--------------------------------------
        ticker += 1; //ship's engines are always animating

        if (ticker % 10 == 0) //the larger the number, the slower the animation
            frameNum += 1;

        if (frameNum > 4)
            frameNum = 0;

        //render section-----------------------------------------
        screen.clear(); //wipes screen, without this things smear

        //draw missiles
        for (iter2 = missiles.begin(); iter2 != missiles.end(); iter2++) {
            (*iter2)->draw(screen);
        }

        screen.draw(player); //draw player
        screen.display(); //flips memory drawings onto screen

    }//######################## end game loop ###################################################################################

    cout << "goodbye!" << endl;
} //end of main