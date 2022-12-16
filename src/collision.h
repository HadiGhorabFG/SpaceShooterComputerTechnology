#pragma once
struct AABB
{
	AABB(float minX, float minY, float maxX, float maxY)
	{
		xMin = minX;
		yMin = minY;
		xMax = maxX;
		yMax = maxY;
	}

	float xMin;
	float xMax;
	float yMin;
	float yMax;
};

struct CollisionData
{
	CollisionData(int i, AABB* collision)
	{
		index = i;
		aabb = collision;
	}

	AABB* aabb;
	int index;
};


bool AABBIntersect(const AABB& a, const AABB& b);