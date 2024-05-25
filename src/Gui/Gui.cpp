#include "Gui.h"

void Gui::showTextCL(char *text, uint16_t xl, uint16_t yc, const GFXfont *font, uint8_t size, uint16_t color, uint16_t numLines, uint16_t lineSpacing, int16_t char4line)
{

    if (font!=NULL) _tft.setFont(font);
    else _tft.setFont(&FreeSans9pt7b);
    if (size>0) _tft.setTextSize(size);
    else _tft.setTextSize(1);
    if (color!=NULL) _tft.setTextColor(color);
    else _tft.setTextColor(CCCC);

    uint16_t str_h = getStrHeight();

    if (numLines<1) numLines=1;
    if (lineSpacing<=0) lineSpacing=str_h/2;
    if (numLines==1 ) lineSpacing=0;
    if (char4line==0) char4line=999;
    
    int printedLineLenght = 0;
    int printedLinesNumber = 0;
    uint16_t yl=yc-(numLines*str_h+(numLines-1)*lineSpacing)/2+str_h;

    char *string = new char[strlen(text)+1];
    strcpy(string, text);

    //sposta il cursore per la prima linea
    _tft.setCursor(xl, yl);

    for (char * token = strtok(string, " ");  token!=NULL && printedLinesNumber<numLines; token=strtok(NULL, " ")){

        if (printedLineLenght+strlen(token)>char4line){
            //calcolo il nuovo yl
            yl+=(str_h+lineSpacing);
            //sposta il cursore alla nuova riga
            _tft.setCursor(xl, yl);
            // incrementa il contatore del numero di righe stampate 
            printedLinesNumber++;
            //modifica printedlinelenght
            printedLineLenght=(strlen(token)+1);
            if (printedLinesNumber<numLines){
                //stampa la nuova parola
                _tft.print(token);_tft.print(" ");
            }
        } else {
            //incrementa il numero di caratteri per riga
            printedLineLenght+=(strlen(token)+1);
            //stampa la nuova parola
            _tft.print(token);_tft.print(" ");
        }
    }
}

void Gui::showTextCL(char *text, uint16_t xl, uint16_t yc, uint16_t color, int16_t char4line)
{
    showTextCL(text, xl, yc, &FreeSans9pt7b, 1, color, 1, 0, char4line);
}

void Gui::showImageBL(const char *dir, int x, int y)
{
    SdFat SD;
    SD.begin(SD_SS, SD_SCK_MHZ(50)); //ev. sd_ss pin

    if (!SD.exists(dir)){
        Serial.print(F("[GUI] Errore apertura file! "));Serial.println(dir);
        return;
    }

    File file = SD.open(dir); //TOEND

    BitmapReader bmpReader(&file);
    uint16_t *buf = new uint16_t[bmpReader.getWidth()];

    uint16_t color;

    int16_t start_y = y - bmpReader.getHeight() + 1;
    for (int yt = y; yt >= start_y && bmpReader.readRow(buf); yt--) {
        uint16_t* buf_ptr = buf;
        for (int16_t i = 0; i < bmpReader.getWidth(); i++, buf_ptr++) {
            color = *buf_ptr;
            if (color <= CUTOFF_COLOR) {
                _tft.drawPixel(x + i, yt, color);
            }
        }
    }

    delete (buf);
    file.close();
    SD.end();
}

void Gui::drawCustomRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, const uint16_t bitmap[])
{
    _tft.startWrite();
    uint16_t pixel;
    for (int16_t j = 0; j < h; j++, y++) {
        for (int16_t i = 0; i < w; i++) {
            pixel=pgm_read_word(&bitmap[j * w + i]);
                if (pixel<=0x39a7) _tft.writePixel(x + i, y, color);
        }
    }
    _tft.endWrite();
}

void Gui::showTileUL(Button* button, char* label)
{
    int16_t x = button->getX1(),y = button->getY1(); 
    uint16_t w = button->getW(),h = button->getH(),radius = button->getRadius();
    uint16_t textbox_h = 40;
    button->drawButton();
    char buf[TILE_CHAR4LINE*2+1];
    sprintf(buf, "%s%s.bmp", IMAGES_DIR, label);
    showImageBL(buf, x + 10 , y + h - textbox_h+10);
    _tft.fillRoundRect(x, y + h - textbox_h, w, textbox_h, radius - 2, CCC);
    _tft.drawRoundRect(x, y + h - textbox_h, w, textbox_h, radius - 2, CCCC);
    sprintf(buf, "%s", label);
    for (size_t i=0; i<strlen(buf); i++) if (buf[i]=='_') buf[i]=' '; 
    showTextCL(buf, x+radius/2, y+h-textbox_h/2, &FreeSans9pt7b, 1, CCCC, 2, 4, TILE_CHAR4LINE);
}

