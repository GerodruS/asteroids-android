#pragma once

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

static const Color colorBlack = { 0.0f, 0.0f, 0.0f, 1.0f };
static const Color colorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
static const Color colorGrey  = { 0.5f, 0.5f, 0.5f, 1.0f };

static const Color colorRed   = { 1.0f, 0.0f, 0.0f, 1.0f };
static const Color colorGreen = { 0.0f, 1.0f, 0.0f, 1.0f };
static const Color colorBlue  = { 0.0f, 0.0f, 1.0f, 1.0f };

static const Color colorYellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
static const Color colorPurple  = { 1.0f, 0.0f, 1.0f, 1.0f };
static const Color colorCyan    = { 0.0f, 1.0f, 1.0f, 1.0f };

static const Color colorBack            = colorBlack;
static const Color colorAsteroids       = colorCyan;
static const Color colorBullet          = { 0.749f,  1.0f,    0.0f,    1.0f };
static const Color colorShip            = { 1.0f,    0.4157f, 0.0f,    1.0f };
static const Color colorButtonPressed   = { 0.749f,  0.0f,    1.0f,    1.0f };
static const Color colorButtonUnpressed = { 1.0f,    0.0f,    0.4157f, 1.0f };