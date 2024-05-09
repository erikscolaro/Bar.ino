#include "Ingredient.h"

Ingredient::Ingredient(){}

Ingredient::Ingredient(const char *name, short actuator, short qty, short maxQty, bool isLiquid, bool isEditable) : _actuator(actuator), _qty(qty), _mqty(maxQty), _isLiquid(isLiquid), _isEditable(isEditable)
{
    _adx=0;
    if (name != nullptr) {
        setName(name);
    } else {
        setName("<NO NAME>");
    }
}

//assuming is csv file line
//name,index,quantity, max quantity, isliquid, iseditable
Ingredient::Ingredient(char *line){
    char *token = strtok(line, DELIMITER_CHAR);
    this->setName(token);
    token = strtok(nullptr, DELIMITER_CHAR);
    this->_actuator=strtol(token, nullptr, 10);
    token = strtok(nullptr, DELIMITER_CHAR);
    this->_qty=strtol(token, nullptr, 10);
    token = strtok(nullptr, DELIMITER_CHAR);
    this->_mqty=strtol(token, nullptr, 10);
    token = strtok(nullptr, DELIMITER_CHAR);
    this->_isLiquid=strtol(token, nullptr, 10)!=0;
    token = strtok(nullptr, DELIMITER_CHAR);
    this->_isEditable=strtol(token, nullptr, 10)!=0;
    _adx=0;
}

// Getter
const char* Ingredient::getName() const {
    return _name;
}

short Ingredient::getActuator() const {
    return _actuator;
}

short Ingredient::getQuantity() const {
    return _qty;
}

short Ingredient::getMaxQuantity() const {
    return _mqty;
}

bool Ingredient::isLiquid() const {
    return _isLiquid;
}

short Ingredient::getAdx() const
{
    return _adx;
}

bool Ingredient::isEditable() const {
    return _isEditable;
}

// Setter
void Ingredient::setName(const char* name) {
    strncpy(_name, name, NAME_LENGTH - 1);
    _name[NAME_LENGTH - 1] = '\0';
}

void Ingredient::setActuator(short actuator) {
    _actuator=actuator;
}

void Ingredient::setQuantity(short qty) {
    _qty = qty;
    EEPROM.put(this->_adx, this);
}

void Ingredient::setAdx(short adx)
{
    _adx=adx;
}

String Ingredient::print() {
    return String("Name: " + String(_name) + "\t" +
                  "Quantità rimasta: " + String(_qty) + "/" + String(_mqty) + "\t" +
                  (_isLiquid ? "is liquid" : "is not liquid") +
                  " and " +
                  (_isEditable ? "is editable" : "is not editable"));
}

bool Ingredient::subtractQuantity(short subQty)
{
    if (this->getQuantity()<subQty) return false;
    else{
        this->setQuantity(this->getQuantity()-subQty);
        return true;
    }
}

bool Ingredient::addQuantity(short addQty)
{
    if (this->getQuantity()+addQty>this->getMaxQuantity()) return false;
    else{
        this->setQuantity(this->getQuantity()+addQty);
        return true;
    }
}

void Ingredient::refillQty()
{
    setQuantity(this->getMaxQuantity());
}
