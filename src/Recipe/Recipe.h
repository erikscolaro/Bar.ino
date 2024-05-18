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
                Step():_action(SKIP), _qty(-1), _modQty(-1), _ingredient(nullptr){};
                Step(char* line, Recipe* recipe);
                Action getAction() const            {return _action;}
                short getModQty() const             {if (_action!=ADD) return 0; return _modQty;}
                void addModQty(short qty)           {_modQty+=qty;}
                void multiplyModQty(float scale)    {_modQty*=scale;}
                void reset()                        {this->_modQty=_qty;}
                Ingredient* getIngredient() const   {return _ingredient;}
            private:
                Action _action;
                short _qty;
                short _modQty;
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
                bool operator==(const StepIterator& other) const    {return current == other.current;}
                bool operator!=(const StepIterator& other) const    {return !(*this == other);}
                Step* operator->()                                  {return &steps[current];}
                StepIterator& operator++()                          {++current;return *this;}
        };
        
        Recipe(){};
        Recipe(File* recipeFile, Warehouse* warehouse);

        void print();
        
        const char* getName() const                 {return _name;}
        short getStepsNum() const                   {return _stepsNum;}
        short getIngredientsNum() const             {return _ingNum;}
        Ingredient* const* getIngredients() const   {return _ingredients;}
        const short getIngredientRequiredQty(Ingredient* ingredient);

        //add qty to moqqty relative to the first step that involves the ingredient
        bool addIngredientQty(const Ingredient* ingredient, short qty);
        void adjustTotalVolume(short volume);
        bool checkEnoughIngredientsInWarehouse();
        void reset();
        
        //iterator
        StepIterator begin()                        {return StepIterator(this->_steps, 0);}
        StepIterator end()                          {return StepIterator(this->_steps, this->_stepsNum);}
};