
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

#include "Recipe/Recipe.h"
#include "Gui/BitmapReader/BitmapReader.h"

#include "Glyphs.h"
#include "general.settings.h"
#include "gui.settings.h"

class Gui{
    protected:
        MCUFRIEND_kbv tft;
        BitmapReader bmpReader;

        //general purpose functions
        void showTextCL(char* text, uint16_t xl, uint16_t yc, uint16_t h, const GFXfont *font, uint8_t size, uint16_t color, uint16_t char4line);
        void showImageUL(char* filename, int x, int y);
        void showTile(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t radius, uint8_t recipe_index, Adafruit_GFX_Button* tile);
        uint16_t getStrHeight();
        
    private:

        //non sono proprio sicuro, devo scegliere se deve essere una classe di utilità per la stampa o se avere anche altre funzioni
        uint8_t _sel_drink;
        int _state;
        int _previous_state;
        bool _change_requested;
        void checkInteration(int16_t x, int16_t y); 
        void changeState(int new_state);
        void showErrorPage(char* error);

    public:
        Gui();

        virtual void draw(); //da implementare per tutte le pagine figlie

};