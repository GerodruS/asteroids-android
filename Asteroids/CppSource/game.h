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

    void init();
    void setupGraphics(const int width, const int height);

    void step();
    void render();

    void reset();
    
    void touchDown(const int id, const float x, const float y);
    void touchMove(const int id, const float x, const float y);
    void touchUp(const int id, const float x, const float y);


private:
    Painter painter_;
    TouchManager touchManager_;
    AsteroidsGenerator asteroidsGenerator_;

    std::vector<SquareButton> buttons_;
    std::vector<Asteroid> asteroids_;
    std::vector<Bullet> bullets_;
    Ship ship_;

    Timer tmrAsteroidNew_;
    Timer tmrBullet_;
    Timer tmrNewGame_;


    void initButtons();
    void initShip();
    
    void checkTouches();
    void shipControl();
    void stepObjects();
    void generateAsteroid();
    void checkCollisiions();
    void delObjects();

};
