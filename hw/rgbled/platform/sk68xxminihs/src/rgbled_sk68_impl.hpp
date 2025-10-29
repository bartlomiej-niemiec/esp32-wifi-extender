#ifndef SK68XXMINIHS_RGBLEDIF_RGBLEDIF
#define SK68XXMINIHS_RGBLEDIF_RGBLEDIF

#include "rgbled_if/rgbled_if.hpp"
#include "driver/rmt_tx.h"

namespace RgbLed
{

class Sk68xxminiHsImpl:
    public RgbLedIf
{
    public:
        Sk68xxminiHsImpl();

        void SetColor(const Color) override;

    private:

        static const gpio_num_t GPIO_PIN_NUM = GPIO_NUM_38;
        static const rmt_symbol_word_t m_reset_sym;
        rmt_channel_handle_t m_txChannel;
        rmt_encoder_handle_t m_encoderHandle;
        rmt_encoder_handle_t m_resetEncoderHandle;
};

}


#endif