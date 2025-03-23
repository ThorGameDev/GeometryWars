#include "foe.h"
#include "CONST.h"
#include "utils.h"
#include "sprites.h"
#include "game.h"
#include <cmath>
#include <iostream>
#include <ostream>


void bump(transform* pos, float* speedX, float* speedY){
    if (pos->x > SCREEN_X - pos->scaleX/2) {
        *speedX = -*speedX;
        pos->x = SCREEN_X - pos->scaleX/2;
    }
    else if (pos->x < pos->scaleX/2) {
        *speedX = -*speedX;
        pos->x = pos->scaleX/2;
    }
    if (pos->y > SCREEN_Y - pos->scaleY/2) {
        *speedY = -*speedY;
        pos->y = SCREEN_Y - pos->scaleY/2;
    }
    else if (pos->y < pos->scaleY/2) {
        *speedY = -*speedY;
        pos->y = pos->scaleY/2;
    }
}

pinwheel::pinwheel(transform pos, Game* master) : foe(pos, 1, master) {
    this->pos.theta = 0;
    this->pos.scaleX = PINWHEEL_SCALE;
    this->pos.scaleY = PINWHEEL_SCALE;
    float angle = RNG::Float(0, 360); 
    speedX = std::cos(angle) * PINWHEEL_SPEED;
    speedY = std::sin(angle) * PINWHEEL_SPEED;
}

void pinwheel::move() {
    pos.theta += 360 * master->deltaTime;
    if (pos.theta > 360)
        pos.theta -= 360;
    pos.x += speedX * master->deltaTime;
    pos.y += speedY * master->deltaTime;
    bump(&pos, &speedX, &speedY);
}

void pinwheel::death(bool byClearing) {
    return;
}

diamond::diamond(transform pos, Game* master) : foe(pos, 2, master) {
    this->pos.theta = 0;
    this->pos.scaleX = DIAMOND_SCALE;
    this->pos.scaleY = DIAMOND_SCALE;
    speedX = master->getPlayerPos().x - pos.x;
    speedY = master->getPlayerPos().y - pos.y;
    float magnitude = std::sqrt(speedX*speedX + speedY*speedY);
    speedX = speedX / magnitude * DIAMOND_SPEED;
    speedY = speedY / magnitude * DIAMOND_SPEED;
    birthday = master->playtime - RNG::Float(0, DIAMOND_SQUISH_SPEED * M_PI);
}

void diamond::move() {
    float redirX = master->getPlayerPos().x - pos.x;
    float redirY = master->getPlayerPos().y - pos.y;
    float magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    redirX = speedX + redirX / magnitude * master->diamondDeltaSpeedConv;
    redirY = speedY + redirY / magnitude * master->diamondDeltaSpeedConv;
    magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    speedX = redirX / magnitude * DIAMOND_SPEED;
    speedY = redirY / magnitude * DIAMOND_SPEED;
    pos.x += speedX * master->deltaTime;
    pos.y += speedY * master->deltaTime;

    float phase = std::sin((master->playtime - birthday) * DIAMOND_SQUISH_SPEED) / 2 + 0.5;
    pos.scaleY = DIAMOND_SMALL_SCALE + phase * (DIAMOND_SCALE - DIAMOND_SMALL_SCALE);

    bump(&pos, &speedX, &speedY);
}

void diamond::death(bool byClearing) {
    return;
}

bubble::bubble(transform pos, Game* master) : foe(pos, 3, master) {
    this->pos.theta = 0;
    this->pos.scaleX = BUBBLE_SCALE;
    this->pos.scaleY = BUBBLE_SCALE;
    speedX = master->getPlayerPos().x - pos.x;
    speedY = master->getPlayerPos().y - pos.y;
    float magnitude = std::sqrt(speedX*speedX + speedY*speedY);
    speedX = speedX / magnitude * BUBBLE_SPEED;
    speedY = speedY / magnitude * BUBBLE_SPEED;
}

void bubble::move() {
    float redirX = master->getPlayerPos().x - pos.x;
    float redirY = master->getPlayerPos().y - pos.y;
    float magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    //redirX = speedX + redirX / magnitude * master->deltaTime * BUBBLE_SPEED * BUBBLE_CONVERGANCE;
    //redirY = speedY + redirY / magnitude * master->deltaTime * BUBBLE_SPEED * BUBBLE_CONVERGANCE;
    redirX = speedX + redirX / magnitude * master->bubbleDeltaSpeedConv;
    redirY = speedY + redirY / magnitude * master->bubbleDeltaSpeedConv;
    magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    speedX = redirX / magnitude * BUBBLE_SPEED;
    speedY = redirY / magnitude * BUBBLE_SPEED;
    pos.x += speedX * master->deltaTime;
    pos.y += speedY * master->deltaTime;
    bump(&pos, &speedX, &speedY);
}

void bubble::death(bool byClearing) {
    return;
}
