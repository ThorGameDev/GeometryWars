#ifndef CONSTANT_DEFINITIONS
#define CONSTANT_DEFINITIONS

#include "utils.h"

//Camera
const Vector2 SCREEN_SIZE = {980, 640};
const float CAMERA_INTERP_SPEED = 3;

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

const int MAX_FOES = 5000;
const int MIN_DIST_FROM_PLAYER = 300;

//Player
const int PLAYER_SCALE = 40;
const int HIDDEN_PLAYER_SCALE = 20;
const int PLAYER_SPEED = 250;
const int PLAYER_ROTATE_SPEED = 4;

const float TIME_PER_BULLET = 0.08;
const float BULLET_SPEED = 900;
const Vector2 BULLET_SIZE = Vector2(7,14);
const int BULLET_BOX = 12;
const float BULLET_PAIR_DIST = 3;

const float BULLET_SUB_STREAMS = 1;
const float BULLET_STREAM_ANGLE_VARIANCE = 0.25;
const float BULLET_STREAM_SPEED_VARIANCE = 50;

//Wave Management
const int TIME_PER_WAVE = 2;

//Effects
const float BULLET_SHAKE = 0.3;
const int BULLET_SHAKE_MS = 40;

const float DEATH_SHAKE = 1;
const int DEATH_SHAKE_MS = 200;

//Particle System
const float PARTICLE_DEATH_LIFETIME = 0.8;
const float PARTICLE_DEATH_SPEED_MIN = 200;
const float PARTICLE_DEATH_SPEED_MAX = 450;
const int PARTICLE_DEATH_COUNT_MIN = 15;
const int PARTICLE_DEATH_COUNT_MAX = 25;

const float PARTICLE_BULLET_LIFETIME = 0.1;
const float PARTICLE_BULLET_SPEED_MIN = 250;
const float PARTICLE_BULLET_SPEED_MAX = 450;
const float PARTICLE_BULLET_ARC = 0.05;
const int PARTICLE_BULLET_COUNT_MIN = 0;
const int PARTICLE_BULLET_COUNT_MAX = 2;

const float PARTICLE_SPAWN_LIFETIME = 0.7;
const float PARTICLE_SPAWN_SPEED_MIN = 30;
const float PARTICLE_SPAWN_SPEED_MAX = 60;
const int PARTICLE_SPAWN_COUNT_MIN = 3;
const int PARTICLE_SPAWN_COUNT_MAX = 4;

const int PARTICLE_COLOR_R[5] = {255, 148, 80 , 244, 152};
const int PARTICLE_COLOR_G[5] = {197, 210, 181, 143, 255};
const int PARTICLE_COLOR_B[5] = {244, 255, 255, 255, 138};

//Grid
const Vector2 GRID_SIZE = {50, 30};
const int GRID_UNIT_SCALE = 32;

//Testing
const bool MUSIC = false;
const bool MULTI_THREADED = false; // Multi threading is slower for some reason.
const bool PLAYER_IMORTAL = false;
const bool SHOW_FPS = true;
const bool MAXIMUM_INITIAL_INTENSITY = false;
const int LAG_MS = 0;

#endif
