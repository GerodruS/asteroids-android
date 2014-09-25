#include "game.h"

#include "constants.h"


using std::vector;


Game::Game()
{
}


void Game::init()
{
    asteroidsGenerator_.init(Constants::getGeneratorEdgeCountMin(),
                             Constants::getGeneratorEdgeCountMax(),
                             Constants::getGeneratorRadiusMin(),
                             Constants::getGeneratorRadiusMax(),
                             Constants::getGeneratorVelocityMin(),
                             Constants::getGeneratorVelocityMax());
    asteroidsGenerator_.setFrame(-Constants::getFieldSize() / 2.0f,
                                 -Constants::getFieldSize() / 2.0f,
                                  Constants::getFieldSize(),
                                  Constants::getFieldSize());
    initButtons();
    initShip();
    generateAsteroid();
}


void Game::setupGraphics(const int width, const int height)
{
    painter_.setupGraphics(Constants::getFieldSize(), width, height);
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


void Game::reset()
{
    buttons_.clear();
    asteroids_.clear();
    bullets_.clear();

    tmrAsteroidNew_.reset();
    tmrBullet_.reset();

    init();

    tmrNewGame_.setAlarm(Constants::getTimeNewGame());
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
    SquareButton& btnLeft_  = buttons_[Constants::getButtonLeftIndex()];
    SquareButton& btnRight_ = buttons_[Constants::getButtonRightIndex()];
    SquareButton& btnMove_  = buttons_[Constants::getButtonMoveIndex()];
    SquareButton& btnFire_  = buttons_[Constants::getButtonFireIndex()];

    const float gameWidth = painter_.getGameWidth();
    const float gameHeight = painter_.getGameHeight();

    const float halfWidth = gameWidth / 2.0f;
    const float quarterHeight = gameHeight / 4.0f;

    btnLeft_.setPosition(-halfWidth, -quarterHeight);
    btnLeft_.setSize(quarterHeight, quarterHeight);
    btnLeft_.setBorder(Constants::getButtonBorder());

    btnRight_.setPosition(-halfWidth + quarterHeight, -2.0f * quarterHeight);
    btnRight_.setSize(quarterHeight, quarterHeight);
    btnRight_.setBorder(Constants::getButtonBorder());

    btnFire_.setPosition(halfWidth - quarterHeight, -quarterHeight);
    btnFire_.setSize(quarterHeight, quarterHeight);
    btnFire_.setBorder(Constants::getButtonBorder());

    btnMove_.setPosition(halfWidth - 2.0f * quarterHeight, -2.0f * quarterHeight);
    btnMove_.setSize(quarterHeight, quarterHeight);
    btnMove_.setBorder(Constants::getButtonBorder());
}


void Game::initShip()
{
    ship_.init(Constants::getShipWidth(),
               Constants::getShipHeight(),
               Constants::getShipFrictionForce(),
               Constants::getShipVelocityMax());
    ship_.setFramePositon(-Constants::getFieldSize() / 2.0f, -Constants::getFieldSize() / 2.0f);
    ship_.setFrameSize(Constants::getFieldSize(), Constants::getFieldSize());
}


void Game::generateAsteroid()
{
    if (tmrAsteroidNew_.isReady())
    {
        const unsigned index = asteroids_.size();
        asteroids_.resize(index + 1);
        asteroidsGenerator_.generate(asteroids_[index]);

        tmrAsteroidNew_.setAlarm(Constants::getTimeWaitAsteroidNew());
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
    const float angle = Constants::getShipAngularVelocity();
    const float speed = Constants::getShipVelocity();
    const float speedBullet = Constants::getBulletVelocity();

    if (buttons_[Constants::getButtonLeftIndex()].isPress() && !buttons_[Constants::getButtonRightIndex()].isPress()) {
        ship_.rotate(angle);
    }
    else if (!buttons_[Constants::getButtonLeftIndex()].isPress() && buttons_[Constants::getButtonRightIndex()].isPress()) {
        ship_.rotate(-angle);
    }
    
    if (buttons_[Constants::getButtonMoveIndex()].isPress()) {
        const Point d = ship_.getDirection();
        ship_.addVelocity(d.x * speed, d.y * speed);
    }

    if (buttons_[Constants::getButtonFireIndex()].isPress()) {
        if (tmrBullet_.isReady()) {
            const Point& startPosition = ship_.getBulletStartPosition();
            const Point& shipVelocity = ship_.getVelocity();
            const Point direction = ship_.getDirection();

            const unsigned index = bullets_.size();
            bullets_.resize(index + 1);
            Bullet& bullet = bullets_[index];
            bullet.init(Constants::getBulletSize());
            bullet.setPosition(startPosition);
            bullet.setVelocity(direction.x * speedBullet + shipVelocity.x,
                               direction.y * speedBullet + shipVelocity.y);

            tmrBullet_.setAlarm(Constants::getTimeWaitBullet());
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
            const float border = Constants::getFieldSize() / 2.0f + 2.0f * asteroids_[i].getRadiusMax();
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