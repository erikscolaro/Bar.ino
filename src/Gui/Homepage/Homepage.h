#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#ifndef Gui_h
  #include "Gui/Gui.h"
#endif

class Homepage {
private:
    Gui* master;
    Adafruit_GFX_Button drinkButtons[TILE4COL * TILE4ROW];
    Adafruit_GFX_Button navigationButtons[MAX_PAGES];
    Adafruit_GFX_Button settingsButton;
    uint8_t recipe_reference[TILE4COL * TILE4ROW];
    uint8_t _pagei;
    uint8_t _pagenum;

public:
    Homepage();
    void showHomeNavigationBar();
    void showHomeDrinkButtons();
};

#endif // HOMEPAGE_H
