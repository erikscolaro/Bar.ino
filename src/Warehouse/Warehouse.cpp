#include "Warehouse.h"


Warehouse::Warehouse(){
    PRINT_DBG(WAREHOUSE,"Checking where ingredients info are stored...");
    uint8_t magic_number= EEPROM.read(0);
    _storedIngredients=0;
    if (magic_number!=MAGIC_CHECK) readIngredientsFromEEPROM();
    else readIngredientsFromSD();
}

Ingredient* Warehouse::getIngredient(const char *name)
{
    for (int i=0;i<_storedIngredients; i++){
        if (strcmp(name, _ingredients[i].getName())==0){
            return &_ingredients[i];
        }
    }

    return nullptr;
}

void Warehouse::readIngredientsFromEEPROM(){
    PRINT_DBG(WAREHOUSE, "Reading from EEPROM...");
    if (EEPROM.read(1)>NUM_INGREDIENTS){
        PRINT_DBG(WAREHOUSE, "Error: too many ingredients stored in EEPROM, increase the size of the ingredients array");
        _storedIngredients=NUM_INGREDIENTS;
    } else _storedIngredients=EEPROM.read(1);

    for (uint8_t i=0; i<_storedIngredients; i++){
        EEPROM.get(4+ i*sizeof(Ingredient), _ingredients[i]);
        PRINT_DBG(WAREHOUSE,String(i)+".\t"+_ingredients[i].print());
    }
}

void Warehouse::readIngredientsFromSD(){
    PRINT_DBG(WAREHOUSE,"Reading ingredients from SD...");

    SdFat SD;
    SD.begin(); //ev. sd_ss pin
    File file = SD.open(INGREDIENT_DIR);
    char buf[50];
    short n;

    while (file.available() && _storedIngredients<NUM_INGREDIENTS){
        n = file.fgets(buf, sizeof(buf)); //read a line

        if (buf[n - 1] != '\n' && n == (sizeof(buf) - 1)) {
            PRINT_DBG(WAREHOUSE, "ERROR: line of csv file ingredients is too long!")
            file.close();
            SD.end();
            while(true);
        } else {
            addIngredient(buf);
        }
    }

    if (sizeof(Ingredient)*_storedIngredients+4 > EEPROM.length()){
        PRINT_DBG(WAREHOUSE, "Error: too many ingredients to store in eeprom.");
        return;
    }

    EEPROM.write(0, MAGIC_CHECK);
    EEPROM.write(1, _storedIngredients);

    for (uint8_t i=0; i<_storedIngredients; i++){
        _ingredients[i].setAdx(4+ i*sizeof(Ingredient));
        EEPROM.put(_ingredients[i].getAdx(), _ingredients[i]);
        PRINT_DBG(WAREHOUSE,String(i)+".\t"+_ingredients[i].print());
    }

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
