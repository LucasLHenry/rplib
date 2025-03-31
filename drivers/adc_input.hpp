#ifndef DRIVERS_ADC_INPUT_H_
#define DRIVERS_ADC_INPUT_H_

#include "hardware/adc.h"
#include "../utils/utils.h"

#define MAX_OVERSAMPLE_AMT 64

enum InputType {
    CV,
    POT
};

class ADCInput {
    public:
        ADCInput() {}
        ~ADCInput() {}

        void init(size_t oversample, uint gpio_pin, InputType pot_or_cv);
        // mux_pins array should be in LSB first order
        void configure_mux(uint mux_idx, uint mux_num_pins, uint* mux_pins);
        void read();
        uint16_t value_u12;
        float value_f;
    
    private:
        void process_pot(uint16_t raw_val);
        void process_cv(uint16_t raw_val);
        void setup_pins();
        uint16_t raw_vals_[MAX_OVERSAMPLE_AMT];
        uint adc_pin_;
        InputType pot_or_cv_;
        size_t oversample_amt_;
        bool on_mux_;
        uint8_t mux_idx_;
        uint mux_num_pins_;
        uint mux_pins_[4];  // for a 16 input mux, more than needed
};


#endif  // DRIVERS_ADC_INPUT_H_