uint16_t Gui::getStrHeight(){
    int16_t x1, y1;
    uint16_t w,h;
    _tft.getTextBounds("ABC", 100, 100, &x1, &y1, &w, &h );
    return h;
}

Gui::Gui(){
    Serial.println(F("[GUI]Called Gui builder"));

    _tft = MCUFRIEND_kbv(CS, RS, WR, RD, _RST);
    _tft.begin(_tft.readID());
    _tft.setRotation(TOUCH_ORIENTATION);

    Serial.println(F("[GUI]_tft object correctly initialized"));

    _recipesNum=0;

    SdFat SD;
    SD.begin();

    File recipesFolder=SD.open(RECIPES_DIR);
    if (!recipesFolder) Serial.println(F("[GUI]Recipes folder not opened"));
    else Serial.println(F("[GUI]Recipes filename list:"));

    File activeRecipe;

    while (activeRecipe.openNext(&recipesFolder, O_RDONLY)) {
        activeRecipe.printName(&Serial);
        Serial.println();
        if (activeRecipe.isFile() && !activeRecipe.isHidden()) _recipesNum++;
        activeRecipe.close();
    }

    recipesFolder.close();
    SD.end();

    Serial.print(F("[GUI] Files counted:"));
    Serial.println(_recipesNum);

    uiStatus._actual=BEGIN;
    requestTransition(STATE_HOMEPAGE);

    _homepage=Homepage(this);
    _drinkPage=DrinkPage(this);
    _settingsPage=SettingsPage(this);
    _executionPage=ExecutionPage(this);
    

    requestRefresh();
}

void Gui::show()
{

    Serial.print(F("[GUI]Gui status:\n[_actual] "));
    Serial.print(uiStatus._actual);
    Serial.print(F(" \t[_next] "));
    Serial.print(uiStatus._next);
    Serial.print(F("\t[_requestrefresh] : "));
    Serial.println(uiStatus._refreshReq);

    State status;
    if (requestedTransition()){
        status = uiStatus._next;
    } else if (requestedRefresh()){
        status = uiStatus._actual;
    } else {
        return; //nothing to update
    }
    
    switch (status){
       case STATE_HOMEPAGE:
           _homepage.show();
           break;
       case STATE_DRINK:
           _drinkPage.show();
           break;
       case STATE_EXECUTER:
           _executionPage.show();
           break;
       case STATE_SETTINGS:
           _settingsPage.show();
           break;
       case BEGIN:
           //power on screen...
           _tft.fillScreen(C_BACK);
           _tft.println("Beginning gui...");
           requestTransition(STATE_HOMEPAGE);
           break;
       default:
           Serial.print(F("[GUI]Error: transition to state "));
           Serial.print(uiStatus._next);
           Serial.println(F("not implemented."));
           requestTransition(BEGIN);
           return;
    }
    completeTransition();
    completeRefresh();
}

bool Gui::interact(int xcc, int ycc)
{
    switch (uiStatus._actual){
        case STATE_HOMEPAGE:
            return _homepage.interact(xcc,ycc);
        case STATE_DRINK:
            return _drinkPage.interact(xcc,ycc);
        case STATE_EXECUTER:
            return _executionPage.interact(xcc,ycc);
        case STATE_SETTINGS:
            return _settingsPage.interact(xcc,ycc);
        case BEGIN: 
            requestTransition(STATE_HOMEPAGE);
            return true;
        default:
            Serial.print( F("[GUI] Error: interaction with "));
            Serial.print(uiStatus._next);
            Serial.println(F(" not implemented."));
            requestTransition(BEGIN);
            return false;
    }
}

