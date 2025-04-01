#include "particles.h"
#include "CONST.h"
#include "sprites.h"
#include "game.h"
#include "utils.h"
#include <cmath>

inline void pop(std::deque<particle*>* target) {
    particle* p = target->back();
    target->pop_back();
    delete p;
}

inline void moveParticles(std::deque<particle*>* target, float dt) {
    for(int i = 0; i < target->size(); i++) {
        (*target)[i]->pos += (*target)[i]->speed * dt; 
        (*target)[i]->life -= dt;
    }
    for(int i = 0; i < target->size(); i++) {
        if (target->back()->life <= 0) 
            pop(target);
        else { break; }
    }
}

ParticleSystem::~ParticleSystem() {
    for(int i = deathParticles.size() - 1; i >= 0; i--)
        pop(&deathParticles);
    for(int i = spawnParticles.size() - 1; i >= 0; i--)
        pop(&spawnParticles);
    for(int i = bulletParticles.size() - 1; i >= 0; i--)
        pop(&bulletParticles);
}

void ParticleSystem::move() {
    float dt = master->deltaTime;
    moveParticles(&deathParticles, dt);
    moveParticles(&spawnParticles, dt);
    moveParticles(&bulletParticles, dt);
}

void ParticleSystem::render() {
    for(int i = 0; i < deathParticles.size(); i++)
        drawer->drawSprite({deathParticles[i]->pos, Vector2(5,18), deathParticles[i]->theta}, deathParticles[i]->idx);
    for(int i = 0; i < spawnParticles.size(); i++)
        drawer->drawSprite({spawnParticles[i]->pos, Vector2(3,10), spawnParticles[i]->theta}, spawnParticles[i]->idx);
    for(int i = 0; i < bulletParticles.size(); i++)
        drawer->drawSprite({bulletParticles[i]->pos, Vector2(4,14), bulletParticles[i]->theta}, bulletParticles[i]->idx);
}

void ParticleSystem::death(Vector2 pos, int spriteIndex) {
    int num_particles = RNG::Int(PARTICLE_DEATH_COUNT_MIN, PARTICLE_DEATH_COUNT_MAX);
    for(int i = 0; i < num_particles; i++) {
        float theta = RNG::Float(0, 360);
        float thetaRad = theta * M_PI / 180;
        float speedMult = RNG::Float(PARTICLE_DEATH_SPEED_MIN, PARTICLE_DEATH_SPEED_MAX);
        Vector2 speed = Vector2(std::cos(thetaRad), std::sin(thetaRad));
        particle* p = new particle(pos + speed, speed * speedMult, theta + 90, PARTICLE_DEATH_LIFETIME, spriteIndex);
        deathParticles.push_front(p);
    }
}

void ParticleSystem::spawn(Vector2 pos, int spriteIndex) {
    int num_particles = 360/RNG::Int(PARTICLE_SPAWN_COUNT_MIN, PARTICLE_SPAWN_COUNT_MAX);
    bool flip = RNG::Int(0, 1);
    float speedMult = RNG::Float(PARTICLE_SPAWN_SPEED_MIN, PARTICLE_SPAWN_SPEED_MAX) * (flip ? 1 : -1);
    for(int i = 0; i < 360; i+= num_particles) {
        float thetaRad = i * M_PI / 180;
        Vector2 speed = Vector2(std::cos(thetaRad + 90), std::sin(thetaRad + 90));
        particle* p = new particle(pos + speed*10, speed * speedMult, i + 180, PARTICLE_SPAWN_LIFETIME, spriteIndex);
        spawnParticles.push_front(p);
    }
}

void ParticleSystem::bullet(Vector2 pos, float radians) {
    int num_particles = RNG::Int(PARTICLE_BULLET_COUNT_MIN, PARTICLE_BULLET_COUNT_MAX);
    for(int i = 0; i < num_particles; i++) {
        float theta = RNG::Float(radians - M_PI*PARTICLE_BULLET_ARC, radians + M_PI*PARTICLE_BULLET_ARC)+M_PI;
        float thetaDeg = theta * 180 / M_PI;
        float speedMult = RNG::Float(PARTICLE_BULLET_SPEED_MIN, PARTICLE_BULLET_SPEED_MAX);
        Vector2 speed = Vector2(std::cos(theta), std::sin(theta));
        particle* p = new particle(pos + speed, speed * speedMult, thetaDeg + 90, PARTICLE_DEATH_LIFETIME, 13);
        bulletParticles.push_front(p);
    }
}
