#include "asteroid.h"

#include <stdlib.h>
#include <math.h>

using namespace std;

const int   maxvalueInt = 1000;
const float maxvalueFlt = 100.0f;


Asteroid::Asteroid(int n)
{
    /*
    points.resize(10);
    points[0] =    0.0f;  points[1] =  100.0f;
    points[2] =  100.0f;  points[3] =    0.0f;
    points[4] =    0.0f;  points[5] = -100.0f;
    points[6] = -100.0f;  points[7] =    0.0f;
    */

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
}


Asteroid::~Asteroid()
{
}


void Asteroid::generate(int n)
{
    srand(0);
    int count = n;// 3 + rand() % 5;
    vector<float> angles(count);
    float summ = 0.0f;
    for (int i = 0; i < count; ++i)
    {
        float value = 0.1f + (rand() % maxvalueInt) / maxvalueFlt;
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

    for (int i = 0; i < count; ++i)
    {
        float a = angles[i];
        angleCurrent += a;
        if (0.0f < angleCurrent && angleCurrent <= 1.0f)
        {
            points[i * 2] = 1.0f;
            points[i * 2 + 1] = 1.0f - 0.0f - angleCurrent;
        }
        else if (1.0f < angleCurrent && angleCurrent <= 2.0f)
        {
            points[i * 2] = 1.0f - (angleCurrent - 1.0);
            points[i * 2 + 1] = 0.0f;
        }
        else if (2.0f < angleCurrent && angleCurrent <= 3.0f)
        {
            points[i * 2] = 0.0f;
            points[i * 2 + 1] = 3.0f - angleCurrent;
        }
        else if (3.0f < angleCurrent && angleCurrent <= 3.9f)
        {
            points[i * 2] = 4.0f - angleCurrent;
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
        points[i * 2] *= 300.0f / summ;
        points[i * 2 + 1] *= 300.0f / summ;
    }
}


void Asteroid::move(float x, float y)
{
    for (int i = 0; i < points.size(); i += 2)
    {
        points[i] += x;
        points[i + 1] += y;
    }
}