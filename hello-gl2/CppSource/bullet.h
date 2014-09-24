#pragma once

#include "gameObject.h"

class Bullet : public GameObject
{
public:
    Bullet();

    virtual const Point& getPosition() const;
    float getSize() const
    {
        return size_;
    }
    /*
    void setSize(float value)
    {
        size_ = value;
    }
    */

    void hit()
    {
        toDel_ = true;
    }

    bool isToDel() const
    {
        return toDel_;
    }

private:
    float size_;
    bool toDel_;

};