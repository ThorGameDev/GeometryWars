#ifndef FOE_DATA
#define FOE_DATA
#include "utils.h"
#include <chrono>

class Game;

class foe {
public:
    foe(transform pos, int spriteID, Game* master) : pos(pos), spriteID(spriteID), master(master) {};
    virtual ~foe() = default;
    virtual void move() = 0;
    virtual void death(bool byClearing) = 0;
    transform getPos() { return pos; };
    int getSpriteID() { return spriteID; }; 
protected:
    Game* master;
    transform pos;
    int spriteID;
};

class pinwheel : public foe {
    float speedX;
    float speedY;
public:
    pinwheel(transform pos, Game* master);
    void move() override;
    void death(bool byClearing) override;
};

class diamond : public foe {
    float speedX;
    float speedY;
    float birthday;
public:
    diamond(transform pos, Game* master);
    void move() override;
    void death(bool byClearing) override;
};

class bubble : public foe {
    float speedX;
    float speedY;
public:
    bubble(transform pos, Game* master);
    void move() override;
    void death(bool byClearing) override;
};
#endif
