#include "game.h"
#include "player.h"
#include "CONST.h"
#include "utils.h"
#include "sprites.h"
#include "AudioPlayer.h"
#include "foe.h"
#include "particles.h"
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
#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void Game::newGame() {
    intensity = 1;
    timeTillWave = TIME_PER_WAVE;
    lost = false;
    lostTime = 0;
    player->init();
    drawer->init();
    audio->playSong();
    lastMeasure = std::chrono::steady_clock::now();
}

Game::Game() {
    player = new Player(this);
    drawer = new ScreenDrawer(this);
    audio = new AudioPlayer();
    particleSys = new ParticleSystem(drawer, this);
    newGame();
    if (MAXIMUM_INITIAL_INTENSITY) {
        intensity = MAX_FOES;
        spawn();
    }
    std::cout << "Game has been created" << std::endl;
}

Game::~Game() {
    delete player;
    delete audio;
    delete particleSys;
    delete drawer;
    SDL_Quit();
    std::cout << "Game has been destroyed" << std::endl;
}

transform Game::getPlayerPos() {
    return player->getPos();
}

void Game::requestRender() {
    for (int x = 0; x < GRID_SIZE.x; x++) {
        for (int y = 0; y < GRID_SIZE.y; y++) {
            transform gridthing;
            gridthing.pos = Vector2(x, y)*GRID_UNIT_SCALE + GRID_UNIT_SCALE/2;
            gridthing.scale = GRID_UNIT_SCALE;
            gridthing.theta = 0;
            int tileID = (y != 0) | (2 * (x != 0)) | (4 * (x != GRID_SIZE.x - 1)) | (8 * (y != GRID_SIZE.y - 1));
            int spt = drawer->getGridOffset() + tileID;
            drawer->drawSprite(gridthing, spt);
        }
    }
    for (int i = 0; i < things.size(); i++)
        drawer->drawSprite(things[i]->getPos(), things[i]->getSpriteID());
    for (int i = 0; i < bullets.size(); i++)
        drawer->drawSprite(bullets[i]->pos, 7);
    particleSys->render();
    drawer->drawSprite(player->getPos(), 0);
    
    drawer->frame();
}

void Game::spawn() {
    int num = intensity;
    if (num > MAX_FOES - things.size())
        num = MAX_FOES - things.size();

    for(int i = 0; i < num; i++) {
        transform pos;
        do {
            pos.pos = RNG::ToVector(GRID_SIZE*GRID_UNIT_SCALE);
        } while ((pos.pos - player->getPos().pos).magnitude() < MIN_DIST_FROM_PLAYER);

        int idx = RNG::Int(0, 100);
        if (idx <= 50)
            things.push_back(new pinwheel(pos, this));
        else if (idx <= 75)
            things.push_back(new diamond(pos, this));
        else if (idx <= 94)
            things.push_back(new splitSquare(pos, this));
        else if (idx <= 98)
            things.push_back(new greenSquare(pos, this));
        else if (idx <= 100)
            things.push_back(new bubble(pos, this));
    }
}

void Game::instantiate(foe* creation) {
    if (MAX_FOES > things.size())
        things.push_back(creation);
    else 
        destroy(creation);
}

void Game::shoot(Vector2 pos, Vector2 speed) {
    bullet* projectile = new bullet();
    projectile->pos.pos = pos;
    projectile->pos.scale = BULLET_SIZE;
    projectile->speed = speed;
    projectile->pos.theta = std::atan2(speed.y, speed.x) * 180 / M_PI + 90;
    bullets.push_back(projectile);
}

bullet* Game::getNearestBullet(Vector2 pos, float radiusSq) {
    bullet* closest = NULL;
    for (int i = 0; i < bullets.size(); i++) {
        float dist = (bullets[i]->pos.pos - pos).sq_magnitude();
        if (dist < radiusSq) {
            radiusSq = dist;
            closest = bullets[i];
        }
    }
    return closest;
}

void Game::manageWaves() {
    timeTillWave -= deltaTime;
    if (timeTillWave <= 0) {
        timeTillWave += TIME_PER_WAVE;
        //int choice = randomRange(0, 2);
        spawn();

        currentWaveID += 1;
        if(currentWaveID % 5 == 0)
            intensity += 1;
    }
}

void Game::destroy(int victimID, bool byClearing) {
    foe* thing = things[victimID];
    thing->death(byClearing);
    things.erase(things.begin() + victimID);
    delete thing;
}

void Game::destroy(foe* victim, bool byClearing) {
    victim->death(byClearing);
    delete victim;
}

