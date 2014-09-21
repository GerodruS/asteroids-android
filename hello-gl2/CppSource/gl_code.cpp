/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

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

#include "asteroid.h"
#include "bullet.h"
#include "ship.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

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
/*
const struct MyScreen
{
    const float width = 800.0f;
    const float height = 800.0f;

} myScreen;
*/

GLfloat gTriangleVertices[] = {
    0.0f, 0.0f,
    10.0f, 790.0f,
    1790.0f, 10.0f };

std::vector<Asteroid> asteroids;
std::vector<Bullet> bullets;
Ship ship;
int screen_width;
int screen_height;
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

void generateAsteroid()
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

    Asteroid asteroidNew_temp;
    int index = asteroids.size();
    asteroids.push_back(asteroidNew_temp);
    Asteroid& asteroidNew = asteroids[index];
    //asteroidNew.setPosition(positionFrom.x + (screen_width - radius) / 2.0f, positionFrom.y + (screen_height - radius) / 2.0f);
    asteroidNew.setPosition(positionFrom.x + 500 - radius / 2.0f, positionFrom.y + 500 - radius / 2.0f);

    positionTo.x -= positionFrom.x;
    positionTo.y -= positionFrom.y;
    const float velocityAsteroid = 2.0f;
    PointFunctions::normalize(positionTo, velocityAsteroid);

    //positionTo.x = 1;
    //positionTo.y = 1;
    asteroidNew.setVelocity(positionTo.x, positionTo.y);
}

bool setupGraphics(int w, int h)
{
    time(&time_prev);

    screen_width = w;
    screen_height = h;

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

    ratioValue = (float)h / (float)w;
    ratioID = glGetUniformLocation(gProgram, "ratio");
    checkGlError("glGetAttribLocation");

    scaleXValue = 2.0f / fieldSize;
    scaleXID = glGetUniformLocation(gProgram, "scaleX");
    checkGlError("glGetAttribLocation");

    scaleYValue = 2.0f / fieldSize;
    scaleYID = glGetUniformLocation(gProgram, "scaleY");
    checkGlError("glGetAttribLocation");

    xzeroValue = fieldSize / 2.0f;
    xzeroID = glGetUniformLocation(gProgram, "x_zero");
    checkGlError("glGetAttribLocation");

    yzeroValue = fieldSize / 2.0f;
    yzeroID = glGetUniformLocation(gProgram, "y_zero");
    checkGlError("glGetAttribLocation");

    colorID = glGetAttribLocation(gProgram, "a_color");
    checkGlError("glGetAttribLocation");

    gTriangleVertices[3] = fieldSize;
    gTriangleVertices[4] = fieldSize;

    //float ratio = (float)w / (float)h;
    //glFrustumx(mProjectionMatrix, 0, -ratio, ratio, -1, 1, 3, 7);

    /*
    asteroids.push_back(Asteroid(6));
    asteroids.push_back(Asteroid(6));
    asteroids.push_back(Asteroid(6));
    asteroids.push_back(Asteroid(6));

    asteroids[0].move( 300.0f,  200.0f);
    asteroids[1].move(-300.0f,  200.0f);
    asteroids[2].move( 300.0f, -200.0f);
    asteroids[3].move(-300.0f, -200.0f);
    */

    ship.move(500, 500);
    /*
    for (int i = 0; i < 100; ++i)
    {
        generateAsteroid();
    }
    */

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

void drawAsteroid(std::vector<float>& modelpoints)
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

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    gVerticesIndexs[count * 3 - 1] = 1;

    glDrawElements(GL_TRIANGLES, 3 * count, GL_UNSIGNED_SHORT, gVerticesIndexs);
    checkGlError("glDrawElements");

    gVerticesIndexs[count * 3 - 1] = count + 1;

    glDisableVertexAttribArray(gvPositionHandle);
    glDisableVertexAttribArray(colorID);

    /*
    int size = modelpoints.size();
    GLsizei count = size / 2 - 1;

    points[4] = modelpoints[size - 2];
    points[5] = modelpoints[size - 1];

    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            points[j] = modelpoints[(j + i * 2) % (size - 2)];
        }
        
        switch (i % 6)
        {
        case 0:
            colorValue[0] = colorValue[4] = colorValue[8] = 1.0f;
            colorValue[1] = colorValue[5] = colorValue[9] = 0.0f;
            colorValue[2] = colorValue[6] = colorValue[10] = 0.0f;
            colorValue[3] = colorValue[7] = colorValue[11] = 1.0f;
            break;
        case 1:
            colorValue[0] = colorValue[4] = colorValue[8] = 0.0f;
            colorValue[1] = colorValue[5] = colorValue[9] = 1.0f;
            colorValue[2] = colorValue[6] = colorValue[10] = 0.0f;
            colorValue[3] = colorValue[7] = colorValue[11] = 1.0f;
            break;
        case 2:
            colorValue[0] = colorValue[4] = colorValue[8] = 0.0f;
            colorValue[1] = colorValue[5] = colorValue[9] = 0.0f;
            colorValue[2] = colorValue[6] = colorValue[10] = 1.0f;
            colorValue[3] = colorValue[7] = colorValue[11] = 1.0f;
            break;
        case 3:
            colorValue[0] = colorValue[4] = colorValue[8] = 1.0f;
            colorValue[1] = colorValue[5] = colorValue[9] = 1.0f;
            colorValue[2] = colorValue[6] = colorValue[10] = 0.0f;
            colorValue[3] = colorValue[7] = colorValue[11] = 1.0f;
            break;
        case 4:
            colorValue[0] = colorValue[4] = colorValue[8] = 1.0f;
            colorValue[1] = colorValue[5] = colorValue[9] = 0.0f;
            colorValue[2] = colorValue[6] = colorValue[10] = 1.0f;
            colorValue[3] = colorValue[7] = colorValue[11] = 1.0f;
            break;
        case 5:
            colorValue[0] = colorValue[4] = colorValue[8] = 0.0f;
            colorValue[1] = colorValue[5] = colorValue[9] = 1.0f;
            colorValue[2] = colorValue[6] = colorValue[10] = 1.0f;
            colorValue[3] = colorValue[7] = colorValue[11] = 1.0f;
            break;
        default:
            colorValue[0] = colorValue[4] = colorValue[8] = 1.0f;
            colorValue[1] = colorValue[5] = colorValue[9] = 1.0f;
            colorValue[2] = colorValue[6] = colorValue[10] = 1.0f;
            colorValue[3] = colorValue[7] = colorValue[11] = 1.0f;
            break;
        }
        
        GLfloat* pnt = &(points[0]);
        glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, pnt);
        checkGlError("glVertexAttribPointer");
        glEnableVertexAttribArray(gvPositionHandle);
        checkGlError("glEnableVertexAttribArray");

        glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, colorValue);
        checkGlError("glVertexAttribPointer");
        glEnableVertexAttribArray(colorID);
        checkGlError("glEnableVertexAttribArray");

        glDrawArrays(GL_TRIANGLES, 0, 3);
        checkGlError("glDrawArrays");

        glDisableVertexAttribArray(gvPositionHandle);
        glDisableVertexAttribArray(colorID);
    }
    */
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
            drawAsteroid(asteroids[i].points);
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
            drawAsteroid(bullets[i].points);
        }
    }
    drawAsteroid(ship.points);
    ship.step();

    if (0.0f < time_remain)
    {
        time_remain -= deltaTime;
    }
    else
    {
        generateAsteroid();
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
    /*
    for (int i = 0; i < asteroids.size(); ++i)
    {
        bool del = false;
        for (int j = i + 1; j < asteroids.size(); ++j)
        {
            if (asteroids[i].asteroidIntersect(asteroids[j]))
            {
                generateAsteroid();
                generateAsteroid();

                asteroids.erase(asteroids.begin() + i);
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
    */
    //
}


