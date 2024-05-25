#include <Adafruit_GFX.h>

class Button: private Adafruit_GFX_Button{

public:
  Button(){};
  void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1, uint16_t w,
                    uint16_t h, uint16_t radius, uint16_t outline, uint16_t fill,
                    uint16_t textcolor, const char *label, uint8_t textsize);
  void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1, uint16_t w,
                    uint16_t h, uint16_t radius, uint16_t outline, uint16_t outline_pressed, uint16_t fill,
                    uint16_t textcolor, const char *label, uint8_t textsize);
  void drawButton();

  void setLabel(char *newLabel);

  bool contains(int16_t x, int16_t y);

  void press();
  void unpress();
  bool isPressed();
  bool justChanged();

  int16_t getX1() const { return _x1; }
  int16_t getY1() const { return _y1; }
  uint16_t getW() const { return _w; }
  uint16_t getH() const { return _h; }
  uint16_t getRadius() const { return _radius; }


private:
  Adafruit_GFX *_gfx;
  int16_t _x1, _y1;
  uint16_t _w, _h, _radius;
  uint8_t _textsize;
  uint16_t _outlinecolor, _outlinepressedcolor, _fillcolor, _textcolor;
  bool _isPressed, _justChanged;
  const char* _label;

  uint16_t darkenColor(uint16_t colore) {
    return ((colore & 0xF81F) >> 1) | ((colore & 0x07E0) >> 1) | ((colore & 0x001F) >> 1);
}
};
