#include "BitmapReader.h"

BitmapReader::BitmapReader(File *bmp):_bmp(bmp){
    _bmp->seek(2);
    _bmp->read(&this->_fileSize, sizeof(uint32_t));
    _bmp->seek(10);
    _bmp->read(&_imageDataOffset, sizeof(uint32_t));
    _bmp->seek(18);
    _bmp->read(&_width, sizeof(uint32_t));
    _bmp->seek(22);
    _bmp->read(&_height, sizeof(uint32_t));
    _bmp->seek(28);
    _bmp->read(&_bitsPerPixel, sizeof(uint16_t));
}