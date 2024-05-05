#include <Arduino.h>

#include "Warehouse/Warehouse.h"
#include "Recipe/Recipe.h"


//global variables are stored in ram! remember that ...
Warehouse magazzino = Warehouse();
Recipe ricetta= Recipe("", &magazzino);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
}
