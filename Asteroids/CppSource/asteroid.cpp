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
    const int   maxvalueInt = 10000;
    const float maxvalueFlt = 100.0f;

    const unsigned count = edgeCountMin + rand() % (edgeCountMax - edgeCountMin);

    //  ������� ��������� �����, ������������� �� ��������
    //    �������� � ������ ����� = 1 � ������� � ����� (0.5, 0.5)
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

    points_[0] = pointZero;
    points_[1] = pointOne;
    
    float angleCurrent = 0.0f;
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
            points_[i + 2].x = angleCurrent - 3.0f;
            points_[i + 2].y = 1.0f;
        }
    }

    //  �������� ����� �������� � ����� (0, 0) �
    //    ��������� ��������� �����
    for (unsigned i = 1; i < count + 1; ++i) {
        points_[i].x -= 0.5f;
        points_[i].y -= 0.5f;
    }

    for (unsigned i = 1; i < count + 1; ++i) {
        const float r = radiusMin + rand() % int(radiusMax - radiusMin);
        PointFunctions::normalize(points_[i], r);
    }

    //  ������� ���������� �� �������� ��������� �
    //    ��������� ������� �� ������
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
    const Point& center = getPosition();
    const unsigned count = points_.size();

    for (unsigned i = 1; i < count; ++i) {
        const unsigned j = i % (count - 1) + 1;
        const bool res = PointFunctions::isPointInsidePolygon(points_[i],
                                                              points_[j],
                                                              center,
                                                              point);
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


void Asteroid::setGeneration(const int value)
{
    generation_ = value;
}