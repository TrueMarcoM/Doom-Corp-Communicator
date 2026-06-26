#include <TFT_eSPI.h>
#include "pins.h"
#include "config.h"

//[=] Review macros and the constants defined by them. What is difference with just using variables
//Recall that constants defined in a header file and included, are available to use in the code, like MC_DATUM.

TFT_eSPI tft = TFT_eSPI(); //tft is the object itself , not reference variable like when we'd use new keyword in java

const char* menuItems[] = {
  "Music",
  "Remote",
  "Settings",
  "About"
};

const int menuCount = sizeof(menuItems) / sizeof(menuItems[0]);

int selectedIndex = 0;
bool needsRedraw = true;

bool lastButton1 = BUTTON_RELEASED;
bool lastButton2 = BUTTON_RELEASED;

void drawBootScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //2 colors? Also, how do I have access to these values. 
  tft.setTextDatum(MC_DATUM); //What is this value

  tft.drawString(APP_NAME, 120, 55, 4);
  tft.drawString("Firmware " APP_VERSION, 120, 90, 2); //This is how concatenation works in C?

  delay(1000);
}

void drawMenu() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString(APP_NAME, 10, 8, 2);

  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawFastHLine(0, 28, 240, TFT_DARKGREY);

  //I believe this just highlights the 
  for (int i = 0; i < menuCount; i++) {
    int y = 42 + i * 22;


    if (i == selectedIndex) { //hovered more accurately
      tft.fillRect(8, y - 2, 224, 20, TFT_BLUE);
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
      tft.drawString("> ", 14, y, 2); //I'm assuming the int arguments are pixel coordinate points on the display.
      tft.drawString(menuItems[i], 34, y, 2); //menuItems[i] is a char pointer, referring to the first letter of the element of the array.
    
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString(menuItems[i], 34, y, 2);
    }
  }

  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("B1: Move B2: Select", 10, 120, 1); //(string, x coord, y coord, font number (size?)
}

void drawPlaceholderApp(const char* appName) {
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(MC_DATUM); //setting the main reference point of the text to be at the middle center of the string
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Press B2 to return", 120, 112, 1);
}

bool buttonPressedOnce(int pin, bool& lastState) {
  bool currentState = digitalRead(pin); //assuming this is a built in function in Arduino IDE or something. 
                                        //It returns the state of the pin (on/off?, high/low?)

  bool pressed = false;
  if (lastState == BUTTON_RELEASED && currentState == BUTTON_PRESSED) {
    pressed = true;
  }

  lastState = currentState;
  return pressed;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Accessing Serial object to use begin() function. The object comes from 

  pinMode(PIN_TFT_BL, OUTPUT); //This is an Arduino function. Configures how a GPIO pin behaves. Set the mode of the pin to be input or output
  digitalWrite(PIN_TFT_BL, HIGH);

  pinMode(PIN_BUTTON_1, INPUT_PULLUP); //setting the mode of this pin to be input_pullup (?)
  pinMode(PIN_BUTTON_2, INPUT_PULLUP);

  tft.init(); //what does init function do
  tft.setRotation(1);
  
  drawBootScreen();
  drawMenu();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonPressedOnce(PIN_BUTTON_1, lastButton1)) { //[=]...
    selectedIndex++;
    if (selectedIndex >= menuCount) {
      selectedIndex = 0;
    }

    drawMenu();
    delay(160);
  }

  if (buttonPressedOnce(PIN_BUTTON_2, lastButton2)) {
    drawPlaceholderApp(menuItems[selectedIndex]);

    bool waiting = true;
    delay(250);

    while (waiting) {
      if (buttonPressedOnce(PIN_BUTTON_2, lastButton2)) {
        waiting = false;
        drawMenu();
        delay(160);
      }
    }
  }
}
