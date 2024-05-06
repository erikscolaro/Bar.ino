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
    _bmp->seek(_imageDataOffset);
}

//assuming buf has length as image width...
bool BitmapReader::readRow(uint16_t *buf){
    return (_bmp->read(buf, _width*_bitsPerPixel/8))>0;
}

uint32_t BitmapReader::getFileSize() const {
    return _fileSize;
}

uint32_t BitmapReader::getImageDataOffset() const {
    return _imageDataOffset;
}

uint32_t BitmapReader::getWidth() const {
    return _width;
}

uint32_t BitmapReader::getHeight() const {
    return _height;
}

uint16_t BitmapReader::getBitsPerPixel() const {
    return _bitsPerPixel;
}

File* BitmapReader::getBmp(){
    return _bmp;
}