#pragma once

#include "gameObject.h"

class Bullet : public GameObject
{
public:
    Bullet();

    virtual const Point& getPosition() const;

private:

};