Gui::Homepage::Homepage(Gui *gui):_gui(gui){
    _pagenum=_gui->_recipesNum/TILE4PAGE;
    if (_gui->_recipesNum%TILE4PAGE>0) _pagenum++;
    _pagei=0;

    uint16_t tile_spacing = 10, side_spacing = 10, radius = 10;
    uint16_t tw = (320 - side_spacing * 2 - tile_spacing * (TILE4ROW - 1)) / TILE4ROW;
    uint16_t th = (480 - 50 - 2 * side_spacing - tile_spacing * (TILE4COL - 1)) / TILE4COL;
    uint16_t x, y;

    for (int i=0; i< TILE4PAGE; i++){
        x = side_spacing + (tw + tile_spacing) * (i % TILE4ROW);
        y = side_spacing + (th + tile_spacing) * (i / TILE4ROW);
        drinkButtons[i].initButtonUL(
            &gui->_tft, x, y, tw, th, radius, CCC, WHITE, TRANSPARENT, NULL, 1
        );
    }

    const uint16_t button_spacing = 5, bottom_spacing = 5, upper_spacing = 5;
    uint16_t bw = (275 - 2 * side_spacing - button_spacing * (_pagenum - 1)) / _pagenum;
    uint16_t bh = 50 - upper_spacing - bottom_spacing;
    radius = (bh < bw ? bw : bh) / 2;
    y=430 + upper_spacing;

    for (int i = 0; i < _pagenum; i++)
    {
        x = side_spacing + (bw + button_spacing) * i;
        navigationButtons[i].initButtonUL(&gui->_tft, x, y, bw, bh, radius, C, CCCC, CC, CCCC, numbers+i*2 ,1);
        if (i==0) navigationButtons[i].press();
    }
    settingsButton.initButtonUL(&gui->_tft, 275, 435, 40, 40, 10, CC, CC, C, "", 1);
}

void Gui::Homepage::show()
{
    _gui->_tft.setFont(&FreeSans9pt7b);

    //stores names relative to the _pagei into _gui->_recipeNames
    SdFat SD;
    SD.begin(SD_SS, SPI_FULL_SPEED);

    File recipesFolder=SD.open(RECIPES_DIR), activeRecipe;
    if (!recipesFolder) Serial.println(F("[GUI]Recipes folder not opened"));

    int openedFiles=0, idx = 0;

    while (idx < TILE4PAGE && activeRecipe.openNext(&recipesFolder, O_RDONLY)) {

        if (activeRecipe.isFile() && !activeRecipe.isHidden()){
            if (openedFiles >= _pagei*TILE4PAGE){

                activeRecipe.getName(_gui->_recipesNames[idx], RECIPE_NAME_LEN);
                //remove the extension
                _gui->_recipesNames[idx][strlen(_gui->_recipesNames[idx])-4]='\0';
                idx++;
            }
            openedFiles++;
        }
        activeRecipe.close();
    }

    recipesFolder.close();
    SD.end();

    if (_gui->requestedTransition()) {
        _gui->_tft.fillScreen(C_BACK);
        _gui->_tft.fillRect(0, 430, 320, 50, C);
        for (int i = 0; i < _pagenum; i++) navigationButtons[i].drawButton();
        settingsButton.drawButton();
        _gui->drawCustomRGBBitmap(275, 435, 40, 40, CCCC ,settings_bmp);
    } else if (_gui->requestedRefresh()){
        _gui->_tft.fillRect(0, 0, 320, 429, C_BACK);
        for (int i = 0; i < _pagenum; i++){
            if (navigationButtons[i].justChanged()) navigationButtons[i].drawButton();
        }
    }

    for (int i=0; i<min(TILE4PAGE, _gui->_recipesNum-TILE4PAGE*_pagei); i++){
        _gui->showTileUL(&(drinkButtons[i]), _gui->_recipesNames[i]);
    }

    for(int i=0; i<TILE4PAGE; i++){
        Serial.print("[GUI] recipeName: "); 
        Serial.println(_gui->_recipesNames[i]);
    } 

}

bool Gui::Homepage::interact(int xcc, int ycc)
{
    for (int i=0; i<TILE4PAGE; i++){
        if (drinkButtons[i].contains(xcc, ycc)){
            _gui->setSelectedRecipeName(_gui->_recipesNames[i]);
            _gui->requestTransition(STATE_DRINK);
            return true;
        }
    }

    for (int i = 0; i < _pagenum; i++)
    {
        if (navigationButtons[i].contains(xcc, ycc)){
            navigationButtons[_pagei].unpress();
            _pagei=i;
            navigationButtons[i].press();
            _gui->requestRefresh();
            return true;
        }
    }

    if (settingsButton.contains(xcc, ycc)){
        _gui->requestTransition(STATE_SETTINGS);
        return true;
    }

    return false;
}

Gui::SettingsPage::SettingsPage(Gui *gui):_gui(gui)
{
}

