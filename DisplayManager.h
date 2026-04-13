#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#include "config.h"

enum Status {
    OPTIMAL,
    HOT,
    COLD,
    DRY,
    HUMID,
    FAULT
};

class DisplayManager {
public:
    void begin();
    void draw(float t, float h, Status s, bool trendUpT, bool trendUpH);

private:
    U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2 =
        U8G2_SSD1306_72X40_ER_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

    const char* statusText(Status s);
};

#endif