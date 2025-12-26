/**
 * @file Encoder_Module.h
 * @brief STM32 HAL tabanlı donanım Encoder okuma modülü (tek dosya)
 */

#ifndef ENCODER_MODULE_H
#define ENCODER_MODULE_H

#include <Arduino.h>
#include "stm32f1xx_hal.h"   // F1 serisi için (kartına göre değişebilir)

class EncoderModule {
private:
    TIM_HandleTypeDef _htim;

public:
    /**
     * @param instance Kullanılacak Timer (örn: TIM3)
     */
    EncoderModule(TIM_TypeDef *instance) {
        _htim.Instance = instance;
    }

    void begin() {
        // ⚠️ TIM3 kullanıyorsan saatini aç
        // --- TIMER CLOCK ---
        if (_htim.Instance == TIM3) {
            __HAL_RCC_TIM3_CLK_ENABLE();
            __HAL_RCC_GPIOA_CLK_ENABLE();   // PA6, PA7 için
            __HAL_RCC_AFIO_CLK_ENABLE();
        }
        // Gerekirse başka timerlar buraya eklenir:
        // else if (_htim.Instance == TIM2) __HAL_RCC_TIM2_CLK_ENABLE();

         // --- GPIO INIT (Encoder pinleri) ---
        if (_htim.Instance == TIM3) {
            GPIO_InitTypeDef GPIO_InitStruct = {0};
            GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        }

        _htim.Init.Prescaler = 0;
        _htim.Init.CounterMode = TIM_COUNTERMODE_UP;
        _htim.Init.Period = 0xFFFF;
        _htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        _htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

        TIM_Encoder_InitTypeDef sConfig = {0};
        sConfig.EncoderMode = TIM_ENCODERMODE_TI12;

        sConfig.IC1Polarity  = TIM_INPUTCHANNELPOLARITY_RISING;
        sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
        sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
        sConfig.IC1Filter    = 4;   // küçük dijital filtre

        sConfig.IC2Polarity  = TIM_INPUTCHANNELPOLARITY_RISING;
        sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
        sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
        sConfig.IC2Filter    = 4;

        HAL_TIM_Encoder_Init(&_htim, &sConfig);
        HAL_TIM_Encoder_Start(&_htim, TIM_CHANNEL_ALL);

        __HAL_TIM_SET_COUNTER(&_htim, 0);
    }

    int32_t getPosition() {
        return (int32_t)__HAL_TIM_GET_COUNTER(&_htim);
    }

    void reset() {
        __HAL_TIM_SET_COUNTER(&_htim, 0);
    }
};

#endif