void Gui::SettingsPage::show()
{
    _gui->_tft.fillScreen(C_BACK);

    int x=5, y=15, h=8, w=100;
    int16_t i=0, aqty, mqty;
    char buffer[BUF_LEN];
    _gui->_tft.setFont(&FreeSans9pt7b);
    _gui->_tft.setTextSize(1);
    Ingredient* tempIng;
    for( int i=0; i<_gui->_warehouse.getIngredientsNumber(); i++){
        tempIng=_gui->_warehouse.getIngredient(i);
        aqty=tempIng->getQuantity();
        mqty=tempIng->getMaxQuantity();
        _gui->_tft.setCursor(x,y);
        sprintf(buffer, "%02d. %-15s%s", i+1, tempIng->getName(), strlen(tempIng->getName())>15?"...":"");
        _gui->_tft.print(buffer);
        _gui->_tft.drawRect(215, y-12, 104, 13, CCCC);
        double perc = (1.0*aqty)/mqty*100.0;
        _gui->_tft.fillRect(217, y-10, floor(perc), 9, perc>50?CCCC:(perc>25?TFT_ORANGE:TFT_RED));
        _gui->_tft.setCursor(165,y);
        sprintf(buffer, "%3d%%", (int) perc);
        _gui->_tft.print(buffer);
        y+=15;
    }
}

bool Gui::SettingsPage::interact(int xcc, int ycc)
{
    _gui->requestTransition(STATE_HOMEPAGE);
    return true;
}

Gui::ExecutionPage::ExecutionPage(Gui *gui): _gui(gui)
{
    //TODO
}

void Gui::ExecutionPage::show()
{
    //TODO
    _gui->_tft.fillScreen(C_BACK);
    Recipe * r = _gui->getSelectedRecipeObj();

    _gui->_tft.setFont(&FreeSans9pt7b);
    _gui->_tft.setCursor(10,20);
    _gui->_tft.println(r->getName());
    for (Recipe::StepIterator it = r->begin(); it!=r->end(); ++it){
        _gui->_tft.print((char) it->getAction());_gui->_tft.print("  ");
        if (it->getAction()==ADD) {
            _gui->_tft.print(it->getIngredient()->getName());_gui->_tft.print("  ");
            _gui->_tft.println(it->getModQty());

            //eeprom ingredient consumption to simulate drink making
            it->getIngredient()->subtractQuantity(it->getModQty());
        } else _gui->_tft.println(" ");
    }
    char* error = ERROR_MSG_not_implemented;
    _gui->showPopup(error);
}

bool Gui::ExecutionPage::interact(int xcc, int ycc)
{
    _gui->requestTransition(STATE_HOMEPAGE);
    return true;
}

Gui::DrinkPage::DrinkPage(Gui *gui): _gui(gui)
{
    MCUFRIEND_kbv* tftptr = _gui->getTftptr();
    uint16_t x,y,h,w,r;
    x=9, y=65, w=50, h=60,r=25;
    this->_back.initButtonUL(tftptr,x,y+5,w,h-10,r,CCCC,TFT_ORANGE,CCCC,nullptr,1);
    x+=w+13, r=10;
    this->_small.initButtonUL(tftptr,x,y,w,h,r, CCCC, WHITE, CCCC, nullptr, 1);
    x+=w+13;
    this->_medium.initButtonUL(tftptr,x,y,w,h,r, CCCC, WHITE, CCCC, nullptr, 1);
    this->_medium.press();
    x+=w+13;
    this->_large.initButtonUL(tftptr,x,y,w,h,r, CCCC, WHITE, CCCC, nullptr, 1);
    x+=w+13, r=25;
    this->_forward.initButtonUL(tftptr,x,y+5,w,h-10,r, CCCC, TFT_OLIVE, CCCC, nullptr, 1);

    x+=10, y=155, h=55, w=55, r=15;
    for (int i=0; i<SETTINGS_MAX_NUM; i++){
        x=145; 
        this->_settings[i][0].initButtonUL(tftptr, x, y, w, h, r, CCCC, CC, CCCC, nullptr, 3);
        x=245;
        this->_settings[i][1].initButtonUL(tftptr, x, y, w, h, r, CCCC, CC, CCCC, nullptr, 3);
        y+=h+10;
    }
}
    
