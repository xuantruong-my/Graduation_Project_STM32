#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <Arduino.h>

bool initCamera();

void capturePhoto();

const uint8_t* getLastPhoto();
size_t getLastPhotoLen();

#endif // CAMERA_MODULE_H