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

    bool polygonsIntersect(const Point& point) const;
    
    void hit()
    {
        setDel(true);
    }

    int generation;

private:
    float radiusMin_;
    float radiusMax_;
};