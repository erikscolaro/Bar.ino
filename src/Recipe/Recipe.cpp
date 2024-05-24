#include "Recipe.h"

Recipe::Recipe(File* file, Warehouse *warehouse) : _warehouse(warehouse)
{
    char buf[50];
    int n;

    //remote .extension in name
    file->getName(this->_name, sizeof(this->_name));
    char* dotptr = strchr(this->_name, '.');
    if (dotptr!=nullptr) *dotptr = '\0';

    while (file->available() && _stepsNum<=STEPS_NUM && _ingNum<=INGREDIENTS_NUM){
        n = file->fgets(buf, sizeof(buf));

        if (buf[n - 1] != '\n' && n == (sizeof(buf) - 1)) {
            Serial.println("[RECIPE]ERROR: line of csv recipe file is too long!");
            file->close();
            return;
        } else {
            addStep(buf);
            if (_steps[_stepsNum-1].getAction()==ADD) addIngredient(_steps[_stepsNum-1]);
        }
    }

    print();
    calculateIngredientQty();
    checkEnoughIngredientsInWarehouse();
}

void Recipe::print()
{
    Serial.print(F("[RECIPE]Recipe dump\nName:"));
    Serial.println(getName());
    Serial.println(F("Steps:"));
    for (Recipe::StepIterator it = begin(); it!=end(); ++it){
        Serial.print(F("Action: "));
        Serial.print((char) it->getAction());
        if (it->getAction()==ADD){
            Serial.print(F("\tQuantità: "));
            Serial.print(it->getModQty());
            Serial.print(F("\t"));
            it->getIngredient()->print();
        } else Serial.println();
    }
}

const short Recipe::getIngredientRequiredQty(Ingredient *ingredient)
{
    for (StepIterator it = begin(); it != end(); ++it) {
        if (it->getIngredient() == ingredient) {
            return it->getModQty();
        }
    }
    return 0;
}

bool Recipe::addIngredientQty(const Ingredient *ingredient, short qty)
{

    for (StepIterator it = begin(); it != end(); ++it) {
        if (it->getIngredient() == ingredient) {
            if (_warehouse->isEnough(ingredient, it->getModQty() + qty) && (it->getModQty() + qty)>=0) {
                it->addModQty(qty);
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

void Recipe::adjustTotalVolume(short volume)
{
    float vv=0;
    for (int i=0; i<_ingNum; i++){
        vv+=_ingredientsQty[i];
    }

    Serial.print(F("[RECIPE]"));
    Serial.println(volume/vv);

    for (Recipe::StepIterator it = begin(); it != end(); ++it) {
        it->multiplyModQty(volume / vv);
    }

}

bool Recipe::checkEnoughIngredientsInWarehouse()
{

    calculateIngredientQty();

    for(int i=0; i<_ingNum; i++){
        if (!this->_warehouse->isEnough(_ingredients[i], _ingredientsQty[i])){
            this->_isAvaiable = false;
            return false;
        }
    }

    this->_isAvaiable = true;
    return true;
}

void Recipe::reset()
{
    for(StepIterator it = begin(); it!=end(); ++it){it->reset();}
}

bool Recipe::addStep(char *info){
    if (_stepsNum>=STEPS_NUM){
        Serial.println("[RECIPE]Error: too many steps to store!");
        return false;
    }else {
        _steps[_stepsNum]=Step(info, this);
        _stepsNum+=1;
        return true;
    }
}

bool Recipe::addIngredient(Step step){
    if (_ingNum>=INGREDIENTS_NUM){
        Serial.println("[RECIPE]Error: too many ingredients to store!");
        return false;
    } else if (step.getIngredient()==nullptr){
        Serial.println("[RECIPE]Error: required to store a nullptr ingredient object!");
        return false;
    }else {
        _ingredients[_ingNum]=step.getIngredient();
        _ingNum+=1;
        return true;
    }
}

void Recipe::calculateIngredientQty(){
    memset(_ingredientsQty, 0, sizeof(_ingredientsQty));
    for (int j=0; j<_stepsNum; j++){
        Step* temp = &_steps[j];
        if (temp->getAction()==ADD && temp->getIngredient()!=nullptr){
            for (int i=0; i<_ingNum; i++){
                if (_ingredients[i]==temp->getIngredient()){
                    _ingredientsQty[i]+=temp->getModQty();
                    break;
                }
            }
        }
    }
}

Recipe::Step::Step(char *line, Recipe *recipe){
    char *token = strtok(line, DELIMITER_CHAR);
    this->_action=static_cast<Action>(token[0]);
    switch(this->_action){
        case ADD:
            token = strtok(nullptr, DELIMITER_CHAR);
            this->_ingredient=recipe->_warehouse->getIngredient(token);
            token = strtok(nullptr, DELIMITER_CHAR);
            this->_qty=strtol(token,nullptr, 10);
            this->_modQty=this->_qty;
            break;
        case MIX:
            break; //TODO
        case SHAKE:
            break; //TODO
        default:
            this->_ingredient=nullptr;
            this->_qty=-1;
            this->_modQty=-1;
    }
   
}