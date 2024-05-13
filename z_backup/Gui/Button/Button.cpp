#include "Button.h"
#include "gui.settings.h"

void Button::initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t radius, uint16_t outline, uint16_t fill, uint16_t textcolor, char *label, uint8_t textsize)
{
    _x1 = x1;
    _y1 = y1;
    _w = w;
    _h = h;
    _radius=radius;
    _outlinecolor = outline;
    _fillcolor = fill;
    _textcolor = textcolor;
    _textsize= textsize;
    _gfx = gfx;
    _label=label;
    _isPressed= false;
    _justChanged=false;
}

void Button::drawButton()
{
    uint8_t r = _radius>1?_radius:1;
    if (_fillcolor != TRANSPARENT) _gfx->fillRoundRect(_x1, _y1, _w, _h, r, _fillcolor);
    if (_outlinecolor != TRANSPARENT) _gfx->drawRoundRect(_x1, _y1, _w, _h, r, _outlinecolor);
    //TOFIX
    _gfx->setCursor(_x1 + (_w / 2) - (strlen(_label) * 3 * _textsize),
                    _y1 + (_h / 2) - (4 * _textsize));
    if (_textcolor != TRANSPARENT && _label!=nullptr){
        _gfx->setTextColor(_textcolor);
        _gfx->setTextSize(_textsize, _textsize);
        _gfx->print(_label);
        }
}

bool Button::contains(int16_t x, int16_t y)
{
    return ((x >= _x1) && (x < (int16_t)(_x1 + _w)) && (y >= _y1) &&
          (y < (int16_t)(_y1 + _h)));
}

void Button::press()
{
    _isPressed==true?_justChanged=false:_isPressed=true, _justChanged=true;
}

void Button::unpress()
{
    _isPressed==false?_justChanged=false:_isPressed=false, _justChanged=true;
}

bool Button::isPressed()
{
    return _isPressed;
}

bool Button::justChanged()
{
    if (_justChanged){
        _justChanged=false;
        return true;
    }
    return false;
}
