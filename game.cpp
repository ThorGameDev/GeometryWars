#include "game.h"
#include "CONST.h"
#include "utils.h"
#include "sprites.h"
#include "AudioPlayer.h"
#include "foe.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <deque>
#include <ostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void Game::newGame()
{
    lastMeasure = std::chrono::steady_clock::now();
    firstFrame = true;
    player.pos.x = SCREEN_X / 2;
    player.pos.y = SCREEN_Y / 2;
    player.pos.theta = 0;
    player.pos.scaleX = PLAYERSCALE;
    player.pos.scaleY = PLAYERSCALE;
    intensity = 1;
    timeTillWave = TIMEPERWAVE;
    currentWaveID = 0;
    lost = false;
    lostTime = 0;
    audio.playSong();
}

Game::Game()
{
    newGame();
    std::cout << "Game has been created" << std::endl;
}

Game::~Game()
{
    SDL_Quit();
    std::cout << "Game has been destroyed" << std::endl;
}

void Game::requestRender()
{
    for (int i = 0; i < things.size(); i++)
    {
        drawer.drawSprite(things[i]->getPos(), things[i]->getSpriteID());
    }
    drawer.drawSprite(player.pos, 0);
    
    drawer.frame();
}

void Game::arroundAttack()
{
    for(int i = 0; i < intensity; i++)
    {
        transform pos;
        double angle = RNG::Double(0, 360); 
        int L = SCREEN_X - PINWHEEL_SCALE;
        int H = SCREEN_Y - PINWHEEL_SCALE;
        pos.x = std::cos(angle) * L/2 + L/2;
        pos.y = std::sin(angle) * H/2 + H/2;
        pos.scaleY = 100;
        pos.scaleX = 100;
        int idx = RNG::Int(0, 100);
        if (idx <= 50)
            things.push_back(new pinwheel(pos, this));
        else if (idx <= 80)
            things.push_back(new diamond(pos, this));
        else if (idx <= 100)
            things.push_back(new bubble(pos, this));
    }
}

void Game::input()
{
    while(SDL_PollEvent(&Event))
    {
#ifndef __EMSCRIPTEN__
        if (Event.type == SDL_EVENT_QUIT)
        {
            quitting = true;
        }
#endif
        if (Event.type == SDL_EVENT_KEY_DOWN)
        {
            switch(Event.key.key)
            {
#ifndef __EMSCRIPTEN__
                case SDLK_ESCAPE:
                    quitting = true;
                    break;
#endif
                case SDLK_UP:
                case SDLK_W:
                    player.up = true;
                    break;
                case SDLK_DOWN:
                case SDLK_S:
                    player.down = true;
                    break;
                case SDLK_LEFT:
                case SDLK_A:
                    player.left = true;
                    break;
                case SDLK_RIGHT:
                case SDLK_D:
                    player.right = true;
                    break;
                case SDLK_SPACE:
                    //player.change = true;
                    break;
            }
        }
        if (Event.type == SDL_EVENT_KEY_UP)
        {
            switch(Event.key.key)
            {
                case SDLK_UP:
                case SDLK_W:
                    player.up = false;
                    break;
                case SDLK_DOWN:
                case SDLK_S:
                    player.down = false;
                    break;
                case SDLK_LEFT:
                case SDLK_A:
                    player.left = false;
                    break;
                case SDLK_RIGHT:
                case SDLK_D:
                    player.right = false;
                    break;
                case SDLK_SPACE:
                    //player.change = false;
                    break;
            }
        }
    }
}

void Game::manageWaves()
{
    timeTillWave -= deltaTime;
    if (timeTillWave <= 0)
    {
        timeTillWave += TIMEPERWAVE;
        //int choice = randomRange(0, 2);
        arroundAttack();

        while(things.size() >= MAX_FOES){
            foe* thing = things.front();
            things.pop_front();
            delete thing;
        }

        currentWaveID += 1;
        if(currentWaveID % 5 == 0)
            intensity += 1;
    }
}

bool Game::checkPlayerDeath()
{
    int topX, topY, bottomX, bottomY;
    topX = player.pos.x + ((double)HIDDENPLAYERSCALE/2);
    topY = player.pos.y + ((double)HIDDENPLAYERSCALE/2);
    bottomX = player.pos.x - ((double)HIDDENPLAYERSCALE/2);
    bottomY = player.pos.y - ((double)HIDDENPLAYERSCALE/2);

    for(int i=0; i < things.size(); i++)
    {
        int _topX = things[i]->getPos().x + ((double)things[i]->getPos().scaleX/2);
        int _topY = things[i]->getPos().y + ((double)things[i]->getPos().scaleY/2);
        int _bottomX = things[i]->getPos().x - ((double)things[i]->getPos().scaleX/2);
        int _bottomY = things[i]->getPos().y - ((double)things[i]->getPos().scaleY/2);
        bool x = false;
        bool y = false;
        if(!(_topX<bottomX||topX<_bottomX))
            x = true;
        if(!(_topY<bottomY||topY<_bottomY))
            y = true;
        if(x && y)
            return true;
    }
    if(topY < 0 || topX < 0 || bottomX > SCREEN_X || bottomY > SCREEN_Y)
        return true;
    return false;
}

void Game::moveObjects()
{
    for(int i = 0; i < things.size(); i++)
    {
        things[i]->move();
    }

    int x = player.right - player.left;
    int y = player.down - player.up;
    double magnitude = std::sqrt(x*x + y*y);
    int speed = PLAYERSPEED;
    if(magnitude >= 0.2)
    {
        player.pos.x += (x/magnitude) * deltaTime * speed;
        player.pos.y += (y/magnitude) * deltaTime * speed;
    }
}

void Game::loopItteration()
{
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    double timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastMeasure).count();
    lastMeasure = now;
    deltaTime = timeElapsed / 1000000;
    if( firstFrame == true)
    {
        firstFrame = false;
        return;
    }
    input();
    if (!lost)
        manageWaves();
    moveObjects();
    bool death = checkPlayerDeath();
    if(death)
    {
        while(things.size() > 0){
            foe* thing = things.front();
            things.pop_front();
            delete thing;
        }
        audio.pauseSong();
        lostTime += deltaTime;
        player.pos.x = 100000;
        if(lostTime >= 2)
            newGame();
    }

    audio.checkRestart();
    requestRender();
}

transform Game::getPlayerPos() {
    return player.pos;
}

Game game;

#ifdef __EMSCRIPTEN__
static void mainloop()
{
    if (game.quitting)
        emscripten_cancel_main_loop();
    game.loopItteration();

#endif

int main(int argc, char* argv[]) 
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
#else
    while(!game.quitting)
    {
        game.loopItteration();
    }
#endif

    return 0;
}
