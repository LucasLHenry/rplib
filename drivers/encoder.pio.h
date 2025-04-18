// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------- //
// encoder //
// ------- //

#define encoder_wrap_target 16
#define encoder_wrap 27
#define encoder_pio_version 1

#define encoder_offset_start 16u

static const uint16_t encoder_program_instructions[] = {
    0x0010, //  0: jmp    16                         
    0x0015, //  1: jmp    21                         
    0x0017, //  2: jmp    23                         
    0x0010, //  3: jmp    16                         
    0x0017, //  4: jmp    23                         
    0x0010, //  5: jmp    16                         
    0x0010, //  6: jmp    16                         
    0x0015, //  7: jmp    21                         
    0x0015, //  8: jmp    21                         
    0x0010, //  9: jmp    16                         
    0x0010, // 10: jmp    16                         
    0x0017, // 11: jmp    23                         
    0x0010, // 12: jmp    16                         
    0x0017, // 13: jmp    23                         
    0x0015, // 14: jmp    21                         
    0x0010, // 15: jmp    16                         
            //     .wrap_target
    0xa0c3, // 16: mov    isr, null                  
    0x4042, // 17: in     y, 2                       
    0xa040, // 18: mov    y, pins                    
    0x4042, // 19: in     y, 2                       
    0xa0a6, // 20: mov    pc, isr                    
    0x005a, // 21: jmp    x--, 26                    
    0x001a, // 22: jmp    26                         
    0xa029, // 23: mov    x, !x                      
    0x0059, // 24: jmp    x--, 25                    
    0xa029, // 25: mov    x, !x                      
    0xa0c1, // 26: mov    isr, x                     
    0x8000, // 27: push   noblock                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program encoder_program = {
    .instructions = encoder_program_instructions,
    .length = 28,
    .origin = 0,
    .pio_version = encoder_pio_version,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config encoder_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + encoder_wrap_target, offset + encoder_wrap);
    return c;
}
#endif

