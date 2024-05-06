#include "Gui.h"

void Gui::showTextCL(const char *text, uint16_t xl, uint16_t yc, int16_t h, const GFXfont *font, uint8_t size, uint16_t color, uint16_t char4line)
{
    if (font!=NULL) _tft.setFont(font);
    _tft.setTextSize(size);
    _tft.setTextColor(color);
    if (h==-1){ 
        h=getStrHeight();
        _tft.setCursor(xl, yc + h - size);
    } else if (h==0){
        _tft.setCursor(xl, yc+h/2);
    } else {
        _tft.setCursor(xl, yc);
    }

    char *buf=new char[strlen(text)];
    strcpy(buf, text);

    if (strlen(buf) < char4line || char4line==-1)
    {
        _tft.print(buf);
        delete(buf);
        return;
    }
    else
    {
        char *token;
        int nchar = 0;
        token = strtok(buf, " ");
        while (token != NULL)
        {
            int tokn = strlen(token);
            if (nchar + tokn <= char4line)
            {
                nchar += tokn;
                _tft.print(token);
                _tft.print(" "); // Aggiungi uno spazio tra le parole
            }
            else if (tokn > char4line)
            { // Caso in cui la parola supera la lunghezza massima per riga
                char *buf2=new char [char4line];
                size_t nc = strlcpy(buf2, token, char4line - nchar);
                _tft.print(buf2);
                _tft.print(" "); // Aggiungi uno spazio
                if ((nchar + strlen(token)) / char4line >= 2){
                    delete buf2, buf; return;
                }
                _tft.setCursor(xl, yc+h/2);
                strcpy(buf2, token + nc);
                _tft.print(buf2);
                nchar = sizeof(tokn);
                
                delete buf2;
            }
            else
            {
                if ((nchar + strlen(token)) / char4line >= 2){delete buf; return;}
                nchar = char4line;
                _tft.setCursor(xl, yc+h/2);
                _tft.print(token);
                _tft.print(" "); // Aggiungi uno spazio
                nchar += strlen(token);
            }

            token = strtok(NULL, " ");
        }
    }
    delete buf;
}

void Gui::showImageBL(const char *dir, int x, int y)
{
    SdFat SD;
    SD.begin(); //ev. sd_ss pin

    if (!SD.exists(dir)){
        PRINT_DBG(GUI, "Errore apertura file! "+String(dir));
        return;
    }

    File file = SD.open(dir); //TOEND

    BitmapReader bmpReader(&file);
    uint16_t *buf = new uint16_t[bmpReader.getWidth()];

    uint16_t color;

    for (int yt = y; 
        (yt >= y - bmpReader.getHeight()) && bmpReader.readRow(buf); 
        yt--){
        for (int16_t i = 0; i < bmpReader.getWidth(); i++){
            color = buf[i];
            if (color <= CUTOFF_COLOR){
                _tft.drawPixel(x + i, yt, buf[i]);
            }
        }
    }

    delete (buf);
    file.close();
    SD.end();
}

void Gui::showTileUL(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t radius, Ingredient *ingredient, Adafruit_GFX_Button *tile)
{
    initButtonVarUL(tile, x, y, w, h, CCC, WHITE, TRANSPARENT, nullptr, -1, 1, radius);
    uint16_t textbox_h = 40;
    drawButtonVar(tile);
    showImageBL(ingredient->getName(), x, y + h - textbox_h);
    _tft.fillRoundRect(x + 1, y + h - textbox_h + 1, w - 2, textbox_h - 2, radius - 2, CCC);
    showTextCL(ingredient->getName(), x + radius / 2, y + (h - radius) / 2, textbox_h - radius / 2, &FreeSans9pt7b, 1, CCCC, TILE_CHAR4LINE);
}

uint16_t Gui::getStrHeight(){
    int16_t x1, y1;
    uint16_t w,h;
    _tft.getTextBounds("ABC", 100, 100, &x1, &y1, &w, &h );
    return h;
}

Gui::Gui(){
    _tft = MCUFRIEND_kbv(CS, RS, WR, RD, _RST);

    _recipesLen=0;

    //warehouse construtor implicitly invoked before gui class construction

    SdFat SD;
    SD.begin();

    File recipesFolder=SD.open(RECIPES_DIR);
    File activeRecipe;

    while (true && _recipesLen<=RECIPEBOOK_LEN) {
        activeRecipe = recipesFolder.openNextFile();
        if (!activeRecipe) break;
        _recipes[_recipesLen]=Recipe(&activeRecipe, &_warehouse);
        activeRecipe.close();
        _recipesLen+=1;
  }

    recipesFolder.close();
    SD.end();
}
