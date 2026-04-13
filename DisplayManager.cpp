#include "DisplayManager.h"

void DisplayManager::begin() {
    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.setBusClock(400000);
}

void DisplayManager::draw(float t, float h, Status s, bool trendUpT, bool trendUpH) {
    u8g2.clearBuffer();

    int x = OLED_X; // Now perfectly 0
    int y = OLED_Y; // Now perfectly 0

    // Outer frame (72x40 physical bounds)
    u8g2.drawFrame(x, y, 72, 40);

    // --- ROW 1: TEMP & HUMIDITY ---
    u8g2.setFont(u8g2_font_6x12_tf); // Ultra clean, standard 6x12
    
    // Auto-spacing Temp
    char tb[10];
    sprintf(tb, "%.1f", t);
    u8g2.drawStr(x + 3, y + 14, tb);
    int tw = u8g2.getStrWidth(tb);
    
    int tx = x + 3 + tw + 4; // Arrow 4px to the right of text
    if(trendUpT) u8g2.drawTriangle(tx, y+6, tx-3, y+13, tx+3, y+13);
    else         u8g2.drawTriangle(tx, y+13, tx-3, y+6, tx+3, y+6);

    // Auto-aligned Humidity
    char hb[10];
    sprintf(hb, "%.0f%%", h);
    int hw = u8g2.getStrWidth(hb);
    
    int hx = x + 66; // Arrow center on far right
    if(trendUpH) u8g2.drawTriangle(hx, y+6, hx-3, y+13, hx+3, y+13);
    else         u8g2.drawTriangle(hx, y+13, hx-3, y+6, hx+3, y+6);
    
    u8g2.drawStr(hx - 4 - hw, y + 14, hb);

    // --- DIVIDER ---
    // Dropped down slightly to balance vertical space
    u8g2.drawHLine(x, y + 19, 72);

    // --- ROW 2: STATUS (CENTERED) ---
    u8g2.setFont(u8g2_font_profont11_tf);
    const char* txt = statusText(s);
    int sw = u8g2.getStrWidth(txt);
    
    // Total width = Icon (10px) + Spacing (4px) + Text (sw) = 14 + sw
    int totalHWidth = 14 + sw;
    int startX = x + (72 - totalHWidth) / 2;
    
    // Status Icon
    int cx = startX + 5;
    int cy = y + 29;
    switch(s){
        case OPTIMAL: u8g2.drawDisc(cx, cy, 3); break;
        case HOT: 
        case HUMID:   u8g2.drawTriangle(cx, cy-4, cx-5, cy+4, cx+5, cy+4); break;
        case COLD: 
        case DRY:     u8g2.drawTriangle(cx, cy+4, cx-5, cy-4, cx+5, cy-4); break;
        case FAULT:   u8g2.drawBox(cx-4, cy-4, 9, 9); break;
    }

    // Status Text
    u8g2.drawStr(startX + 14, y + 34, txt);

    u8g2.sendBuffer();
}

const char* DisplayManager::statusText(Status s){
    switch(s){
        case OPTIMAL: return "OPTIMAL";
        case HOT: return "HOT";
        case COLD: return "COLD";
        case DRY: return "DRY";
        case HUMID: return "HUMID";
        case FAULT: return "FAULT";
    }
    return "";
}