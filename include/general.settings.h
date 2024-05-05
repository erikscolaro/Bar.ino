#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

//***********DEBUG SETTINGS*************//

enum DbgLevel: bool {
    NONE = false,
    INGREDIENT = true,
    WAREHOUSE = true,
    RECIPE = false
};

#define PRINT_DBG(dbgLevel, message) if (dbgLevel) Serial.println(message); 

//***********SD SETTINGS*************//

#define SD_SS 53


//***********CSV SETTINGS*************//
#define DELIMITER_CHAR ","

#endif //GENERAL_SETTINGS_H