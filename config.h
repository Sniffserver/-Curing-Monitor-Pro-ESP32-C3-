#ifndef CONFIG_H
#define CONFIG_H

// I2C
#define I2C_SDA 5
#define I2C_SCL 6

// Timing
#define SENSOR_INTERVAL 5000

// Thresholds
#define TEMP_MIN 18.0
#define TEMP_MAX 24.0
#define HUM_MIN 55.0
#define HUM_MAX 62.0

// OLED coordinates (mapped correctly by 72x40 driver)
#define OLED_X 0
#define OLED_Y 0

#endif