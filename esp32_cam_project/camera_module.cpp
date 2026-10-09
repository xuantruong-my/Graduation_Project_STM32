#include "camera_module.h"
#include "esp_camera.h"
#include "camera_pin.h"


static uint8_t *lastPhoto = nullptr;
static size_t lastPhotoLen = 0;

//-----------------------------------------------------------------------------------------//
bool initCamera() 
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())       //check whether there is PSRAM
  {
    config.frame_size = FRAMESIZE_VGA;    // 640 x 480 pixels
    config.jpeg_quality = 12;             //less compression
    config.fb_count = 2;                  //2 buffers
  } 
  
  else 
  {
    config.frame_size = FRAMESIZE_QVGA;   //320 x 240 pixels 
    config.jpeg_quality = 15;             //more compression
    config.fb_count = 1;                  //1 buffer
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera khoi tao that bai: 0x%x\n", err);
    return false;
  }

  sensor_t *s = esp_camera_sensor_get();      //get a pointer to the running sensor's control struct
  s->set_vflip(s, 1);                         //flip the image
  return true;
}

//-----------------------------------------------------------------------------------------//
void capturePhoto() 
{
  camera_fb_t * fb = esp_camera_fb_get();     //trigger a capture
  if (!fb) 
  {
    Serial.println("Chup anh that bai");
    return;
  }

  //free the old buffer for the new img
  if (lastPhoto != nullptr) {
    free(lastPhoto);
    lastPhoto = nullptr;
  }

  //copy new img into our own buffer
  lastPhoto = (uint8_t *) malloc(fb->len);
  if (lastPhoto != nullptr) 
  {
    memcpy(lastPhoto, fb->buf, fb->len);
    lastPhotoLen = fb->len;
    Serial.printf("Da chup anh, kich thuoc: %d bytes\n", lastPhotoLen);
  }

  esp_camera_fb_return(fb);
}

//-----------------------------------------------------------------------------------------//
const uint8_t* getLastPhoto() 
{
  return lastPhoto;
}

//-----------------------------------------------------------------------------------------//
size_t getLastPhotoLen()
{
  return lastPhotoLen;
}