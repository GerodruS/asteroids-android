#pragma once

#include "painter.h"
#include "squareButton.h"
#include "touchManager.h"
#include "asteroidsGenerator.h"
#include "bullet.h"
#include "ship.h"
#include "timer.h"

class Game
{
public:
    Game();

    void setupGraphics(int width, int height);
    void init();
    void step();
    void render();

    void touchDown(int id, float x, float y);
    void touchMove(int id, float x, float y);
    void touchUp(int id, float x, float y);

    void setFieldSize(float value)
    {
        fieldSize_ = value;
    };
    float getFieldSize() const
    {
        return fieldSize_;
    };

    void reset();

private:
    Painter painter_;
    TouchManager touchManager_;
    AsteroidsGenerator asteroidsGenerator_;

    std::vector<SquareButton> buttons_;
    std::vector<Asteroid> asteroids_;
    std::vector<Bullet> bullets_;
    Ship ship_;

    float fieldSize_;
    /*
    time_t time_prev;
    float timeLeft_;
    float timeWaiting_;
    */
    Timer tmrAsteroidNew_;
    float timeWaitAsteroidNew_;

    Timer tmrBullet_;
    float timeWaitBullet_;

    Timer tmrNewGame_;
    float timeNewGame_;
    /*
    SquareButton btnLeft_;
    SquareButton btnRight_;
    SquareButton btnFire_;
    SquareButton btnMove_;
    */

};
