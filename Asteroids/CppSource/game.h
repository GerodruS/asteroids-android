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
    Game(const float fieldSize = 1000.0f,
         const float timeWaitAsteroidNew = 2.0f,
         const float timeWaitBullet = 0.5f,
         const float timeNewGame = 1.0f);

    void init();
    void setupGraphics(const int width, const int height);

    void step();
    void render();

    float getFieldSize() const;

    void setFieldSize(const float value);

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

    float fieldSize_;

    Timer tmrAsteroidNew_;
    float timeWaitAsteroidNew_;

    Timer tmrBullet_;
    float timeWaitBullet_;

    Timer tmrNewGame_;
    float timeNewGame_;


    void initButtons();
    void initShip();
    
    void checkTouches();
    void shipControl();
    void stepObjects();
    void generateAsteroid();
    void checkCollisiions();
    void delObjects();

};
