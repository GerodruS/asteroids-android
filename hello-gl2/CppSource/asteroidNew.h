#pragma once

#include "gameObject.h"
#include "bullet.h"

class Asteroid : public GameObject
{
public:
    Asteroid();

    void generate(const int edgeCountMin,
                  const int edgeCountMax,
                  const float radiusMin,
                  const float radiusMax);

    virtual const Point& getPosition() const;

    virtual void step();

    float getRadiusMin() const
    {
        return radiusMin_;
    }

    float getRadiusMax() const
    {
        return radiusMax_;
    }

    bool isCollisionWithBullet(std::vector<Bullet>& bullets);

    bool polygonsIntersect(const Point& point);
    bool polygonIntersect(const Point& a, const Point& b, const Point& c, const Point& point);

    void hit()
    {
        toDel_ = true;
    }

    bool isToDel() const
    {
        return toDel_;
    }

private:
    float radiusMin_;
    float radiusMax_;
    bool toDel_;
};