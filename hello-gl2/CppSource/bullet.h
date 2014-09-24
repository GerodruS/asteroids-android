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
        setDel(true);
    }

private:
    float size_;

};