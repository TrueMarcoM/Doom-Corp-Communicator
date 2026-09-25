#include "DimensionsCoordinates.h"
#include <TFT_eSPI.h>
#include <time.h>
#include <esp_sntp.h>
#include <WiFi.h>


extern TFT_eSPI tft; //My function defs imply we need the tft variable. 
//That variable is declared and defined in the "parent" source file.
//It's not normal (or correct?) to include the "parent" source file from a source file.
//It's supposed to be the other way around.
//Here we're saying we're going to use the tft variable.
//It seems like we are explicitly declaring the variable (instead of using #include directive)
//But that definition comes from the main source file, which feels a bit odd.

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