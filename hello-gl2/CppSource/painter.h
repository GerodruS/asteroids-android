#pragma once

#include <vector>
#include "asteroidNew.h"
#include "squareButton.h"
#include "color.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class Painter
{
public:
    Painter();

    void setupGraphics(int width, int height);

    void drawPrepare();
    void drawAsteroids(const std::vector<Asteroid>& asterods);
    void drawSquareButton(const std::vector<SquareButton>& buttons);

    float getScreenWidth() const
    {
        return screenWidth_;
    };
    float getScreenHeight() const
    {
        return screenHeight_;
    };
    float getGameWidth() const
    {
        return gameWidth_;
    };
    float getGameHeight() const
    {
        return gameHeight_;
    };
    float getRatioValue() const
    {
        return ratioValue_;
    };
    bool isLandscape() const
    {
        return isLandscape_;
    };

private:
    std::vector<Point> points_;
    std::vector<GLushort> indexes_;
    std::vector<Color> colors_;

    //
    GLuint gProgram_;
    GLuint aPositionID_;
    GLuint aColorID_;
    GLuint uRatioID_;
    GLuint uScaleXID_;
    GLuint uScaleYID_;
    GLuint uXzeroID_;
    GLuint uYzeroID_;
    //

    float fieldSize_;
    float screenWidth_;
    float screenHeight_;
    float gameWidth_;
    float gameHeight_;
    float ratioValue_;

    bool isLandscape_;

    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
    GLuint loadShader(GLenum shaderType, const char* pSource);
    void drawTriangles(GLfloat* points, GLushort* indexes, GLfloat* colors, GLsizei countPoints);

};
