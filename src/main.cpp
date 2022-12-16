#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "engine.h"
#include "game.h"
#include "collision.h"
#include <iostream>
#include <string>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSizeX, screenSizeY, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	TTF_Font* font = TTF_OpenFont("res/roboto.ttf", 20);

	float projectileTimer = 0;
	bool running = true;
	Uint64 previous_ticks = SDL_GetPerformanceCounter();

	//player start spawn
	AABB aabb = AABB(0, 0, 100, 100);
	CollisionData cData = CollisionData(0, &aabb);
	EntityCollisionDatas.push_back(cData);

	PositionData pData = PositionData(0, screenSizeX/2, 600);
	EntityPositionDatas.push_back(pData);

	//projectile pool init
	for (size_t i = 0; i < projectilePoolSize; i++)
	{
		AABB aabb = AABB(0, 0, 10, 10);
		CollisionData cData = CollisionData(i, &aabb);
		ProjectileCollisionDatas.push_back(cData);

		PositionData pData = PositionData(i, 5000, 5000);
		ProjectilesPositionDatas.push_back(pData);
		ProjectileInUse.push_back(false);
	}

	//enemy start spawn
	for (size_t i = 1; i < 10; i++)
	{
		AABB aabb = AABB(0, 0, 100, 100);
		CollisionData cData = CollisionData(i, &aabb);
		EntityCollisionDatas.push_back(cData);

		PositionData pData = PositionData(i, (i) * 120, 100);
		EntityPositionDatas.push_back(pData);
	}

	while (running)
	{
		Uint64 ticks = SDL_GetPerformanceCounter();
		Uint64 delta_ticks = ticks - previous_ticks;
		previous_ticks = ticks;
		delta_time = (float)delta_ticks / SDL_GetPerformanceFrequency();

		SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
		SDL_RenderClear(renderer);

		frameNumber++;
		
		//Input
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
			{
				int scancode = event.key.keysym.scancode;
				if (scancode == SDL_SCANCODE_ESCAPE)
					running = false;

				keys[scancode].state = true;
				keys[scancode].changeFrame = frameNumber;
				break;
			}

			case SDL_KEYUP:
			{
				int scancode = event.key.keysym.scancode;
				keys[scancode].state = false;
				keys[scancode].changeFrame = frameNumber;
				break;
			}

			default:
				break;
			}
		}

		//update player
		PositionData* p = &EntityPositionDatas[0];
		
		//update collision to match positions
		CollisionData* playerAABB = &ProjectileCollisionDatas[0];
		*playerAABB->aabb = AABB(p->xPos, p->yPos, p->xPos + 100, p->yPos + 100);
		
		if(health > 0)
			UpdatePlayer(*p);
		
		draw(p->xPos, p->yPos, 100, 100);

		if (GetKeyDown(SDL_SCANCODE_SPACE))
		{
			for (size_t i = 0; i < 5; i++)
			{
				if (!ProjectileInUse[i])
				{
					PositionData* projectileP = &ProjectilesPositionDatas[i];
					projectileP->xPos = p->xPos + 50;
					projectileP->yPos = p->yPos + 50;
					ProjectileInUse[i] = true;
					break;
				}
			}
		}

		for (size_t i = 0; i < 5; i++)
		{
			if (!ProjectileInUse[i])
				continue;

			PositionData* projectileP = &ProjectilesPositionDatas[i];
			draw(projectileP->xPos, projectileP->yPos, 10, 10);

			AABB projectileAABB = *ProjectileCollisionDatas[i].aabb;
			projectileAABB.xMin = projectileP->xPos;
			projectileAABB.yMin = projectileP->yPos;
			projectileAABB.xMax = projectileP->xPos + 10;
			projectileAABB.yMax = projectileP->yPos + 10;

			//player bullet collision check
			for (size_t j = 1; j < EntityPositionDatas.size(); j++)
			{
				PositionData* enemyEntityP = &EntityPositionDatas[j];

				AABB enemyEntityAABB = *EntityCollisionDatas[j].aabb;
				enemyEntityAABB.xMin = enemyEntityP->xPos;
				enemyEntityAABB.yMin = enemyEntityP->yPos;
				enemyEntityAABB.xMax = enemyEntityP->xPos + 100;
				enemyEntityAABB.yMax = enemyEntityP->yPos + 100;

				if (AABBIntersect(projectileAABB, enemyEntityAABB))
				{
					cout << "collided with enemy" << j << endl;
					score++;
					enemyEntityP->xPos = 5000;
					enemyEntityP->yPos = 5000;
					projectileP->xPos = 5000;
					projectileP->yPos = 5000;
					ProjectileInUse[i] = false;
				}
			}

			//if bullet collides or leaves screen return to pool
			if (!calcstep(*projectileP, 0, -300 * delta_time))
			{
				projectileP->xPos = 5000;
				projectileP->yPos = 5000;
				ProjectileInUse[i] = false;
			}
		}

		//update entinties
		for (size_t i = 1; i < EntityPositionDatas.size(); i++)
		{
			PositionData* p = &EntityPositionDatas[i];
			draw(p->xPos, p->yPos, 100, 100);

			p->xPos += sin(frameNumber * delta_time) * 2;
			p->yPos += delta_time * 2;
			
			if (projectileTimer > 3.0f)
			{
				for (size_t j = 5; j < projectilePoolSize; j++)
				{
					if (!ProjectileInUse[j])
					{
						PositionData* projectileP = &ProjectilesPositionDatas[j];
						projectileP->xPos = p->xPos + 50;
						projectileP->yPos = p->yPos + 50;
						ProjectileInUse[j] = true;
						break;
					}			
				}
			}
		}

		//update projectiles
		for (size_t i = 5; i < projectilePoolSize; i++)
		{
			if (!ProjectileInUse[i])
				continue;

			PositionData* projectileP = &ProjectilesPositionDatas[i];
			draw(projectileP->xPos, projectileP->yPos, 10, 10);
			
			//update collision to match positions
			AABB projectileAABB = *ProjectileCollisionDatas[i].aabb;
			projectileAABB.xMin = projectileP->xPos;
			projectileAABB.yMin = projectileP->yPos;
			projectileAABB.xMax = projectileP->xPos + 10;
			projectileAABB.yMax = projectileP->yPos + 10;

			//check collision
			if (AABBIntersect(projectileAABB, *playerAABB->aabb))
			{
				//cout << "collided with bullet" << i << endl;
				health--;
				projectileP->xPos = 5000;
				projectileP->yPos = 5000;
				ProjectileInUse[i] = false;
			}

			//if bullet collides or leaves screen return to pool
			if (!calcstep(*projectileP, 0, 300 * delta_time))
			{
				projectileP->xPos = 5000;
				projectileP->yPos = 5000;
				ProjectileInUse[i] = false;
			}
		}

		if (projectileTimer < 3.0f)
		{
			projectileTimer += delta_time;
		}
		else
		{
			projectileTimer = 0;
		}

		if (health < 1)
		{
			p->xPos = 7000;
			p->yPos = 7000;
		}

		string scoreString = "Score: " + to_string(score);
		const char* scoreText = scoreString.c_str();

		string healthString = "Health: " + to_string(health);
		const char* healthText = healthString.c_str();

		SDL_Color textColor = { 255, 255, 255, 255 };

		RenderText(font, scoreText, textColor, 20, 20);
		RenderText(font, healthText, textColor, 20, 50);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}
}