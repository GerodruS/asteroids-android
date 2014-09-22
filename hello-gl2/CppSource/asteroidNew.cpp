#include "asteroidNew.h"

#include <stdlib.h>

using std::vector;


Asteroid::Asteroid()
{
}


void Asteroid::generate(const int countMin,
                        const int countMax,
                        const float radiusMin,
                        const float radiusMax)
{
    const int   maxvalueInt = 1000;
    const float maxvalueFlt = 100.0f;

    const int count = countMin + rand() % (countMax - countMin);

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

/*
#include <stdlib.h>
#include <math.h>

using namespace std;

const int   maxvalueInt = 1000;
const float maxvalueFlt = 100.0f;

const int   radiusMaxAll = 150;
const int   radiusMinAll = 50;

const int   pointsCountMax = 6;
const int   pointsCountMin = 10;


AsteroidOld::AsteroidOld() :
    radiusMax(0.0f),
    radiusMin(0.0f)
    , angularVelocity(0.0f)
{
    int n = pointsCountMin + rand() % (pointsCountMax - pointsCountMin);

    generate(n);

    int size = points.size();
    int count = size / 2 - 1;

    float x = 0.0f;
    float y = 0.0f;
    for (int i = 0; i < size - 2; i += 2)
    {
        x += points[i];
        y += points[i + 1];
    }
    x /= count;
    y /= count;

    points[size - 2] = x;
    points[size - 1] = y;

    for (int i = 0; i < size; ++i)
    {
        points[i] += 500;
    }

    velocity.x = 0.0f;
    velocity.y = 0.0f;

    angularVelocity = ((rand() % 20) - 10) / 7.0f;
}


AsteroidOld::~AsteroidOld()
{
}


void AsteroidOld::generate(int n)
{
    //srand(0);
    int count = n;// 3 + rand() % 5;
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

    points.resize(count * 2 + 2);

    float angleCurrent = 0.0f;

    points[0] = 1.0f;
    points[1] = 1.0f;

    for (int i = 1; i < count; ++i)
    {
        float a = angles[i];
        angleCurrent += a;
        if (0.0f < angleCurrent && angleCurrent <= 1.0f)
        {
            points[i * 2] = 1.0f;
            points[i * 2 + 1] = 1.0f - angleCurrent;
        }
        else if (1.0f < angleCurrent && angleCurrent <= 2.0f)
        {
            points[i * 2] = 2.0f - angleCurrent;
            points[i * 2 + 1] = 0.0f;
        }
        else if (2.0f < angleCurrent && angleCurrent <= 3.0f)
        {
            points[i * 2] = 0.0f;
            points[i * 2 + 1] = angleCurrent - 2.0f;
        }
        else if (3.0f < angleCurrent)
        {
            points[i * 2] = angleCurrent - 3.0;
            points[i * 2 + 1] = 1.0f;
        }
    }

    for (int i = 0; i < count * 2; ++i)
    {
        points[i] -= 0.5f;
    }

    for (int i = 0; i < count; ++i)
    {
        float summ = sqrtf(points[i * 2] * points[i * 2] + points[i * 2 + 1] * points[i * 2 + 1]);
        float r = (radiusMinAll + rand() % (radiusMaxAll - radiusMinAll));
        points[i * 2] *= r / summ;
        points[i * 2 + 1] *= 100.0f / summ;
    }

    radiusMin = 0.0f;
    radiusMax = 0.0f;
    Point c;
    c.x = points[count * 2];
    c.y = points[count * 2 + 1];
    for (int i = 0; i < count - 1; ++i)
    {
        Point a;
        a.x = points[i * 2];
        a.y = points[i * 2 + 1];
        float r = PointFunctions::distance(a, c);
        if (r < radiusMin || 0.0f == radiusMin)
        {
            radiusMin = r;
        }
        if (radiusMax < r || 0.0f == radiusMax)
        {
            radiusMax = r;
        }
    }
}


void AsteroidOld::move(float x, float y)
{
    for (int i = 0; i < points.size(); i += 2)
    {
        points[i] += x;
        points[i + 1] += y;
    }
}


void AsteroidOld::setPosition(float x, float y)
{
    int last = points.size() - 1;
    float dx = x - points[last - 1];
    float dy = y - points[last];
    move(dx, dy);
}


void AsteroidOld::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}


float AsteroidOld::getPositionX()
{
    return points[points.size() - 2];
}


float AsteroidOld::getPositionY()
{
    return points[points.size() - 1];
}


void AsteroidOld::step()
{
    move(velocity.x, velocity.y);
    rotate(angularVelocity);
}


bool AsteroidOld::pointIntersect(const Point& point)
{
    Point c;
    getCenter(c);
    float d = PointFunctions::distance(point, c);
    if (radiusMax <= d)
    {
        return false;
    }
    else if (d <= radiusMin)
    {
        return true;
    }
    else
    {
        return polygonsIntersect(point);
    }
}


bool AsteroidOld::polygonsIntersect(const Point& point)
{
    Point a, b, c;
    getCenter(c);
    int count = points.size();
    for (int i = 0; i < count - 2; i += 2)
    {
        int j = (i + 2) % (count - 2);
        a.x = points[i];
        a.y = points[i + 1];
        b.x = points[j];
        b.y = points[j + 1];
        bool res = polygonIntersect(a, b, c, point);
        if (res)
        {
            return true;
        }
    }
    return false;
}


bool AsteroidOld::polygonIntersect(const Point& a, const Point& b, const Point& c, const Point& point)
{
    float xMin = fminf(a.x, fminf(b.x, c.x));
    float yMin = fminf(a.y, fminf(b.y, c.y));
    float xMax = fmaxf(a.x, fmaxf(b.x, c.x));
    float yMax = fmaxf(a.y, fmaxf(b.y, c.y));
    if (point.x < xMin || xMax < point.x ||
        point.y < yMin || yMax < point.y)
    {
        return false;
    }
    else
    {
        float sqrA = PointFunctions::polygonSquare(a, b, c);
        float sqrB = 0.0f;
        sqrB += PointFunctions::polygonSquare(a, b, point);
        sqrB += PointFunctions::polygonSquare(b, c, point);
        sqrB += PointFunctions::polygonSquare(c, a, point);
        if (fabsf(sqrA - sqrB) < 0.001f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


void AsteroidOld::getCenter(Point& point)
{
    point.x = getPositionX();
    point.y = getPositionY();
}


bool AsteroidOld::asteroidIntersect(AsteroidOld& asteroid)
{
    Point p;
    int count = asteroid.points.size();
    for (int i = 0; i < count; i += 2)
    {
        p.x = asteroid.points[i];
        p.y = asteroid.points[i + 1];
        bool res = pointIntersect(p);
        if (res)
        {
            return true;
        }
    }
    count = points.size();
    for (int i = 0; i < count; i += 2)
    {
        p.x = points[i];
        p.y = points[i + 1];
        bool res = asteroid.pointIntersect(p);
        if (res)
        {
            return true;
        }
    }
    return false;
}


bool AsteroidOld::bulletIntersect(Bullet& bullet)
{
    Point p;
    bullet.getCenter(p);
    return pointIntersect(p);
}


void AsteroidOld::rotate(float angle)
{
    int count = points.size();
    float x_center = points[count - 2];
    float y_center = points[count - 1];
    float x, y;
    for (int i = 0; i < count; i += 2)
    {
        rotate(points[i], points[i + 1], x_center, y_center, x, y, angle);
        points[i] = x;
        points[i + 1] = y;
    }
}


void AsteroidOld::rotate(float  x_in, float  y_in,
    float  x_center, float  y_center,
    float& x_out, float& y_out,
    float  angle)
{
    x_in -= x_center;
    y_in -= y_center;

    rotate(x_in, y_in, x_out, y_out, angle);

    x_out += x_center;
    y_out += y_center;
}

void AsteroidOld::rotate(float  x_in, float  y_in,
    float& x_out, float& y_out,
    float  angle)
{
    angle *= 0.0174532925f;

    x_out = x_in * cos(angle) - y_in * sin(angle);
    y_out = x_in * sin(angle) + y_in * cos(angle);
}
*/