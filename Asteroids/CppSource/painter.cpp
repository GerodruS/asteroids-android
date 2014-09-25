#include "painter.h"

#include "color.h"


using std::vector;


static const char gVertexShader[] =
    "uniform float uRatio;      \n"
    "uniform float uScaleX;     \n"
    "uniform float uScaleY;     \n"
    "uniform float uZeroX;      \n"
    "uniform float uZeroY;      \n"

    "attribute vec4 aPosition;  \n"
    "attribute vec4 aColor;     \n"

    "varying vec4 vColor;       \n"

    "void main() {                      \n"
    "   vColor = aColor;                \n"

    "   mat3 mProjection = mat3(        \n"
    "       vec3(uRatio, 0.0, 0.0),     \n"
    "       vec3(   0.0, 1.0, 0.0),     \n"
    "       vec3(   0.0, 0.0, 1.0)      \n"
    "   );                              \n"

    "   mat3 mScale = mat3(             \n"
    "       vec3(uScaleX,    0.0, 0.0), \n"
    "       vec3(   0.0, uScaleY, 0.0), \n"
    "       vec3(   0.0,    0.0, 1.0)   \n"
    "   );                              \n"

    "   vec3 vMove = vec3(-uZeroX, -uZeroY, 0.0); \n"
    "   gl_Position = vec4(mProjection * mScale * (aPosition.xyz + vMove), 1.0); \n"
    "}\n";


static const char gFragmentShader[] =
    "precision mediump float; \n"
    "varying vec4 vColor;     \n"
    "void main() {            \n"
    "  gl_FragColor = vColor; \n"
    "}                        \n";


Painter::Painter() :
    gProgram_(0),
    aPositionID_(0),
    aColorID_(0),
    uRatioID_(0),
    uScaleXID_(0),
    uScaleYID_(0),
    uXzeroID_(0),
    uYzeroID_(0),
    
    fieldSize_(0.0f),
    screenWidth_(0.0f),
    screenHeight_(0.0f),
    gameWidth_(0.0f),
    gameHeight_(0.0f),
    ratioValue_(0.0f)
{
}


void Painter::setupGraphics(const float fieldSize,
                            const int w,
                            const int h)
{
    setFieldSize(fieldSize);

    screenWidth_ = w;
    screenHeight_ = h;
    ratioValue_ = screenHeight_ / screenWidth_;

    gameHeight_ = fieldSize_;
    gameWidth_ = gameHeight_ / ratioValue_;

    gProgram_ = createProgram(gVertexShader, gFragmentShader);
    if (0 == gProgram_) {
        return;
    }

    aPositionID_ = glGetAttribLocation(gProgram_, "aPosition");
    aColorID_    = glGetAttribLocation(gProgram_, "aColor");

    uRatioID_  = glGetUniformLocation(gProgram_, "uRatio");
    uScaleXID_ = glGetUniformLocation(gProgram_, "uScaleX");
    uScaleYID_ = glGetUniformLocation(gProgram_, "uScaleY");
    uXzeroID_  = glGetUniformLocation(gProgram_, "uZeroX");
    uYzeroID_  = glGetUniformLocation(gProgram_, "uZeroY");

    glViewport(0, 0, screenWidth_, screenHeight_);
}


float Painter::getFieldSize() const
{
    return fieldSize_;
}


float Painter::getScreenWidth() const
{
    return screenWidth_;
}


float Painter::getScreenHeight() const
{
    return screenHeight_;
}


float Painter::getGameWidth() const
{
    return gameWidth_;
}


float Painter::getGameHeight() const
{
    return gameHeight_;
}


float Painter::getRatioValue() const
{
    return ratioValue_;
}


float Painter::getXFromScreenToGame(const float value) const
{
    const float sw = getScreenWidth();
    return (value - sw / 2.0f) * getGameWidth() / sw;
}


