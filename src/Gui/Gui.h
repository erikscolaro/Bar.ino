
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

#include "Recipe/Recipe.h"
#include "Warehouse/Warehouse.h"
#include "Gui/BitmapReader/BitmapReader.h"
#include "Gui/Button/Button.h"

#include "Glyphs.h"
#include "general.settings.h"
#include "gui.settings.h"

class Gui{
    private:
        //transversal
        MCUFRIEND_kbv _tft;
        Warehouse _warehouse;
        char _recipesNames[TILE4PAGE][RECIPE_NAME_LEN];
        
        
        short _recipesNum;
        MCUFRIEND_kbv* getTftptr(){return &_tft;}

        enum State {STATE_HOMEPAGE, STATE_DRINK, STATE_SETTINGS, STATE_EXECUTER, ERROR, BEGIN};

        class Homepage {
            private:
                Gui* _gui;
                Button drinkButtons[TILE4PAGE];
                Button navigationButtons[HOMEPAGE_MAX_PAGES];
                Button settingsButton;
                short _pagei;
                short _pagenum;

            public:
                Homepage(){};
                Homepage(Gui* gui);
                void show();
                bool interact(int xcc, int ycc);
        };
        

        class SettingsPage{
            private: 
                Gui* _gui;
            
            public:
                SettingsPage(){};
                SettingsPage(Gui* gui);
                void show();
                bool interact(int xcc, int ycc);
        };

        
        class ExecutionPage{
            private: 
                Gui* _gui;
            
            public:
                ExecutionPage(){};
                ExecutionPage(Gui* gui);
                void show();
                bool interact(int xcc, int ycc);
        };

        class DrinkPage{
            private: 
                Gui* _gui;
                Button _back, _forward;
                Button _small, _medium, _large;
                Button _settings[SETTINGS_MAX_NUM][2];
            
            public:
                DrinkPage(){};
                DrinkPage(Gui *gui);
                void show();
                bool interact(int xcc, int ycc);
        };
        //pages

        Homepage _homepage;
        SettingsPage _settingsPage;
        ExecutionPage _executionPage;
        DrinkPage _drinkPage;

        //needed to manage page transition
        typedef struct {
            State _actual;
            State _next;
            Recipe _activeRecipe;
            bool  _refreshReq;
        } uiStatus_t;

        uiStatus_t uiStatus;

        //status methods
        void setSelectedRecipeName(char* recipeName);
        Recipe* getSelectedRecipeObj();

        void requestTransition(State newState);
        bool requestedTransition();
        void completeTransition();

        void requestRefresh();
        bool requestedRefresh();
        void completeRefresh();

        //interact
        void checkInteractions(int16_t x, int16_t y);

        //error 
        void showPopup(char* error);

        //drawing methods
        void showTextCL(const char* text, uint16_t xl, uint16_t yc, int16_t h, const GFXfont *font, uint8_t size, uint16_t color, int16_t char4line);
        void showImageBL(const char* dir, int x, int y);
        void drawCustomRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h ,uint16_t color,const uint16_t bitmap[]);
        void showTileUL(Button *button, const char* label);
        uint16_t getStrHeight();

    public:
        Gui();
        void show();
        bool interact(int xcc, int ycc);
        int screenWidth(){return _tft.width();}
        int screenHeight(){return _tft.height();}
};