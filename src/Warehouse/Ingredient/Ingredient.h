#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <Arduino.h>
#include <EEPROM.h>

#include "ingredient.settings.h"
#include "general.settings.h"


class Ingredient {
private:
    char _name[NAME_LENGTH];
    short  _actuator, _qty, _mqty, _adx;
    bool _isLiquid {false};
    bool _isEditable {false};
    
    // Setter
    void setQuantity(short qty);
    void setActuator(short actuator);
    void setName(const char* name);
    void setMaxQuantity(short qty);

public:
    Ingredient();
    Ingredient(const char *name, short actuator, short qty, short maxQty, bool isLiquid, bool isEditable);
    Ingredient(char *allInfo);

    void setAdx(short adx);

    // Getter
    const char* getName() const;
    short getQuantity() const;
    short getMaxQuantity() const;
    short getActuator() const;
    short getAdx() const;
    bool isLiquid() const;
    bool isEditable() const;

    String print();

    //interacts with eeprom!!!
    bool subtractQuantity(short subQty);
    bool addQuantity(short addQty);
    void refillQty();
};

#endif
