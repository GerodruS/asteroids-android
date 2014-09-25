#include "game.h"

//#include <time.h>

using std::vector;


Game::Game() :
    fieldSize_(1000.0f),
    timeWaitAsteroidNew_(2.0f),
    timeWaitBullet_(0.5f),
    timeNewGame_(1.0f)
{
}


void Game::setupGraphics(int width, int height)
{
    painter_.setupGraphics(getFieldSize(), width, height);

}


void Game::init()
{
    asteroidsGenerator_.setFrame(-getFieldSize() / 2.0f, -getFieldSize() / 2.0f,
        getFieldSize() / 1.0f, getFieldSize() / 1.0f);

    {
        buttons_.resize(4);
        SquareButton& btnLeft_ = buttons_[0];
        SquareButton& btnRight_ = buttons_[1];
        SquareButton& btnFire_ = buttons_[2];
        SquareButton& btnMove_ = buttons_[3];

        const float gameWidth = painter_.getGameWidth();
        const float gameHeight = painter_.getGameHeight();
        if (painter_.isLandscape())
        {
            const float halfWidth = gameWidth / 2.0f;
            const float quarterHeight = gameHeight / 4.0f;

            btnLeft_.setPosition(-halfWidth, -quarterHeight);
            btnLeft_.setSize(quarterHeight, quarterHeight);

            btnRight_.setPosition(-halfWidth + quarterHeight, -2.0f * quarterHeight);
            btnRight_.setSize(quarterHeight, quarterHeight);

            btnMove_.setPosition(halfWidth - quarterHeight, -quarterHeight);
            btnMove_.setSize(quarterHeight, quarterHeight);

            btnFire_.setPosition(halfWidth - 2.0f * quarterHeight, -2.0f * quarterHeight);
            btnFire_.setSize(quarterHeight, quarterHeight);
        }
        else
        {
            const float halfHeight = gameHeight / 2.0f;
            const float quarterWidth = gameWidth / 4.0f;

            btnLeft_.setPosition(-gameWidth / 2.0f, -halfHeight + quarterWidth);
            btnLeft_.setSize(quarterWidth, quarterWidth);

            btnRight_.setPosition(-quarterWidth, -halfHeight);
            btnRight_.setSize(quarterWidth, quarterWidth);

            btnFire_.setPosition(quarterWidth, -halfHeight + quarterWidth);
            btnFire_.setSize(quarterWidth, quarterWidth);

            btnMove_.setPosition(0.0f, -halfHeight + 500);
            btnMove_.setSize(quarterWidth, quarterWidth);
        }
    }

    asteroids_.resize(asteroids_.size() + 1);
    asteroidsGenerator_.generate(asteroids_[asteroids_.size() - 1]);

    //ship_ = Ship();
    ship_.setDel(false);
    ship_.setPosition(0.0f, 0.0f);
    ship_.setVelocity(0.0f, 0.0f);
    ship_.setFramePositon(-getFieldSize() / 2.0f, -getFieldSize() / 2.0f);
    ship_.setFrameSize(getFieldSize() / 1.0f, getFieldSize() / 1.0f);
}


