#ifndef _SPRITE_H_
#define _SPRITE_H_

/********************************************************************************************/
/* Sprite structure                                                                         */
/********************************************************************************************/
class Sprite {
  public:
    virtual uint16_t getWidth() = 0;
    virtual uint16_t getHeight() = 0;
    virtual uint16_t isPixelOn(uint16_t x, uint16_t y) = 0;
};

#endif
