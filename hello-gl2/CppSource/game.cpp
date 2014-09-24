#include "game.h"

#include <time.h>

using std::vector;


Game::Game() :
    fieldSize_(1000.0f),
    timeWaiting_(1.0f),
    timeLeft_(0.0f)
{
    time(&time_prev);
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

            btnFire_.setPosition(halfWidth - quarterHeight, -quarterHeight);
            btnFire_.setSize(quarterHeight, quarterHeight);

            btnMove_.setPosition(halfWidth - 2.0f * quarterHeight, -2.0f * quarterHeight);
            btnMove_.setSize(quarterHeight, quarterHeight);
        }
        else
        {
            const float halfHeight = gameHeight / 2.0f;
            const float quarterWidth = gameWidth / 4.0f;

            btnLeft_.setPosition(-gameWidth / 2.0f, -halfHeight + quarterWidth);
            btnLeft_.setSize(quarterWidth, quarterWidth);

            btnRight_.setPosition(-quarterWidth, -halfHeight);
            btnRight_.setSize(quarterWidth, quarterWidth);

            btnFire_.setPosition(0.0f, -halfHeight + 500);
            btnFire_.setSize(quarterWidth, quarterWidth);

            btnMove_.setPosition(quarterWidth, -halfHeight + quarterWidth);
            btnMove_.setSize(quarterWidth, quarterWidth);
        }
    }

    asteroids_.resize(asteroids_.size() + 1);
    asteroidsGenerator_.generate(asteroids_[asteroids_.size() - 1]);
}


void Game::step()
{
    {
        const unsigned countButtons = buttons_.size();
        const std::vector<Point>& touchesList = touchManager_.getTouches();
        for (unsigned i = 0; i < countButtons; ++i)
        {
            bool r = buttons_[i].check(touchesList);
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

        if (0.0f < timeLeft_)
        {
            time_t time_now = time(NULL);
            float deltaTime = difftime(time_now, time_prev);
            time_prev = time_now;
            timeLeft_ -= deltaTime;
        }
        else
        {
            asteroids_.resize(asteroids_.size() + 1);
            asteroidsGenerator_.generate(asteroids_[asteroids_.size() - 1]);
            timeLeft_ = timeWaiting_;
        }
    }
}


void Game::render()
{
    painter_.drawPrepare();
    painter_.drawAsteroids(asteroids_);
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