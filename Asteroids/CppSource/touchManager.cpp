#include "touchManager.h"


TouchManager::TouchManager()
{
}


void TouchManager::touchDown(int id, float x, float y)
{
    Point p;
    p.x = x;
    p.y = y;
    touches_[id] = p;
}


void TouchManager::touchMove(int id, float x, float y)
{
    Point& p = touches_[id];
    p.x = x;
    p.y = y;
}


void TouchManager::touchUp(int id, float x, float y)
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