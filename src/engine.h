#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "game.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

struct PositionData;

struct KeyStates
{
	bool state = false;
	int changeFrame = 0;
};

bool GetKey(SDL_Scancode key);
bool GetKeyDown(SDL_Scancode key);
bool GetKeyUp(SDL_Scancode key);

extern KeyStates keys[SDL_NUM_SCANCODES];

extern int frameNumber;
extern float delta_time;

void draw(int posX, int posY, int sizeX, int sizeY);
bool calcstep(PositionData& p, float dx, float dy);
bool calcsweep(PositionData& p, float dx, float dy);

void RenderText(TTF_Font* font, const char* text, SDL_Color& color, int posX, int posY);