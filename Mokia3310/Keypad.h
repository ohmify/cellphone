#ifndef KEYPAD_H
#define KEYPAD_H

enum FUNC_KEYS
{
  SELECT,
  UP,
  DOWN,
  BACK,
  STAR,
  HASHTAG
};

int getFunctionKey();
int getNumberKey(); 

#endif