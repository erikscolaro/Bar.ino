#include <Adafruit_GFX.h>

class Button: private Adafruit_GFX_Button{

public:
  Button();
  void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1, uint16_t w,
                    uint16_t h, uint16_t radius, uint16_t outline, uint16_t fill,
                    uint16_t textcolor, char *label, uint8_t textsize);
  void drawButton();
  bool contains(int16_t x, int16_t y);

private:
  Adafruit_GFX *_gfx;
  int16_t _x1, _y1;
  uint16_t _w, _h, _radius;
  uint8_t _textsize;
  uint16_t _outlinecolor, _fillcolor, _textcolor;
  char* _label;
};
