#include "player.h"
#include "game.h"
#include "CONST.h"
#include "utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
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
    if ( !SDL_Init( SDL_INIT_GAMEPAD ) ){
        std::cout << "Couldn't initialize SDL: " <<  SDL_GetError() << std::endl;
        exit(1);
    }
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
        if (Event.type == SDL_EVENT_GAMEPAD_ADDED) {
            if (controller == NULL){
                controller = SDL_OpenGamepad(Event.gdevice.which);
                if (!controller) {
                    SDL_Log("Failed to open gamepad ID %u: %s", (unsigned int) Event.gdevice.which, SDL_GetError());
                }
            }
        }
        else if (Event.type == SDL_EVENT_GAMEPAD_REMOVED) {
            if (controller && (SDL_GetGamepadID(controller) == Event.gdevice.which)) {
                SDL_CloseGamepad(controller);  /* our joystick was unplugged. */
                controller = NULL;
            }
        }
        else if (Event.type == SDL_EVENT_QUIT)
        {
            master->quitting = true;
        }
        else if (Event.type == SDL_EVENT_KEY_DOWN)
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
        else if (Event.type == SDL_EVENT_KEY_UP)
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
    if (controller && magnitude == 0){
        x = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTX)) / 32767.0f);
        y = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTY)) / 32767.0f);
        magnitude = 1;
    }
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

    x = shootRight - shootLeft;
    y = shootDown - shootUp;
    magnitude = std::sqrt(x*x + y*y);
    if (controller && magnitude == 0){
        x = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTX)) / 32767.0f);
        y = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTY)) / 32767.0f);
        magnitude = std::sqrt(x*x + y*y);
    }

    if (magnitude > 0.4f) {
        timeTillShot -= master->deltaTime;
        if (timeTillShot > 0)
            return;
        timeTillShot += TIME_PER_BULLET;

        float theta = std::atan2(y, x);
        float dirX = std::cos(theta);
        float dirY = std::sin(theta);
        master->shoot(pos.x + BULLET_PAIR_DIST*dirY, pos.y - BULLET_PAIR_DIST*dirX, dirX * BULLET_SPEED, dirY * BULLET_SPEED);
        master->shoot(pos.x - BULLET_PAIR_DIST*dirY, pos.y + BULLET_PAIR_DIST*dirX, dirX * BULLET_SPEED, dirY * BULLET_SPEED);
        for(int i = 0; i < BULLET_SUB_STREAMS; i++){
            float theta2 = theta + RNG::Float(-BULLET_STREAM_ANGLE_VARIANCE, BULLET_STREAM_ANGLE_VARIANCE);
            dirX = std::cos(theta2);
            dirY = std::sin(theta2);
            float speed = BULLET_SPEED + RNG::Float(-BULLET_STREAM_SPEED_VARIANCE, BULLET_STREAM_SPEED_VARIANCE);
            master->shoot(pos.x, pos.y, dirX * speed, dirY * speed);
        }

        SDL_RumbleGamepad(controller, 65535 * BULLET_SHAKE, 65535 * BULLET_SHAKE, BULLET_SHAKE_MS);
    }
}

void Player::kill() {
    if (PLAYER_IMORTAL) return;
    dead = true;
    pos.x = 1000000;
    pos.y = 0;
    SDL_RumbleGamepad(controller, 65535 * DEATH_SHAKE, 65535 * DEATH_SHAKE, DEATH_SHAKE_MS);
}

