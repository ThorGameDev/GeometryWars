#include "foe.h"
#include "CONST.h"
#include "utils.h"
#include "sprites.h"
#include "game.h"
#include "particles.h"
#include <cmath>
#include <iostream>
#include <ostream>


void bump(transform* pos, Vector2* speed){
    if (pos->pos.x > GRID_SIZE_X*GRID_UNIT_SCALE - pos->scale.x/2) {
        speed->x = -speed->x;
        pos->pos.x = GRID_SIZE_X*GRID_UNIT_SCALE - pos->scale.x/2;
    }
    else if (pos->pos.x < pos->scale.x/2) {
        speed->x = -speed->x;
        pos->pos.x = pos->scale.x/2;
    }
    else if (pos->pos.y > GRID_SIZE_Y*GRID_UNIT_SCALE - pos->scale.y/2) {
        speed->y = -speed->y;
        pos->pos.y = GRID_SIZE_Y*GRID_UNIT_SCALE - pos->scale.y/2;
    }
    else if (pos->pos.y < pos->scale.y/2) {
        speed->y = -speed->y;
        pos->pos.y = pos->scale.y/2;
    }
}

pinwheel::pinwheel(transform pos, Game* master) : foe(pos, 1, master) {
    this->pos.theta = RNG::Float(0, 360);
    this->pos.scale = PINWHEEL_SCALE;
    float angle = RNG::Float(0, 360); 
    speed = Vector2(std::cos(angle), std::sin(angle)) * PINWHEEL_SPEED;
    master->particleSys->spawn(pos.pos, spriteID + 1);
}

void pinwheel::move() {
    pos.theta += PINWHEEL_ROTATION_SPEED * master->deltaTime;
    if (pos.theta > 360)
        pos.theta -= 360;
    pos.pos += speed * master->deltaTime;
    bump(&pos, &speed);
}

void pinwheel::death(bool byClearing) {
    master->particleSys->death(pos.pos, spriteID + 1);
    return;
}

diamond::diamond(transform pos, Game* master) : foe(pos, 3, master) {
    this->pos.theta = 0;
    this->pos.scale = DIAMOND_SCALE;
    Vector2 dir = master->getPlayerPos().pos - pos.pos;
    speed = dir.normalized() * DIAMOND_SPEED;
    birthday = master->playtime - RNG::Float(0, DIAMOND_SQUISH_SPEED * M_PI);
    master->particleSys->spawn(pos.pos, spriteID + 1);
}

void diamond::move() {
    Vector2 dir = master->getPlayerPos().pos - pos.pos;
    Vector2 redir = speed + dir.normalized() * master->diamondDeltaSpeedConv;
    speed = redir.normalized() * DIAMOND_SPEED;
    pos.pos += speed * master->deltaTime;

    float phase = std::sin((master->playtime - birthday) * DIAMOND_SQUISH_SPEED) / 2 + 0.5;
    pos.scale.y = DIAMOND_SMALL_SCALE + phase * (DIAMOND_SCALE - DIAMOND_SMALL_SCALE);

    bump(&pos, &speed);
}

void diamond::death(bool byClearing) {
    master->particleSys->death(pos.pos, spriteID + 1);
    return;
}

bubble::bubble(transform pos, Game* master) : foe(pos, 5, master) {
    this->pos.theta = 0;
    this->pos.scale = BUBBLE_SCALE;
    Vector2 dir = master->getPlayerPos().pos - pos.pos;
    speed = dir.normalized() * BUBBLE_SPEED;
    master->particleSys->spawn(pos.pos, spriteID + 1);
}

void bubble::move() {
    Vector2 redir = master->getPlayerPos().pos - pos.pos;
    redir = speed + redir.normalized() * master->bubbleDeltaSpeedConv;
    speed = redir.normalized() * BUBBLE_SPEED;
    pos.pos += speed * master->deltaTime;
    bump(&pos, &speed);
}

void bubble::death(bool byClearing) {
    master->particleSys->death(pos.pos, spriteID + 1);
    return;
}