void Gui::DrinkPage::show()
{
    //new text
    Recipe* selRecipe = _gui->getSelectedRecipeObj();
    MCUFRIEND_kbv* tftptr = _gui->getTftptr();
    uint16_t x,y,h,w,r;

    if (_gui->requestedTransition()){ //complete show
        tftptr->fillScreen(C_BACK);

        char buf[BUF_LEN];
        sprintf(buf, "%.10s%s\0", selRecipe->getName(), strlen(selRecipe->getName())>10?"...":"");
        for (int i=0; i<strlen(buf); i++) if (buf[i]=='_') buf[i]=' '; 

        x=9, y=65, w=50, h=60, r=25;
        _gui->showTextCL(buf, x, y/2, &FreeSansBold12pt7b, 2, CCCC, 1, 0, 0);

        _gui->drawCustomRGBBitmap(x,y,w,h,CCCC,back_bmp);
        x+=w+13;
        _small.unpress();
        _small.drawButton();
        _gui->drawCustomRGBBitmap(x,65,w,h, _small.isPressed()?CCCC:CCC, shot_bmp);
        x+=w+13;
        _medium.press();
        _medium.drawButton();
        _gui->drawCustomRGBBitmap(x,65,w,h, _medium.isPressed()?CCCC:CCC, medium_bmp);
        x+=w+13;
        _large.unpress();
        _large.drawButton();
        _gui->drawCustomRGBBitmap(x,65,w,h, _large.isPressed()?CCCC:CCC, large_bmp);
        x+=w+13;
        _forward.drawButton();
        _gui->drawCustomRGBBitmap(x,y,w,h,CCCC,ok_bmp);

        x=10, y+=10+h;
        tftptr->drawFastHLine(x, y, tftptr->width()-x*2, CCCC);
    
        x=10, y=145, r=15;
        tftptr->fillRoundRect(x, y, tftptr->width()-x*2, tftptr->height(), r, C_BACK);
        tftptr->drawRoundRect(x, y, tftptr->width()-x*2, tftptr->height(), r, CCCC);

        x+=10, y+=10, h=55, w=55;
        int i=0, printed=0; 

        for( Recipe::StepIterator r = selRecipe->begin(); r!=selRecipe->end() && printed<SETTINGS_MAX_NUM; ++r){
            if (r->getAction()==ADD && r->getIngredient()->isEditable()){
                //label
                x=20;
                sprintf(buf, "%.12s%s\0", r->getIngredient()->getName(), strlen(r->getIngredient()->getName())>12?"...":"");
                buf[0]>=97?buf[0]-=32:false;//to capitolize the first letter
                _gui->showTextCL(buf, x, y+h/2, &FreeSansBold12pt7b, 1, CCCC, 1, 0, 0);

                //qty
                x=145+w+10;
                sprintf(buf, "%d\0", r->getModQty());
                _gui->showTextCL(buf, x, y+h/2, &FreeSansBold12pt7b, 1, CCCC, 1, 0, 0);

                //buttons
                x=145;
                _settings[printed][0].drawButton();
                _gui->drawCustomRGBBitmap(x,y,w,h,CCCC,minus);
                x=245;
                _settings[printed][1].drawButton();
                _gui->drawCustomRGBBitmap(x,y,w,h,CCCC,plus);

                y+=h+10;
                printed++;
            }
            i++;
        }

    } else if (_gui->requestedRefresh()){ //only refresh
        char buf[BUF_LEN];

        x=9, y=65, w=50, h=60, r=25;
        x+=w+13;
        if (_small.justChanged()){
            _small.drawButton();
            _gui->drawCustomRGBBitmap(x,65,w,h, _small.isPressed()?CCCC:CCC, shot_bmp);
        }
        x+=w+13;
        if (_medium.justChanged()){
            _medium.drawButton();
            _gui->drawCustomRGBBitmap(x,65,w,h, _medium.isPressed()?CCCC:CCC, medium_bmp);
        }
        x+=w+13;
        if (_large.justChanged()){
            _large.drawButton();
            _gui->drawCustomRGBBitmap(x,65,w,h, _large.isPressed()?CCCC:CCC, large_bmp);
        }
        x=20, y=155, h=55, w=55, r=15;

        int i=0, printed=0; 
        for( Recipe::StepIterator r = selRecipe->begin(); r!=selRecipe->end() && printed<SETTINGS_MAX_NUM; ++r){
            if (r->getAction()==ADD && r->getIngredient()->isEditable()){
                //buttons
                if (_settings[printed][0].justChanged()){ //minus
                    bool thereis = selRecipe->addIngredientQty(r->getIngredient(), r->getIngredient()->isLiquid()?-10:-1);
                    sprintf(buf, "%d\0", selRecipe->getIngredientRequiredQty(r->getIngredient()));
                    _gui->_tft.fillRect(145+w+1, y, w-15,h, C_BACK);
                    _gui->showTextCL(buf, 145+w+10, y+h/2, &FreeSansBold12pt7b, 1, thereis?CCCC:TFT_RED, 1, 0, 0);
                    break;
                } else if (_settings[printed][1].justChanged()){ //plus
                    bool thereis = selRecipe->addIngredientQty(r->getIngredient(), r->getIngredient()->isLiquid()?10:1);
                    sprintf(buf, "%d\0", selRecipe->getIngredientRequiredQty(r->getIngredient()));
                    _gui->_tft.fillRect(145+w+1, y, w-15,h, C_BACK);
                    _gui->showTextCL(buf, 145+w+10, y+h/2, &FreeSansBold12pt7b, 1, thereis?CCCC:TFT_RED, 1, 0, 0);
                    break;                
                }

                y+=h+10;
                printed++;
            }
            i++;
        }
    }
}