void Game::collision() {
    int topX, topY, bottomX, bottomY;
    topX = player->getPos().pos.x + ((float)HIDDEN_PLAYER_SCALE/2);
    topY = player->getPos().pos.y + ((float)HIDDEN_PLAYER_SCALE/2);
    bottomX = player->getPos().pos.x - ((float)HIDDEN_PLAYER_SCALE/2);
    bottomY = player->getPos().pos.y - ((float)HIDDEN_PLAYER_SCALE/2);

    for(int i = things.size() - 1; i >= 0; i--) {
        int _topX = things[i]->getPos().pos.x + ((float)things[i]->getPos().scale.x/2);
        int _topY = things[i]->getPos().pos.y + ((float)things[i]->getPos().scale.y/2);
        int _bottomX = things[i]->getPos().pos.x - ((float)things[i]->getPos().scale.x/2);
        int _bottomY = things[i]->getPos().pos.y - ((float)things[i]->getPos().scale.y/2);
        bool skip = false;
        for (int j = bullets.size() - 1; j >= 0; j--) {
            int topX = bullets[j]->pos.pos.x + ((float)BULLET_BOX/2);
            int topY = bullets[j]->pos.pos.y + ((float)BULLET_BOX/2);
            int bottomX = bullets[j]->pos.pos.x - ((float)BULLET_BOX/2);
            int bottomY = bullets[j]->pos.pos.y - ((float)BULLET_BOX/2);

            if( !(_topX<bottomX||topX<_bottomX) &&
                !(_topY<bottomY||topY<_bottomY)  ) {

                destroy(i); 

                bullet* bt = bullets[j];
                bullets.erase(bullets.begin() + j);
                particleSys->bullet(bt->pos.pos, bt->pos.theta);
                delete bt;


                skip = true;
                break;
            }
        }
        if (skip) continue;
        if( !(_topX<bottomX||topX<_bottomX) &&
            !(_topY<bottomY||topY<_bottomY)  ) {
            destroy(i);
            player->kill();
            continue;
        }
    }
}

void Game::moveObjects() {
    if (MULTI_THREADED) {
        int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

        // Split the work across available threads
        size_t chunkSize = things.size() / numThreads;
        for (int t = 0; t < numThreads; t++) {
            threads.push_back(std::thread([&, t]() {
                size_t start = t * chunkSize;
                size_t end = (t == numThreads - 1) ? things.size() : start + chunkSize;
                for (size_t i = start; i < end; ++i) {
                    things[i]->move();
                }
            }));
        }

        // Join all threads
        for (auto& th : threads) {
            th.join();
        }
    }
    else {
        for (int i = things.size() - 1; i >= 0; i--)
            things[i]->move();
    }

    for(int i = bullets.size() - 1; i >= 0; i--) {
        bullets[i]->pos.pos += bullets[i]->speed * deltaTime;
        if (bullets[i]->pos.pos.x > GRID_SIZE.x*GRID_UNIT_SCALE - BULLET_BOX/2 || bullets[i]->pos.pos.x <= BULLET_BOX/2 ||
            bullets[i]->pos.pos.y > GRID_SIZE.y*GRID_UNIT_SCALE - BULLET_BOX/2 || bullets[i]->pos.pos.y <= BULLET_BOX/2) {
            bullet* bt = bullets[i];
            bullets.erase(bullets.begin() + i);
            particleSys->bullet(bt->pos.pos, bt->pos.theta);
            delete bt;
        }
    }
    particleSys->move();
    player->move();
}

void Game::loopItteration() {
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    float timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastMeasure).count();
    lastMeasure = now;
    deltaTime = timeElapsed / 1000000;
    
    playtime += deltaTime;

    if (SHOW_FPS) {
        fpsSum += deltaTime;
        framesElapsed += 1;
        if (fpsSum >= 1) {
            std::cout << "FPS: " << framesElapsed << std::endl;
            fpsSum -= 1;
            framesElapsed = 0;
        }
    }

    diamondDeltaSpeedConv = deltaTime * DIAMOND_SPEED * DIAMOND_CONVERGANCE;
    bubbleDeltaSpeedConv = deltaTime * BUBBLE_SPEED * BUBBLE_CONVERGANCE;
    splitSquareDeltaSpeedConv = deltaTime * SPLIT_SQUARE_SPEED * SPLIT_SQUARE_CONVERGANCE;
    subSquareDeltaSpeedConv = deltaTime * SUB_SQUARE_SPEED * SUB_SQUARE_CONVERGANCE;
    greenSquareDeltaSpeedConv = deltaTime * GREEN_SQUARE_SPEED * GREEN_SQUARE_CONVERGANCE;

    if (!lost)
        manageWaves();
    moveObjects();
    collision();
    if(player->isDead()) {
        for (int i = things.size() - 1; i >= 0; i--)
            destroy(i, true);
        audio->pauseSong();
        lostTime += deltaTime;
        if(lostTime >= 2)
            newGame();
    }

    audio->checkRestart();
    requestRender();
}

Game game;

#ifdef __EMSCRIPTEN__
static void mainloop() {
    if (game.quitting)
        emscripten_cancel_main_loop();
    game.loopItteration();
}
#endif

int main(int argc, char* argv[]) {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
#else
    while(!game.quitting) {
        if (LAG_MS > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(LAG_MS));
        game.loopItteration();
    }
#endif

    return 0;
}
