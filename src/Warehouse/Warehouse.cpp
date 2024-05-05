#include "Warehouse.h"

Warehouse::Warehouse(){
    PRINT_DBG(WAREHOUSE,"Checking where ingredients info are stored...");
    EEPROM.begin(); uint8_t magic_number= EEPROM.read(0); EEPROM.end();
    _storedIngredients=0;
    if (magic_number!=MAGIC_CHECK) readIngredientsFromEEPROM();
    else readIngredientsFromSD();
}

void Warehouse::readIngredientsFromEEPROM(){
    EEPROM.begin();
    PRINT_DBG(WAREHOUSE, "Reading from EEPROM...");
    if (EEPROM.read(1)>NUM_INGREDIENTS){
        PRINT_DBG(WAREHOUSE, "Error: too many ingredients stored in EEPROM, increase the size of the ingredients array");
        _storedIngredients=NUM_INGREDIENTS;
    } else _storedIngredients=EEPROM.read(1);

    for (uint8_t i=0; i<_storedIngredients; i++){
        EEPROM.get(4+ i*sizeof(Ingredient), _ingredients[i]);
        PRINT_DBG(WAREHOUSE,String(i)+".\t"+_ingredients[i].print());
    }
    EEPROM.end();
}

void Warehouse::readIngredientsFromSD(){
    PRINT_DBG(WAREHOUSE,"Reading ingredients from SD...");

    SdFat SD;
    SD.begin(SD_SS); //REVIEW
    //VELOCITY MAY BE IMPROVED, SEE DOCS
    File file = SD.open(INGREDIENT_DIR);
    char buf[50];
    int n;
    bool thereIsSpace;

    while (file.available() && thereIsSpace){
        n = file.fgets(buf, sizeof(buf)); //read a line

        if (buf[n - 1] != '\n' && n == (sizeof(buf) - 1)) {
            PRINT_DBG(WAREHOUSE, "ERROR: line of csv file ingredients is too long!")
            file.close();
            SD.end();
            while(true);
        } else {
            thereIsSpace=addIngredient(buf);
        }
    }

    EEPROM.begin();

    if (sizeof(Ingredient)*_storedIngredients+4 > EEPROM.length()){
        PRINT_DBG(WAREHOUSE, "Error: too many ingredients to store in eeprom.");
        EEPROM.end();
        return;
    }

    EEPROM.write(0, MAGIC_CHECK);
    EEPROM.write(1, _storedIngredients);

    for (uint8_t i=0; i<_storedIngredients; i++){
        EEPROM.put(4+ i*sizeof(Ingredient), _ingredients[i]);
        PRINT_DBG(WAREHOUSE,String(i)+".\t"+_ingredients[i].print());
    }

    EEPROM.end();
    file.close();
    SD.end();
}

bool Warehouse::addIngredient(char *info){
    if (_storedIngredients>=NUM_INGREDIENTS){
        PRINT_DBG(WAREHOUSE, "Error: too many ingredients to store!");
        return false;
    }

    _ingredients[_storedIngredients]=Ingredient(info);
    _storedIngredients+=1;
    return true;
}
