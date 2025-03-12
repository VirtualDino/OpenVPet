// These are the 32x32 Monochrome Menu Icons stored in 32 32bit unsigned integer to decrease memory Usage.
// Each image needs 128 Byte
#pragma once
#include <Arduino.h>
#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(__PIC32MX__)
#define PROGMEM
#elif defined(__arm__)
#define PROGMEM
#endif


#define SPRITES_MENU_RESOLUTION 32
#define N_SPRITES_MENU 10

const uint32_t MENU_ITEMS[N_SPRITES_MENU][SPRITES_MENU_RESOLUTION] PROGMEM = {

  { 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000001111100000000000000,
    0b00000000001111111111100000000000,
    0b00000000111110000011111000000000,
    0b00000001110000010000011100000000,
    0b00000011000000010000000110000000,
    0b00000110000100010001000011000000,
    0b00000110000100010001000011000000,
    0b00001100000010000010000001100000,
    0b00001100100010010010001001100000,
    0b00001100010000010000010001100000,
    0b00000110001000111000100011000000,
    0b00000110000000111000000011000000,
    0b00000011000001111100000110000000,
    0b00000011000000000000000110000000,
    0b00000001100011111110001100000000,
    0b00000001111111111111111100000000,
    0b00000000111100000001111000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },

  { 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000001100000000000000000000000,
    0b00000010010001111100000000000000,
    0b00000110010011111110000000000000,
    0b00001000010011101111000000000000,
    0b00001000001001111111000000000000,
    0b00000111001011111111100000000000,
    0b00000000110011111111111000000000,
    0b00000000000111111111111100000000,
    0b00000000111111011111111110000000,
    0b00000001111111111101110110000000,
    0b00000001101111111111111110000000,
    0b00000001111111111111111110000000,
    0b00000001111111111111111100000000,
    0b00000000111111011111101100000000,
    0b00000000011111111111111100000000,
    0b00000000000011111111111011100000,
    0b00000000000000111101110000010000,
    0b00000000000000111111101000010000,
    0b00000000000000011110001001100000,
    0b00000000000000000000001001000000,
    0b00000000000000000000000110000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },

  { 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000011000000000000000,
    0b00000000000000111100000000000000,
    0b00000000000000111100000000000000,
    0b00000000000000011000000000000000,
    0b00000000010000000000001000000000,
    0b00000000110000111100001100000000,
    0b00000001110110111101101110000000,
    0b00000011110110111101101111000000,
    0b00000011110110111101101111000000,
    0b00000001110110111101101110000000,
    0b00000000110000111100001100000000,
    0b00000000010001111110001000000000,
    0b00000000000001111110000000000000,
    0b00000000000011100111000000000000,
    0b00000000000011100111000000000000,
    0b00000000000111000011100000000000,
    0b00000000000111000011100000000000,
    0b00000000000111000011100000000000,
    0b00000000000111000011100000000000,
    0b00000000000111000011100000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },
  { 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000011001000000000010011000000,
    0b00000011101100000000110111000000,
    0b00000001111110000001111110000000,
    0b00001100111111000011111100110000,
    0b00001110011011100111011001110000,
    0b00000111000001100110000011110000,
    0b00000010011100000000111001000000,
    0b00000000100011000011000100000000,
    0b00000001000010100101000010000000,
    0b00001111010010100101001011110000,
    0b00001001010011100111001010010000,
    0b00001000001110100101110000010000,
    0b00001000001000100100010000010000,
    0b00001000000111100111100000010000,
    0b00001000001000100100010000010000,
    0b00001111001000100100010011110000,
    0b00000000100111100111100100000000,
    0b00000010011110000001111001000000,
    0b00000111000001100110000011100000,
    0b00001110011011100111011001110000,
    0b00001100111111000011111100110000,
    0b00000001111110000001111110000000,
    0b00000011101100000000110111000000,
    0b00000011001000000000010011000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },

  { 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000010000000000000000,
    0b00000000000000001000000000000000,
    0b00000000000000011100000000000000,
    0b00000000000001111110000000000000,
    0b00000000000011111111000000000000,
    0b00000000000011111111000000000000,
    0b00000000000001111110010000000000,
    0b00000000001100000000111000000000,
    0b00000000011111000011111000000000,
    0b00000000011111111111111000000000,
    0b00000000001111111111110000000000,
    0b00000001100111111111100110000000,
    0b00000011110001111110001111000000,
    0b00000011111100000000111111000000,
    0b00000011111111111111111110000000,
    0b00000001111111111111111110000000,
    0b00000000111111111111111100000000,
    0b00000000011111111111111000000000,
    0b00000000000111111111100000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },

  { 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000001000000000000000,
    0b00000000000000001000000000000000,
    0b00000010000000000000000001000000,
    0b00000001000000000000000010000000,
    0b00000000000011111111000000000000,
    0b00000000001111111111110000000000,
    0b00000000011111111111111000000000,
    0b00000000011111111111111000000000,
    0b00000000111110011001111100000000,
    0b00000000111101100110111100000000,
    0b00001100111101011010111100110000,
    0b00000000111101011010111100000000,
    0b00000000011101100110111000000000,
    0b00000000011110111101111000000000,
    0b00000000001111011011110000000000,
    0b00000000000011111111000000000000,
    0b00000001000000000000000010000000,
    0b00000010000001111110000001000000,
    0b00000000000000000000000000000000,
    0b00000000000001111110000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000011000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },
  { 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000001111111111111111110000000,
    0b00000010010101000010101001000000,
    0b00000111010101000010101011100000,
    0b00000100100000000000000100100000,
    0b00000110100001111110000101100000,
    0b00000100100011111111000100100000,
    0b00000101111110000001111110100000,
    0b00000101111100111100111110100000,
    0b00000101000000111100000010100000,
    0b00000101000011111111000010100000,
    0b00000101000011111111000010100000,
    0b00000100100011111111000100100000,
    0b00000110100011111111000101100000,
    0b00000100100000111100000100100000,
    0b00000111100000111100000111100000,
    0b00000111011110000001111011100000,
    0b00000010100001000010000101000000,
    0b00000011111111000011111111000000,
    0b00000011111111111111111111000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },

  { 0b00000000000000000000000000000000,
    0b00000000000111111111100000000000,
    0b00000000000011111110000000000000,
    0b00000000001110000001110000000000,
    0b00000000011100000000111000000000,
    0b00000000011000000000011000000000,
    0b00000000110000111100001100000000,
    0b00000000110001111110001100000000,
    0b00001111110011111111001111110000,
    0b00001111110011111111001111110000,
    0b00001111110011111111001111110000,
    0b00000000110001111110001100000000,
    0b00000000110000111100001100000000,
    0b00000000011000000000011000000000,
    0b00000000011100000000111000000000,
    0b00000000001100000000110000000000,
    0b00000000001110000001110000000000,
    0b00000000000111111111100000000000,
    0b00000000000011111111000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000001111110000000000000,
    0b00000000000001111110000000000000,
    0b00000000000000111100000000000000,
    0b00000000000000111100000000000000,
    0b00000000000000011000000000000000,
    0b00000000000000011000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
  },

  {
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000001111100000000000000,
    0b00000000000011111110000000000000,
    0b00000000000111000111000000000000,
    0b00000000001110000011100000000000,
    0b00000000001100000001100000000000,
    0b00000000001100000001100000000000,
    0b00000000001100000001100000000000,
    0b00000000001110000011100000000000,
    0b00000000000111000111000000000000,
    0b00000000000011111110000000000000,
    0b00000000000001111100000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000111000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
},











  //duplicates to fill 10

  // { 0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000001000000000000000,
  //   0b00000000000000001000000000000000,
  //   0b00000010000000000000000001000000,
  //   0b00000001000000000000000010000000,
  //   0b00000000000011111111000000000000,
  //   0b00000000001111111111110000000000,
  //   0b00000000011111111111111000000000,
  //   0b00000000011111111111111000000000,
  //   0b00000000111110011001111100000000,
  //   0b00000000111101100110111100000000,
  //   0b00001100111101011010111100110000,
  //   0b00000000111101011010111100000000,
  //   0b00000000011101100110111000000000,
  //   0b00000000011110111101111000000000,
  //   0b00000000001111011011110000000000,
  //   0b00000000000011111111000000000000,
  //   0b00000001000000000000000010000000,
  //   0b00000010000001111110000001000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000001111110000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000011000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  // },
  // { 0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000001111111111111111110000000,
  //   0b00000010010101000010101001000000,
  //   0b00000111010101000010101011100000,
  //   0b00000100100000000000000100100000,
  //   0b00000110100001111110000101100000,
  //   0b00000100100011111111000100100000,
  //   0b00000101111110000001111110100000,
  //   0b00000101111100111100111110100000,
  //   0b00000101000000111100000010100000,
  //   0b00000101000011111111000010100000,
  //   0b00000101000011111111000010100000,
  //   0b00000100100011111111000100100000,
  //   0b00000110100011111111000101100000,
  //   0b00000100100000111100000100100000,
  //   0b00000111100000111100000111100000,
  //   0b00000111011110000001111011100000,
  //   0b00000010100001000010000101000000,
  //   0b00000011111111000011111111000000,
  //   0b00000011111111111111111111000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  //   0b00000000000000000000000000000000,
  // },

};

