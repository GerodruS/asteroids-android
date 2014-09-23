#include <jni.h>

#include "game.h"

#define  LOG_TAG    "libgl2jni"

Game game_;

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);

    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionDown(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionMove(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionUp(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height)
{
    game_.setupGraphics(width, height);
    game_.init();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    game_.step();
    game_.render();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionDown(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    //touchDown(id, x, y);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionMove(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    //touchMove(id, x, y);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionUp(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    //touchUp(id, x, y);
}

// OpenGL ES 2.0 code
/*
#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <time.h>
#include <map>

#include "asteroid.h"
#include "bullet.h"
#include "ship.h"
#include "squareButton.h"
#include "color.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


int screen_width, screen_height;
float game_width, game_height;

float widthFromScreenToGame(float value)
{
    return (value - screen_width / 2.0f) * game_width / screen_width;
}

float heightFromScreenToGame(float value)
{
    return (screen_height - value - screen_height / 2.0f) * game_height / screen_height;
}

std::vector<SquareButton> buttons;
bool isLandscape;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] =
   "uniform float ratio;\n\
    uniform float scaleX;\n\
    uniform float scaleY;\n\
    uniform float x_zero;\n\
    uniform float y_zero;\n\
    attribute vec4 vPosition;\n\
    attribute vec4 a_color;\n\
    varying vec4 v_color;\n\
    void main() {\n\
        v_color = a_color; \n\
        mat3 m_projection = mat3(     \n\
            vec3(ratio, 0.0, 0.0),    \n\
            vec3(  0.0, 1.0, 0.0),    \n\
            vec3(  0.0, 0.0, 1.0)     \n\
        );                            \n\
        mat3 m_scale = mat3(          \n\
            vec3(scaleX, 0.0, 0.0),    \n\
            vec3(  0.0, scaleY, 0.0),    \n\
            vec3(  0.0, 0.0, 1.0)     \n\
        );                            \n\
        vec3 m_move = vec3(-x_zero, -y_zero, 0.0); \n\
        gl_Position = vec4(m_projection * m_scale * (vPosition.xyz + m_move), 1.0); \n\
     }\n";

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = v_color;\n"
    //"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
    "}\n";

std::map<int, Point> touches;

GLuint loadShader(GLenum shaderType, const char* pSource) {
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
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
	std::string h = "hello";
	h.clear();

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
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
				char* buf = new char[bufLength];
				glGetProgramInfoLog(program, bufLength, NULL, buf);
				LOGE("Could not link program:\n%s\n", buf);
				delete [] buf;
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;
GLuint ratioID;
GLfloat ratioValue;
GLuint scaleXID;
GLfloat scaleXValue;
GLuint scaleYID;
GLfloat scaleYValue;
GLuint xzeroID;
GLfloat xzeroValue;
GLuint yzeroID;
GLfloat yzeroValue;
GLuint colorID;
GLfloat colorValue[] = {
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f
};

const GLfloat fieldSize = 1000.0f;

GLfloat gTriangleVertices[] = {
    0.0f, 0.0f,
    10.0f, 790.0f,
    1790.0f, 10.0f };

std::vector<AsteroidOld> asteroids;
std::vector<Bullet> bullets;
Ship ship;
time_t time_prev;

void setPoint(Point& point, float value, float radius)
{
    if (0.0f <= value && value < radius)
    {
        point.x = radius;
        point.y = value;
    }
    else if (radius <= value && value < 2.0f * radius)
    {
        point.x = value - radius;
        point.y = 0.0f;
    }
    else if (2.0f * radius <= value && value < 3.0f * radius)
    {
        point.x = 0.0f;
        point.y = value - 2.0f * radius;
    }
    else // (3.0f * radius <= p < 4.0f * radius)
    {
        point.x = value - 3.0f * radius;
        point.y = radius;
    }
}

void generateAsteroidOld()
{
    float radius = 500.0f;
    Point positionFrom, positionTo;
    float p = rand() % int(4.0f * radius);
    setPoint(positionFrom, p, radius);
    float d = p + 2.0f * radius + (rand() % int(2.0f * radius) - radius);
    if (4.0f * radius < d)
    {
        d -= 4.0f * radius;
    }
    setPoint(positionTo, d, radius);

    AsteroidOld asteroidNew_temp;
    int index = asteroids.size();
    asteroids.push_back(asteroidNew_temp);
    AsteroidOld& asteroidNew = asteroids[index];
    //asteroidNew.setPosition(positionFrom.x + (screen_width - radius) / 2.0f, positionFrom.y + (screen_height - radius) / 2.0f);
    asteroidNew.setPosition(positionFrom.x + 500 - radius / 2.0f, positionFrom.y + 500 - radius / 2.0f);

    positionTo.x -= positionFrom.x;
    positionTo.y -= positionFrom.y;
    const float velocityAsteroidOld = 2.0f;
    PointFunctions::normalize(positionTo, velocityAsteroidOld);

    //positionTo.x = 1;
    //positionTo.y = 1;
    asteroidNew.setVelocity(positionTo.x, positionTo.y);
}

bool setupGraphics(int w, int h)
{
    time(&time_prev);

    w = w < 0 ? -w : w;
    h = h < 0 ? -h : h;

    screen_width = w;
    screen_height = h;
    ratioValue = (float)h / (float)w;
    isLandscape = h < w;

    if (isLandscape)
    {
        game_height = 1000.0f;
        game_width = game_height / ratioValue;
    }
    else
    {
        game_width = 1000.0f;
        game_height = game_width * ratioValue;
    }

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    ratioID = glGetUniformLocation(gProgram, "ratio");
    checkGlError("glGetAttribLocation");

    scaleXValue = 2.0f / fieldSize;
    scaleXID = glGetUniformLocation(gProgram, "scaleX");
    checkGlError("glGetAttribLocation");

    scaleYValue = 2.0f / fieldSize;
    scaleYID = glGetUniformLocation(gProgram, "scaleY");
    checkGlError("glGetAttribLocation");

    xzeroValue = 0.0f; // fieldSize / 2.0f;
    xzeroID = glGetUniformLocation(gProgram, "x_zero");
    checkGlError("glGetAttribLocation");

    yzeroValue = 0.0f; // fieldSize / 2.0f;
    yzeroID = glGetUniformLocation(gProgram, "y_zero");
    checkGlError("glGetAttribLocation");

    colorID = glGetAttribLocation(gProgram, "a_color");
    checkGlError("glGetAttribLocation");

    gTriangleVertices[3] = fieldSize;
    gTriangleVertices[4] = fieldSize;

    //float ratio = (float)w / (float)h;
    //glFrustumx(mProjectionMatrix, 0, -ratio, ratio, -1, 1, 3, 7);
    
    ship.move(500, 500);

    buttons.resize(4);
    SquareButton& btnLeft = buttons[0];
    SquareButton& btnRight = buttons[1];
    SquareButton& btnFire = buttons[2];
    SquareButton& btnMove = buttons[3];
    if (isLandscape)
    {
        const float halfWidth = game_width / 2.0f;
        const float quarterHeight = game_height / 4.0f;

        btnLeft.setPosition(-halfWidth, -quarterHeight);
        btnLeft.setSize(quarterHeight, quarterHeight);

        btnRight.setPosition(-halfWidth + quarterHeight, -2.0f * quarterHeight);
        btnRight.setSize(quarterHeight, quarterHeight);

        btnFire.setPosition(halfWidth - quarterHeight, -quarterHeight);
        btnFire.setSize(quarterHeight, quarterHeight);

        btnMove.setPosition(halfWidth - 2.0f * quarterHeight, -2.0f * quarterHeight);
        btnMove.setSize(quarterHeight, quarterHeight);
    }
    else
    {
        const float halfHeight = game_height / 2.0f;
        const float quarterWidth = game_width / 4.0f;

        btnLeft.setPosition(-game_width / 2.0f, -halfHeight + quarterWidth);
        btnLeft.setSize(quarterWidth, quarterWidth);

        btnRight.setPosition(-quarterWidth, -halfHeight);
        btnRight.setSize(quarterWidth, quarterWidth);

        btnFire.setPosition(0.0f, -halfHeight + 500);
        btnFire.setSize(quarterWidth, quarterWidth);

        btnMove.setPosition(quarterWidth, -halfHeight + quarterWidth);
        btnMove.setSize(quarterWidth, quarterWidth);
    }

    return true;
}

const GLfloat gTriangleVertices2[] = {  0.5f,  0.5f,
                                      -0.5f, -0.5f,
                                      0.5f, -0.5f };

std::vector<GLfloat> points(6);

GLushort gVerticesIndexs[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    0, 7, 8,
    0, 8, 9,
    0, 9, 10,
    0, 10, 11,
    0, 11, 12,
    0, 12, 13,
    0, 13, 14,
    0, 14, 15,
    0, 15, 16,
    0, 16, 17,
    0, 17, 18,
    0, 18, 19,
    0, 19, 20
};

void drawAsteroidOld(std::vector<float>& modelpoints)
{
    int size = modelpoints.size();
    GLsizei count = size / 2 - 1;

    GLfloat* pnt = &(modelpoints[0]);
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, pnt);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");

    glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, colorValue);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(colorID);
    checkGlError("glEnableVertexAttribArray");

    gVerticesIndexs[count * 3 - 1] = 1;

    glDrawElements(GL_TRIANGLES, 3 * count, GL_UNSIGNED_SHORT, gVerticesIndexs);
    checkGlError("glDrawElements");

    gVerticesIndexs[count * 3 - 1] = count + 1;

    glDisableVertexAttribArray(gvPositionHandle);
    glDisableVertexAttribArray(colorID);
}

void drawTriangles(GLfloat* points, GLushort* indexes, GLfloat* colors, GLsizei countPoints)
{
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, points);
    glEnableVertexAttribArray(gvPositionHandle);

    glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(colorID);

    glDrawElements(GL_TRIANGLES, countPoints, GL_UNSIGNED_SHORT, indexes);
    checkGlError("glDrawElements");

    glDisableVertexAttribArray(gvPositionHandle);
    glDisableVertexAttribArray(colorID);
}

void drawButtons()
{
    const unsigned countButtons = buttons.size();
    for (unsigned i = 0; i < countButtons; ++i)
    {
        bool r = buttons[i].check(touches);
        buttons[i].setIsPress(r);
    }

    std::vector<Point> points(4 * countButtons);
    std::vector<GLushort> indexes(6 * countButtons);
    std::vector<Color> colors(4 * countButtons);
    
    for (unsigned i = 0; i < countButtons; ++i)
    {
        const Point& pos = buttons[i].getPosition();
        const Point& size = buttons[i].getSize();

        points[4 * i + 0] = pos;

        points[4 * i + 1] = pos;
        points[4 * i + 1].x += size.x;

        points[4 * i + 2] = pos;
        points[4 * i + 2].x += size.x;
        points[4 * i + 2].y += size.y;

        points[4 * i + 3] = pos;
        points[4 * i + 3].y += size.y;

        indexes[6 * i + 0] = 4 * i + 0;
        indexes[6 * i + 1] = 4 * i + 1;
        indexes[6 * i + 2] = 4 * i + 2;

        indexes[6 * i + 3] = 4 * i + 0;
        indexes[6 * i + 4] = 4 * i + 3;
        indexes[6 * i + 5] = 4 * i + 2;

        Color c = { 0.0f, 0.0f, 0.0f, 1.0f };
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
            colors[4 * i + j] = c;
        }
    }

    drawTriangles((GLfloat*)&(points[0]), &(indexes[0]), (GLfloat*)&(colors[0]), 6 * countButtons);
}


void rotate(float  x_in, float  y_in,
    float& x_out, float& y_out,
    float  angle)
{
    angle *= 0.0174532925f;

    x_out = x_in * cos(angle) - y_in * sin(angle);
    y_out = x_in * sin(angle) + y_in * cos(angle);
}


void rotate(float  x_in, float  y_in,
    float  x_center, float  y_center,
    float& x_out, float& y_out,
    float  angle)
{
    x_in -= x_center;
    y_in -= y_center;

    rotate(x_in, y_in, x_out, y_out, angle);

    x_out += x_center;
    y_out += y_center;
}


void rotate(std::vector<float>& point, float angle)
{
    int count = point.size();
    float x_center = point[count - 2];
    float y_center = point[count - 1];
    float x, y;
    for (int i = 0; i < count; i += 2)
    {
        rotate(point[i], point[i + 1], x_center, y_center, x, y, angle);
        point[i] = x;
        point[i + 1] = y;
    }
}


void rotate(Point& point, const Point& center, float angle)
{
    float x, y;
    rotate(point.x, point.y, center.x, center.y, x, y, angle);
    point.x = x;
    point.y = y;
}


void rotate(Point& point, float angle)
{
    float x, y;
    rotate(point.x, point.y, x, y, angle);
    point.x = x;
    point.y = y;
}

float time_cooldown = 3.0f;
float time_remain = 0.0f;

void renderFrame()
{
    time_t time_now = time(NULL);
    float deltaTime = difftime(time_now, time_prev);
    time_prev = time_now;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");
	
    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glUniform1f(ratioID, ratioValue);
    checkGlError("glUniform1f");

    glUniform1f(scaleXID, scaleXValue);
    checkGlError("glUniform1f");

    glUniform1f(scaleYID, scaleYValue);
    checkGlError("glUniform1f");

    glUniform1f(xzeroID, xzeroValue);
    checkGlError("glUniform1f");

    glUniform1f(yzeroID, yzeroValue);
    checkGlError("glUniform1f");

    for (int i = 0; i < asteroids.size(); ++i)
    {
        asteroids[i].step();

        float x = asteroids[i].getPositionX();
        float y = asteroids[i].getPositionY();
        float border = 100;
        if (x < -border || y < -border || 1000 + border < x || 1000 + border < y)
        {
            asteroids.erase(asteroids.begin() + i);
        }
        else
        {
            drawAsteroidOld(asteroids[i].points);
        }
    }
    for (int i = 0; i < bullets.size(); ++i)
    {
        bullets[i].step();
        float x = bullets[i].getPositionX();
        float y = bullets[i].getPositionY();
        float border = 100;
        if (x < -border || y < -border || 1000 + border < x || 1000 + border < y)
        {
            bullets.erase(bullets.begin() + i);
        }
        else
        {
            drawAsteroidOld(bullets[i].points);
        }
    }
    drawAsteroidOld(ship.points);
    ship.step();

    if (0.0f < time_remain)
    {
        time_remain -= deltaTime;
    }
    else
    {
        generateAsteroidOld();
        time_remain = time_cooldown;
    }

    // collisions
    for (int i = 0; i < bullets.size(); ++i)
    {
        bool del = false;
        for (int j = 0; j < asteroids.size(); ++j)
        {
            if (asteroids[j].bulletIntersect(bullets[i]))
            {
                bullets.erase(bullets.begin() + i);
                asteroids.erase(asteroids.begin() + j);
                del = true;
                break;
            }
        }
        if (del)
        {
            --i;
        }
    }
    
    //
    drawButtons();

    {
        float angle = 10.0f;
        float speed = 10.0f;
        if (buttons[0].isPress())
        {
            rotate(ship.points, angle);
            //Point center = ship.getCenter();
            rotate(ship.direction, angle);
        }
        else if (buttons[1].isPress())
        {
            rotate(ship.points, -angle);
            //Point center = ship.getCenter();
            rotate(ship.direction, -angle);
        }
        
        if (buttons[2].isPress())
        {
            ship.move(ship.direction.x * speed, ship.direction.y * speed);
        }
        
        if (buttons[3].isPress())
        {
            //ship.move(-ship.direction.x * speed, -ship.direction.y * speed);
            // fire
            Bullet bullet_tmp;
            bullets.push_back(bullet_tmp);
            Bullet& bullet = bullets[bullets.size() - 1];

            Point startPosition = ship.getBulletStartPosition();
            bullet.setPosition(startPosition.x, startPosition.y);
            Point velocity = ship.getBulletStartMove();
            bullet.setVelocity(velocity.x, velocity.y);
        }
    }
}


void touchDown(int id, float x, float y)
{
    Point p;
    p.x = widthFromScreenToGame(x);
    p.y = heightFromScreenToGame(y);
    touches[id] = p;
    
    //ship.move(x - (screen_width / 2.0), -(y - (screen_height / 2.0)));
}


void touchMove(int id, float x, float y)
{
    Point& p = touches[id];
    p.x = widthFromScreenToGame(x);
    p.y = heightFromScreenToGame(y);
}


void touchUp(int id, float x, float y)
{
    touches.erase(id);
}


extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);

    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionDown(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionMove(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionUp(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionDown(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    touchDown(id, x, y);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionMove(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    touchMove(id, x, y);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionUp(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    touchUp(id, x, y);
}
*/