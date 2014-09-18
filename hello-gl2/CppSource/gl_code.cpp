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
    void main() {\n\
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

/*
static const char gVertexShader[] =
   "attribute vec4 vPosition;          \n\
    void main() {                      \n\
        const mat3 projection = mat3(  \n\
            vec3(1.0 / 1.0, 0.0, 0.0), \n\
            vec3(0.0, 1.0, 0.0),       \n\
            vec3(0.0, 0.0, 1.0)        \n\
        );                             \n\
        gl_Position = vec4(projection * position.xyz, 1.0); \n\
        gl_Position = vPosition; \n\
    }\n";
*/

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
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

/*
const struct MyScreen
{
    const float width = 800.0f;
    const float height = 800.0f;

} myScreen;
*/

bool setupGraphics(int w, int h)
{
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

    scaleXValue = 2.0f / h;
    scaleXID = glGetUniformLocation(gProgram, "scaleX");
    checkGlError("glGetAttribLocation");

    scaleYValue = 2.0f / h;
    scaleYID = glGetUniformLocation(gProgram, "scaleY");
    checkGlError("glGetAttribLocation");

    xzeroValue = w / 2.0f;
    xzeroID = glGetUniformLocation(gProgram, "x_zero");
    checkGlError("glGetAttribLocation");

    yzeroValue = h / 2.0f;
    yzeroID = glGetUniformLocation(gProgram, "y_zero");
    checkGlError("glGetAttribLocation");

    //float ratio = (float)w / (float)h;
    //glFrustumx(mProjectionMatrix, 0, -ratio, ratio, -1, 1, 3, 7);

    return true;
}

const GLfloat gTriangleVertices2[] = {  0.5f,  0.5f,
                                      -0.5f, -0.5f,
                                      0.5f, -0.5f };

const GLfloat gTriangleVertices[] = {
 200.0f, 200.0f,
 0.0f, 1200.0f,
 800.0f, 0.0f };

void renderFrame()
{
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    //glClearColor(grey, grey, grey, 1.0f);
	//glClearColor((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, 1.0f);
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

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_POLYGON_OFFSET_FILL, 0, 3);
    checkGlError("glDrawArrays");
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}
