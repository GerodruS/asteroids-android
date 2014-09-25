#include <jni.h>

#include "game.h"

#define  LOG_TAG    "libAsteroids"

Game game_;

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_init(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_step(JNIEnv * env, jobject obj);

    JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_actionDown(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_actionMove(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_actionUp(JNIEnv * env, jobject obj, jint tid, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_init(JNIEnv * env, jobject obj, jint width, jint height)
{
    game_.setupGraphics(width, height);
    game_.init();
}

JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_step(JNIEnv * env, jobject obj)
{
    game_.step();
    game_.render();
}

JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_actionDown(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    game_.touchDown(id, x, y);
}

JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_actionMove(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    game_.touchMove(id, x, y);
}

JNIEXPORT void JNICALL Java_com_android_asteroids_AsteroidsLib_actionUp(JNIEnv * env, jobject obj, jint tid, jfloat jx, jfloat jy)
{
    int id = tid;
    float x = jx;
    float y = jy;
    game_.touchUp(id, x, y);
}