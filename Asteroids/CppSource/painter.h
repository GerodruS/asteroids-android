#pragma once

#include <vector>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "asteroid.h"
#include "squareButton.h"
#include "ship.h"
#include "bullet.h"
#include "color.h"


class Painter
{
public:
    Painter();

    void setupGraphics(const float fieldSize,
                       const int width,
                       const int height);

    float getFieldSize() const;
    float getScreenWidth() const;
    float getScreenHeight() const;
    float getGameWidth() const;
    float getGameHeight() const;
    float getRatioValue() const;

    float getXFromScreenToGame(const float value) const;
    float getYFromScreenToGame(const float value) const;
    float getXFromGameToScreen(const float value) const;
    float getYFromGameToScreen(const float value) const;
    float getWidthFromGameToScreen(const float value) const;
    float getHeightFromGameToScreen(const float value) const;

    void setFieldSize(const float value);

    void drawPrepare() const;
    void drawScissorEnable() const;
    void drawScissorDisable() const;

    void drawAsteroids(const std::vector<Asteroid>& asterods);
    void drawBullets(const std::vector<Bullet>& bullets);
    void drawShip(const Ship& ship);
    void drawSquareButton(const std::vector<SquareButton>& buttons);

private:
    std::vector<Point> points_;
    std::vector<GLushort> indexes_;
    std::vector<Color> colors_;

    GLuint gProgram_;
    GLuint aPositionID_;
    GLuint aColorID_;
    GLuint uRatioID_;
    GLuint uScaleXID_;
    GLuint uScaleYID_;
    GLuint uXzeroID_;
    GLuint uYzeroID_;

    float fieldSize_;
    float screenWidth_;
    float screenHeight_;
    float gameWidth_;
    float gameHeight_;
    float ratioValue_;


    GLuint createProgram(const char* vertexSource,
                         const char* fragmentSource);
    GLuint loadShader(const GLenum shaderType, const char* pSource);
    void drawTriangles(const GLfloat* points,
                       const GLushort* indexes,
                       const GLfloat* colors,
                       const GLsizei countPoints);

};
