#include "game.h"
#include "engine.h"

int projectilePoolSize = 25;
int screenSizeX = 1280;
int screenSizeY = 720;
int score = 0;
int health = 3;

int playerVelocityX;
int playerVelocityY;

vector<CollisionData> EntityCollisionDatas;
vector<PositionData> EntityPositionDatas;
vector<CollisionData> ProjectileCollisionDatas;
vector<PositionData> ProjectilesPositionDatas;

vector<bool> ProjectileInUse;

void UpdatePlayer(PositionData& p)
{
	if (GetKey(SDL_SCANCODE_D))
	{
		playerVelocityX = 500;
	}

	if (GetKey(SDL_SCANCODE_A))
	{
		playerVelocityX = -500;
	}

	if (GetKeyUp(SDL_SCANCODE_D) || GetKeyUp(SDL_SCANCODE_A))
	{
		playerVelocityX = 0;
	}

	calcstep(p, playerVelocityX * delta_time, 0);
}