void rotate(float  x_in, float  y_in,
    float& x_out, float& y_out,
    float  angle)
{
    angle *= 0.0174532925f;

    x_out = x_in * cos(angle) - y_in * sin(angle);
    y_out = x_in * sin(angle) + y_in * cos(angle);
}


void rotate(float  x_in,     float  y_in,
            float  x_center, float  y_center,
            float& x_out,    float& y_out,
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


void touchDown(float x, float y)
{
    y = screen_height - y;
    if (0 <= y && y < 300)
    {
        float angle = 10.0f;
        float speed = 10.0f;
        if (0 <= x && x < 300)
        {
            rotate(ship.points, angle);
            //Point center = ship.getCenter();
            rotate(ship.direction, angle);
        }
        else if (300 <= x && x < 600)
        {
            rotate(ship.points, -angle);
            //Point center = ship.getCenter();
            rotate(ship.direction, -angle);
        }
        else if (600 <= x && x < 900)
        {
            ship.move(ship.direction.x * speed, ship.direction.y * speed);
        }
        else if (900 <= x && x < 1200)
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
    //ship.move(x - (screen_width / 2.0), -(y - (screen_height / 2.0)));
}


void touchMove(float x, float y)
{
}


void touchUp(float x, float y)
{
    //y = screen_height / 2.0 - y;
    /*
    if (0 <= y && y < 300)
    {
        if (0 <= x && x < 300)
        {
            ship.move(-10, 0);
        }
        else if (300 <= x && x < 600)
        {
            ship.move(10, 0);
        }
        else if (600 <= x && x < 900)
        {
            ship.move(0, -10);
        }
        else if (900 <= x && x < 1200)
        {
            ship.move(0, 10);
        }
    }
    */
    //ship.move(-(x - (screen_width / 2.0)), y - (screen_height / 2.0));
}


extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);

    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionDown(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionMove(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionUp(JNIEnv * env, jobject obj, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionDown(JNIEnv * env, jobject obj, jfloat jx, jfloat jy)
{
    float x = jx;
    float y = jy;
    touchDown(x, y);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionMove(JNIEnv * env, jobject obj, jfloat jx, jfloat jy)
{
    float x = jx;
    float y = jy;
    touchMove(x, y);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_actionUp(JNIEnv * env, jobject obj, jfloat jx, jfloat jy)
{
    float x = jx;
    float y = jy;
    touchUp(x, y);
}
