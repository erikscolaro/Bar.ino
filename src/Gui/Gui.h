
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

class test {
private:
    //soluzione al problema del gui: usare puntatori opachi?
    void* _gui;
    Adafruit_GFX_Button drinkButtons[TILE4COL * TILE4ROW];
    Adafruit_GFX_Button navigationButtons[MAX_PAGES];
    Adafruit_GFX_Button settingsButton;
    uint8_t recipe_reference[TILE4COL * TILE4ROW];
    uint8_t _pagei;
    uint8_t _pagenum;

public:
    test(void* gui):_gui(gui){};
    void showHomeNavigationBar();
    void showHomeDrinkButtons();
};

class Gui{
    private:
        MCUFRIEND_kbv _tft;
        Recipe _recipes[RECIPEBOOK_LEN];
        Warehouse _warehouse;
        test tt=test(this);
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

        //non sono proprio sicuro, devo scegliere se deve essere una classe di utilità per la stampa o se avere anche altre funzioni
        int _state;
        int _previous_state;
        bool _change_requested;
        void checkInteration(int16_t x, int16_t y); 
        void changeState(int new_state);
        void showErrorPage(char* error);

    public:
        Gui();

        void draw(){}; //da implementare per tutte le pagine figlie

};