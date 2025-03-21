#ifndef FOE_DATA
#define FOE_DATA
#include "utils.h"

class Game;

class foe {
public:
    foe(transform pos, int spriteID, Game* master) : pos(pos), spriteID(spriteID), master(master) {};
    virtual ~foe() = default;
    virtual void move() = 0;
    transform getPos() { return pos; };
    int getSpriteID() { return spriteID; }; 
protected:
    Game* master;
    transform pos;
    int spriteID;
};

class pinwheel : public foe {
    double speedX;
    double speedY;
public:
    pinwheel(transform pos, Game* master);
    void move() override;
};

class diamond : public foe {
    double speedX;
    double speedY;
public:
    diamond(transform pos, Game* master);
    void move() override;
};

class bubble : public foe {
    double speedX;
    double speedY;
public:
    bubble(transform pos, Game* master);
    void move() override;
};
#endif
