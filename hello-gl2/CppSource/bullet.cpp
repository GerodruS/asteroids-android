#include "bullet.h"


Bullet::Bullet() :
    size_(10.0f),
    toDel_(false)
{
    points_.resize(4);

    points_[0].x = 0.0f;   points_[0].y = -size_;
    points_[1].x = -size_;  points_[1].y = 0.0f;
    points_[2].x = 0.0f;   points_[2].y = size_;
    points_[3].x = size_;   points_[3].y = 0.0f;
}


const Point& Bullet::getPosition() const
{
    return points_[0];
}