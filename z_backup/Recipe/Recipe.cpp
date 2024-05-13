#include "Recipe.h"


Recipe::Iterator::Iterator():_index(0), _maxIndex(0){}

Recipe::Iterator::Iterator(uint8_t maxIndex):_index(-1), _maxIndex(maxIndex)
{
}

Recipe::Recipe(){
}

Recipe::Recipe(File* file, Warehouse *warehouse) : _warehouse(warehouse)
{
    char buf[50];
    int n;

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

    calculateIngredientQty();
    _isAvaiable = checkIngredientsQty();
}

bool Recipe::checkIngredientsQty()
{
    for(int i=0; i<_ingNum; i++){
        if (this->_warehouse->isEnough(_ingredients[i], _ingredientsQty[i])) return false;
    }

    return true;
}

const char *Recipe::getName() const
{
    return this->_name;
}

short Recipe::getStepsNum() const
{
    return this->_stepsNum;
}

short Recipe::getIngredientRequiredQty(Ingredient *ingredient) const
{
    for (int i=0; i<_ingNum; i++){
        if (_ingredients[i]==ingredient) return _ingredientsQty[i];
    }
    return 0;
}

bool Recipe::checkQtyInWarehouse() const
{
    return this->_isAvaiable;
}

bool Recipe::addIngredientQty(const Ingredient *ingredient, short qty)
{
    for (int i=0; i<_stepsNum; i++){
        if (_steps[i].getIngredient()==ingredient){
            _steps[i].addModQty(qty);
            return true;
        }
    }
    return false;
}

void Recipe::adjustTotalVolume(short volume)
{
    short vv=0;
    for (int i=0; i<_ingNum; i++){
        vv+=_ingredientsQty[i];
    }

    for (int i=0; i<_stepsNum; i++){
        _steps[i].multiplyModQty(volume/vv);
    }

    calculateIngredientQty();

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

Recipe::Step::Step(){
    this->_action=SKIP;
    this->_ingredient=nullptr;
    this->_qty=-1;
}

Recipe::Step::Step(char *line, Recipe *recipe){
    char *token = strtok(line, DELIMITER_CHAR);
    this->_action=static_cast<Action>(token[0]);
    switch(this->_action){
        case ADD:
            token = strtok(nullptr, DELIMITER_CHAR);
            this->_ingredient=recipe->_warehouse->getIngredient(token);
            token = strtok(nullptr, DELIMITER_CHAR);
            this->_qty=strtol(DELIMITER_CHAR,nullptr, 10);
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

Action Recipe::Step::getAction()
{
    return this->_action;
}

short Recipe::Step::getQty()
{
    return this->_qty;
}

short Recipe::Step::getModQty()
{
    return this->_modQty;
}

void Recipe::Step::addModQty(short qty)
{
    _modQty+=qty;
}

void Recipe::Step::multiplyModQty(float scale)
{
    this->_modQty*=scale;
}

Ingredient *Recipe::Step::getIngredient()
{
    return this->_ingredient;
}


