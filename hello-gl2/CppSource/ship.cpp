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