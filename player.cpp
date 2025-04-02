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
    pos.pos = GRID_SIZE * GRID_UNIT_SCALE / 2;
    pos.theta = 0;
    pos.scale = PLAYER_SCALE;
    targetRotation = 0;
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
    if (controller) {
        SDL_CloseGamepad(controller);
        controller = NULL;
    }
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
        if (Event.type == SDL_EVENT_QUIT) {
            master->quitting = true;
            return;
        }
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


    Vector2 dir = Vector2(right - left, down - up);

    if (controller && dir.magnitude() == 0){
        dir.x = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTX)) / 32767.0f);
        dir.y = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTY)) / 32767.0f);
    }
    if (pos.pos.x >= (GRID_SIZE.x*GRID_UNIT_SCALE) - pos.scale.x/2 && dir.x > 0) {
        dir.x = 0;
        pos.pos.x = (GRID_SIZE.x*GRID_UNIT_SCALE) - pos.scale.x/2;
    }
    if (pos.pos.x <= pos.scale.x/2 && dir.x < 0) {
        dir.x = 0;
        pos.pos.x = pos.scale.x/2;
    }
    if (pos.pos.y >= (GRID_SIZE.x*GRID_UNIT_SCALE) - pos.scale.y/2 && dir.y > 0) {
        dir.y = 0;
        pos.pos.y = (GRID_SIZE.x*GRID_UNIT_SCALE) - pos.scale.y/2;
    }
    if (pos.pos.y <= pos.scale.y/2 && dir.y < 0) {
        dir.y = 0;
        pos.pos.y = pos.scale.y/2;
    }

    if(dir.magnitude() >= 0.2)
    {
        pos.pos += dir.normalized() * master->deltaTime * PLAYER_SPEED;
        targetRotation = std::atan2(dir.y, dir.x)/M_PI * 180 + 90; // No clue why the +90
    }

    if(targetRotation - pos.theta > 180)
        pos.theta = pos.theta + (targetRotation-360 - pos.theta) * PLAYER_ROTATE_SPEED * master->deltaTime;
    else if(targetRotation - pos.theta < -180)
        pos.theta = pos.theta + (targetRotation+360 - pos.theta) * PLAYER_ROTATE_SPEED * master->deltaTime;
    else
        pos.theta = pos.theta + (targetRotation - pos.theta) * PLAYER_ROTATE_SPEED * master->deltaTime;

    if (pos.theta > 360)
        pos.theta -= 360;
    if (pos.theta < 0)
        pos.theta += 360;

    Vector2 shootdir = Vector2(shootRight - shootLeft, shootDown - shootUp);
    if (controller && shootdir.magnitude() == 0){
        shootdir.x = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTX)) / 32767.0f);
        shootdir.y = (((float) SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTY)) / 32767.0f);
    }

    if (shootdir.magnitude() > 0.4f) {
        timeTillShot -= master->deltaTime;
        if (timeTillShot > 0)
            return;
        timeTillShot += TIME_PER_BULLET;

        float theta = std::atan2(shootdir.y, shootdir.x);
        Vector2 dir = Vector2(std::cos(theta), std::sin(theta));
        master->shoot(pos.pos + dir.perp(false)*BULLET_PAIR_DIST, dir * BULLET_SPEED);
        master->shoot(pos.pos + dir.perp(true)*BULLET_PAIR_DIST, dir * BULLET_SPEED);
        for(int i = 0; i < BULLET_SUB_STREAMS; i++){
            float theta2 = theta + RNG::Float(-BULLET_STREAM_ANGLE_VARIANCE, BULLET_STREAM_ANGLE_VARIANCE);
            dir = Vector2(std::cos(theta2), std::sin(theta2));
            float speed = BULLET_SPEED + RNG::Float(-BULLET_STREAM_SPEED_VARIANCE, BULLET_STREAM_SPEED_VARIANCE);
            master->shoot(pos.pos, dir * speed);
        }

        SDL_RumbleGamepad(controller, 65535 * BULLET_SHAKE, 65535 * BULLET_SHAKE, BULLET_SHAKE_MS);
    }
}

void Player::kill() {
    if (PLAYER_IMORTAL) return;
    dead = true;
    pos.pos = 1000000;
    SDL_RumbleGamepad(controller, 65535 * DEATH_SHAKE, 65535 * DEATH_SHAKE, DEATH_SHAKE_MS);
}

