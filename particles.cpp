#include "particles.h"
#include "CONST.h"
#include "sprites.h"
#include "game.h"
#include "utils.h"
#include <cmath>
#include <deque>
#include <iostream>
#include <utility>

inline void pop(std::deque<particle*>* target) {
    particle* p = target->back();
    target->pop_back();
    delete p;
}

ParticleSystem::ParticleSystem(ScreenDrawer* drawer, Game* master) : drawer(drawer), master(master) {
    std::cout << "ParticleSystem has been created" << std::endl;
}

ParticleSystem::~ParticleSystem() {
    for (auto sys : particles)
        for(int i = sys.second.size() - 1; i >= 0; i--)
            pop(&sys.second);
    std::cout << "ParticleSystem has been destroyed" << std::endl;
}

void ParticleSystem::move() {
    float dt = master->deltaTime;
    for (auto system : particles) {
        auto key = system.first;
        for(int i = 0; i < particles[key].size(); i++) {
            particles[key][i]->pos += particles[key][i]->speed * dt; 
            particles[key][i]->life -= dt;
        }
        for(int i = 0; i < particles[key].size(); i++) {
            if (particles[key].back()->life <= 0) {
                pop(&particles[key]);
            }
            else { break; }
        }

    }
}

void ParticleSystem::render() {
    for (auto sys : particles)
        for(int i = 0; i < sys.second.size(); i++)
            drawer->drawColoredSprite({sys.second[i]->pos, Vector2(4,14), sys.second[i]->theta}, 
                8, sys.first.first, 255);
}

void ParticleSystem::death(Vector2 pos, int spriteIndex) {
    int num_particles = RNG::Int(PARTICLE_DEATH_COUNT_MIN, PARTICLE_DEATH_COUNT_MAX);
    auto key = std::make_pair(drawer->getPallet(spriteIndex), PARTICLE_DEATH_LIFETIME);
    for(int i = 0; i < num_particles; i++) {
        float theta = RNG::Float(0, 360);
        float thetaRad = theta * M_PI / 180;
        float speedMult = RNG::Float(PARTICLE_DEATH_SPEED_MIN, PARTICLE_DEATH_SPEED_MAX);
        Vector2 speed = Vector2(std::cos(thetaRad), std::sin(thetaRad));
        particle* p = new particle(pos + speed, speed * speedMult, theta + 90, PARTICLE_DEATH_LIFETIME);
        particles[key].push_front(p);
    }
}

void ParticleSystem::spawn(Vector2 pos, int spriteIndex) {
    int num_particles = 360/RNG::Int(PARTICLE_SPAWN_COUNT_MIN, PARTICLE_SPAWN_COUNT_MAX);
    bool flip = RNG::Int(0, 1);
    float speedMult = RNG::Float(PARTICLE_SPAWN_SPEED_MIN, PARTICLE_SPAWN_SPEED_MAX) * (flip ? 1 : -1);
    auto key = std::make_pair(drawer->getPallet(spriteIndex), PARTICLE_SPAWN_LIFETIME);
    for(int i = 0; i < 360; i+= num_particles) {
        float thetaRad = i * M_PI / 180;
        Vector2 speed = Vector2(std::cos(thetaRad + 90), std::sin(thetaRad + 90));
        particle* p = new particle(pos + speed*10, speed * speedMult, i + 90, PARTICLE_SPAWN_LIFETIME);
        particles[key].push_front(p);
    }
}

void ParticleSystem::bullet(Vector2 pos, float theta) {
    int num_particles = RNG::Int(PARTICLE_BULLET_COUNT_MIN, PARTICLE_BULLET_COUNT_MAX);
    auto key = std::make_pair(0, PARTICLE_BULLET_LIFETIME);
    for(int i = 0; i < num_particles; i++) {
        float thetaDeg = RNG::Float(theta - 180*PARTICLE_BULLET_ARC, theta + 180*PARTICLE_BULLET_ARC) + 90;
        float thetaRad = thetaDeg * M_PI / 180;
        float speedMult = RNG::Float(PARTICLE_BULLET_SPEED_MIN, PARTICLE_BULLET_SPEED_MAX);
        Vector2 speed = Vector2(std::cos(thetaRad), std::sin(thetaRad));
        particle* p = new particle(pos + speed, speed * speedMult, thetaDeg + 90, PARTICLE_BULLET_LIFETIME);
        particles[key].push_front(p);
    }
}
