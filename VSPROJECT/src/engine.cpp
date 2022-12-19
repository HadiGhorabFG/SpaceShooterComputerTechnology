#include "engine.h"

SDL_Window* window;
SDL_Renderer* renderer;

int frameNumber = 0;
float delta_time = 0.0f;

KeyStates keys[SDL_NUM_SCANCODES];

bool GetKey(SDL_Scancode key)
{
	return keys[key].state;
}

bool GetKeyDown(SDL_Scancode key)
{
	return keys[key].state && keys[key].changeFrame == frameNumber;
}

bool GetKeyUp(SDL_Scancode key)
{
	return !keys[key].state && keys[key].changeFrame == frameNumber;
}

void draw(int posX, int posY, int sizeX, int sizeY)
{
	SDL_SetRenderDrawColor(renderer, 25, 200, 25, 255);
	SDL_Rect rect = { posX, posY, sizeX, sizeY };
	SDL_RenderDrawRect(renderer, &rect);
}


bool calcstep(PositionData& p, float dx, float dy)
{
	if (calcsweep(p, dx, dy))
		return false;

	p.xPos += dx;
	p.yPos += dy;
	return true;
}

bool calcsweep(PositionData& p, float dx, float dy)
{
	if (p.xPos + dx < 0 || p.xPos + dx > screenSizeX /* - sizeX */ ||
		p.yPos + dy < 0 || p.yPos + dy > screenSizeY)
	{
		return true;
	}

	return false;
}

void RenderText(TTF_Font* font, const char* text, SDL_Color& color, int posX, int posY)
{
	SDL_Surface* text_surf = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);

	SDL_Rect textRect = { posX, posY, text_surf->w, text_surf->h };
	SDL_RenderCopy(renderer, text_texture, NULL, &textRect);

	SDL_FreeSurface(text_surf);
	SDL_DestroyTexture(text_texture);
}