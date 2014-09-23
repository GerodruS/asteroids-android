#include "game.h"


using std::vector;


Game::Game()
{
}


void Game::setupGraphics(int width, int height)
{
    painter_.setupGraphics(width, height);
}


void Game::init()
{
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

    }
}


void Game::render()
{
    painter_.drawPrepare();
    painter_.drawSquareButton(buttons_);
}


void Game::touchDown(int id, float x, float y)
{
    touchManager_.touchDown(id, getXFromScreenToGame(x), getYFromScreenToGame(y));
}


void Game::touchMove(int id, float x, float y)
{
    touchManager_.touchMove(id, getXFromScreenToGame(x), getYFromScreenToGame(y));
}


void Game::touchUp(int id, float x, float y)
{
    touchManager_.touchUp(id, getXFromScreenToGame(x), getYFromScreenToGame(y));
}


float Game::getXFromScreenToGame(float value)
{
    const float sw = painter_.getScreenWidth();
    return (value - sw / 2.0f) * painter_.getGameWidth() / sw;
}

float Game::getYFromScreenToGame(float value)
{
    const float sh = painter_.getScreenHeight();
    return (sh - value - sh / 2.0f) * painter_.getGameHeight() / sh;
}