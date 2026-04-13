#include "SensorManager.h"
#include "config.h"

#define ADDR 0x44

void SensorManager::begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setTimeOut(200);
}

bool SensorManager::read(float &t, float &h) {
    if (_state == SENSOR_SIM) {
        simulate(t, h);
        return true;
    }

    if (readRaw(t, h)) {
        _failCount = 0;
        _state = SENSOR_OK;
        return true;
    }

    _failCount++;

    if (_failCount > 3) {
        resetSensor();
    }

    if (_failCount > 6) {
        _state = SENSOR_SIM;
    }

    return false;
}

bool SensorManager::readRaw(float &t, float &h) {
    Wire.beginTransmission(ADDR);
    Wire.write(0xFD);
    if (Wire.endTransmission() != 0) return false;

    delay(10);

    Wire.requestFrom(ADDR, 6);
    if (Wire.available() != 6) return false;

    uint8_t d[6];
    for (int i = 0; i < 6; i++) d[i] = Wire.read();

    if (crc8(d, 2) != d[2]) return false;
    if (crc8(d + 3, 2) != d[5]) return false;

    uint16_t rt = (d[0] << 8) | d[1];
    uint16_t rh = (d[3] << 8) | d[4];

    t = -45 + 175 * (rt / 65535.0);
    h = -6 + 125 * (rh / 65535.0);

    return true;
}

void SensorManager::resetSensor() {
    // 1. Hard reset the I2C peripheral to un-stick the bus
    Wire.end();
    delay(10);
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setTimeOut(200);
    delay(10);

    // 2. Soft reset the SHT40 sensor
    Wire.beginTransmission(ADDR);
    Wire.write(0x94);
    Wire.endTransmission();
    delay(20); // Give SHT40 extra time to boot
}

void SensorManager::simulate(float &t, float &h) {
    t = 21 + sin(millis()/10000.0)*2;
    h = 58 + cos(millis()/15000.0)*3;
}

uint8_t SensorManager::crc8(const uint8_t *data, int len) {
    uint8_t crc = 0xFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
    }
    return crc;
}

SensorState SensorManager::state() {
    return _state;
}