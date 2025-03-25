#include "player.h"
#include "game.h"
#include "CONST.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_joystick.h>
#include <cmath>
#include <iostream>
#include <ostream>

void Player::init() {
    pos.x = SCREEN_X / 2;
    pos.y = SCREEN_Y / 2;
    pos.theta = 0;
    pos.scaleX = PLAYERSCALE;
    pos.scaleY = PLAYERSCALE;
    timeTillShot = 0;
    dead = false;
}

Player::Player(Game* master) {
    this->master = master;
    if ( !SDL_Init( SDL_INIT_JOYSTICK ) ){
        std::cout << "Couldn't initialize SDL: " <<  SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_Joystick* fireController = NULL;
    std::cout << *SDL_GetJoysticks(NULL);

}

Player::~Player(){

}

void Player::move()
{
    if (dead) return;

    static bool up;
    static bool down;
    static bool left;
    static bool right;
    static bool shootUp;
    static bool shootDown;
    static bool shootLeft;
    static bool shootRight;

    while(SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_EVENT_QUIT)
        {
            master->quitting = true;
        }
        if (Event.type == SDL_EVENT_KEY_DOWN)
        {
            switch(Event.key.key)
            {
                case SDLK_ESCAPE:
                    master->quitting = true;
                    break;
                case SDLK_UP:
                    up = true;
                    break;
                case SDLK_DOWN:
                    down = true;
                    break;
                case SDLK_LEFT:
                    left = true;
                    break;
                case SDLK_RIGHT:
                    right = true;
                    break;
                case SDLK_W:
                    shootUp = true;
                    break;
                case SDLK_S:
                    shootDown = true;
                    break;
                case SDLK_A:
                    shootLeft = true;
                    break;
                case SDLK_D:
                    shootRight = true;
                    break;
            }
        }
        if (Event.type == SDL_EVENT_KEY_UP)
        {
            switch(Event.key.key)
            {
                case SDLK_UP:
                    up = false;
                    break;
                case SDLK_DOWN:
                    down = false;
                    break;
                case SDLK_LEFT:
                    left = false;
                    break;
                case SDLK_RIGHT:
                    right = false;
                    break;
                case SDLK_W:
                    shootUp = false;
                    break;
                case SDLK_S:
                    shootDown = false;
                    break;
                case SDLK_A:
                    shootLeft = false;
                    break;
                case SDLK_D:
                    shootRight = false;
                    break;
            }
        }
    }

    float x = right - left;
    float y = down - up;
    float magnitude = std::sqrt(x*x + y*y);
    if (pos.x >= SCREEN_X - pos.scaleX/2 && x > 0) {
        x = 0;
        pos.x = SCREEN_X - pos.scaleX/2;
    }
    if (pos.x <= pos.scaleX/2 && x < 0) {
        x = 0;
        pos.x = pos.scaleX/2;
    }
    if (pos.y >= SCREEN_Y - pos.scaleY/2 && y > 0) {
        y = 0;
        pos.y = SCREEN_Y - pos.scaleY/2;
    }
    if (pos.y <= pos.scaleY/2 && y < 0) {
        y = 0;
        pos.y = pos.scaleY/2;
    }

    if(magnitude >= 0.2)
    {
        pos.x += (x/magnitude) * master->deltaTime * PLAYERSPEED;
        pos.y += (y/magnitude) * master->deltaTime * PLAYERSPEED;
    }

    if (shootUp || shootDown || shootLeft || shootRight) {
        timeTillShot -= master->deltaTime;
        if (timeTillShot > 0)
            return;
        timeTillShot += TIME_PER_BULLET;
        float x = shootRight - shootLeft;
        float y = shootDown - shootUp;
        float magnitude = std::sqrt(x*x + y*y);
        x = x / magnitude;
        y = y / magnitude;
        float theta = std::atan2(y, x);
        master->shoot(pos.x + BULLET_PAIR_DIST*std::cos(theta - M_PI/2), 
                pos.y + BULLET_PAIR_DIST*std::sin(theta - M_PI/2), x * BULLET_SPEED, y * BULLET_SPEED);
        master->shoot(pos.x + BULLET_PAIR_DIST*std::cos(theta + M_PI/2), 
                pos.y + BULLET_PAIR_DIST*std::sin(theta + M_PI/2), x * BULLET_SPEED, y * BULLET_SPEED);

    }
}

void Player::kill() {
    if (PLAYER_IMORTAL) return;
    dead = true;
    pos.x = 1000000;
    pos.y = 0;
}

