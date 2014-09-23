#pragma once

#include "painter.h"
#include "squareButton.h"

class Game
{
public:
    Game();

    void setupGraphics(int width, int height);
    void init();
    void step();
    void render();

private:
    Painter painter_;

    std::vector<SquareButton> buttons_;
    /*
    SquareButton btnLeft_;
    SquareButton btnRight_;
    SquareButton btnFire_;
    SquareButton btnMove_;
    */

};
