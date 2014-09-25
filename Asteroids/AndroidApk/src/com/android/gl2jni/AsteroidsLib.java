package com.android.asteroids;


public class AsteroidsLib
{
     static {
         System.loadLibrary("Asteroids");
     }

     public static native void init(int width, int height);
     public static native void step();

     //  touches
     public static native void actionDown(int touchID, float x, float y);
     public static native void actionMove(int touchID, float x, float y);
     public static native void actionUp(int touchID, float x, float y);
}
