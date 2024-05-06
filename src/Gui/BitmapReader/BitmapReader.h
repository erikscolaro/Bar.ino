#include <Arduino.h>
#include <SdFat.h>
#include <SPI.h>


class BitmapReader{
    public:
        BitmapReader();
        BitmapReader(File* bmp);

        void readBmp();


    private:
        uint32_t _fileSize;
        uint32_t _imageDataOffset;
        uint32_t _width;
        uint32_t _height;
        uint16_t _bitsPerPixel;
        File* _bmp;

};