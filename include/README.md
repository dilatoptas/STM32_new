## 📌 Donanım Bağlantıları & Pin Tanımları

Bu proje STM32 (özellikle **STM32F103C8 – Blue Pill**) kartı üzerinde aşağıdaki sensörlerle çalışacak şekilde yapılandırılmıştır:

* Encoder → Timer (TIM3) donanımsal encoder modu
* IMU → MPU9255 (I2C)
* NTC → ADS1115 (I2C) üzerinden sıcaklık ölçümü

---

### 🧭 Quadrature Encoder Bağlantısı

**Kullanılan Timer:** `TIM3`
**Encoder Modu:** HAL TIM Encoder Interface

| Encoder Sinyali | STM32 Pin | Timer Kanalı      |
| --------------- | --------- | ----------------- |
| A (CHA)         | **PA6**   | TIM3_CH1          |
| B (CHB)         | **PA7**   | TIM3_CH2          |
| GND             | GND       | –                 |
| VCC             | 3.3V / 5V | – (encodera göre) |

> Not: Kod içinde GPIO init ve TIM3 clock enable yapılmıştır.
> Encoder modülü `EncoderModule wheelEnc(TIM3);` ile başlatılır.

---

### 🧠 IMU – MPU9255 (I2C) Bağlantısı

**I2C Hattı:** `Wire` (STM32 core varsayılanı)

| MPU9255 Pin | STM32 Pin            |
| ----------- | -------------------- |
| VCC         | 3.3V                 |
| GND         | GND                  |
| SDA         | **PB7**              |
| SCL         | **PB6**              |
| AD0         | GND → adres **0x68** |

> IMU I2C adresi: `0x68`
> Kodda `Wire.begin();` ile başlatılır.

---

### 🌡️ NTC + ADS1115 (I2C) Bağlantısı

ADS1115, IMU ile **aynı I2C hattını** paylaşır.

| ADS1115 Pin | STM32 Pin            |
| ----------- | -------------------- |
| VDD         | 3.3V                 |
| GND         | GND                  |
| SDA         | **PB7**              |
| SCL         | **PB6**              |
| ADDR        | GND → adres **0x48** |

**NTC bağlantısı:**

* NTC + referans direnç → ADS1115 A0 & A1 diferansiyel giriş
* Okuma: `readADC_Differential_0_1()`

> ADS1115 varsayılan adres: `0x48`

---

### 🖥️ Seri Haberleşme (PC / Raspberry Pi)

| Sinyal | Açıklama                            |
| ------ | ----------------------------------- |
| USB    | STM32 USB veya UART-USB dönüştürücü |
| Baud   | **115200**                          |

Kod:

```cpp
Serial.begin(115200);
```

---

### ⚙️ Özet Pin Tablosu

| Fonksiyon | STM32 Pin            |
| --------- | -------------------- |
| Encoder A | PA6                  |
| Encoder B | PA7                  |
| I2C SDA   | PB7                  |
| I2C SCL   | PB6                  |
| UART/USB  | Kartın USB/UART pini |

---

### ⚠️ Notlar

* Bu pinler **STM32F103 (Blue Pill)** için geçerlidir.
* Farklı kart kullanılıyorsa (Nucleo, farklı F1/F4 vs.),
  Timer ve I2C pinleri değişebilir.
* Encoder için `TIM3` kullanımı kod içinde sabittir.

