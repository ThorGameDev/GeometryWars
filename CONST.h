#ifndef CONSTANT_DEFINITIONS
#define CONSTANT_DEFINITIONS

//Screen
const int SCREEN_X = 980;
const int SCREEN_Y = 640;

//foes
const int PINWHEEL_SCALE = 30;
const float PINWHEEL_SPEED = 120;
const float PINWHEEL_ROTATION_SPEED = 360;

const int DIAMOND_SCALE = 30;
const int DIAMOND_SMALL_SCALE = 20;
const float DIAMOND_SPEED = 160;
const float DIAMOND_SQUISH_SPEED = 5;
const float DIAMOND_CONVERGANCE = 3.5;

const int BUBBLE_SCALE = 15;
const float BUBBLE_SPEED = 290;
const float BUBBLE_CONVERGANCE = 2.8;

const int SPLIT_SQUARE_SCALE = 30;
const float SPLIT_SQUARE_SPEED = 150;
const float SPLIT_SQUARE_CONVERGANCE = 3.5;

const int SUB_SQUARE_SCALE = 17;
const float SUB_SQUARE_SPEED = 110;
const float SUB_SQUARE_CONVERGANCE = 3.5;

const int GREEN_SQUARE_SCALE = 30;
const float GREEN_SQUARE_SPEED = 70;
const float GREEN_SQUARE_CONVERGANCE = 4.0;
const float GREEN_SQUARE_ROTATION_SPEED = 80;

const float GREEN_SQUARE_PANIC_SQDISTANCE = 5500;
const float GREEN_SQUARE_PANIC_SPEED = 400;
const float GREEN_SQUARE_PANIC_ROTATION_SPEED = -400;

const int MAX_FOES = 1000;
const int MIN_DIST_FROM_PLAYER = 300;

//Player
const int PLAYERSCALE = 40;
const int HIDDENPLAYERSCALE = 20;
const int PLAYERSPEED = 250;

const float TIME_PER_BULLET = 0.08;
const float BULLET_SPEED = 900;
const float BULLET_SIZE = 3;
const float BULLET_PAIR_DIST = 3;

const float BULLET_SUB_STREAMS = 1;
const float BULLET_STREAM_ANGLE_VARIANCE = 0.1;
const float BULLET_STREAM_SPEED_VARIANCE = 50;

//Wave Management
const int TIMEPERWAVE = 2;

//Effects
const float BULLET_SHAKE = 0.3;
const int BULLET_SHAKE_MS = 40;

const float DEATH_SHAKE = 1;
const int DEATH_SHAKE_MS = 200;

//Testing
const bool MUSIC = false;
const bool PLAYER_IMORTAL = false;
const bool SHOW_FPS = false;
const bool MAXIMUM_INITIAL_INTENSITY = false;

#endif
