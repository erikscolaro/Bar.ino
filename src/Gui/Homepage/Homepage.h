#include "Gui\Gui.h"


class Homepage: public Gui{

    private:
        Adafruit_GFX_Button drinkButtons[TILE4COL*TILE4ROW]; //suggerisco di creare una nuova classe bottone per sminchiarmi di meno dopo
        Adafruit_GFX_Button navigationButtons[MAX_PAGES];
        Adafruit_GFX_Button settingsButton;
        uint8_t recipe_reference[TILE4COL*TILE4ROW];
        uint8_t _pagei;
        uint8_t _pagenum;

    public:
        Homepage();

        void showHomeNavigationBar();
        void showHomeDrinkButtons();
};