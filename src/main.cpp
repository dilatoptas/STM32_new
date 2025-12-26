#include <Arduino.h>
#include <Wire.h>
#include "NTC_ADS1115.h"
#include "IMU_Module.h"
#include "Encoder_Module.h"

// STM32'de hangi Timer'ı kullanacağınızı seçin.
// Genellikle: 
// TIM2 -> PA0 (A), PA1 (B) pinlerini kullanır.
// TIM3 -> PA6 (A), PA7 (B) pinlerini kullanır.
EncoderModule wheelEnc(TIM3);

// Modül Nesneleri
NTCSensorADS podNTC;
IMUModule podIMU;

void setup() {
    Serial.begin(115200); // Terminal hızı (Raspberry Pi ile uyumlu)
    delay(2000); 

    Wire.begin(); // I2C hattını (PB6, PB7) aktif et

    // --- Başlatma ve Hata Kontrolü ---
    if (!podNTC.begin()) Serial.println("[HATA] NTC/ADS1115 Baslatilamadi!");
    if (!podIMU.begin()) Serial.println("[HATA] MPU9255 Baslatilamadi!");

    wheelEnc.begin();

    Serial.println(">>> SISTEM HAZIR. VERI AKISI BASLIYOR...");
}

void loop() {
    // Verileri modüllerden çek
    float temp = podNTC.readTemperature();
    float ax, ay, az;
    podIMU.readAccel(ax, ay, az);
    int32_t pos = wheelEnc.getPosition();

    // Terminale yazdır (Raspberry Pi ileride bu satırları okuyacak)
    Serial.print("Temp:"); Serial.print(temp, 1);
    Serial.print("C | Pos:"); Serial.print(pos);
    Serial.print(" | AccY:"); Serial.println(ay, 2);

    delay(100); // 10 Hz döngü hızı
}