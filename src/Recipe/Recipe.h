#include <Arduino.h>

#include "recipe.settings.h"
#include "general.settings.h"
#include "Warehouse\Ingredient\Ingredient.h"
#include "Warehouse\Warehouse.h"

#include "SdFat.h"
#include "SdFatConfig.h"

enum Action {
    MIX='M',
    ADD='A',
    SHAKE='S',
    SKIP='-'
};

class Recipe {
    private:
        class Step {
            public:
                Step();
                Step(char* line, Recipe* recipe);
                Action getAction();
                short getQty();
                short getModQty();
                Ingredient* getIngredient();
            private:
                Action _action = SKIP;
                short _qty = -1;
                short _modQty = -1;
                Ingredient* _ingredient = nullptr;
        };


        char _name[RECIPE_NAME_LEN];
        Warehouse* _warehouse;
        bool _isAvaiable;

        //to manage data storage
        uint8_t _ingNum=0, _stepsNum=0;
        Step _steps[STEPS_NUM];
        Ingredient* _ingredients[INGREDIENTS_NUM];
        short _ingredientsQty[INGREDIENTS_NUM];

        //to manage iteration on steps
        class Iterator {
            private:
                uint8_t _index, _maxIndex;
            public:
                Iterator();
                Iterator(uint8_t maxIndex);
                bool next(){_index++; return _index<_maxIndex;}
                int index(){return _index;}
        };

        Iterator _stepItr;
        
        bool addStep(char* info);
        bool addIngredient(Step step);
        void calculateIngredientQty();
        bool checkIngredientsQty();

    public:
        Recipe();
        Recipe(File* recipeFile, Warehouse* warehouse);
        
        const char* getName() const;
        short getStepsNum() const;

        //TODO, per la visualizzazione e la modifica della ricetta 
        short getIngredientsNum() const;
        Ingredient* getIngredients() const;
        short getIngredientRequiredQty(const Ingredient* ingredient) const;

        //add or subtract qtys in drink setup page, than adjust volume and finally check if mod qtys
        //are avaiable in warehouse
        bool checkQtyInWarehouse() const;
        bool addIngredientQty(const Ingredient* ingredient, short qty);
        bool adjusttotalVolume(short volume);
        
        //who shold manage ingredients qty stored in eeprom?
        //recipe iterator or the executer?
        //maybe is more concectually correct the executer does.

        
        // questo iteratore si può fare meglio per ottenere il puntatore allo step
        //TODO, per l'esecuzione. tipo iteratore
        bool beginIteration(){_stepItr=Iterator(this->_stepsNum); return this->_stepsNum>0;};
        bool nextStep(){return _stepItr.next();};
        Action getStepAction(){return _steps[_stepItr.index()].getAction();};
        short getStepQty(){return _steps[_stepItr.index()].getModQty();};
        Ingredient* getStepIngredient(){return _steps[_stepItr.index()].getIngredient();};
};