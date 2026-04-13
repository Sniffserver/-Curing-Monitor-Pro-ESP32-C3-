#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Wire.h>

enum SensorState {
    SENSOR_OK,    // Normal I2C reads succeeding
    SENSOR_FAIL,  // Consecutive read failures (trying to recover)
    SENSOR_SIM    // Too many failures; fallen back to simulated data
};

class SensorManager {
public:
    void begin();
    bool read(float &t, float &h);
    SensorState state();

private:
    SensorState _state = SENSOR_OK;
    int _failCount = 0;

    bool readRaw(float &t, float &h);
    void resetSensor();
    void simulate(float &t, float &h);
    uint8_t crc8(const uint8_t *data, int len);
};

#endif