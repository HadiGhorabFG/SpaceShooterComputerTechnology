#include "collision.h"
#include "engine.h"

bool AABBIntersect(const AABB& a, const AABB& b)
{
	return a.xMin < b.xMax&& b.xMin < a.xMax&& a.yMin < b.yMax&& b.yMin < a.yMax;
}