splitSquare::splitSquare(transform pos, Game* master) : foe(pos, 7, master) {
    this->pos.theta = 0;
    this->pos.scale = SPLIT_SQUARE_SCALE;
    Vector2 dir = master->getPlayerPos().pos - pos.pos;
    speed = dir.normalized() * SPLIT_SQUARE_SPEED;
    master->particleSys->spawn(pos.pos, spriteID + 1);
}

void splitSquare::move() {
    Vector2 redir = master->getPlayerPos().pos - pos.pos;
    redir = speed + redir.normalized() * master->splitSquareDeltaSpeedConv;
    speed = redir.normalized() * SPLIT_SQUARE_SPEED;
    pos.pos += speed * master->deltaTime;
    bump(&pos, &speed);
}

void splitSquare::death(bool byClearing) {
    if ( !byClearing ){
        float diag = std::sqrt(2)/2 * SUB_SQUARE_SPEED;
        master->instantiate(new subSquare(pos, master, Vector2(diag, diag)));
        master->instantiate(new subSquare(pos, master, Vector2(diag, -diag)));
        master->instantiate(new subSquare(pos, master, Vector2(-diag, diag)));
        master->instantiate(new subSquare(pos, master, Vector2(-diag, -diag)));
    }
    master->particleSys->death(pos.pos, spriteID + 1);
    return;
}

subSquare::subSquare(transform pos, Game* master, Vector2 speed) : foe(pos, 9, master) {
    this->pos.theta = 0;
    this->pos.scale = SUB_SQUARE_SCALE;
    this->speed = speed;
}

void subSquare::move() {
    Vector2 redir = master->getPlayerPos().pos - pos.pos;
    redir = speed + redir.normalized() * master->subSquareDeltaSpeedConv;
    speed = redir.normalized() * SUB_SQUARE_SPEED;
    pos.pos += speed * master->deltaTime;
    bump(&pos, &speed);
}

void subSquare::death(bool byClearing) {
    master->particleSys->death(pos.pos, spriteID + 1);
    return;
}

greenSquare::greenSquare(transform pos, Game* master) : foe(pos, 11, master) {
    this->pos.theta = RNG::Float(0, 360);
    this->pos.scale = GREEN_SQUARE_SCALE;
    Vector2 dir = master->getPlayerPos().pos - pos.pos;
    speed = dir.normalized() * GREEN_SQUARE_SPEED;
    master->particleSys->spawn(pos.pos, spriteID + 1);
}

void greenSquare::chase() {
    pos.theta += GREEN_SQUARE_ROTATION_SPEED * master->deltaTime;
    if (pos.theta > 360)
        pos.theta -= 360;
    Vector2 redir = master->getPlayerPos().pos - pos.pos;
    redir = speed + redir.normalized() * master->greenSquareDeltaSpeedConv;
    speed = redir.normalized() * GREEN_SQUARE_SPEED;
}

void greenSquare::dodge(bullet* bt) {
    pos.theta += GREEN_SQUARE_PANIC_ROTATION_SPEED * master->deltaTime;
    if (pos.theta > 360)
        pos.theta -= 360;
    Vector2 bulletMotion = bt->speed / BULLET_SPEED;

    Vector2 dir = pos.pos - bt->pos;

    float dodgeDist1 = dir.x * bulletMotion.y - dir.y * bulletMotion.x;
    float dodgeDist2 = - dir.x * bulletMotion.y + dir.y * bulletMotion.x;
    if (dodgeDist1 > dodgeDist2)
        speed = bulletMotion.perp(true) * GREEN_SQUARE_PANIC_SPEED;
    else 
        speed = bulletMotion.perp(false) * GREEN_SQUARE_PANIC_SPEED;
}

void greenSquare::move() {
    bullet* closest = master->getNearestBullet(pos.pos, GREEN_SQUARE_PANIC_SQDISTANCE);
    if (closest == NULL)
        chase();
    else
        dodge(closest);
    pos.pos += speed * master->deltaTime;
    bump(&pos, &speed);
}

void greenSquare::death(bool byClearing) {
    master->particleSys->death(pos.pos, spriteID + 1);
    return;
}
