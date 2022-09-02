#include "Graphics.h"

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay() {
  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.display();
}

void drawSmallLine() {
  display.drawPixel(SMALL_LINE_X, 8, SH110X_WHITE);
  display.drawPixel(SMALL_LINE_X + 1, 8, SH110X_WHITE);
  display.drawPixel(SMALL_LINE_X, 7, SH110X_WHITE);
  display.drawPixel(SMALL_LINE_X + 1, 7, SH110X_WHITE);
}

void drawSMediumLine() {
  for (int i = SIG_LINE_Y; i > 5; i--) {
    display.drawPixel(SMEDIUM_LINE_X, i, SH110X_WHITE);
    display.drawPixel(SMEDIUM_LINE_X + 1, i, SH110X_WHITE);
  }
}

void drawBMediumLine() {
  for (int i = SIG_LINE_Y; i > 3; i--) {
    display.drawPixel(BMEDIUM_LINE_X, i, SH110X_WHITE);
    display.drawPixel(BMEDIUM_LINE_X + 1, i, SH110X_WHITE);
  }
}

void drawBigLine() {
  for (int i = SIG_LINE_Y; i > 0; i--) {
    display.drawPixel(BIG_LINE_X, i, SH110X_WHITE);
    display.drawPixel(BIG_LINE_X + 1, i, SH110X_WHITE);
  }
}

void updateHomeDisplay(char sigStr[], char batPrct[], char hr[], char min[], char mnth[], char day[]) {
  int signalStrength = atoi(sigStr) / 8;
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.drawBitmap(0, 0, battery_bmp, 16, 8, 1);
  display.setCursor(17, 0);
  display.write(batPrct);
  display.write('%');
  display.drawBitmap(101, 0, cell_bmp, 16, 8, 1);
  switch (signalStrength) {
    case (3):
      drawSmallLine();
      break;
    case (2):
      drawSmallLine();
      drawSMediumLine();
      break;
    case (1):
      drawSmallLine();
      drawSMediumLine();
      drawBMediumLine();
      break;
    case (0):
      drawSmallLine();
      drawSMediumLine();
      drawBMediumLine();
      drawBigLine();
      break;
    default:
      display.setCursor(110, 0);
      display.write('x');
      break;
  }

  display.setTextSize(2);
  display.setCursor(32, 20);
  display.write(hr);
  display.write(":");
  display.write(min);
  display.setTextSize(1);
  display.setCursor(47, 38);
  display.write(day);
  display.write("/");
  display.write(mnth);
  display.setCursor(0, 56);
  display.write("Menu");
  display.display();
}

void updateCallDisplay(char number[], bool callFlag) {
  if (callFlag) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(12, 30);
    display.write("Call in progress...");
    display.setCursor(79, 56);
    display.write("Hang up");
    display.display();
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 56);
  display.write("Call");
  display.setCursor(0, 0);
  display.write("Enter number: ");
  display.setCursor(12, 30);
  display.write(number);
  display.display();
}

void updateMessageDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 56);
  display.write("Send");
  display.setCursor(0, 0);
  display.write("Message: ");
  display.display();
}

void updateListDisplay(int position) {
  display.clearDisplay();
  display.setTextSize(2);
  if (position == 1)
    display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  else
    display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.write("CALL");
  display.setCursor(0, 18);
  if (position == 2)
    display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  else
    display.setTextColor(SH110X_WHITE);
  display.write("MESSAGE");
  display.display();
}

void updateIncomingCallDisplay(char number[]) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(12, 30);
  display.write(number);
  display.setCursor(0, 56);
  display.write("Answer");
  display.setCursor(79, 56);
  display.write("Decline");
  display.display();
}