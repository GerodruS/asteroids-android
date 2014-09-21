#include "ship.h"

#include <stdlib.h>
#include <math.h>

using namespace std;

const int   maxvalueInt = 1000;
const float maxvalueFlt = 100.0f;


Ship::Ship()
{
    points.resize(4 * 2);

    const float width = 50;
    const float height = 60;

    points[0] = -width; points[1] = -height;
    points[2] = 0.0f;   points[3] = height;
    points[4] = width;  points[5] = -height;
    points[6] = 0.0f;   points[7] = 0.0f;

    direction.x = 0.0f;
    direction.y = 1.0f;
}


Ship::~Ship()
{
}


void Ship::move(float x, float y)
{
    for (int i = 0; i < points.size(); i += 2)
    {
        points[i] += x;
        points[i + 1] += y;
    }
}


Point Ship::getCenter()
{
    int count = points.size();
    Point p;
    p.x = points[count - 2];
    p.y = points[count - 1];
    return p;
}


Point Ship::getBulletStartPosition()
{
    Point p;
    p.x = points[2];
    p.y = points[3];
    return p;
}


Point Ship::getBulletStartMove()
{
    Point p;
    p.x = points[2] - points[6];
    p.y = points[3] - points[7];
    PointFunctions::normalize(p, 10.0f);
    return p;
}


void Ship::step()
{
    const float minX = 500.0f - 300.0f;
    const float maxX = 500.0f + 300.0f;
    const float deltaX = maxX - minX;

    const float minY = 500.0f - 300.0f;
    const float maxY = 500.0f + 300.0f;
    const float deltaY = maxY - minY;

    Point c = getCenter();
    if (c.x < minX)
    {
        c.x += deltaX;
        move(deltaX, 0.0f);
    }
    else if (maxX < c.x)
    {
        c.x -= deltaX;
        move(-deltaX, 0.0f);
    }
    
    if (c.y < minY)
    {
        c.y += deltaY;
        move(0.0f, deltaY);
    }
    else if (maxY < c.y)
    {
        c.y -= deltaY;
        move(0.0f, -deltaY);
    }
}