#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include "utils.h"
#include <deque>
#include <map>
#include <queue>

struct particle {
    Vector2 pos;
    Vector2 speed;
    float theta;
    float life;
    particle(Vector2 pos, Vector2 speed, float theta, float lifetime) : 
        pos(pos), speed(speed), theta(theta), life(lifetime) {};
};
class ScreenDrawer;
class Game;

class ParticleSystem {
    /// {<index, lifetime>, particles}
    std::map<std::pair<int, float>, std::deque<particle*>> particles;
    ScreenDrawer* drawer;
    Game* master;
public:
    ParticleSystem(ScreenDrawer* drawer, Game* master);
    ~ParticleSystem();
    void move();
    void render();
    void death(Vector2 pos, int spriteIndex);
    void spawn(Vector2 pos, int spriteIndex);
    void bullet(Vector2 pos, float theta);
};

#endif
