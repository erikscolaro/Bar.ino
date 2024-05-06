#include "Gui\Gui.h"

class DrinkPage: public Gui{
    private: 
        Adafruit_GFX_Button back, forward;
        Adafruit_GFX_Button small, medium, large;
        Adafruit_GFX_Button settings[SETTINGS_MAX_NUM][2];
        uint8_t settings_num;
        uint8_t editIngrIdx[SETTINGS_MAX_NUM];
    
    public:
        DrinkPage();

        void showDrinksetPage(bool redraw_all, bool new_cc);
};