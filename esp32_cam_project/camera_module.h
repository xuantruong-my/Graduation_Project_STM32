#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <Arduino.h>

// Khởi tạo camera (gọi trong setup())
bool initCamera();

// Chụp ảnh mới và lưu vào buffer nội bộ (gọi khi nút được bấm)
void capturePhoto();

// Truy cập ảnh mới nhất đang lưu trong RAM
const uint8_t* getLastPhoto();
size_t getLastPhotoLen();

#endif // CAMERA_MODULE_H