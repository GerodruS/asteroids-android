#include "game.h"


using std::vector;


Game::Game(const float fieldSize,
           const float timeWaitAsteroidNew,
           const float timeWaitBullet,
           const float timeNewGame) :
    fieldSize_(fieldSize),
    timeWaitAsteroidNew_(timeWaitAsteroidNew),
    timeWaitBullet_(timeWaitBullet),
    timeNewGame_(timeNewGame)
{
}


void Game::init()
{
    asteroidsGenerator_.setFrame(-getFieldSize() / 2.0f,
                                 -getFieldSize() / 2.0f,
                                  getFieldSize(),
                                  getFieldSize());
    initButtons();
    initShip();
    generateAsteroid();
}


void Game::setupGraphics(const int width, const int height)
{
    painter_.setupGraphics(getFieldSize(), width, height);
}


void Game::step()
{
    if (tmrNewGame_.isReady()) {
        checkTouches();
        shipControl();
        stepObjects();
        generateAsteroid();
        checkCollisiions();
        delObjects();
    }
}


void Game::render()
{
    painter_.drawPrepare();
    if (tmrNewGame_.isReady())
    {
        painter_.drawScissorEnable();
        painter_.drawAsteroids(asteroids_);
        painter_.drawBullets(bullets_);
        painter_.drawShip(ship_);
        painter_.drawScissorDisable();
    }
    painter_.drawSquareButton(buttons_);
}


float Game::getFieldSize() const
{
    return fieldSize_;
}


void Game::setFieldSize(const float value)
{
    fieldSize_ = value;
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


void Game::touchDown(const int id, const float x, const float y)
{
    touchManager_.touchDown(id,
                            painter_.getXFromScreenToGame(x),
                            painter_.getYFromScreenToGame(y));
}


void Game::touchMove(const int id, const float x, const float y)
{
    touchManager_.touchMove(id,
                            painter_.getXFromScreenToGame(x),
                            painter_.getYFromScreenToGame(y));
}


void Game::touchUp(const int id, const float x, const float y)
{
    touchManager_.touchUp(id,
                          painter_.getXFromScreenToGame(x),
                          painter_.getYFromScreenToGame(y));
}


void Game::initButtons()
{
    buttons_.resize(4);
    SquareButton& btnLeft_ = buttons_[0];
    SquareButton& btnRight_ = buttons_[1];
    SquareButton& btnFire_ = buttons_[2];
    SquareButton& btnMove_ = buttons_[3];

    const float gameWidth = painter_.getGameWidth();
    const float gameHeight = painter_.getGameHeight();

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
}


void Game::initShip()
{
    ship_.reset();
    ship_.setFramePositon(-getFieldSize() / 2.0f, -getFieldSize() / 2.0f);
    ship_.setFrameSize(getFieldSize() / 1.0f, getFieldSize() / 1.0f);
}


void Game::generateAsteroid()
{
    if (tmrAsteroidNew_.isReady())
    {
        const unsigned index = asteroids_.size();
        asteroids_.resize(index + 1);
        asteroidsGenerator_.generate(asteroids_[index]);

        tmrAsteroidNew_.setAlarm(timeWaitAsteroidNew_);
    }
}


void Game::checkTouches()
{
    const std::vector<Point>& touchesList = touchManager_.getTouches();

    const unsigned countButtons = buttons_.size();
    for (unsigned i = 0; i < countButtons; ++i)
    {
        SquareButton& btn = buttons_[i];
        const bool r = btn.checkTouches(touchesList);
        btn.setIsPress(r);
    }
}


void Game::shipControl()
{
    const float angle = 2.0f;
    const float speed = 2.0f;
    const float speedBullet = 5.0f;

    if (buttons_[0].isPress() && !buttons_[1].isPress()) {
        ship_.rotate(angle);
    }
    else if (!buttons_[0].isPress() && buttons_[1].isPress()) {
        ship_.rotate(-angle);
    }
    
    if (buttons_[2].isPress()) {
        const Point d = ship_.getDirection();
        ship_.addVelocity(d.x * speed, d.y * speed);
    }

    if (buttons_[3].isPress()) {
        if (tmrBullet_.isReady()) {
            const Point& startPosition = ship_.getBulletStartPosition();
            const Point velocity = ship_.getDirection();

            const unsigned index = bullets_.size();
            bullets_.resize(index + 1);
            Bullet& bullet = bullets_[index];
            bullet.setPosition(startPosition);
            bullet.setVelocity(velocity.x * speedBullet,
                               velocity.y * speedBullet);

            tmrBullet_.setAlarm(timeWaitBullet_);
        }
    }
    else {
        tmrBullet_.reset();
    }
}


void Game::stepObjects()
{
    ship_.step();

    const unsigned asteroidsCount = asteroids_.size();
    for (unsigned i = 0; i < asteroidsCount; ++i) {
        asteroids_[i].step();
    }

    const unsigned bulletsCount = bullets_.size();
    for (unsigned i = 0; i < bulletsCount; ++i)
    {
        bullets_[i].step();
    }
}


void Game::checkCollisiions()
{
    const unsigned asteroidsCount = asteroids_.size();

    for (unsigned i = 0; i < asteroidsCount; ++i) {
        if (ship_.isCollisionWithAsteroid(asteroids_[i])) {
            ship_.setDel(true);
        }
    }

    if (!ship_.isDel()) {
        const unsigned bulletsCount = bullets_.size();

        for (unsigned i = 0; i < asteroidsCount; ++i) {
            const Point& p = asteroids_[i].getPosition();
            const float border = getFieldSize() / 2.0f + 2.0f * asteroids_[i].getRadiusMax();
            if (p.x < -border || border < p.x || p.y < -border || border < p.y) {
                asteroids_[i].setDel(true);
            }
        }

        for (unsigned i = 0; i < asteroidsCount; ++i) {
            for (unsigned j = 0; j < bulletsCount; ++j) {
                if (!asteroids_[i].isDel() && !bullets_[j].isDel() &&
                    asteroids_[i].isCollisionWithBullet(bullets_[j])) {

                    asteroids_[i].setDel(true);
                    bullets_[j].setDel(true);

                    if (1 == asteroids_[i].getGeneration()) {
                        const unsigned index = asteroids_.size();
                        asteroids_.resize(index + 2);
                        const Point& pos = asteroids_[i].getPosition();

                        asteroidsGenerator_.generate(asteroids_[index + 0], &pos);
                        asteroidsGenerator_.generate(asteroids_[index + 1], &pos);
                    }
                }
            }
        }
    }
}


void Game::delObjects()
{
    if (ship_.isDel()) {
        reset();
    }
    else {
        unsigned asteroidsCount = asteroids_.size();
        for (unsigned i = 0; i < asteroidsCount; ++i) {
            if (asteroids_[i].isDel()) {
                asteroids_.erase(asteroids_.begin() + i);
                --i;
                --asteroidsCount;
            }
        }

        unsigned bulletsCount = bullets_.size();
        for (unsigned i = 0; i < bulletsCount; ++i) {
            if (bullets_[i].isDel()) {
                bullets_.erase(bullets_.begin() + i);
                --i;
                --bulletsCount;
            }
        }
    }
}