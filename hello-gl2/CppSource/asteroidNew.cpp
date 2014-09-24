#include "asteroidNew.h"

#include <stdlib.h>

using std::vector;


Asteroid::Asteroid()
{
    points_.push_back(pointZero);
}


void Asteroid::generate(const int edgeCountMin,
                        const int edgeCountMax,
                        const float radiusMin,
                        const float radiusMax)
{
    const int   maxvalueInt = 1000;
    const float maxvalueFlt = 100.0f;

    const int count = edgeCountMin + rand() % (edgeCountMax - edgeCountMin);

    vector<float> angles(count);
    float summ = 0.0f;
    for (int i = 0; i < count; ++i)
    {
        float value = 0.1f + (rand() % maxvalueInt) / maxvalueFlt;
        value += 0.1f + (rand() % maxvalueInt) / maxvalueFlt;
        value += 0.1f + (rand() % maxvalueInt) / maxvalueFlt;
        
        angles[i] = value;
        summ += value;
    }
    for (int i = 0; i < count; ++i)
    {
        angles[i] *= 4.0f / summ;
    }

    points_.resize(count + 1);

    float angleCurrent = 0.0f;

    points_[0] = pointZero;
    points_[1] = pointOne;

    for (int i = 0; i < count - 1; ++i)
    {
        float a = angles[i];
        angleCurrent += a;
        if (0.0f < angleCurrent && angleCurrent <= 1.0f)
        {
            points_[i + 2].x = 1.0f;
            points_[i + 2].y = 1.0f - angleCurrent;
        }
        else if (1.0f < angleCurrent && angleCurrent <= 2.0f)
        {
            points_[i + 2].x = 2.0f - angleCurrent;
            points_[i + 2].y = 0.0f;
        }
        else if (2.0f < angleCurrent && angleCurrent <= 3.0f)
        {
            points_[i + 2].x = 0.0f;
            points_[i + 2].y = angleCurrent - 2.0f;
        }
        else if (3.0f < angleCurrent)
        {
            points_[i + 2].x = angleCurrent - 3.0;
            points_[i + 2].y = 1.0f;
        }
    }

    for (int i = 1; i < count + 1; ++i)
    {
        points_[i].x -= 0.5f;
        points_[i].y -= 0.5f;
    }

    for (int i = 1; i < count + 1; ++i)
    {
        float r = radiusMin + rand() % int(radiusMax - radiusMin);
        PointFunctions::normalize(points_[i], r);
    }

    radiusMin_ = 0.0f;
    radiusMax_ = 0.0f;
    const Point& center = points_[0];
    for (int i = 1; i < count + 1; ++i)
    {
        const Point& pnt = points_[i];
        float r = PointFunctions::distance(pnt, center);
        if (r < radiusMin_ || 0.0f == radiusMin_)
        {
            radiusMin_ = r;
        }
        if (radiusMax_ < r || 0.0f == radiusMax_)
        {
            radiusMax_ = r;
        }
    }
}


const Point& Asteroid::getPosition() const
{
    return points_[0];
}


void Asteroid::step()
{
    GameObject::step();
}