#include <Arduino.h>
#include <SdFat.h>
#include <SPI.h>


class BitmapReader{
    public:
        BitmapReader();
        BitmapReader(File* bmp);
        bool readRow(uint16_t *buf);

        uint32_t getFileSize() const;
        uint32_t getImageDataOffset() const;
        uint32_t getWidth() const;
        uint32_t getHeight() const;
        uint16_t getBitsPerPixel() const;
        File* getBmp();

    private:
        uint32_t _fileSize;
        uint32_t _imageDataOffset;
        uint32_t _width;
        uint32_t _height;
        uint16_t _bitsPerPixel;
        File* _bmp;

};