#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <Arduino.h>

#include "ingredient.settings.h"
#include "general.settings.h"


class Ingredient {
private:
    char _name[NAME_LENGTH] {""};
    int  _actuator=-1, _qty=0, _mqty=999;
    bool _isLiquid {false};
    bool _isEditable {false};

public:
    Ingredient();
    Ingredient(char *name, int actuator, int qty, int maxQty, bool isLiquid, bool isEditable);
    Ingredient(char *allInfo);

    // Getter
    const char* getName() const;
    int getQuantity() const;
    int getMaxQuantity() const;
    int getActuator() const;
    bool isLiquid() const;
    bool isEditable() const;

    // Setter
    void setName(const char* name);
    void setQuantity(int qty);
    void setMaxQuantity(int maxQty);
    void setIsLiquid(bool isLiquid);
    void setIsEditable(bool isEditable);
    void setActuator(int actuator);

    String print();
};

#endif
