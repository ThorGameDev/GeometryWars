#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include "utils.h"
#include <deque>
#include <queue>

struct particle {
    Vector2 pos;
    Vector2 speed;
    float theta;
    float life;
    int idx;
    particle(Vector2 pos, Vector2 speed, float theta, float lifetime, int idx) : 
        pos(pos), speed(speed), theta(theta), life(lifetime), idx(idx) {};
};
class ScreenDrawer;
class Game;

class ParticleSystem {
    std::deque<particle*> deathParticles;
    std::deque<particle*> spawnParticles;
    std::deque<particle*> bulletParticles;
    ScreenDrawer* drawer;
    Game* master;
public:
    ParticleSystem(ScreenDrawer* drawer, Game* master) : drawer(drawer), master(master) {};
    ~ParticleSystem();
    void move();
    void render();
    void death(Vector2 pos, int spriteIndex);
    void spawn(Vector2 pos, int spriteIndex);
    void bullet(Vector2 pos, float radians);
};

#endif
