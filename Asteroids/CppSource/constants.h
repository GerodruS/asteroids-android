#pragma once


class Constants
{
public:
    static float getFieldSize();
    static float getTimeWaitAsteroidNew();
    static float getTimeWaitBullet();
    static float getTimeNewGame();

    static float getBulletSize();

    static float getShipWidth();
    static float getShipHeight();
    static float getShipFrictionForce();
    static float getShipVelocityMax();

    static int getGeneratorEdgeCountMin();
    static int getGeneratorEdgeCountMax();
    static float getGeneratorRadiusMin();
    static float getGeneratorRadiusMax();
    static float getGeneratorVelocityMin();
    static float getGeneratorVelocityMax();

    static unsigned getButtonLeftIndex();
    static unsigned getButtonRightIndex();
    static unsigned getButtonMoveIndex();
    static unsigned getButtonFireIndex();
    static float getButtonBorder();

    static float getBulletVelocity();
    static float getShipVelocity();
    static float getShipAngularVelocity();

};
