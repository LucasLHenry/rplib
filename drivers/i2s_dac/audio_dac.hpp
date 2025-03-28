#ifndef DRIVERS_AUDIO_DAC_H_
#define DRIVERS_AUDIO_DAC_H_

#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"

#include "audio_dac.pio.h"

// based on the structure of the audio_dac from Emilie Gillet's Plaits

const size_t k_max_block_size = 24;

class AudioDAC {
    public:
        AudioDAC() {}
        ~AudioDAC() {}

        typedef struct {
            int16_t l;
            int16_t r;
        } Frame;

        typedef void (*FillBufferCallback)(Frame* tx, size_t size);

        void init(uint32_t sample_rate, size_t block_size, uint8_t pio_state_machine
                  uint i2s_data_pin, uint i2s_clk_pin);
        void start(FillBufferCallback callback);
        void stop();
        void fill();
        void swap_buffers();
        Frame* get_next_buf_addr();

        uint8_t dma_chan, pio_sm;
        
    private:
        void init_gpio(uint i2s_data_pin, uint i2s_clk_pin);
        void init_audio_interface(uint32_t sample_rate);
        void init_dma(size_t block_size);

        size_t block_size_;
        FillBufferCallback callback_;
        uint32_t sample_rate_;
        Frame tx_dma_buffer_1[k_max_block_size];
        Frame tx_dma_buffer_2[k_max_block_size];
        Frame* current_read_buffer;
        Frame* next_read_buffer;
};

void dma_handler_();

#endif // DRIVERS_AUDIO_DAC_H_