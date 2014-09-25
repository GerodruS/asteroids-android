#pragma once

#include "gameObject.h"


class Bullet;


class Asteroid : public GameObject
{
public:
    Asteroid();

    //  генерация случайного многоугольника
    void generate(const int edgeCountMin,
                  const int edgeCountMax,
                  const float radiusMin,
                  const float radiusMax);

    bool isCollisionWithBullet(const Bullet& bullet) const;
    bool isPointInsidePolygons(const Point& point) const;
    
    //  растояние от центра до ближайшей вершины
    float getRadiusMin() const;
    //  растояние от центра до наиболее отдалённой вершины
    float getRadiusMax() const;
    int getGeneration() const;

    void setGeneration(int value);
    
private:
    float radiusMin_;
    float radiusMax_;
    int generation_;

};