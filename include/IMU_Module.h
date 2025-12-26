/**
 * @file IMU_Module.h
 * @brief MPU9255 I2C İvmeölçer okuma modülü (Gyro hariç)
 */
#ifndef IMU_MODULE_H
#define IMU_MODULE_H

#include <Wire.h>

class IMUModule {
private:
    const uint8_t ADDR = 0x68; // MPU9255 I2C Adresi

public:
    bool begin() {
        // Cihazı uyandır (0x6B adresine 0 gönderilir)
        Wire.beginTransmission(ADDR);
        Wire.write(0x6B); 
        Wire.write(0x00); 
        if (Wire.endTransmission() != 0) return false;

        // Hassasiyet ayarı: +/- 4g (Hyperloop kalkış/frenleme için uygundur)
        Wire.beginTransmission(ADDR);
        Wire.write(0x1C); 
        Wire.write(0x08); 
        Wire.endTransmission();
        
        return true;
    }

    // X, Y ve Z ivme değerlerini (g biriminde) referansla doldurur
    void readAccel(float &ax, float &ay, float &az) {
        Wire.beginTransmission(ADDR);
        Wire.write(0x3B); // İvme verilerinin başlangıç adresi
        Wire.endTransmission(false);
        
        Wire.requestFrom(ADDR, (uint8_t)6); // Sadece ivme (6 byte) oku

        // Byte birleştirme (High << 8 | Low)
        int16_t rawAX = (Wire.read() << 8) | Wire.read();
        int16_t rawAY = (Wire.read() << 8) | Wire.read();
        int16_t rawAZ = (Wire.read() << 8) | Wire.read();

        // Skala dönüşümü (4g / 32768)
        ax = (float)rawAX * 4.0 / 32768.0;
        ay = (float)rawAY * 4.0 / 32768.0;
        az = (float)rawAZ * 4.0 / 32768.0;
    }
};

#endif