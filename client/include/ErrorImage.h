#ifndef ERROR_IMAGE_H
#define ERROR_IMAGE_H

// Minimal embedded image data (placeholder) for error and wifi
const uint16_t kErrorBackground = 0xF800;  // red
const uint16_t kErrorWidth = 40;
const uint16_t kErrorHeight = 40;
const PROGMEM uint8_t kErrorImageBlack[] = {0};
const PROGMEM uint8_t kErrorImageRed[] = {0};

#endif  // ERROR_IMAGE_H