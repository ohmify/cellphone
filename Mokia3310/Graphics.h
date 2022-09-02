#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BIG_LINE_X     122
#define BMEDIUM_LINE_X 118
#define SMEDIUM_LINE_X 114
#define SMALL_LINE_X   110
#define SIG_LINE_Y       8

enum MENU {
  HOME,
  CALL,
  MESSAGE,
  LIST,
  INCOMING
};

static const unsigned char PROGMEM battery_bmp[] =
{ B00000000, B00000000,
  B11111111, B11110000,
  B10000000, B00010000,
  B10000000, B00011000,
  B10000000, B00011000,
  B10000000, B00010000,
  B11111111, B11110000,
  B00000000, B00000000
};

static const unsigned char PROGMEM cell_bmp[] =
{ B11111111, B10000000,
  B01000001, B00000000,
  B00100010, B00000000,
  B00010100, B00000000,
  B00001000, B00000000,
  B00001000, B00000000,
  B00001000, B00000000,
  B00001000, B00000000
};

void initDisplay();
void updateHomeDisplay(char sigStr[], char batPrct[], char hr[], char min[], char mnth[], char day[]);
void updateCallDisplay(char number[], bool callFlag);
void updateMessageDisplay();
void updateListDisplay(int position);
void updateIncomingCallDisplay(char number[]);

#endif