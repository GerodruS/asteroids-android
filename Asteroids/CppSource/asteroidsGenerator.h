#pragma once

#include "point.h"

class Asteroid;


class AsteroidsGenerator
{
public:
    AsteroidsGenerator();

    void init(const int edgeCountMin,
              const int edgeCountMax,
              const float radiusMin,
              const float radiusMax,
              const float velocityMin,
              const float velocityMax);

    //  создаёт новый астероид и устанавливает траекторию его движения
    //    если позиции нет, то астероид стартует за экраном и Generation = 1
    //    если позиция есть, то астероид стартует из неё,
    //      уменьшенного размера и Generation = 2
    void generate(Asteroid& asteroid, const Point* const position = 0) const;

    void setFrame(const float positionX, const float positionY,
                  const float sizeX,     const float sizeY);

private:
    Point position_;
    Point size_;

    int edgeCountMin_;
    int edgeCountMax_;

    float radiusMin_;
    float radiusMax_;
    float velocityMin_;
    float velocityMax_;


    //  генерирует две случайные точки (старт и финиш астероида),
    //    рассположенные на рёбрах квадрата, заданного position_ и size_
    //    точки расположены не на одной грани
    void getPoints(Point& from, Point& to) const;
    void setPointAtPosition(Point& point, const float position) const;
    float getPosition() const;
    float getPosition(const float from) const;

};