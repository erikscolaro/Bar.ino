#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <Arduino.h>

#include "ingredient.settings.h"
#include "general.settings.h"


class Ingredient {
private:
    char _name[NAME_LENGTH];
    short  _actuator, _qty, _mqty, _adx;
    bool _isLiquid {false};
    bool _isEditable {false};

public:
    Ingredient();
    Ingredient(char *name, short actuator, short qty, short maxQty, bool isLiquid, bool isEditable);
    Ingredient(char *allInfo);

    // Getter
    const char* getName() const;
    short getQuantity() const;
    short getMaxQuantity() const;
    short getActuator() const;
    short getAdx() const;
    bool isLiquid() const;
    bool isEditable() const;

    // Setter
    void setName(const char* name);
    void setQuantity(short qty);
    void setMaxQuantity(short maxQty);
    void setAdx(short adx);
    void setIsLiquid(bool isLiquid);
    void setIsEditable(bool isEditable);
    void setActuator(short actuator);

    String print();
};

#endif
