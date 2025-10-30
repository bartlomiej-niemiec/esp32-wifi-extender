#ifndef SK68XXMINIHS_RGBLEDIF_RGBLEDIF
#define SK68XXMINIHS_RGBLEDIF_RGBLEDIF

#include "rgbled_if/rgbled_if.hpp"
#include "esp_timer.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace RgbLed
{

class Sk68xxminiHsImpl:
    public RgbLedIf
{
    public:
        Sk68xxminiHsImpl();

        void Solid(const Color);

        void Blink(const Color, const uint32_t frequency_hz);

    private:

        static const gpio_num_t GPIO_PIN_NUM = GPIO_NUM_38;
        static const rmt_symbol_word_t m_reset_sym;
        rmt_channel_handle_t m_txChannel;
        rmt_encoder_handle_t m_encoderHandle;
        rmt_encoder_handle_t m_resetEncoderHandle;
        bool m_BlinkState;
        Color m_BlinkyColor;
        esp_timer_handle_t m_BlinkyTimer;
        SemaphoreHandle_t m_Semaphore;
        static void BlinkTimerCallback(void * pArg);
        void SetColor(const Color & color);
        void StopBlinkyTimer();
        static constexpr Color offColor = ColorCreator::CreateColor(ColorType::BLACK);
};

}


#endif