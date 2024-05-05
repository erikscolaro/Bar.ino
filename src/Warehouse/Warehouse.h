#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>

#include "warehouse.settings.h"
#include "Ingredient\Ingredient.h"
#include "general.settings.h"
#include "SdFat.h"

class Warehouse{
    public:

        /**
         * @brief inizializza il magazzino, leggendo da sd o da eeprom.
        */
        Warehouse();
        Ingredient* getIngredient(const char* name);

    private:
        Ingredient _ingredients[NUM_INGREDIENTS];
        uint8_t _storedIngredients;

        void readIngredientsFromEEPROM();
        void readIngredientsFromSD();
        bool addIngredient(char *info);
        
};

#endif // WAREHOUSE_H