#pragma once

#include <map>
#include <vector>
#include "point.h"


class TouchManager
{
public:
    TouchManager();
    
    void touchDown(const int id, const float x, const float y);
    void touchMove(const int id, const float x, const float y);
    void touchUp(const int id, const float x, const float y);

    const std::vector<Point>& getTouches() const;

private:
    typedef std::map<int, Point> Touches;
    Touches touches_;

};
