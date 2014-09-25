#include "touchManager.h"


TouchManager::TouchManager()
{
}


void TouchManager::touchDown(const int id, const float x, const float y)
{
    touches_[id] = {x, y};
}


void TouchManager::touchMove(const int id, const float x, const float y)
{
    touches_[id] = {x, y};
}


void TouchManager::touchUp(const int id, const float x, const float y)
{
    touches_.erase(id);
}


const std::vector<Point>& TouchManager::getTouches() const
{
    static std::vector<Point> points;

    points.clear();
    for (Touches::const_iterator it = touches_.begin(); it != touches_.end(); ++it) {
        points.push_back(it->second);
    }

    return points;
}