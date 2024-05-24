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

void print(String text){
  Serial.println(text);
}

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
  Serial.println("Start");

  int i=0;
  SdFat SD;
  while (!SD.begin(SD_SS, SD_SCK_MHZ(16)) && i!= 5) {
    Serial.println(F("SD card not ready..."));
    i++;
  }

  if (i==5){ Serial.println(F("HOHO...SD card not recognized! FATAL ERROR")); while(true);}

  Serial.println(F("SD READY."));
  SD.end();

}

void loop() {
  /*
  Warehouse warehouse = Warehouse();
  SdFat SD;
  SD.begin(SD_SS, SD_SCK_MHZ(16));
  File file; file  = SD.open("recipes/Bellini.csv");
  Recipe r = Recipe(&file, &warehouse);
  Ingredient *prosecco = warehouse.getIngredient("prosecco");

  //to debug

  printRecipe(&r);
  print("Aggiungo 1000 ml di prosecco");
  print( "check sulla memorizzazione del valore "+ String(r.addIngredientQty(prosecco, 1000)));
  printRecipe(&r);
  print("Ci sono abbastanza ingredienti?:" + String(r.checkEnoughIngredientsInWarehouse()));
  print("\naggiusto il volume..");
  r.adjustTotalVolume(100);
  print("Ci sono abbastanza ingredienti?:" + String(r.checkEnoughIngredientsInWarehouse()));
  printRecipe(&r);
  
  print("\ntesto l'accesso mediante lista di ingredienti");
  Ingredient* const* ingredienti = r.getIngredients();
  for (int i=0; i<r.getIngredientsNum(); i++){
    print(ingredienti[i]->print());
    print("quantità totale richiesta: "+String(r.getIngredientRequiredQty(ingredienti[i])));
  }

  print("\nreset delle quantità e stampa");
  r.reset();
  printRecipe(&r);

  while (true){delay(10);}
  */
  
  Serial.println("GUI BUILDING");
  Gui interfaccia=Gui();
  Serial.println("GUI READY.");
  interfaccia.show();
  while (true){
    if (getXY(&interfaccia)){
      if (interfaccia.interact(cc.mapped_x, cc.mapped_y)) interfaccia.show();
    }
    delay(10);
  } 
}
