#include "game.h"
#include "player.h"
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
#include <algorithm>
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
    intensity = 1;
    timeTillWave = TIMEPERWAVE;
    lost = false;
    lostTime = 0;
    audio.playSong();
    player->init();
    lastMeasure = std::chrono::steady_clock::now();
}

Game::Game()
{
    player = new Player(this);
    newGame();
    if (MAXIMUM_INITIAL_INTENSITY) {
        intensity = MAX_FOES;
        spawn();
    }
    std::cout << "Game has been created" << std::endl;
}

Game::~Game()
{
    delete player;
    SDL_Quit();
    std::cout << "Game has been destroyed" << std::endl;
}

transform Game::getPlayerPos(){
    return player->getPos();
}

void Game::requestRender()
{
    for (int i = 0; i < things.size(); i++)
    {
        drawer.drawSprite(things[i]->getPos(), things[i]->getSpriteID());
    }
    for (int i = 0; i < bullets.size(); i++) 
    {
        drawer.drawSquare(bullets[i]->x, bullets[i]->y, BULLET_SIZE, BULLET_SIZE, 255, 255, 255);
    }
    drawer.drawSprite(player->getPos(), 0);
    
    drawer.frame();
}

void Game::spawn()
{
    int num = intensity;
    if (num > MAX_FOES - things.size())
        num = MAX_FOES - things.size();

    for(int i = 0; i < num; i++)
    {
        transform pos;
        do {
            pos.x = RNG::Float(0, SCREEN_X);
            pos.y = RNG::Float(0, SCREEN_Y);
        } while (std::sqrt(std::pow(pos.x - player->getPos().x, 2) + std::pow(pos.y - player->getPos().y, 2)) < MIN_DIST_FROM_PLAYER);

        int idx = RNG::Int(0, 100);
        if (idx <= 50)
            things.push_back(new pinwheel(pos, this));
        else if (idx <= 80)
            things.push_back(new diamond(pos, this));
        else if (idx <= 100)
            things.push_back(new bubble(pos, this));
    }
}

void Game::shoot(float posX, float posY, float speedX, float speedY)
{
    bullet* projectile = new bullet();
    projectile->x = posX;
    projectile->y = posY;
    projectile->speedX = speedX;
    projectile->speedY = speedY;
    bullets.push_back(projectile);
}

void Game::manageWaves()
{
    timeTillWave -= deltaTime;
    if (timeTillWave <= 0)
    {
        timeTillWave += TIMEPERWAVE;
        //int choice = randomRange(0, 2);
        spawn();

        currentWaveID += 1;
        if(currentWaveID % 5 == 0)
            intensity += 1;
    }
}

void Game::collision()
{
    int topX, topY, bottomX, bottomY;
    topX = player->getPos().x + ((float)HIDDENPLAYERSCALE/2);
    topY = player->getPos().y + ((float)HIDDENPLAYERSCALE/2);
    bottomX = player->getPos().x - ((float)HIDDENPLAYERSCALE/2);
    bottomY = player->getPos().y - ((float)HIDDENPLAYERSCALE/2);

    for(int i = things.size() - 1; i >= 0; i--)
    {
        int _topX = things[i]->getPos().x + ((float)things[i]->getPos().scaleX/2);
        int _topY = things[i]->getPos().y + ((float)things[i]->getPos().scaleY/2);
        int _bottomX = things[i]->getPos().x - ((float)things[i]->getPos().scaleX/2);
        int _bottomY = things[i]->getPos().y - ((float)things[i]->getPos().scaleY/2);
        bool skip = false;
        for (int j = bullets.size() - 1; j >= 0; j--) {
            if (!(_topX>bullets[j]->x && bullets[j]->x>_bottomX &&
                _topY>bullets[j]->y&&bullets[j]->y>_bottomY))
                continue;

            foe* thing = things[i];
            thing->death(false);
            things.erase(things.begin() + i);
            delete thing;

            bullet* bt = bullets[j];
            bullets.erase(bullets.begin() + j);
            delete bt;

            skip = true;
            break;
        }
        if (skip) continue;
        if( !(_topX<bottomX||topX<_bottomX) &&
            !(_topY<bottomY||topY<_bottomY)  ) {
            foe* thing = things[i];
            thing->death(false);
            delete thing;
            things.erase(things.begin() + i);
            player->kill();
            continue;
        }
    }
}

void Game::moveObjects()
{
    for(int i = things.size() - 1; i >= 0; i--) {
        things[i]->move();
    }
    for(int i = bullets.size() - 1; i >= 0; i--) {
        bullets[i]->x += bullets[i]->speedX * deltaTime;
        bullets[i]->y += bullets[i]->speedY * deltaTime;
        if (bullets[i]->x > SCREEN_X - BULLET_SIZE/2 || bullets[i]->x <= BULLET_SIZE/2 ||
            bullets[i]->y > SCREEN_Y - BULLET_SIZE/2 || bullets[i]->y <= BULLET_SIZE/2) {
            bullet* bt = bullets[i];
            bullets.erase(bullets.begin() + i);
            delete bt;
        }
    }
    player->move();
}

void Game::loopItteration()
{
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    float timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastMeasure).count();
    lastMeasure = now;
    deltaTime = timeElapsed / 1000000;
    
    playtime += deltaTime;

    if (SHOW_FPS){
        fpsSum += deltaTime;
        framesElapsed += 1;
        if (fpsSum >= 1){
            std::cout << "FPS: " << framesElapsed << std::endl;
            fpsSum -= 1;
            framesElapsed = 0;
        }
    }

    diamondDeltaSpeedConv = deltaTime * DIAMOND_SPEED * DIAMOND_CONVERGANCE;
    bubbleDeltaSpeedConv = deltaTime * BUBBLE_SPEED * BUBBLE_CONVERGANCE;

    if (!lost)
        manageWaves();
    moveObjects();
    collision();
    if(player->isDead())
    {
        while(things.size() > 0){
            foe* thing = things.front();
            things.pop_front();
            delete thing;
        }
        audio.pauseSong();
        lostTime += deltaTime;
        if(lostTime >= 2)
            newGame();
    }

    audio.checkRestart();
    requestRender();
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
