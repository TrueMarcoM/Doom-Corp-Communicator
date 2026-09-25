//DIRECTIVES
#include <stdio.h>
#include <stdint.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include <esp_sntp.h>






//Library reference variable? setup
TFT_eSPI tft = TFT_eSPI();

//Wifi setup
const char *ssid = "NETGEAR21";
const char *password = "fuzzyship481";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = -28800;
const long daylightOffset_sec = 3600;










//States
int encoderCenterButtonState = LOW;
int lastCenterButtonState = HIGH;

//Battery voltage information
const float referenceVoltage = 3.3;                  //analog reference voltage (the board is powered by 3.3V)
const int adcResBits = 12;                           //ADC resolution (bits) - ???
const float ADC_STEPS = (1 << int(adcResBits)) - 1;  //number of steps -> 2^adcResBits - 1

const float batteryFullVoltage = 4.2;

//Time
char hourMinuteString[10];  //recall later I cant assign a string to it. Has to be initialized.
                            // Probably have to use strcpy, or array subscripting/pointer arithmetic???
time_t currentTime;
//This function returns the current calendar time encoded as a time_t object (itll be in seconds format)
//Input this into the other time.h functions so I can just display the hours and minutes.

//This localtime function will convert the calendar time since epoch of type time_t,
//into same time but in local format and in a struct tm format. (textual representation, instead of just seconds)
// localtime(&currentTime);
//May be an old convention thing. I suspect they just use the normal value within the function definition.
//This is meant to be used as an argument as it returns the struct pointer we need.


char activeApp[15];
float batteryVoltage;
int batteryPercent;

char batteryPercentString[10];


//============================================================//
//Coordinates, dimensions, margins, etc
int borderWidth = 4;
int spacerWidth = 4;

int screenWidth = 240;  //for now going to hardcode the width because of the tft.init() being invovled somewhere else, etc.
int screenHeight = 135;
int screenX = 0;
int screenY = 0;

//display as in the actual area where the content is located
int displayWidth = screenWidth - borderWidth - borderWidth;
int displayHeight = screenHeight - borderWidth - borderWidth;
int displayX = 0 + borderWidth;
int displayY = 0 + borderWidth;


const int centerOfScreenX = 120;  //240 / 2
const int centerOfScreenY = 67;   //135 / 2



//Menu Bar

//Menu elements width and position declarations

int menuBarWidth = displayWidth;
int menuBarHeight = 8 + borderWidth + borderWidth;  //A bar that contains text. Text characters generally have a height of 8. Bar must be taller.
int menuBarX = displayX;
int menuBarY = displayY;


int menuBarBorderWidth = 4;
int menuBarInteriorWidth = menuBarWidth - menuBarBorderWidth - menuBarBorderWidth;
int menuBarInteriorHeight = menuBarHeight - menuBarBorderWidth - menuBarBorderWidth;
int menuBarInteriorX = menuBarX + menuBarBorderWidth;
int menuBarInteriorY = menuBarY + menuBarBorderWidth;

int radius = 10;
int straightStartX = menuBarX + radius;
int straightEndX = menuBarX + menuBarWidth - radius;
int straightWidth = menuBarWidth - 2 * radius;

int timeWidth = tft.textWidth("12:00 PM");
int timeHeight = 8;
int timeX = menuBarInteriorX + 4;
int timeY = menuBarInteriorY;

int activeAppWidth = tft.textWidth("Remote");
int activeAppHeight = 8;
int activeAppX = timeX + timeWidth + spacerWidth;
int activeAppY = displayY;

int batteryBarWidth = 32;                              //30 pixels wide interior
int batteryBarHeight = 7;                              //5 pixels high interior
int batteryBarX = displayWidth - batteryBarWidth - 4;  //width of the screen - the width of the bar - the space from the right border
// int batteryBarX = straightEndX - batteryBarWidth;
int batteryBarY = menuBarY + menuBarBorderWidth;  //width of the screen - the width of the bar - the space from the right border

/*
     The batteryBarX + x should equal straightEndX */

int batteryBarInteriorWidth = 30;  //full
int batteryBarInteriorHeight = 5;  //full
int batteryBarInteriorX = batteryBarX + 1;
int batteryBarInteriorY = batteryBarY + 1;



// int percentValueWidth = tft.textWidth("100"); //18, 6 pixels wide for each character
int batteryPercentWidth = tft.textWidth("100%");
int batteryPercentHeight = 8;
int batteryPercentX = batteryBarX - spacerWidth - batteryPercentWidth;
int batteryPercentY = menuBarY + menuBarBorderWidth;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(460800);
  WiFi.begin(ssid, password);

  //Calling TFT_eSPI object methods.
  tft.init();
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE);
  tft.fillScreen(TFT_BLACK);  //didnt include the dimensions, its fine I assume  //correct, after rotating, the w and heights are reversed
  tft.setTextSize(1);

  analogReadResolution(adcResBits);               //why?
  analogSetPinAttenuation(batteryPin, ADC_11db);  //why not able to do 12db

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

