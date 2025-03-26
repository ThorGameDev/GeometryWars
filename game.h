#ifndef GAME
#define GAME

#include <chrono>
#include <deque>
#include "utils.h"
#include "CONST.h"
#include "sprites.h"
#include "AudioPlayer.h"

class foe;
class Player;

class Game
{
    public:
        Game();
        ~Game();
        void loopItteration();
        bool quitting = false;
        bool lost = false;
        float deltaTime;
        transform getPlayerPos();
        bullet* getNearestBullet(float posX, float posY, float radiusSq);
        void shoot(float posX, float posY, float speedX, float speedY);
        void instantiate(foe* creation);
        
        float playtime = 0;

        // Optimizations
        float diamondDeltaSpeedConv;
        float bubbleDeltaSpeedConv;
        float splitSquareDeltaSpeedConv;
        float subSquareDeltaSpeedConv;
        float greenSquareDeltaSpeedConv;

        // Diagnostics
        float fpsSum;
        int framesElapsed;


    private:
        //Timing
        std::chrono::time_point<std::chrono::steady_clock> lastMeasure;

        //Game Logic
        void newGame();
        void manageWaves();
        void moveObjects();
        void collision();
        void requestRender();
        void showScore();
        void destroy(int victimID, bool byClearing = false);
        void destroy(foe* victim, bool byClearing = false);

        //Attacks
        void spawn();

        //Necessary Variables
        std::deque<foe*> things;
        std::deque<bullet*> bullets;
        float lostTime;

        //Wave Manager
        float timeTillWave = TIMEPERWAVE;
        int currentWaveID = 0;
        int intensity;
        
        //Player
        Player* player;

        //Sound
        AudioPlayer audio;
        
        //Images
        ScreenDrawer drawer;
};

#endif
