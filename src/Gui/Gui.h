
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

#include "Recipe/Recipe.h"
#include "Warehouse/Warehouse.h"
#include "Gui/BitmapReader/BitmapReader.h"

//we need a new way to store pages, i can attempt a multiple class file
//in order not to fragmentate too much the

#include "Glyphs.h"
#include "general.settings.h"
#include "gui.settings.h"

class Gui{
    private:
        class Homepage {
            private:
                Gui* _gui;
                Adafruit_GFX_Button drinkButtons[TILE4PAGE];
                Adafruit_GFX_Button navigationButtons[HOMEPAGE_MAX_PAGES];
                Adafruit_GFX_Button settingsButton;
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
                Adafruit_GFX_Button _back, _forward;
                Adafruit_GFX_Button _small, _medium, _large;
                Adafruit_GFX_Button _settings[SETTINGS_MAX_NUM][2];
                uint8_t _settingsNum;
                uint8_t _editIngrIdx[SETTINGS_MAX_NUM];
            
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
            short _state;
            short _previous;
            bool _request;
            Recipe* _selectedRecipe;
        } uiStatus_t;

        uiStatus_t uiStatus;

        void checkInteration(int16_t x, int16_t y);  //da rimuovere
        void changeState(int new_state);
        void showErrorPage(char* error);    //da implementare come classe?

        //transversal
        MCUFRIEND_kbv _tft;
        Warehouse _warehouse;
        Recipe _recipes[RECIPEBOOK_LEN];
        short _recipesLen;

        //general purpose functions
        void showTextCL(const char* text, uint16_t xl, uint16_t yc, int16_t h, const GFXfont *font, uint8_t size, uint16_t color, uint16_t char4line);
        void showImageBL(const char* dir, int x, int y);
        void showTileUL(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t radius, Ingredient* ingredient, Adafruit_GFX_Button* tile);
        void initButtonVarUL(Adafruit_GFX_Button* button, int16_t x1,
                                       int16_t y1, uint16_t w, uint16_t h,
                                       uint16_t outline, uint16_t fill,
                                       uint16_t textcolor, char *label,
                                       uint8_t textsize_x, uint8_t textsize_y, uint16_t radius);
        void drawButtonVar(Adafruit_GFX_Button* button);
        uint16_t getStrHeight();

    public:
        Gui();

        void show();
        bool interact(int xcc, int ycc);

};