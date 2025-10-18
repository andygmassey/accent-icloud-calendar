#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <GFX.h>
#include "ErrorImage.h"
#include "WifiImage.h"

extern const String kVariant7Color;
extern const String kVariant3Color;

#ifdef DISPLAY_GDEW075Z09
#include <GxEPD2_3C.h>
#define GFX_CLASS GxEPD2_3C
#define DISPLAY_TYPE GxEPD2_750c
#define PAGE_HEIGHT GxEPD2_750c::HEIGHT
#define VARIANT "bwr"
#endif

#ifdef DISPLAY_GDEW075Z08
#include <GxEPD2_3C.h>
#define GFX_CLASS GxEPD2_3C
#define DISPLAY_TYPE GxEPD2_750c_Z08
#define PAGE_HEIGHT GxEPD2_750c_Z08::HEIGHT
#define VARIANT "bwr"
#endif

#ifdef DISPLAY_GDEH075Z90
#include <GxEPD2_3C.h>
#define GFX_CLASS GxEPD2_3C
#define DISPLAY_TYPE GxEPD2_750c_Z90
#define PAGE_HEIGHT (GxEPD2_750c_Z90::HEIGHT / 2)
#define VARIANT "bwr"
#endif

#ifdef DISPLAY_GDEY1248Z51
#include <GxEPD2_3C.h>
#define GFX_CLASS GxEPD2_3C
#define DISPLAY_TYPE GxEPD2_1248c
#define PAGE_HEIGHT (GxEPD2_1248c::HEIGHT / 8)
#define VARIANT "bwr"
#endif

#ifdef DISPLAY_GDEY073D46
#include <GxEPD2_7C.h>
#define GFX_CLASS GxEPD2_7C
#define DISPLAY_TYPE GxEPD2_730c_GDEY073D46
#define PAGE_HEIGHT (GxEPD2_730c_GDEY073D46::HEIGHT / 2)
#define VARIANT "7color"
#endif

class Display {
 public:
  Display(uint32_t serial_speed) : serial_speed_(serial_speed), gx_epd_(nullptr) {}
  void Initialize();
  void Load(const uint8_t* image_data, uint32_t size, uint32_t offset);
  void Finalize();
  void ShowError();
  void ShowWifiSetup();
  int16_t Width();
  int16_t Height();
  String Variant();

 private:
  uint16_t ConvertPixel(uint8_t input, uint8_t mask, uint8_t shift);
  void ShowStatic(const uint8_t* black_data, const uint8_t* red_data,
                  uint16_t width, uint16_t height, uint16_t background);
  GFX_CLASS<DISPLAY_TYPE, PAGE_HEIGHT>* gx_epd_;
  uint32_t serial_speed_;
};

#endif  // DISPLAY_H
