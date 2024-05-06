#include <Arduino.h>

#include "Gui/Gui.h"


//global variables are stored in ram! remember that ...
//Warehouse magazzino = Warehouse();


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  Gui interfaccia=Gui();
  interfaccia.draw();
}
