#include "foe.h"
#include "CONST.h"
#include "utils.h"
#include "sprites.h"
#include "game.h"
#include <cmath>

void bump(transform* pos, double* speedX, double* speedY){
    if (pos->x > SCREEN_X - pos->scaleX/2) {
        *speedX = -*speedX;
        pos->x = SCREEN_X - pos->scaleX/2;
    }
    if (pos->x < pos->scaleX/2) {
        *speedX = -*speedX;
        pos->x = pos->scaleX/2;
    }
    if (pos->y > SCREEN_Y - pos->scaleY/2) {
        *speedY = -*speedY;
        pos->y = SCREEN_Y - pos->scaleY/2;
    }
    if (pos->y < pos->scaleY/2) {
        *speedY = -*speedY;
        pos->y = pos->scaleY/2;
    }
}

pinwheel::pinwheel(transform pos, Game* master) : foe(pos, 1, master) {
    double angle = RNG::Double(0, 360); 
    speedX = std::cos(angle) * PINWHEEL_SPEED;
    speedY = std::sin(angle) * PINWHEEL_SPEED;
    this->pos.scaleX = PINWHEEL_SCALE;
    this->pos.scaleY = PINWHEEL_SCALE;
}

void pinwheel::move() {
    pos.theta += 360 * master->deltaTime;
    if (pos.theta > 360)
        pos.theta -= 360;
    pos.x += speedX * master->deltaTime;
    pos.y += speedY * master->deltaTime;
    bump(&pos, &speedX, &speedY);
}

diamond::diamond(transform pos, Game* master) : foe(pos, 2, master) {
    pos.theta = 0;
    speedX = master->getPlayerPos().x - pos.x;
    speedY = master->getPlayerPos().y - pos.y;
    double magnitude = std::sqrt(speedX*speedX + speedY*speedY);
    speedX = speedX / magnitude * DIAMOND_SPEED;
    speedY = speedY / magnitude * DIAMOND_SPEED;
    this->pos.scaleX = DIAMOND_SCALE;
    this->pos.scaleY = DIAMOND_SCALE;
}

void diamond::move() {
    double redirX = master->getPlayerPos().x - pos.x;
    double redirY = master->getPlayerPos().y - pos.y;
    double magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    redirX = speedX + redirX / magnitude * master->deltaTime * DIAMOND_SPEED * DIAMOND_CONVERGANCE;
    redirY = speedY + redirY / magnitude * master->deltaTime * DIAMOND_SPEED * DIAMOND_CONVERGANCE;
    magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    speedX = redirX / magnitude * DIAMOND_SPEED;
    speedY = redirY / magnitude * DIAMOND_SPEED;
    pos.x += speedX * master->deltaTime;
    pos.y += speedY * master->deltaTime;
    bump(&pos, &speedX, &speedY);
}

bubble::bubble(transform pos, Game* master) : foe(pos, 3, master) {
    pos.theta = 0;
    speedX = master->getPlayerPos().x - pos.x;
    speedY = master->getPlayerPos().y - pos.y;
    double magnitude = std::sqrt(speedX*speedX + speedY*speedY);
    speedX = speedX / magnitude * BUBBLE_SPEED;
    speedY = speedY / magnitude * BUBBLE_SPEED;
    this->pos.scaleX = BUBBLE_SCALE;
    this->pos.scaleY = BUBBLE_SCALE;
}

void bubble::move() {
    double redirX = master->getPlayerPos().x - pos.x;
    double redirY = master->getPlayerPos().y - pos.y;
    double magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    redirX = speedX + redirX / magnitude * master->deltaTime * BUBBLE_SPEED * BUBBLE_CONVERGANCE;
    redirY = speedY + redirY / magnitude * master->deltaTime * BUBBLE_SPEED * BUBBLE_CONVERGANCE;
    magnitude = std::sqrt(redirX*redirX + redirY*redirY);
    speedX = redirX / magnitude * BUBBLE_SPEED;
    speedY = redirY / magnitude * BUBBLE_SPEED;
    pos.x += speedX * master->deltaTime;
    pos.y += speedY * master->deltaTime;
    bump(&pos, &speedX, &speedY);
}
