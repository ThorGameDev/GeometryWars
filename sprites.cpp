#include "CONST.h"
#include "utils.h"
#include "sprites.h"
#include "game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

struct GameSprite {
    SDL_Texture* texture;    
    Vector2 dimensions;
};

static GameSprite* LoadSprite(const char* path, SDL_Renderer* renderer) {
    GameSprite* sprite = new GameSprite();
    char *full_path = NULL;

    /* Load the .png files from wherever the app is being run from. */
    SDL_asprintf(&full_path, "%s%s", SDL_GetBasePath(), path);
    std::cout << "Loading " << full_path << std::endl;
    SDL_Surface* surface = IMG_Load(full_path); 
    sprite->texture = SDL_CreateTextureFromSurface(renderer, surface); 

    sprite->dimensions = Vector2(surface->w, surface->h);
    SDL_DestroySurface(surface);
    SDL_free(full_path);

    return sprite;
}

void ScreenDrawer::init() {
    cam.pos = GRID_SIZE * GRID_UNIT_SCALE / 2;
}

ScreenDrawer::ScreenDrawer(Game* master) {
    SDL_Init(SDL_INIT_VIDEO );
    if (!SDL_CreateWindowAndRenderer("RescizableRectangles", SCREEN_SIZE.x, SCREEN_SIZE.y, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_SIZE.x, SCREEN_SIZE.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    sprites.push_back(LoadSprite("resources/player.png", renderer));             // 0
    sprites.push_back(LoadSprite("resources/pinwheel.png", renderer));           // 1
    sprites.push_back(LoadSprite("resources/diamond.png", renderer));            // 2
    sprites.push_back(LoadSprite("resources/bubble.png", renderer));             // 3
    sprites.push_back(LoadSprite("resources/splitSquare.png", renderer));        // 4
    sprites.push_back(LoadSprite("resources/subSquare.png", renderer));          // 5
    sprites.push_back(LoadSprite("resources/greenSquare.png", renderer));        // 6
    sprites.push_back(LoadSprite("resources/gradient.png", renderer));           // 7
    gridOffset = sprites.size();
    for(int i = 0; i <= 15; i++){
        char str[26];
        sprintf(str, "resources/tileset/%i.png", i);
        sprites.push_back(LoadSprite(str, renderer));
    }
    std::cout << "ScreenDrawer has been created" << std::endl;

    cam.master = master;
    init();
}

ScreenDrawer::~ScreenDrawer() {
    for (int i = sprites.size() - 1; i > 0; i--) {
        GameSprite* gs = sprites[i];
        sprites.pop_back();
        std::cout << "Destroy a sprite" << std::endl;
        delete gs;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    std::cout << "ScreenDrawer has been destroyed" << std::endl;
}

void ScreenDrawer::drawSprite(transform pos, int index) {
    pos.pos -= cam.pos - (SCREEN_SIZE - pos.scale)/2;
    SDL_FRect rect = {pos.pos.x, pos.pos.y, pos.scale.x, pos.scale.y};
    SDL_RenderTextureRotated(renderer, sprites[index]->texture, NULL, &rect, pos.theta, NULL, SDL_FLIP_NONE);
}

void ScreenDrawer::drawColoredSprite(transform pos, int index, float r, float g, float b, float a) {
    pos.pos -= cam.pos - (SCREEN_SIZE - pos.scale)/2;
    SDL_FRect rect = {pos.pos.x, pos.pos.y, pos.scale.x, pos.scale.y};
    SDL_RenderTextureRotated(renderer, sprites[index]->texture, NULL, &rect, pos.theta, NULL, SDL_FLIP_NONE);
}

void ScreenDrawer::frame() {
    cam.pos += (cam.master->getPlayerPos().pos - cam.pos) * CAMERA_INTERP_SPEED * cam.master->deltaTime;
    SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}
