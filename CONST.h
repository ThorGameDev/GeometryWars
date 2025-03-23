#ifndef CONSTANT_DEFINITIONS
#define CONSTANT_DEFINITIONS

//Screen
const int SCREEN_X = 980;
const int SCREEN_Y = 640;

//foes
const int PINWHEEL_SCALE = 30;
const float PINWHEEL_SPEED = 120;

const int DIAMOND_SCALE = 30;
const int DIAMOND_SMALL_SCALE = 20;
const float DIAMOND_SPEED = 180;
const float DIAMOND_SQUISH_SPEED = 5;
const float DIAMOND_CONVERGANCE = 3.5;

const int BUBBLE_SCALE = 15;
const float BUBBLE_SPEED = 280;
const float BUBBLE_CONVERGANCE = 2.8;

const int MAX_FOES = 50000;
const int MIN_DIST_FROM_PLAYER = 300;

//Player
const int PLAYERSCALE = 40;
const int HIDDENPLAYERSCALE = 25;
const int PLAYERSPEED = 250;

const float TIME_PER_BULLET = 0.3;
const float BULLET_SPEED = 1000;
const float BULLET_SIZE = 2;
const float BULLET_PAIR_DIST = 3;

//Wave Management
const int TIMEPERWAVE = 2;

//Effects
const float FORCE = 200;
const float PHASETIME = 0.04;
const int PHASES = 2;

//Testing
const bool MUSIC = false;
const bool PLAYER_IMORTAL = true;
const bool SHOW_FPS = true;
const bool MAXIMUM_INITIAL_INTENSITY = false;

#endif
