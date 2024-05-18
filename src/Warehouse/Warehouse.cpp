#include "Warehouse.h"


Warehouse::Warehouse(){
    Serial.println("[WAREHOUSE]Checking where ingredients info are stored...");
    uint8_t magic_number= EEPROM.read(0);
    _storedIngredients=0;
    Serial.println("Numero salvato in eeprom:"+ String(magic_number) + "  Numero magico:" + String(MAGIC_CHECK));
    if (magic_number==MAGIC_CHECK) readIngredientsFromEEPROM();
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

Ingredient *Warehouse::getIngredient(int index) const
{
    if (index>=_storedIngredients) return nullptr;

    return const_cast<Ingredient*>(&_ingredients[index]);
}

bool Warehouse::isEnough(Ingredient const *ingredient, short qty)
{
    return ingredient->getQuantity()>=qty;
}

void Warehouse::readIngredientsFromEEPROM(){
    Serial.println("[WAREHOUSE]Reading from EEPROM...");
    if (EEPROM.read(1)>NUM_INGREDIENTS){
        Serial.println("[WAREHOUSE]Error: too many ingredients stored in EEPROM, increase the size of the ingredients array");
        _storedIngredients=NUM_INGREDIENTS;
    } else _storedIngredients=EEPROM.read(1);

    for (uint8_t i=0; i<_storedIngredients; i++){
        EEPROM.get(4+ i*sizeof(Ingredient), _ingredients[i]);
        Serial.println("[WAREHOUSE]"+String(i)+".\t"+_ingredients[i].print());
    }
}

void Warehouse::readIngredientsFromSD(){
    Serial.println("[WAREHOUSE]Reading ingredients from SD...");

    SdFat SD;
    SD.begin();
    File file = SD.open(INGREDIENT_DIR);
    char buf[50], buf2[50];
    short n;
    int filePos;

    while (file.available() && _storedIngredients<NUM_INGREDIENTS){
        memset(buf, 0, sizeof(buf));
        memset(buf2, 0, sizeof(buf2));
        filePos=file.position();
        n = file.fgets(buf, sizeof(buf)); //read a line

        if (buf[n - 1] != '\n' && n == (sizeof(buf) - 1)) {
            Serial.println("[WAREHOUSE]ERROR: line of csv file ingredients is too long!");
            file.close();
            SD.end();
            while(true);
        } else {
            file.seek(filePos);
            file.fgets(buf2, sizeof(buf2));
            while (memcmp(buf, buf2, sizeof(buf))!=0){
                Serial.println("[WAREHOUSE]WARNING: inconsistency in reading data from sd.");
                Serial.println(String(buf)+"\n"+String(buf2));
                memset(buf, 0, sizeof(buf));
                memset(buf2, 0, sizeof(buf2));
                file.seek(filePos);
                file.fgets(buf, sizeof(buf));
                file.seek(filePos);
                file.fgets(buf2, sizeof(buf2));
            }
            addIngredient(buf);
        }
    }

    if (sizeof(Ingredient)*_storedIngredients+4 > EEPROM.length()){
        Serial.println("[WAREHOUSE]Error: too many ingredients to store in eeprom.");
        return;
    }

    EEPROM.write(0, MAGIC_CHECK);
    EEPROM.write(1, _storedIngredients);

    for (uint8_t i=0; i<_storedIngredients; i++){
        _ingredients[i].setAdx(4+ i*sizeof(Ingredient));
        EEPROM.put(_ingredients[i].getAdx(), _ingredients[i]);
        Serial.println("[WAREHOUSE]"+String(i)+".\t"+_ingredients[i].print());
    }

    SD.end();
    file.close();
}

bool Warehouse::addIngredient(char *info){
    if (_storedIngredients>=NUM_INGREDIENTS){
        Serial.println("[WAREHOUSE]Error: too many ingredients to store!");
        return false;
    }

    _ingredients[_storedIngredients]=Ingredient(info);
    _storedIngredients+=1;
    return true;
}
