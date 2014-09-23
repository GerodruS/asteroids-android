#include "painter.h"

#include "color.h"

using std::vector;


static const char gVertexShader[] =
    "uniform float ratio;\n"
    "uniform float scaleX;\n"
    "uniform float scaleY;\n"
    "uniform float x_zero;\n"
    "uniform float y_zero;\n"
    "attribute vec4 aPosition;\n"
    "attribute vec4 a_color;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "   v_color = a_color; \n"
    "   mat3 m_projection = mat3(      \n"
    "       vec3(ratio, 0.0, 0.0),     \n"
    "       vec3(  0.0, 1.0, 0.0),     \n"
    "       vec3(  0.0, 0.0, 1.0)      \n"
    "   );                             \n"
    "   mat3 m_scale = mat3(           \n"
    "       vec3(scaleX,    0.0, 0.0), \n"
    "       vec3(   0.0, scaleY, 0.0), \n"
    "       vec3(   0.0,    0.0, 1.0)  \n"
    "   );                             \n"
    "   vec3 m_move = vec3(-x_zero, -y_zero, 0.0); \n"
    "   gl_Position = vec4(m_projection * m_scale * (aPosition.xyz + m_move), 1.0); \n"
    "}\n";

static const char gFragmentShader[] =
    "precision mediump float;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = v_color;\n"
    "}\n";


Painter::Painter() :
    fieldSize_(1000.0f)
{
}


void Painter::setupGraphics(int w, int h)
{
    screenWidth_ = w;
    screenHeight_ = h;
    ratioValue_ = screenHeight_ / screenWidth_;
    isLandscape_ = screenHeight_ < screenWidth_;

    if (isLandscape_)
    {
        gameHeight_ = fieldSize_;
        gameWidth_ = gameHeight_ / ratioValue_;
    }
    else
    {
        gameWidth_ = fieldSize_;
        gameHeight_ = gameWidth_ * ratioValue_;
    }

    gProgram_ = createProgram(gVertexShader, gFragmentShader);
    if (0 == gProgram_)
    {
        return;
    }

    aPositionID_ = glGetAttribLocation(gProgram_, "aPosition");
    aColorID_ = glGetAttribLocation(gProgram_, "a_color");

    uRatioID_ = glGetUniformLocation(gProgram_, "ratio");
    uScaleXID_ = glGetUniformLocation(gProgram_, "scaleX");
    uScaleYID_ = glGetUniformLocation(gProgram_, "scaleY");
    uXzeroID_ = glGetUniformLocation(gProgram_, "x_zero");
    uYzeroID_ = glGetUniformLocation(gProgram_, "y_zero");

    glViewport(0, 0, screenWidth_, screenHeight_);
}


GLuint Painter::createProgram(const char* pVertexSource, const char* pFragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
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


GLuint Painter::loadShader(GLenum shaderType, const char* pSource)
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


void Painter::drawTriangles(GLfloat* points, GLushort* indexes, GLfloat* colors, GLsizei countPoints)
{
    glVertexAttribPointer(aPositionID_, 2, GL_FLOAT, GL_FALSE, 0, points);
    glEnableVertexAttribArray(aPositionID_);

    glVertexAttribPointer(aColorID_, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(aColorID_);

    glDrawElements(GL_TRIANGLES, countPoints, GL_UNSIGNED_SHORT, indexes);

    glDisableVertexAttribArray(aPositionID_);
    glDisableVertexAttribArray(aColorID_);
}


void Painter::drawPrepare()
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
    indexes_.reserve(3 * edgesCountSum);
    colors_.reserve(countAsteroids + edgesCountSum);

    const Color& c = colorRed;
    for (unsigned i = 0; i < countAsteroids; ++i)
    {
        const vector<Point>& ps = asterods[i].getPoints();
        points_.insert(points_.end(), ps.begin(), ps.end());

        const unsigned edgesCount = ps.size() - 1;
        for (unsigned j = 0; j < edgesCount + 1; ++j)
        {
            colors_.push_back(c);
        }
        for (unsigned j = 0; j < edgesCount; ++j)
        {
            indexes_.push_back(i);
            indexes_.push_back(i + j + 1);
            indexes_.push_back((i + j + 1) % (edgesCount - 1) + 1);
        }
    }

    drawTriangles((GLfloat*)&(points_[0]), &(indexes_[0]), (GLfloat*)&(colors_[0]), 3 * edgesCountSum);
}


void Painter::drawSquareButton(const vector<SquareButton>& buttons)
{    
    const unsigned countButtons = buttons.size();

    points_.resize(4 * countButtons);
    indexes_.resize(6 * countButtons);
    colors_.resize(4 * countButtons);

    Color c;
    for (unsigned i = 0; i < countButtons; ++i)
    {
        const Point& pos = buttons[i].getPosition();
        const Point& size = buttons[i].getSize();

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

        c = { 0.0f, 0.0f, 0.0f, 1.0f };
        if (buttons[i].isPress())
        {
            c.r = 1.0f;
        }
        else
        {
            c.b = 1.0f;
        }
        for (unsigned j = 0; j < 4; ++j)
        {
            colors_[4 * i + j] = c;
        }
    }

    drawTriangles((GLfloat*)&(points_[0]), &(indexes_[0]), (GLfloat*)&(colors_[0]), 6 * countButtons);
}