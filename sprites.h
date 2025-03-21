#ifndef SPRITES
#define SPRITES

#include <vector>
class SDL_Renderer;
class SDL_Window;
class GameSprite;
class transform;

class ScreenDrawer {
    SDL_Renderer* renderer;
    SDL_Window* window;
    std::vector<GameSprite*> sprites;
public:
    ScreenDrawer();
    ~ScreenDrawer();
    void drawSquare(float x, float y, float scaleX, float scaleY, int r, int g, int b);
    void drawSprite(transform pos, int index);
    void frame();
};

#endif
