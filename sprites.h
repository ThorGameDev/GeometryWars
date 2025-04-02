#ifndef SPRITES
#define SPRITES

#include "utils.h"
#include <vector>
class SDL_Renderer;
class SDL_Window;
class GameSprite;
class transform;
class Game;

struct camera {
    Vector2 pos;
    Game* master;
};

class ScreenDrawer {
    SDL_Renderer* renderer;
    SDL_Window* window;
    std::vector<GameSprite*> sprites;
    int gridOffset;
    camera cam;
public:
    ScreenDrawer(Game* master);
    ~ScreenDrawer();
    void drawSprite(transform pos, int index);
    void drawColoredSprite(transform pos, int index, float r, float g, float b, float a);
    void frame();
    void init();
    int getGridOffset() { return gridOffset; };
};

#endif
