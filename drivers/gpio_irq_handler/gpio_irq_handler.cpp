#include "gpio_irq_handler.hpp"

void GPIO_IRQ_Handler::init() {
    for (uint core = 0; core < 2; core++) {
        for (uint pin = 0; pin < HIGHEST_INTERRUPT_PIN; pin++) {
            isr_list_[core][pin] = empty_callback;
        }
    }
}

void GPIO_IRQ_Handler::register_interrupt(uint gpio_pin, GPIO_IRQ_Handler::IRQ_Event events, gpio_irq_callback_t callback) {
    if (gpio_pin > HIGHEST_INTERRUPT_PIN) return;
    if (num_registered_interrupts_ == 0) {
        gpio_set_irq_enabled_with_callback(gpio_pin, events, true, &general_callback);
    } else {
        gpio_set_irq_enabled(gpio_pin, events, true);
    }

    isr_list_[get_core_num()][gpio_pin] = callback;
    num_registered_interrupts_++;
}

void GPIO_IRQ_Handler::unregister_interrupt(uint gpio_pin) {
    if (isr_list_[get_core_num()][gpio_pin] != empty_callback) {
        gpio_set_irq_enabled(gpio_pin, IRQ_Event::none, false);
        isr_list_[get_core_num()][gpio_pin] = empty_callback;
        num_registered_interrupts_--;
    }
}

void GPIO_IRQ_Handler::empty_callback(uint gpio_pin, uint32_t events) {}

void GPIO_IRQ_Handler::general_callback(uint gpio_pin, uint32_t events) {
    isr_list_[get_core_num()][gpio_pin](gpio_pin, events);
}