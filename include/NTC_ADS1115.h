/**
 * @file NTC_ADS1115.h
 * @brief ADS1115 16-bit ADC üzerinden NTC okuma modülü
 */
#ifndef NTC_ADS1115_H
#define NTC_ADS1115_H

#include <Adafruit_ADS1X15.h> // ADS1115 kütüphanesi (lib_deps'e eklenmeli)
#include <math.h>

class NTCSensorADS {
private:
    Adafruit_ADS1115 _ads;
    // Devre Sabitleri (Senin sistemine göre güncellendi)
    const float R_REF = 9470.0;     // 10k yerine ölçülen gerçek direnç
    const float B_VALUE = 3950.0;   // NTC Beta katsayısı
    const float R_NOMINAL = 4780.0; // 25C (veya 20C) referans direnci
    const float T_NOMINAL = 20.0;   // Referans sıcaklık
    const float V_SOURCE = 3.3;     // STM32 besleme voltajı

public:
    // Sensörü başlatan fonksiyon
    bool begin() {
        if (!_ads.begin()) {
            return false; // ADS1115 bulunamazsa hata dön
        }
        // +/- 2.048V aralığı: 3.3V bölücü devresi için en yüksek çözünürlüğü sağlar
        _ads.setGain(GAIN_TWO); 
        return true;
    }

    // Sıcaklığı Celsius cinsinden döndüren fonksiyon
    float readTemperature() {
        // A0 ve A1 arası farkı oku (Diferansiyel - Gürültü engelleme)
        int16_t results = _ads.readADC_Differential_0_1();
        
        // Ham veriyi Voltaj'a çevir (Gain TWO için çarpan: 0.0625mV)
        float vDiff = results * 0.0625 / 1000.0;

        // Güvenlik: Voltaj limit dışıysa hata döndür
        if (vDiff <= 0 || vDiff >= V_SOURCE) return -999.0;

        // 1. Adım: Voltajdan NTC direncini hesapla
        float rNTC = (vDiff * R_REF) / (V_SOURCE - vDiff);

        // 2. Adım: Steinhart-Hart / Beta Denklemi ile sıcaklığa çevir
        float temp;
        temp = rNTC / R_NOMINAL;          // R/Ro
        temp = log(temp);                 // ln(R/Ro)
        temp /= B_VALUE;                  // 1/B * ln(R/Ro)
        temp += 1.0 / (T_NOMINAL + 273.15); // + (1/To)
        temp = 1.0 / temp;                // T = 1 / (...)
        temp -= 273.15;                   // Kelvin -> Celsius

        return temp;
    }
};

#endif