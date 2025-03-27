#ifndef SPRITES
#define SPRITES

#include <vector>
class SDL_Renderer;
class SDL_Window;
class GameSprite;
class transform;
class Game;

struct camera {
    float x;
    float y;
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
    void drawSquare(float x, float y, float scaleX, float scaleY, int r, int g, int b);
    void drawSprite(transform pos, int index);
    void frame();
    int getGridOffset() { return gridOffset; };
};

#endif
