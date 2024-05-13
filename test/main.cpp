#include <Arduino.h>
#include <TouchScreen.h>
#include <SdFat.h>
#include "Gui/Gui.h"

#include "main.settings.h"
#include "general.settings.h"

typedef struct {
  int16_t mapped_x, mapped_y;
} Point;

TouchScreen ts(XP, YP, XM, YM, TOUCH_OHM);
TSPoint p;
Point cc;

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
  Serial.println("CIAO porco");

  int i=0;
  SdFat SD;
  while (!SD.begin(SD_SS, SD_SCK_MHZ(16)) && i!= 5) {
    Serial.println("SD card not ready...");
    i++;
  }

  if (i==5){ Serial.println("HOHO...SD card not recognized! FATAL ERROR"); while(true);}

  Serial.println("SD READY.");
  SD.end();

}

void loop() {
  //Gui interfaccia=Gui();
  Warehouse gioco = Warehouse();
  Serial.println("test accesso per indice");
  Ingredient* malibu = gioco.getIngredient(1);
  Serial.println(String(malibu->print()));
  Serial.println("test accesso per nome");
  malibu = gioco.getIngredient("gin");
  if (malibu==nullptr) Serial.println("nullpointer returned");
  Serial.println(String(malibu->print()));
  Serial.println("test metodi della classe ingredient");
  malibu->subtractQuantity(137);
  Serial.println(String(malibu->print()));
  malibu->refillQty();
  Serial.println(String(malibu->print()));
  malibu->subtractQuantity(9999);
  Serial.println(String(malibu->print()));
  malibu->addQuantity(9999);
  Serial.println(String(malibu->print()));
  while (true){}
/*   Gui interfaccia=Gui();
  Serial.println("GUI READY.");
  interfaccia.show();
  while (true){
    if (getXY(&interfaccia)){
      interfaccia.interact(cc.mapped_x, cc.mapped_y);
    }
    delay(10);
  } */
}
