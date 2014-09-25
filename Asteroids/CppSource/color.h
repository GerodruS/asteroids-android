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
static const Color colorSkyBlue = { 0.0f, 1.0f, 1.0f, 1.0f };