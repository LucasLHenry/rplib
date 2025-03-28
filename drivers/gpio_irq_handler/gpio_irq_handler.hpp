#ifndef DRIVERS_GPIO_IRQ_HANDLER_H_
#define DRIVERS_GPIO_IRQ_HANDLER_H_

// code adapted from tobiglaser on github
// https://github.com/tobiglaser/RP2040-Encoder/tree/main

#include "hardware/gpio.h"

#define HIGHEST_INTERRUPT_PIN 28  // not sure if this is right

class GPIO_IRQ_Handler {
    public:
        GPIO_IRQ_Handler() {}
        ~GPIO_IRQ_Handler() {}

        enum IRQ_Event {
            none = 0x0,
            low = GPIO_IRQ_LEVEL_LOW,
            high = GPIO_IRQ_LEVEL_HIGH,
            fall = GPIO_IRQ_EDGE_FALL,
            rise = GPIO_IRQ_EDGE_RISE,
            change = GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL
        };
    
        void init();
        void register_interrupt(uint gpio_pin, IRQ_Event events, gpio_irq_callback_t callback);
        void unregister_interrupt(uint gpio_pin);

    private:
        uint32_t num_registered_interrupts_;
        static gpio_irq_callback_t isr_list_[NUM_CORES][HIGHEST_INTERRUPT_PIN + 1];

        static void empty_callback(uint gpio_pin, uint32_t events);
        static void general_callback(uint gpio_pin, uint32_t events);
};

#endif  // DRIVERS_GPIO_IRQ_HANDLER_H_