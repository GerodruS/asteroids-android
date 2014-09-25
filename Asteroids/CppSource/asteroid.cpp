#include "asteroid.h"

#include <stdlib.h>

#include "bullet.h"


using std::vector;


Asteroid::Asteroid() :
    radiusMin_(0.0f),
    radiusMax_(0.0f),
    generation_(0)
{
}


void Asteroid::generate(const int edgeCountMin,
                        const int edgeCountMax,
                        const float radiusMin,
                        const float radiusMax)
{
    const int   maxvalueInt = 1000;
    const float maxvalueFlt = 100.0f;

    const unsigned count = edgeCountMin + rand() % (edgeCountMax - edgeCountMin);

    //  находим случайные точки, расположенные на сторонах
    //    квадрата с длиной ребра = 1 и центром в точке (0.5, 0.5)
    vector<float> angles(count);
    float summ = 0.0f;
    for (unsigned i = 0; i < count; ++i) {
        const float value = 0.1f + (rand() % maxvalueInt) / maxvalueFlt;        
        angles[i] = value;
        summ += value;
    }
    for (unsigned i = 0; i < count; ++i) {
        angles[i] *= 4.0f / summ;
    }

    points_.resize(count + 1);

    float angleCurrent = 0.0f;

    points_[0] = pointZero;
    points_[1] = pointOne;

    for (unsigned i = 0; i < count - 1; ++i) {
        angleCurrent += angles[i];
        if (0.0f < angleCurrent && angleCurrent <= 1.0f) {
            points_[i + 2].x = 1.0f;
            points_[i + 2].y = 1.0f - angleCurrent;
        }
        else if (1.0f < angleCurrent && angleCurrent <= 2.0f) {
            points_[i + 2].x = 2.0f - angleCurrent;
            points_[i + 2].y = 0.0f;
        }
        else if (2.0f < angleCurrent && angleCurrent <= 3.0f) {
            points_[i + 2].x = 0.0f;
            points_[i + 2].y = angleCurrent - 2.0f;
        }
        else if (3.0f < angleCurrent) {
            points_[i + 2].x = angleCurrent - 3.0;
            points_[i + 2].y = 1.0f;
        }
    }

    //  сдвигаем центр квадрата в точку (0, 0) и
    //    нормируем найденные точки
    for (unsigned i = 1; i < count + 1; ++i) {
        points_[i].x -= 0.5f;
        points_[i].y -= 0.5f;
    }

    for (unsigned i = 1; i < count + 1; ++i) {
        const float r = radiusMin + rand() % int(radiusMax - radiusMin);
        PointFunctions::normalize(points_[i], r);
    }

    //  находим расстояния от наиболее отдалённой и
    //    ближайшей вершины до центра
    radiusMin_ = 0.0f;
    radiusMax_ = 0.0f;
    const Point& center = points_[0];
    for (unsigned i = 1; i < count + 1; ++i) {
        const float r = PointFunctions::distance(points_[i], center);
        if (r < radiusMin_ || 0.0f == radiusMin_) {
            radiusMin_ = r;
        }
        if (radiusMax_ < r || 0.0f == radiusMax_) {
            radiusMax_ = r;
        }
    }
}


bool Asteroid::isCollisionWithBullet(const Bullet& bullet) const
{
    const Point& bulletPosition = bullet.getPosition();
    const float d = PointFunctions::distance(getPosition(), bulletPosition);
    
    if (d < radiusMin_) {
        return true;
    }
    else if (radiusMax_ < d) {
        return false;
    }
    else if (isPointInsidePolygons(bulletPosition)) {
        return true;
    }

    return false;
}


bool Asteroid::isPointInsidePolygons(const Point& point) const
{
    const Point& c = getPosition();
    const unsigned count = points_.size();

    for (unsigned i = 1; i < count; ++i) {
        const Point& a = points_[i];
        const Point& b = points_[i % (count - 1) + 1];

        const bool res = PointFunctions::isPointInsidePolygon(a, b, c, point);
        if (res) {
            return true;
        }
    }

    return false;
}


float Asteroid::getRadiusMin() const
{
    return radiusMin_;
}


float Asteroid::getRadiusMax() const
{
    return radiusMax_;
}


int Asteroid::getGeneration() const
{
    return generation_;
}


void Asteroid::setGeneration(int value)
{
    generation_ = value;
}