void Game::step()
{
    if (!tmrNewGame_.isReady())
    {
        return;
    }

    {
        const unsigned countButtons = buttons_.size();
        const std::vector<Point>& touchesList = touchManager_.getTouches();
        for (unsigned i = 0; i < countButtons; ++i)
        {
            bool r = buttons_[i].checkTouches(touchesList);
            buttons_[i].setIsPress(r);
        }
    }
    {
        //const unsigned count = asteroids_.size();
        for (unsigned i = 0; i < asteroids_.size(); ++i)
        {
            asteroids_[i].step();

            const Point& p = asteroids_[i].getPosition();
            const float border = getFieldSize() / 2.0f + 2.0f * asteroids_[i].getRadiusMax();
            if (p.x < -border || border < p.x || p.y < -border || border < p.y)
            {
                asteroids_.erase(asteroids_.begin() + i);
                --i;
            }
        }

        if (tmrAsteroidNew_.isReady())
        {
            asteroids_.resize(asteroids_.size() + 1);
            asteroidsGenerator_.generate(asteroids_[asteroids_.size() - 1]);
            tmrAsteroidNew_.setAlarm(timeWaitAsteroidNew_);
        }
    }
    {
        ship_.step();

        float angle = 5.0f;
        float speed = 2.0f;
        if (buttons_[0].isPress())
        {
            ship_.rotate(angle);
        }
        
        if (buttons_[1].isPress())
        {
            ship_.rotate(-angle);
        }

        if (buttons_[2].isPress())
        {
            Point d = ship_.getDirection();
            ship_.addVelocity(d.x * speed, d.y * speed);
            //ship.move(ship.direction.x * speed, ship.direction.y * speed);
        }

        if (buttons_[3].isPress())
        {
            if (tmrBullet_.isReady())
            {
                float speedBullet = 5.0f;
                const Point& startPosition = ship_.getBulletStartPosition();
                Point velocity = ship_.getDirection();
                bullets_.resize(bullets_.size() + 1);
                Bullet& bullet = bullets_[bullets_.size() - 1];
                bullet.setPosition(startPosition);
                bullet.setVelocity(velocity.x * speedBullet, velocity.y * speedBullet);

                tmrBullet_.setAlarm(timeWaitBullet_);
            }
        }
        else
        {
            tmrBullet_.reset();
        }
    }
    {
        for (unsigned i = 0; i < bullets_.size(); ++i)
        {
            bullets_[i].step();
        }
    }

    {

        for (unsigned i = 0; i < asteroids_.size(); ++i)
        {
            for (unsigned j = 0; j < bullets_.size(); ++j)
            {
                if (asteroids_[i].isCollisionWithBullet(bullets_[j]))
                {
                    asteroids_[i].setDel(true);
                    bullets_[j].setDel(true);

                    if (1 == asteroids_[i].getGeneration()) {
                        const Point& pos = asteroids_[i].getPosition();
                        asteroids_.resize(asteroids_.size() + 2);

                        asteroidsGenerator_.generate(asteroids_[asteroids_.size() - 2], &pos);
                        asteroidsGenerator_.generate(asteroids_[asteroids_.size() - 1], &pos);
                    }
                }
            }
        }

        for (unsigned i = 0; i < asteroids_.size(); ++i)
        {
            if (ship_.isCollisionWithAsteroid(asteroids_[i]))
            {
                ship_.setDel(true);
                asteroids_[i].setDel(true);
            }
        }
    }

    {
        for (unsigned i = 0; i < asteroids_.size(); ++i)
        {
            if (asteroids_[i].isDel())
            {
                asteroids_.erase(asteroids_.begin() + i);
                --i;
            }
        }
        for (unsigned i = 0; i < bullets_.size(); ++i)
        {
            if (bullets_[i].isDel())
            {
                bullets_.erase(bullets_.begin() + i);
                --i;
            }
        }
        if (ship_.isDel())
        {
            reset();
        }
    }
}


void Game::reset()
{
    buttons_.clear();
    asteroids_.clear();
    bullets_.clear();
    tmrAsteroidNew_.reset();
    tmrBullet_.reset();

    init();

    tmrNewGame_.setAlarm(timeNewGame_);
}


void Game::render()
{
    painter_.drawPrepare();
    if (tmrNewGame_.isReady())
    {
        painter_.drawAsteroids(asteroids_);
        painter_.drawBullets(bullets_);
        painter_.drawShip(ship_);
    }
    painter_.drawSquareButton(buttons_);
}


void Game::touchDown(int id, float x, float y)
{
    touchManager_.touchDown(id, painter_.getXFromScreenToGame(x), painter_.getYFromScreenToGame(y));
}


void Game::touchMove(int id, float x, float y)
{
    touchManager_.touchMove(id, painter_.getXFromScreenToGame(x), painter_.getYFromScreenToGame(y));
}


void Game::touchUp(int id, float x, float y)
{
    touchManager_.touchUp(id, painter_.getXFromScreenToGame(x), painter_.getYFromScreenToGame(y));
}