#include <Arduino.h>

#include "recipe.settings.h"
#include "general.settings.h"

#include "SdFat.h"
#include "Ingredient\Ingredient.h"
#include "Warehouse\Warehouse.h"

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
                Ingredient* getIngredient();
            private:
                Action _action = SKIP;
                short _qty = -1;
                Ingredient* _ingredient = nullptr;
        };

        char _name[RECIPE_NAME_LEN];
        uint8_t _ingNum=0, _stepsNum=0;;
        Ingredient* _ingredients[INGREDIENTS_NUM];
        uint8_t _ingredientsQty[INGREDIENTS_NUM];
        Step _steps[STEPS_NUM];
        Warehouse* _warehouse;

        bool addStep(char* info);
        bool addIngredient(Step step);

    public:
        Recipe(const char* dir, Warehouse* warehouse);

};