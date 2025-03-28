#include "adc_input.hpp"

void ADCInput::init(size_t oversample, uint gpio_pin, InputType pot_or_cv) {
    oversample_amt_ = oversample;
    adc_pin_ = gpio_pin;
    pot_or_cv_ = pot_or_cv;
    on_mux_ = false;
    adc_gpio_init(adc_pin_);
}

void ADCInput::configure_mux(uint mux_idx, uint mux_num_pins, uint* mux_pins) {
    on_mux_ = true;
    mux_idx_ = mux_idx;
    mux_num_pins_ = mux_num_pins;
    for (uint i = 0; i < mux_num_pins_; i++) {
        mux_pins_[i] = mux_pins[i];
        gpio_init(mux_pins_[i]);
        gpio_set_dir(mux_pins_[i], GPIO_OUT);
    }
}

void ADCInput::setup_pins() {
    adc_select_input(adc_pin_ - ADC_BASE_PIN);
    if (on_mux_) {
        for (uint i = 0; i < mux_num_pins_; i++) {
            gpio_put(mux_pins_[i], ((mux_idx_ >> i) & 0x01));
        }
    }
}

void ADCInput::read() {
    setup_pins();
    uint64_t output = 0;
    for (uint i = 0; i < oversample_amt_; i++) {
        output += adc_read();
    }
    uint16_t oversampled_val_ = static_cast<uint16_t>(output / oversample_amt_);
    if (pot_or_cv_ == POT) process_pot(oversampled_val_);
    else process_cv(oversampled_val_);
    value_f = value_u12 / 4096.0;
}

void ADCInput::process_pot(uint16_t raw_val_) {
    ONE_POLE_LPF(value_u12, raw_val_, ADC_POT_LPF_AMT);
}

void ADCInput::process_cv(uint16_t raw_val_) {

}