void loop() {
  // put your main code here, to run repeatedly:
  displayMenuBar();

  delay(10000);
}


// float getBatteryVoltage() { //TBH this should just update the global variable instead of returning value.
// }


void displayMenuBar() {
  drawMenuBarBackground();
  displayTime();
  //  displayActiveApp();
  displayBattery();
  //DISPLAY TIME and ACTIVE APP
}

void setBatteryPercent() {
  //First step is to get the battery voltage. Later, convert it to a percentage.


  //Instructed by https://done.land/components/microcontroller/families/esp/esp32/developmentboards/esp32s/t-display/

  //https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/?_gl=1*1ygmw4a*_up*MQ..*_ga*MTE0OTM0NTUxNi4xNzg2NTgwNTY1*_ga_NEXN8H46L5*czE3ODY1ODA1NjIkbzEkZzAkdDE3ODY1ODA1NjIkajYwJGwwJGg4NzIyMjc4Mw..
  int rawValue = analogRead(batteryPin);
  float voltage = (rawValue / ADC_STEPS) * referenceVoltage;

  float batteryVoltage = voltage * 2.04;

  //     Serial.print("Voltage: ");
  //     Serial.print(batteryVoltage, 3); //note differences between standard C print()
  //     Serial.println(" V");
  //float batteryPercent = (batteryVoltage / batteryFullVoltage) * 100; //needs adjustment, it treats 0V as 0% which isn't accurate for us.
  //3.2V should be the lowest you can go, to record percent as 0%.
  batteryPercent = ((batteryVoltage - 3.2) / (batteryFullVoltage - 3.2)) * 100;
}

void drawMenuBarBackground() {
  tft.fillRoundRect(menuBarX, menuBarY, menuBarWidth, menuBarHeight, 8, TFT_BLUE);
  //temp addition
  // tft.fillRoundRect(menuBarX, menuBarY + menuBarHeight + borderWidth, menuBarWidth, menuBarHeight, 8, TFT_BLUE);
  // tft.fillRoundRect(menuBarX, menuBarY + menuBarHeight * 2 + borderWidth * 2, menuBarWidth, menuBarHeight, 8, TFT_BLUE);
  // tft.fillRoundRect(menuBarX, menuBarY + menuBarHeight * 3 + borderWidth * 3, menuBarWidth, menuBarHeight, 8, TFT_BLUE);
  // tft.fillRoundRect(menuBarX, menuBarY + menuBarHeight * 4 + borderWidth * 4, menuBarWidth, menuBarHeight, 8, TFT_BLUE);
  // tft.fillRoundRect(menuBarX, menuBarY + menuBarHeight * 5 + borderWidth * 5, menuBarWidth, menuBarHeight, 8, TFT_BLUE);
}

void displayTime() {
  currentTime = time(NULL);
  if (currentTime != (time_t)(-1)) {
    // Serial.printf("The current time is %s local);
    strftime(hourMinuteString, sizeof(hourMinuteString), "%I:%M %p", localtime(&currentTime));
    tft.drawString(&hourMinuteString[0], timeX, timeY);
    Serial.printf("The time is: %s\n", hourMinuteString);  //read the time via a library or something, write the values to the hour and minute variables
  } else {
    Serial.print("Time didn't work");
  }
}

void displayBattery() {
  setBatteryPercent();
  sprintf(batteryPercentString, "%3d%%", batteryPercent);  //destination, source data (in this case, a variable so we include the format specifier format like normal)

  tft.drawRect(batteryBarX, batteryBarY, batteryBarWidth, batteryBarHeight, TFT_WHITE);
  // tft.fillRect(batteryBarInteriorX, batteryBarInteriorY, (batteryPercent * 30)/100, batteryBarInteriorHeight, TFT_GREEN); //TBC width and height, they are variable.
  //TEMP commented out, fill with hardcoded value
  if (batteryPercent > 100) {
     tft.fillRect(batteryBarInteriorX, batteryBarInteriorY, 30, batteryBarInteriorHeight, TFT_GREEN);
     tft.drawString(" >>>", batteryPercentX, batteryPercentY);  //TBC coordinates
  } else {
     tft.fillRect(batteryBarInteriorX, batteryBarInteriorY, (batteryPercent * 30)/100, batteryBarInteriorHeight, TFT_GREEN);
     tft.drawString(batteryPercentString, batteryPercentX, batteryPercentY);  //TBC coordinates

  }
}


// //GOALS
// /*

// 1. Status Bar
// - Battery meter
// - Digital clock
// const int attenuation

// 2. Menu
// - Songs
// - Remote
// - Settings
// - TBD
// */
