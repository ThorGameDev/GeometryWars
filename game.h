#ifndef GAME
#define GAME

#include <SDL3/SDL_events.h>
#include <chrono>
#include <deque>
#include "utils.h"
#include "CONST.h"
#include "sprites.h"
#include "AudioPlayer.h"

class foe;

struct Player
{
    transform pos;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};

class Game
{
    public:
        Game();
        ~Game();
        void input();
        void loopItteration();
        transform getPlayerPos();
        bool quitting = false;
        bool lost = false;
        double deltaTime;
    private:
        //Timing
        std::chrono::time_point<std::chrono::steady_clock> lastMeasure;

        //Game Logic
        void newGame();
        void manageWaves();
        void moveObjects();
        bool checkPlayerDeath();
        void requestRender();
        void showScore();

        //Attacks
        void arroundAttack();

        //Necessary Variables
        std::deque<foe*> things;
        SDL_Event Event;
        bool firstFrame;
        double lostTime;

        //Wave Manager
        double timeTillWave = TIMEPERWAVE;
        int currentWaveID = 0;
        int intensity;
        
        //Player
        Player player;

        //Sound
        AudioPlayer audio;
        
        //Images
        ScreenDrawer drawer;
};

#endif
