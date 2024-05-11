#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

//***********DEBUG SETTINGS*************//

enum DbgLevel: bool {
    NONE = false,
    INGREDIENT = true,
    WAREHOUSE = true,
    RECIPE = true,
    GUI = true,
    HOMEPAGE = true,
    DRINKPAGE = true,
    SETTINGS = true,
    MAIN = true
};

#define PRINT_DBG(dbgLevel, message) if (dbgLevel) Serial.println(message); 

//***********SD SETTINGS*************//

#define SD_SS 53

//***********CSV SETTINGS*************//
#define DELIMITER_CHAR ","

//***********SCEEN SETTINGS*************//
#define CS A3
#define RS A2
#define WR A1
#define RD A0
#define _RST A4


#define RECIPES_DIR "/res/recipes"

#endif; //GENERAL_SETTINGS_H