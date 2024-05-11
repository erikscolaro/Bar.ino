#include <Arduino.h>
#include <TouchScreen.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include "Gui/Gui.h"

#include "main.settings.h"
#include "general.settings.h"

typedef struct {
  int16_t mapped_x, mapped_y;
} Point;

TouchScreen ts(XP, YP, XM, YM, TOUCH_OHM);
TSPoint p;
Point cc;

void (* restart)(void) = 0;

bool getXY(Gui* gui){
    p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        cc.mapped_x = map(p.x, TS_LEFT, TS_RT, 0, gui->screenWidth());
        cc.mapped_y = map(p.y, TS_TOP, TS_BOT, 0, gui->screenHeight());
    }
    return pressed;
}

void setup() {
  Serial.begin(115200);

  int i=0;
  SdFat SD;
  while (!SD.begin(SD_SS) && i!= 5) {
    PRINT_DBG(MAIN, "SD card not ready...");
    i++;
  }

  if (i==5){ PRINT_DBG(MAIN, "...SD card not recognized! FATAL ERROR"); restart();}

  PRINT_DBG(MAIN, "... now it is.");

  SD.end();
}

void loop() {
  Gui interfaccia=Gui();
  interfaccia.show();
  while (true){
    if (getXY(&interfaccia)){
      interfaccia.interact(cc.mapped_x, cc.mapped_y);
    }
    delay(10);
  }
}
