#include "constants.h"


static const float FIELD_SIZE = 1000.0f;
static const float TIME_WAIT_ASTEROID_NEW = 2.0f;
static const float TIME_WAIT_BULLET = 0.5f;
static const float TIME_NEW_GAME = 1.0f;

static const float BULLET_SIZE = 10.0f;

static const float SHIP_WIDTH = 40.0f;
static const float SHIP_HEIGHT = 50.0f;
static const float SHIP_FRICTION_FORCE = 0.01f;
static const float SHIP_VELOCITY_MAX = 4.0f;

static const int   GENERATOR_EDGE_COUNT_MIN = 6;
static const int   GENERATOR_EDGE_COUNT_MAX = 12;
static const float GENERATOR_RADIUS_MIN = 30.0f;
static const float GENERATOR_RADIUS_MAX = 90.0f;
static const float GENERATOR_VELOCITY_MIN = 0.5f;
static const float GENERATOR_VELOCITY_MAX = 3.0f;

static const unsigned BUTTON_LEFT  = 0;
static const unsigned BUTTON_RIGHT = 1;
static const unsigned BUTTON_MOVE  = 2;
static const unsigned BUTTON_FIRE  = 3;
static const float    BUTTON_BORDER = 0.3f;

static const float CONTROLL_BULLET_VELOCITY = 6.0f;
static const float CONTROLL_SHIP_VELOCITY = 2.0f;
static const float CONTROLL_SHIP_ANGULAR_VELOCITY = 3.0f;


float Constants::getFieldSize()
{
    return FIELD_SIZE;
}


float Constants::getTimeWaitAsteroidNew()
{
    return TIME_WAIT_ASTEROID_NEW;
}


float Constants::getTimeWaitBullet()
{
    return TIME_WAIT_BULLET;
}


float Constants::getTimeNewGame()
{
    return TIME_NEW_GAME;
}


float Constants::getBulletSize()
{
    return BULLET_SIZE;
}


float Constants::getShipWidth()
{
    return SHIP_WIDTH;
}


float Constants::getShipHeight()
{
    return SHIP_HEIGHT;
}


float Constants::getShipFrictionForce()
{
    return SHIP_FRICTION_FORCE;
}


float Constants::getShipVelocityMax()
{
    return SHIP_VELOCITY_MAX;
}


int Constants::getGeneratorEdgeCountMin()
{
    return GENERATOR_EDGE_COUNT_MIN;
}


int Constants::getGeneratorEdgeCountMax()
{
    return GENERATOR_EDGE_COUNT_MAX;
}


float Constants::getGeneratorRadiusMin()
{
    return GENERATOR_RADIUS_MIN;
}


float Constants::getGeneratorRadiusMax()
{
    return GENERATOR_RADIUS_MAX;
}


float Constants::getGeneratorVelocityMin()
{
    return GENERATOR_VELOCITY_MIN;
}


float Constants::getGeneratorVelocityMax()
{
    return GENERATOR_VELOCITY_MAX;
}


unsigned Constants::getButtonLeftIndex()
{
    return BUTTON_LEFT;
}


unsigned Constants::getButtonRightIndex()
{
    return BUTTON_RIGHT;
}


unsigned Constants::getButtonMoveIndex()
{
    return BUTTON_MOVE;
}


unsigned Constants::getButtonFireIndex()
{
    return BUTTON_FIRE;
}


float Constants::getButtonBorder()
{
    return BUTTON_BORDER;
}


float Constants::getBulletVelocity()
{
    return CONTROLL_BULLET_VELOCITY;
}


float Constants::getShipVelocity()
{
    return CONTROLL_SHIP_VELOCITY;
}


float Constants::getShipAngularVelocity()
{
    return CONTROLL_SHIP_ANGULAR_VELOCITY;
}