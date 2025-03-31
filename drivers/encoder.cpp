#include "encoder.hpp"

void Encoder::init(uint gpio_pin_base, bool pullup, bool invert) {
    invert_ = invert;

    init_gpio(gpio_pin_base, pullup);
    init_pio(gpio_pin_base);
    init_dma();

    count_ = 0;
    pio_sm_exec(pio1, pio_sm, pio_encode_set(pio_x, count_));
    pio_sm_exec(pio1, pio_sm, pio_encode_mov(pio_y, pio_pins));
    pio_sm_set_enabled(pio1, pio_sm, true);
}

void Encoder::init_gpio(uint gpio_pin_base, bool pullup) {
    gpio_init(gpio_pin_base);
    gpio_set_dir(gpio_pin_base, GPIO_IN);
    gpio_pull_up(gpio_pin_base);

    gpio_init(gpio_pin_base + 1);
    gpio_set_dir(gpio_pin_base + 1, GPIO_IN);
    gpio_pull_up(gpio_pin_base + 1);

    if (!pullup) {
        gpio_disable_pulls(gpio_pin_base);
        gpio_disable_pulls(gpio_pin_base + 1);
    }
}

void Encoder::init_pio(uint gpio_pin_base) {
    pio_add_program(pio1, &encoder_program);
    pio_sm = pio_claim_unused_sm(pio1, true);
    uint program_offset = 0;  // need it to be at this position for jump tables
    pio_sm_config pio_config = encoder_program_get_default_config(program_offset);
    sm_config_set_in_shift(&pio_config, false, false, 32);
    sm_config_set_in_pins(&pio_config, gpio_pin_base);
    sm_config_set_fifo_join(&pio_config, PIO_FIFO_JOIN_RX);
    pio_sm_init(pio1, pio_sm, encoder_offset_start, &pio_config);
}

void Encoder::init_dma() {
    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config dma_config = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&dma_config, false);
    channel_config_set_write_increment(&dma_config, false);
    channel_config_set_dreq(&dma_config, pio_get_dreq(pio1, pio_sm, false));
    dma_channel_configure(
        dma_chan,
        &dma_config,
        &count_,
        &pio1->rxf[pio_sm],
        0xFFFFFFFF,
        true
    );
}


void Encoder::read() {
    int32_t dif = invert_? prev_count_ - count_ : count_ - prev_count_;
    // clamp change to help with debouncing. We're sampling at a rate that
    // means that we can guarantee it won't be turning more than one click
    // per read, and it's fast enough to not cause any issues
    if (dif < -1) dif = -1;
    if (dif > 1) dif = 1;
    prev_count_ = count_;
    value_i32 += dif;
    value_i32 >> 1;
    change = dif;
}