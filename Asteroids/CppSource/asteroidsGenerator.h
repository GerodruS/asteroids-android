#pragma once

#include "point.h"

class Asteroid;


class AsteroidsGenerator
{
public:
    AsteroidsGenerator(int edgeCountMin = 6,
                       int edgeCountMax = 12,
                       float radiusMin = 25.0f,
                       float radiusMax = 75.0f,
                       float velocityMin = 0.5f,
                       float velocityMax = 3.0f);

    //  ������ ����� �������� � ������������� ���������� ��� ��������
    //    ���� ������� ���, �� �������� �������� �� ������� � Generation = 1
    //    ���� ������� ����, �� �������� �������� �� ��,
    //      ������������ ������� � Generation = 2
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


    float getPosition() const;
    float getPosition(const float from) const;
    void setPointAtPosition(Point& point, const float position) const;
    //  ���������� ��� ��������� ����� (����� � ����� ���������),
    //    �������������� �� ����� ��������, ��������� position_ � size_
    //    ����� ����������� �� �� ����� �����
    void getPoints(Point& from, Point& to) const;

};