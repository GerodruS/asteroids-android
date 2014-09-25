#pragma once

#include "gameObject.h"


class Bullet;


class Asteroid : public GameObject
{
public:
    Asteroid();

    //  ��������� ���������� ��������������
    void generate(const int edgeCountMin,
                  const int edgeCountMax,
                  const float radiusMin,
                  const float radiusMax);

    bool isCollisionWithBullet(const Bullet& bullet) const;
    bool isPointInsidePolygons(const Point& point) const;
    
    //  ��������� �� ������ �� ��������� �������
    float getRadiusMin() const;
    //  ��������� �� ������ �� �������� ��������� �������
    float getRadiusMax() const;
    int getGeneration() const;

    void setGeneration(int value);
    
private:
    float radiusMin_;
    float radiusMax_;
    int generation_;

};