float Painter::getYFromScreenToGame(const float value) const
{
    const float sh = getScreenHeight();
    return (sh - value - sh / 2.0f) * getGameHeight() / sh;
}


float Painter::getXFromGameToScreen(const float value) const
{
    const float sw = getScreenWidth();
    return value * sw / getGameWidth() + sw / 2.0f;
}


float Painter::getYFromGameToScreen(const float value) const
{
    const float sh = getScreenHeight();
    return value * sh / getGameHeight() + sh / 2.0f;
}


float Painter::getWidthFromGameToScreen(const float value) const
{
    return value * getScreenWidth() / getGameWidth();
}


float Painter::getHeightFromGameToScreen(const float value) const
{
    return value * getScreenHeight() / getGameHeight();
}


void Painter::setFieldSize(const float value)
{
    fieldSize_ = value;
}


void Painter::drawPrepare() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gProgram_);
    glUniform1f(uRatioID_, ratioValue_);
    glUniform1f(uScaleXID_, 2.0f / fieldSize_);
    glUniform1f(uScaleYID_, 2.0f / fieldSize_);
    glUniform1f(uXzeroID_, 0.0f);
    glUniform1f(uYzeroID_, 0.0f);
}


void Painter::drawScissorEnable() const
{
    glScissor(getXFromGameToScreen(-fieldSize_ / 2.0f),
        getYFromGameToScreen(-fieldSize_ / 2.0f),
        getWidthFromGameToScreen(fieldSize_ / 1.0f),
        getHeightFromGameToScreen(fieldSize_ / 1.0f));
    glEnable(GL_SCISSOR_TEST);
}


void Painter::drawScissorDisable() const
{
    glDisable(GL_SCISSOR_TEST);
}


void Painter::drawAsteroids(const vector<Asteroid>& asterods)
{    
    const unsigned countAsteroids = asterods.size();

    unsigned edgesCountSum = 0;
    for (unsigned i = 0; i < countAsteroids; ++i)
    {
        edgesCountSum += asterods[i].getPoints().size() - 1;
    }

    points_.clear();
    points_.reserve(countAsteroids + edgesCountSum);

    indexes_.clear();
    indexes_.reserve(3 * edgesCountSum);

    colors_.clear();
    colors_.reserve(countAsteroids + edgesCountSum);

    unsigned startIndex = 0;
    for (unsigned i = 0; i < countAsteroids; ++i) {
        const vector<Point>& ps = asterods[i].getPoints();
        points_.insert(points_.end(), ps.begin(), ps.end());

        const unsigned edgesCount = ps.size() - 1;
        for (unsigned j = 0; j < edgesCount + 1; ++j) {
            colors_.push_back(colorAsteroids);
        }
        for (unsigned j = 1; j <= edgesCount; ++j) {
            indexes_.push_back(startIndex);
            indexes_.push_back(startIndex + j);
            indexes_.push_back(startIndex + j % edgesCount + 1);
        }
        startIndex += edgesCount + 1;
    }

    drawTriangles((GLfloat*)&(points_[0]), &(indexes_[0]), (GLfloat*)&(colors_[0]), 3 * edgesCountSum);
}


void Painter::drawBullets(const std::vector<Bullet>& bullets)
{
    const unsigned countBullets = bullets.size();

    points_.clear();
    points_.reserve(4 * countBullets);
    indexes_.resize(6 * countBullets);
    colors_.resize(4 * countBullets);

    for (unsigned i = 0; i < countBullets; ++i) {
        const vector<Point>& ps = bullets[i].getPoints();
        points_.insert(points_.end(), ps.begin(), ps.end());

        indexes_[i * 6 + 0] = 4 * i; indexes_[i * 6 + 1] = 4 * i + 1; indexes_[i * 6 + 2] = 4 * i + 2;
        indexes_[i * 6 + 3] = 4 * i; indexes_[i * 6 + 4] = 4 * i + 2; indexes_[i * 6 + 5] = 4 * i + 3;

        for (unsigned j = 0; j < 4; ++j) {
            colors_[i * 4 + j] = colorBullet;
        }
    }

    drawTriangles((GLfloat*)&(points_[0]), &(indexes_[0]), (GLfloat*)&(colors_[0]), 6 * countBullets);
}


