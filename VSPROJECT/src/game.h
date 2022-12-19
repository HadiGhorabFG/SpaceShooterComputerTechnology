#pragma once
#include <SDL/SDL.h>
#include "engine.h"
#include <vector>
#include "collision.h"
using namespace std;

extern int projectilePoolSize;
extern int screenSizeX;
extern int screenSizeY;
extern int score;
extern int health;

extern int playerVelocityX;
extern int playerVelocityY;

struct PositionData
{
	PositionData(int i, float posX, float posY)
	{
		index = i;
		xPos = posX;
		yPos = posY;
	}

	int index;

	float xPos;
	float yPos;
};

extern vector<CollisionData> EntityCollisionDatas;
extern vector<PositionData> EntityPositionDatas;
extern vector<CollisionData> ProjectileCollisionDatas;
extern vector<PositionData> ProjectilesPositionDatas;

extern vector<bool> ProjectileInUse;

void UpdatePlayer(PositionData& p);


