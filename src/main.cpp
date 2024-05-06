#include <Arduino.h>

#include "Warehouse/Warehouse.h"
#include "Recipe/Recipe.h"


//global variables are stored in ram! remember that ...
Warehouse magazzino = Warehouse();


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  Recipe ricetta= Recipe("", &magazzino);
  if (ricetta.beginIteration()){
    while (ricetta.nextStep()){
      Serial.println(ricetta.getStepAction()+ricetta.getStepQty()+ricetta.getStepIngredient()->print());
    }
  }
  
}
