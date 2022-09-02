#include <Ticker.h>
#include "Graphics.h"
#include "Keypad.h"

#define TONE_OUTPUT_PIN 15
#define TONE_PWM_CHANNEL 0

char number[11];
String tempNum = "";
MENU menuState = HOME;
int position = 1;
bool callFlag = false;

const byte numChars = 64;
char receivedChars[numChars];

char batteryPrecentage[4] = "x";
char signalStrength[3] = "33";
char timeHours[3] = "00";
char timeMinutes[3] = "00";
char dateDay[3] = "00";
char dateMonth[3] = "00";

boolean newData = false;

Ticker ticker;

void updateValues() {
  Serial.println("Sending battery command:");
  Serial2.write("AT+CBC\r\n");
  recvWithEndMarker();
  Serial.println("Sending signal command:");
  Serial2.write("AT+CSQ\r\n");
  recvWithEndMarker();
  Serial.println("Sending time command:");
  Serial2.write("AT+CCLK?\r\n");
  recvWithEndMarker();
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);

  delay(20);

  initDisplay();

  ticker.attach_ms(60000, updateValues);

  Serial2.write("AT\r\n");
  Serial2.println("AT+CLIP=1");
  Serial2.println("ATL9");
  updateValues();
}

void loop() {
  handleKeys();
  recvWithEndMarker();
  updateDisplay();
}

void updateDisplay() {
  switch (menuState) {
    case HOME:
      updateHomeDisplay(signalStrength, batteryPrecentage, timeHours, timeMinutes, dateMonth, dateDay);
      break;
    case CALL:
      updateCallDisplay(number, callFlag);
      break;
    case MESSAGE:
      updateMessageDisplay();
      break;
    case LIST:
      updateListDisplay(position);
      break;
    case INCOMING:
      updateIncomingCallDisplay(number);
      break;
  }
}

void handleKeys() {
  int funKey = getFunctionKey();
  int numKey = getNumberKey();

  if (funKey != 0) {
    if (menuState == HOME && funKey == 15)
      menuState = LIST;
    else if (menuState == LIST && funKey == 11)
      position = 1;
    else if (menuState == LIST && funKey == 12)
      position = 2;
    else if (menuState == LIST && funKey == 13)
      menuState = HOME;
    else if (menuState == LIST && funKey == 15) {
      if (position == 1)
        menuState = CALL;
      else if (position == 2)
        menuState = MESSAGE;
    } else if (menuState == CALL && funKey == 13) {
      menuState = HOME;
      tempNum = "";
      number[0] = '\0';
    } else if (menuState == CALL && funKey == 15)
      call();
    else if (menuState == MESSAGE && funKey == 13)
      menuState = HOME;
    else if (menuState == MESSAGE && funKey == 15)
      sendSMS();
    else if (menuState == INCOMING && funKey == 15)
      receiveCall();
    else if (menuState == INCOMING && funKey == 13) {
      menuState = HOME;
      declineCall();
    }
  }

  if (numKey != 10) {
    if (menuState == CALL) {
      tempNum = tempNum + numKey;
      tempNum.toCharArray(number, 11);
      Serial.println(tempNum);
    }
  }
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial2.available() > 0 && newData == false) {
    rc = Serial2.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
        ndx = numChars - 1;
    } else {
      Serial.println("Received some data...");
      receivedChars[ndx] = '\0';
      ndx = 0;
      newData = true;
    }
  }
  handleData();
}

void handleData() {
  if (newData == true) {
    if (receivedChars[0] == '+' && receivedChars[1] == 'C' && receivedChars[2] == 'B' && receivedChars[3] == 'C')
      handleBatteryLvlResponse();
    if (receivedChars[0] == '+' && receivedChars[1] == 'C' && receivedChars[2] == 'S' && receivedChars[3] == 'Q')
      handleNetworkStrengthResponse();
    if (receivedChars[0] == '+' && receivedChars[1] == 'C' && receivedChars[2] == 'C' && receivedChars[3] == 'L'
        && receivedChars[4] == 'K')
      handleTimeResponse();
    if (receivedChars[0] == '+' && receivedChars[1] == 'C' && receivedChars[2] == 'L' && receivedChars[3] == 'I'
        && receivedChars[4] == 'P')
      handleIncomingCall();
    Serial.println(receivedChars);
    newData = false;
  }
}

void handleIncomingCall() {
  Serial.println("Handling incoming call...");
  Serial.println(receivedChars);

  number[0] = receivedChars[9];
  number[1] = receivedChars[10];
  number[2] = receivedChars[11];
  number[3] = receivedChars[12];
  number[4] = receivedChars[13];
  number[5] = receivedChars[14];
  number[6] = receivedChars[15];
  number[7] = receivedChars[16];
  number[8] = receivedChars[17];
  number[9] = receivedChars[18];
  number[10] = receivedChars[19];
  number[10] = '\0';

  menuState = INCOMING;
}

void handleTimeResponse() {
  Serial.println("Handling time response...");
  Serial.println(receivedChars);

  dateDay[0] = receivedChars[14];
  dateDay[1] = receivedChars[15];
  dateDay[2] = '\0';

  dateMonth[0] = receivedChars[11];
  dateMonth[1] = receivedChars[12];
  dateMonth[2] = '\0';

  timeHours[0] = receivedChars[17];
  timeHours[1] = receivedChars[18];
  timeHours[2] = '\0';

  timeMinutes[0] = receivedChars[20];
  timeMinutes[1] = receivedChars[21];
  timeMinutes[2] = '\0';
}

void handleNetworkStrengthResponse() {
  Serial.println("Handling network response...");
  char *value = NULL;
  value = strtok(receivedChars, ",");
  if (strlen(value) == 7) {
    signalStrength[0] = value[6];
    signalStrength[1] = '\0';
  } else if (strlen(value) == 8) {
    signalStrength[0] = value[6];
    signalStrength[1] = value[7];
    signalStrength[2] = '\0';
  }

  Serial.print("Signal strength is at ");
  Serial.println(signalStrength);
}

void handleBatteryLvlResponse() {
  Serial.println("Handling battery response...");
  int i = 0;
  char *value = NULL;
  value = strtok(receivedChars, ",");
  while (value != NULL) {
    i++;
    if (i == 2) {
      strcpy(batteryPrecentage, value);
      Serial.print("Battery is at ");
      Serial.print(batteryPrecentage);
      Serial.println("%");
    }
    value = strtok(NULL, ",");
  }
}

void receiveCall() {
  Serial2.println("ATA");
}

void declineCall() {
  Serial2.println("ATH");
}

void sendSMS() {

  Serial2.print("AT+CMGF=1\r");
  delay(100);
  Serial2.print("AT+CMGS=\"+38762012205\"\r");
  delay(500);
  Serial2.print("Hi");
  delay(500);
  Serial2.print((char)26);
  delay(500);
  Serial2.println();
}

void call() {
  callFlag = true;
  String commandNumber = "ATD+ +387";
  commandNumber = commandNumber + tempNum;
  commandNumber = commandNumber + ";";
  Serial2.println(commandNumber);
}