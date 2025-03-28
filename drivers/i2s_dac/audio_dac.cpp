#include "audio_dac.hpp"

AudioDAC audio_dac;

void AudioDAC::init(uint32_t sample_rate, size_t block_size) {
    sample_rate_ = sample_rate;
    block_size_ = block_size;
    current_read_buffer = &tx_dma_buffer_1[0];
    next_read_buffer = &tx_dma_buffer_2[0];
    pio_sm = DAC_PIO_SM;
    init_gpio();
    init_audio_interface(sample_rate);
    init_dma(block_size_);
}

void AudioDAC::start(FillBufferCallback callback) {
    callback_ = callback;
    dma_channel_start(dma_chan);
}

void AudioDAC::stop() {
    dma_channel_abort(dma_chan);
}

void AudioDAC::fill() {
    callback_(current_read_buffer, block_size_);
}

void AudioDAC::swap_buffers() {
    // swap buffers
    Frame* temp = current_read_buffer;
    current_read_buffer = next_read_buffer;
    next_read_buffer = temp;
    // Start DMA transfer again for the new buffer
    dma_channel_set_read_addr(dma_chan, current_read_buffer, true);
}

void AudioDAC::init_gpio() {
    gpio_set_function(I2S_DATA_PIN, GPIO_FUNC_PIO0);
    gpio_set_function(I2S_BASE_CLK_PIN, GPIO_FUNC_PIO0);
    gpio_set_function(I2S_BASE_CLK_PIN + 1, GPIO_FUNC_PIO0);
}

void AudioDAC::init_audio_interface(uint32_t sample_rate) {
    uint offset = pio_add_program(pio0, &audio_i2s_program);
    
    audio_i2s_program_init(pio0, pio_sm, offset, I2S_DATA_PIN, I2S_BASE_CLK_PIN);
    
    float divider = (float)clock_get_hz(clk_sys) / (sample_rate *  2 * 32);
 
    pio_sm_set_clkdiv(pio0, pio_sm, divider);
 
    pio_sm_restart(pio0, pio_sm);
    pio_sm_set_enabled(pio0, pio_sm, true);
}

void AudioDAC::init_dma(size_t block_size) {
    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config dma_config = dma_channel_get_default_config(dma_chan);
    // Configure DMA to transfer from memory (buffer) to the PIO FIFO
    channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_32);  // 32-bit transfers
    channel_config_set_dreq(&dma_config, pio_get_dreq(pio0, pio_sm, true));  // Data request when PIO needs more data
    dma_channel_configure(
        dma_chan,
        &dma_config,
        &pio0->txf[pio_sm],   // Write address (PIO FIFO)
        current_read_buffer,  // Read address (audio buffer)
        block_size,           // Number of 32-bit words to transfer
        false
    );

    // Enable the DMA IRQ and set the handler
    irq_set_exclusive_handler(DMA_IRQ_1, dma_handler_);
    irq_set_enabled(DMA_IRQ_1, true);
 
    // Enable DMA interrupt for when the buffer is fully transferred
    dma_channel_set_irq1_enabled(dma_chan, true);
}

void dma_handler_() {
    dma_hw->ints1 = 1u << audio_dac.dma_chan;
    audio_dac.swap_buffers();
    audio_dac.fill();
}