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

void print(String text){
  Serial.println(text);
}

void printRecipe(Recipe *r){
  String nome = String(r->getName());
  print("Nome ricetta: "+ String(nome));
  print("Passaggi:");
  r->iteratorBegin();
  while (r->iteratorNext()){
    print("Azione: " + String((char) r->iteratorGetAction())+ "  Quantità: " + String(r->iteratorGetQty())+ "  " + r->iteratorGetIngredient()->print());
  }
}

void loop() {
  Warehouse warehouse = Warehouse();
  SdFat SD;
  SD.begin(SD_SS, SD_SCK_MHZ(16));
  File file; file  = SD.open("recipes/Bellini.csv");
  Recipe r = Recipe(&file, &warehouse);
  Ingredient *prosecco = warehouse.getIngredient("prosecco");

  //to debug

  printRecipe(&r);
  Serial.println("Aggiungo 100 ml di prosecco");
  Serial.println( "check sulla memorizzazione del valore"+ String(r.addIngredientQty(prosecco, 100)));
  printRecipe(&r);
  Serial.println("Ci sono abbastanza ingredienti?:" + String(r.checkEnoughIngredientsInWarehouse()));
  Serial.println("aggiusto il volume..");
  r.adjustTotalVolume(100);
  r.checkEnoughIngredientsInWarehouse();
  printRecipe(&r);
  
  Serial.println("testo l'accesso mediante lista di ingredienti");
  Ingredient* const* ingredienti = r.getIngredients();
  for (int i=0; i<r.getIngredientsNum(); i++){
    Serial.println(ingredienti[i]->print());
    Serial.println("quantità totale richiesta: "+String(r.getIngredientRequiredQty(ingredienti[i])));

  }

  while (true){delay(10);}
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