void Painter::drawShip(const Ship& ship)
{
    const vector<Point>& ps = ship.getPoints();

    points_.clear();
    points_.insert(points_.end(), ps.begin(), ps.end());

    indexes_.resize(6);
    indexes_[0] = 0; indexes_[1] = 1; indexes_[2] = 2;
    indexes_[3] = 0; indexes_[4] = 2; indexes_[5] = 3;

    colors_.resize(4);
    for (unsigned i = 0; i < 4; ++i) {
        colors_[i] = colorShip;
    }

    drawTriangles((GLfloat*)&(points_[0]), &(indexes_[0]), (GLfloat*)&(colors_[0]), 6);
}


void Painter::drawSquareButton(const vector<SquareButton>& buttons)
{
    float border = 0.2f;

    const unsigned countButtons = buttons.size();

    points_.resize(4 * countButtons);
    indexes_.resize(6 * countButtons);
    colors_.resize(4 * countButtons);

    for (unsigned i = 0; i < countButtons; ++i) {
        Point pos = buttons[i].getPosition();
        Point size = buttons[i].getSize();

        pos.x += size.x * border / 2.0f;
        pos.y += size.y * border / 2.0f;
        size.x *= 1.0f - border;
        size.y *= 1.0f - border;

        points_[4 * i + 0] = pos;

        points_[4 * i + 1] = pos;
        points_[4 * i + 1].x += size.x;

        points_[4 * i + 2] = pos;
        points_[4 * i + 2].x += size.x;
        points_[4 * i + 2].y += size.y;

        points_[4 * i + 3] = pos;
        points_[4 * i + 3].y += size.y;

        indexes_[6 * i + 0] = 4 * i + 0;
        indexes_[6 * i + 1] = 4 * i + 1;
        indexes_[6 * i + 2] = 4 * i + 2;

        indexes_[6 * i + 3] = 4 * i + 0;
        indexes_[6 * i + 4] = 4 * i + 3;
        indexes_[6 * i + 5] = 4 * i + 2;

        if (buttons[i].isPress()) {
            for (unsigned j = 0; j < 4; ++j) {
                colors_[4 * i + j] = colorButtonPressed;
            }
        }
        else {
            for (unsigned j = 0; j < 4; ++j) {
                colors_[4 * i + j] = colorButtonUnpressed;
            }
        }

    }

    drawTriangles((GLfloat*)&(points_[0]), &(indexes_[0]), (GLfloat*)&(colors_[0]), 6 * countButtons);
}


GLuint Painter::createProgram(const char* vertexSource,
                              const char* fragmentSource)
{
    const GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        return 0;
    }

    const GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, pixelShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = new char[bufLength];
                glGetProgramInfoLog(program, bufLength, NULL, buf);
                delete[] buf;
            }
            glDeleteProgram(program);
            program = 0;
        }
    }

    return program;
}


GLuint Painter::loadShader(const GLenum shaderType, const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = new char[infoLen];
                glGetShaderInfoLog(shader, infoLen, NULL, buf);
                delete[] buf;
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}


void Painter::drawTriangles(const GLfloat*  points,
                            const GLushort* indexes,
                            const GLfloat*  colors,
                            const GLsizei   countPoints)
{
    glVertexAttribPointer(aPositionID_, 2, GL_FLOAT, GL_FALSE, 0, points);
    glEnableVertexAttribArray(aPositionID_);

    glVertexAttribPointer(aColorID_, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(aColorID_);

    glDrawElements(GL_TRIANGLES, countPoints, GL_UNSIGNED_SHORT, indexes);

    glDisableVertexAttribArray(aPositionID_);
    glDisableVertexAttribArray(aColorID_);
}