/*********************************************************************
* FIREKEY - PROJECT
* 
* Firmware Version 0.1V
* 
* Required libs:
* - Keyboard(https://github.com/arduino-libraries/Keyboard)
*   -> can be installed via arduino ide (defauled installed)
* - USBHost
*   -> can be installed via arduino ide
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
* - Adafruit SH1106 Lib(https://github.com/wonho-maker/Adafruit_SH1106)
*    -> is located in the 20_Display folder
* - Adafruit GFX Lib(https://github.com/adafruit/Adafruit-GFX-Library)
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include <KeyboardLayout.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "00_Key.h"

// Begin CONSTANTS

// Keys
#define ROW_COUNT 5       // number of rows
#define COL_COUNT 3       // number of columns
#define SPAM_SPEED 15     // how often is a key pressed while holding it down
#define HOLD_DELAY 100    // delay before a button is marked as held. max 255, otherwise change keyDownCounter to int
#define DEBOUNCE_TIME 10  // Limit how often the metric is scanned.

// Layers
#define LAYER_BACK_KEY 2     // define the back button led index
#define LAYER_HOME_KEY 1     // define the home button led index
#define LAYER_FORWARD_KEY 0  // define the forward button led index
#define MAX_LAYER 5
#define HOME_LAYER 0

// Button constants defining the led index
#define KEY_1 3
#define KEY_2 4
#define KEY_3 5
#define KEY_4 8
#define KEY_5 7
#define KEY_6 6
#define KEY_7 9
#define KEY_8 10
#define KEY_9 11
#define KEY_10 14
#define KEY_11 13
#define KEY_12 12

// Led
#define NUM_LEDS 6  // number of LEDs in the strip
#define LED_PIN 21

// End CONSTANTS

unsigned long startTime;  // debounce last check

byte rows[ROW_COUNT] = { 5, 6, 7, 8, 9 };  // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };     // define the column pins
byte currentLayer = 0;

//TODO Move to key class?
int keyDownCounter[COL_COUNT * ROW_COUNT];
bool keySpamMode[COL_COUNT * ROW_COUNT];  // defines if a key is in spam mode or not

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

Key keys[ROW_COUNT][COL_COUNT];


void setup() {
  Serial.begin(9600);

  // Setup key matrix
  for (byte r = 0; r < ROW_COUNT; r++) {
    for (byte c = 0; c < COL_COUNT; c++) {
      keys[r][c] = Key(rows[r], cols[c], getLedIndex(r, c), ledStrip);
    }
  }
}

void loop() {
  if ((millis() - startTime) > DEBOUNCE_TIME) {
    readMatrix();
    startTime = millis();
  }
}

byte getLedIndex(byte rowIdx, byte colIdx) {
  // calculate led index out of row and col index
  byte index = rowIdx * COL_COUNT;
  index += rowIdx % 2 == 0 ? colIdx : COL_COUNT - 1 - colIdx;
  return index;
}

void readMatrix() {
  // scan matrix
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {

    // is any column pulled to low due to a button being pressed?
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      // Get key object
      Key key = keys[rowIndex][colIndex];

      // Check if the key is pressed
      if (key.update()) {
        keyPressed(key);
      } else if (keyDownCounter[key.getIndex()] != 0) {
        resetKey(key.getIndex());
      }
    }
  }
}

void resetKey(byte keyIndex) {
  keySpamMode[keyIndex] = false;
  keyDownCounter[keyIndex] = 0;
}

void keyPressed(Key key) {
  //TODO Move to key class?
  if (keyDownCounter[key.getIndex()] == 0) {
    Serial.println("pressed");
    handleKeyPress(key);
  } else if (keySpamMode[key.getIndex()] && keyDownCounter[key.getIndex()] > SPAM_SPEED) {
    Serial.println("pressed");
    handleKeyPress(key);
    keyDownCounter[key.getIndex()] = 0;
  } else if (keyDownCounter[key.getIndex()] > HOLD_DELAY) {
    keySpamMode[key.getIndex()] = true;
  }
  keyDownCounter[key.getIndex()]++;
}

void handleKeyPress(Key key) {
  switch (key.getIndex()) {
    case LAYER_BACK_KEY:
    case LAYER_HOME_KEY:
    case LAYER_FORWARD_KEY:
      handleLayerKeyPress(key);
      break;
    case KEY_1:
      Serial.println(1);
      keyOnePressed(KEY_1);
      break;
    case KEY_2:
      Serial.println(2);
      keyTwoPressed(KEY_2);
      break;
    case KEY_3:
      Serial.println(3);
      keyThreePressed(KEY_3);
      break;
    case KEY_4:
      Serial.println(4);
      keyFourPressed(KEY_4);
      break;
    case KEY_5:
      Serial.println(5);
      keyFivePressed(KEY_5);
      break;
    case KEY_6:
      Serial.println(6);
      keySixPressed(KEY_6);
      break;
    case KEY_7:
      Serial.println(7);
      keySevenPressed(KEY_7);
      break;
    case KEY_8:
      Serial.println(8);
      keyEightPressed(KEY_8);
      break;
    case KEY_9:
      Serial.println(9);
      keyNinePressed(KEY_9);
      break;
    case KEY_10:
      Serial.println(10);
      keyTenPressed(KEY_10);
      break;
    case KEY_11:
      Serial.println(11);
      keyElevenPressed(KEY_11);
      break;
    case KEY_12:
      Serial.println(12);
      keyTwelvePressed(KEY_12);
      break;
    default:
      Serial.println("No Action");
  }
}

void handleLayerKeyPress(Key key) {
  switch (key.getIndex()) {
    case LAYER_BACK_KEY:
      currentLayer = (currentLayer - 1 + MAX_LAYER) % MAX_LAYER;
      break;
    case LAYER_FORWARD_KEY:
      currentLayer = (currentLayer + 1) % MAX_LAYER;
      break;
    case LAYER_HOME_KEY:
    default:
      currentLayer = HOME_LAYER;
  }
  Serial.println("Layer:");
  Serial.println(currentLayer);
}

void keyOnePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyTwoPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyThreePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyFourPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyFivePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keySixPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keySevenPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyEightPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyNinePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyTenPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyElevenPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyTwelvePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}
