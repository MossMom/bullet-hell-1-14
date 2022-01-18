#include "missiles.h"
#include "globals.h"
#include "SFML/Graphics.hpp"
#include<math.h>

//default constructor
missile::missile() {
	xpos = 0;
	ypos = 0;
	alive = false;
}

//destructor
missile::~missile() {}

//parameterized constructor with SFML image
missile::missile(int x, int y, sf::Sprite sprite) {
	xpos = x;
	ypos = y;
	alive = false;
	image = sprite;
}

void missile::move() {
	ypos -= 5; //simply move the missile up when called
}

//we pass a pointer to the SFML game screen here
//so the function knows where to draw it
void missile::draw(sf::RenderWindow& window) {
	if (alive) { //dont draw dead missiles
		image.setPosition(xpos, ypos);
		window.draw(image); //draw to gamescreen
	}
}

void missile::kill() {
	alive = false;
}

bool missile::offScreen() {
	if (ypos < 0)
		return true;
	else
		return false;
}

void missile::shoot(int x, int y) {
	alive = true;
	xpos = x;
	ypos = y;
}

bool missile::isAlive() {
	return alive;
}

bool missile::collide(double x, double y) {
	if (sqrt((x - xpos) * (x - xpos) + (y - ypos) * (y - ypos)) < 32) { //circular collision
		return true;
	}
	else
		return false;
}

int missile::getx() {
	return xpos;
}
int missile::gety() {
	return ypos;
}