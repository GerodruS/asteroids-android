#include "bullet.h"

#include <stdlib.h>
#include <math.h>

using namespace std;

const float bulletRadius = 10.0f;


Bullet::Bullet()
{
    points.resize(18);

    float r = bulletRadius / 1.41421356237f;
    points[0] = 0.0f;           points[1] = bulletRadius;
    points[2] = r;              points[3] = r;
    points[4] = bulletRadius;   points[5] = 0.0f;
    points[6] = r;              points[7] = -r;
    points[8] = 0.0f;           points[9] = -bulletRadius;
    points[10] = -r;            points[11] = -r;
    points[12] = -bulletRadius; points[13] = 0.0f;
    points[14] = -r;            points[15] = r;
    points[16] = 0.0f;          points[17] = 0.0f;

    velocity.x = 0.0f;
    velocity.y = 0.0f;
}


Bullet::~Bullet()
{
}



void Bullet::move(float x, float y)
{
    for (int i = 0; i < points.size(); i += 2)
    {
        points[i] += x;
        points[i + 1] += y;
    }
}


void Bullet::setPosition(float x, float y)
{
    int last = points.size() - 1;
    float dx = x - points[last - 1];
    float dy = y - points[last];
    move(dx, dy);
}


void Bullet::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}


float Bullet::getPositionX()
{
    return points[points.size() - 2];
}


float Bullet::getPositionY()
{
    return points[points.size() - 1];
}


void Bullet::step()
{
    move(velocity.x, velocity.y);
}


void Bullet::getCenter(Point& point)
{
    point.x = getPositionX();
    point.y = getPositionY();
}