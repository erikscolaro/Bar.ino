#include "Recipe.h"

Recipe::Recipe(const char *dir, Warehouse* warehouse):_warehouse(warehouse){
    SdFat SD;
    SD.begin(SD_SS);
    File file=SD.open(dir);

    char buf[50];
    int n;

    while (file.available() && _stepsNum<=STEPS_NUM && _ingNum<=INGREDIENTS_NUM){
        n = file.fgets(buf, sizeof(buf));

        if (buf[n - 1] != '\n' && n == (sizeof(buf) - 1)) {
            PRINT_DBG(RECIPE, "ERROR: line of csv recipe file is too long!")
            file.close();
            SD.end();
            while(true);
        } else {
            addStep(buf);
            if (_steps[_stepsNum-1].getAction()==ADD) addIngredient(_steps[_stepsNum-1]);
        }
    }

    file.close();
    SD.end();

    _isAvaiable = checkIngredientsQty();
}

bool Recipe::checkIngredientsQty()
{
    for(int i=0; i<_ingNum; i++){
        if (_ingredients[i]->isEditable() && _ingredients[i]->getQuantity()<_ingredientsQty[i]) return false;
    }

    return true;
}

bool Recipe::addStep(char *info){
    if (_stepsNum>=STEPS_NUM){
        PRINT_DBG(RECIPE, "Error: too many steps to store!");
        return false;
    }else {
        _steps[_stepsNum]=Step(info, this);
        _stepsNum+=1;
        return true;
    }
}

bool Recipe::addIngredient(Step step){
    if (_ingNum>=INGREDIENTS_NUM){
        PRINT_DBG(RECIPE, "Error: too many ingredients to store!");
        return false;
    } else if (step.getIngredient()==nullptr){
        PRINT_DBG(RECIPE, "Error: required to store a nullptr ingredient object!");
        return false;
    }else {
        for (int i=0; i<_ingNum; i++){
            if (_ingredients[i]==step.getIngredient()){
                _ingredientsQty[i]+=step.getQty();
                return true;
            }
        }
        _ingredients[_ingNum]=step.getIngredient();
        _ingNum+=1;
        return true;
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
    token = strtok(nullptr, DELIMITER_CHAR);
    this->_ingredient=recipe->_warehouse->getIngredient(token);
    token = strtok(nullptr, DELIMITER_CHAR);
    this->_qty=strtol(DELIMITER_CHAR,nullptr, 10);
}

Action Recipe::Step::getAction()
{
    return this->_action;
}

short Recipe::Step::getQty()
{
    return this->_qty;
}

Ingredient *Recipe::Step::getIngredient()
{
    return this->_ingredient;
}
