#include "bullet.h"


Bullet::Bullet(const float size)
{
    points_.resize(4);

    points_[0].x = 0.0f;   points_[0].y = -size;
    points_[1].x = -size;  points_[1].y = 0.0f;
    points_[2].x = 0.0f;   points_[2].y = size;
    points_[3].x = size;   points_[3].y = 0.0f;
}