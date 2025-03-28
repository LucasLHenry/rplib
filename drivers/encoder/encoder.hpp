#ifndef DRIVERS_ENCODER_H_
#define DRIVERS_ENCODER_H_

#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "encoder.pio.h"
#include "gpio_irq_handler.hpp"

// based on Adam Green's Quadrature Decoder driver for the RP2040
// https://github.com/adamgreen/QuadratureDecoder

class Encoder {
    public:
        Encoder() {}
        ~Encoder() {}

        void init(uint gpio_pin_base, bool pullup, bool invert);
        void read();
        int32_t value_i32, change;
        uint8_t dma_chan, pio_sm;

    private:
        int32_t prev_count_, count_;
        bool invert_;
        
        void init_gpio(uint gpio_pin_base, bool pullup);
        void init_pio(uint gpio_pin_base);
        void init_dma();
};

#endif  // DRIVERS_ENCODER_H_