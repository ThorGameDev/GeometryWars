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
    Vector2 speed;
public:
    pinwheel(transform pos, Game* master);
    void move() override;
    void death(bool byClearing) override;
};

class diamond : public foe {
    Vector2 speed;
    float birthday;
public:
    diamond(transform pos, Game* master);
    void move() override;
    void death(bool byClearing) override;
};

class bubble : public foe {
    Vector2 speed;
public:
    bubble(transform pos, Game* master);
    void move() override;
    void death(bool byClearing) override;
};

class splitSquare : public foe {
    Vector2 speed;
public:
    splitSquare(transform pos, Game* master); 
    void move() override;
    void death(bool byClearing) override;
};

class subSquare : public foe {
    Vector2 speed;
public:
    subSquare(transform pos, Game* master, Vector2 speed);
    void move() override;
    void death(bool byClearing) override;
};

class greenSquare : public foe {
    Vector2 speed;
    void chase();
    void dodge(bullet* bullet);
public:
    greenSquare(transform pos, Game* master);
    void move() override;
    void death(bool byClearing) override;
};
#endif
