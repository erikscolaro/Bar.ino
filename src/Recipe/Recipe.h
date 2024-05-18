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
                short getModQty();
                void addModQty(short qty);
                void multiplyModQty(float scale);
                Ingredient* getIngredient();
            private:
                Action _action = SKIP;
                short _qty = -1;
                short _modQty = -1;
                Ingredient* _ingredient = nullptr;
        };
        
        char _name[RECIPE_NAME_LEN];
        bool _isAvaiable;
        Warehouse* _warehouse;
        Step _steps[STEPS_NUM];
        Ingredient* _ingredients[INGREDIENTS_NUM];
        short _ingredientsQty[INGREDIENTS_NUM];
        uint8_t _ingNum=0, _stepsNum=0;

        bool addStep(char* info);
        bool addIngredient(Step step);

        //stores in the array the qtys of ingredients
        void calculateIngredientQty();

    public:

        class StepIterator {
            private:
                Step* steps;
                size_t current;
            public:
                StepIterator(Step * recipeSteps, size_t index):steps(recipeSteps), current(index) {};
                bool operator==(const StepIterator& other) const {return current == other.current;}
                bool operator!=(const StepIterator& other) const {return !(*this == other);}
                Step& operator*() { return steps[current];}
                StepIterator& operator++() {++current;return *this;}
        };
        
        Recipe();
        Recipe(File* recipeFile, Warehouse* warehouse);
        
        const char* getName() const;
        short getStepsNum() const;
        short getIngredientsNum() const {return _ingNum;}
        Ingredient* const* getIngredients() const {return _ingredients;}
        short getIngredientRequiredQty(Ingredient* ingredient) const;

        //add qty to moqqty relative to the first step that involves the ingredient
        bool addIngredientQty(const Ingredient* ingredient, short qty);
        void adjustTotalVolume(short volume);
        bool checkEnoughIngredientsInWarehouse();
        void reset();
        
        //iterator
        StepIterator begin(){return StepIterator(this->_steps, 0);}
        StepIterator end(){return StepIterator(this->_steps, this->_stepsNum);}
};