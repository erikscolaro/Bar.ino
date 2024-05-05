#include <Arduino.h>

#include "recipe.settings.h"
#include "general.settings.h"
#include "Ingredient\Ingredient.h"
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
        uint8_t _ingNum=0, _stepsNum=0;
        Warehouse* _warehouse;
        Step _steps[STEPS_NUM];
        Ingredient* _ingredients[INGREDIENTS_NUM];
        short _ingredientsQty[INGREDIENTS_NUM];
        bool _isAvaiable;
        
        bool addStep(char* info);
        bool addIngredient(Step step);
        void calculateIngredientQty();

    public:
        Recipe();
        Recipe(const char* dir, Warehouse* warehouse);
        bool checkIngredientsQty();



};