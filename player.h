#ifndef PLAYER
#define PLAYER

#include "utils.h"
#include <SDL3/SDL_events.h>

class Game;

class Player {
    transform pos;
    SDL_Event Event;
    bool dead;
    Game* master;
    float timeTillShot;
    SDL_Gamepad* controller;
    float targetRotation;
public:
    Player(Game* master);
    ~Player();
    transform getPos() { return pos; };
    bool isDead() { return dead; }; 
    void move();
    void init();
    void kill();
};

#endif
