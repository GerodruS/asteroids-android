#pragma once

#include <map>
#include <vector>
#include "point.h"

class TouchManager
{
public:
    TouchManager();
    
    void touchDown(int id, float x, float y);
    void touchMove(int id, float x, float y);
    void touchUp(int id, float x, float y);

    const std::vector<Point>& getTouches() const;

private:
    typedef std::map<int, Point> Touches;
    Touches touches_;

};