bool Gui::DrinkPage::interact(int xcc, int ycc)
{
    Recipe* selRecipe = &_gui->uiStatus._activeRecipe;
    if (_back.contains(xcc,ycc)) {
        _gui->requestTransition(STATE_HOMEPAGE);
        return true;
    } else if (_forward.contains(xcc,ycc)){
        if (_large.isPressed()) selRecipe->adjustTotalVolume(DrinkCapacity::LARGE);
        else if (_medium.isPressed()) selRecipe->adjustTotalVolume(DrinkCapacity::MEDIUM);
        else if (_small.isPressed()) selRecipe->adjustTotalVolume(DrinkCapacity::SMALL);
        else {
            char* error = ERROR_MSG_select_dimension;
            _gui->showPopup(error);
            return true;
        }
        if (selRecipe->checkEnoughIngredientsInWarehouse()){
            _gui->requestTransition(STATE_EXECUTER);
        } else {
            char* error = ERROR_MSG_not_enought_ingredients;
            _gui->showPopup(error);
        }
        return true;
    } else if (_small.contains(xcc,ycc)){
        _small.press();
        _medium.unpress();
        _large.unpress();
        _gui->requestRefresh();
        return true;
    } else if (_medium.contains(xcc,ycc)){
        _small.unpress();
        _medium.press();
        _large.unpress();
        _gui->requestRefresh();
        return true;
    } else if (_large.contains(xcc,ycc)){
        _small.unpress();
        _medium.unpress();
        _large.press();
        _gui->requestRefresh();
        return true;
    } else {
        for (int i=0; i<SETTINGS_MAX_NUM; i++){
            if (this->_settings[i][0].contains(xcc,ycc)){
                this->_settings[i][0].press();
                _gui->requestRefresh();
                return true;
            }

            if (this->_settings[i][1].contains(xcc,ycc)){
                this->_settings[i][1].press();
                _gui->requestRefresh();
                return true;
            }
        }    
    }
    return false;
}

void Gui::setSelectedRecipeName(char *recipeName)
{
    SdFat SD;
    SD.begin(SD_SS, SD_SCK_MHZ(16));
    char buf[RECIPE_NAME_LEN+10];
    sprintf(buf, "%s/%s.csv", RECIPES_DIR, recipeName);
    File f = SD.open(buf);
    uiStatus._activeRecipe=Recipe(&f, &_warehouse);
    f.close();
    SD.end();
}

Recipe *Gui::getSelectedRecipeObj()
{
    return &uiStatus._activeRecipe;
}

void Gui::requestTransition(State newState)
{
    uiStatus._next=newState;
}

bool Gui::requestedTransition()
{
    return uiStatus._actual!=uiStatus._next;
}

void Gui::completeTransition()
{
    uiStatus._actual=uiStatus._next;
}

void Gui::requestRefresh()
{
    uiStatus._refreshReq=true;
}

void Gui::completeRefresh()
{
    uiStatus._refreshReq=false;
}

void Gui::showPopup(char *error)
{
    _tft.fillRoundRect(20, _tft.height()/5*2, _tft.width()-40, _tft.height()/5, 20, C);
    _tft.drawRoundRect(20, _tft.height()/5*2, _tft.width()-40, _tft.height()/5, 20, CCCC);
    showTextCL(error, 25, _tft.height()/2, &FreeSans9pt7b, 1, CCCC, 3, 0, 30);
    delay(2000);

    requestTransition(uiStatus._actual);
    uiStatus._actual=ERROR;

    show();
}

bool Gui::requestedRefresh()
{
    return uiStatus._refreshReq;
}
