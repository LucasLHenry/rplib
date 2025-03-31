#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include "pow2f_lut.h"
#include "sin2pif_lut.h"

// courtesy of Emilie Gillet
#define MAKE_INTEGRAL_FRACTIONAL(x) \
    int32_t x ## _integral = static_cast<int32_t>(x); \
    float x ## _fractional = x - static_cast<float>(x ## _integral);

#define CONSTRAIN(var, min, max) \
    if (var < (min)) { \
        var = (min); \
    } else if (var > (max)) { \
        var = (max); \
    }

inline uint32_t rand_u32() {
    static uint64_t val = 1234;  // seed
    val = (val*134775813 + 1) % (1 << 31);
    return static_cast<uint32_t>(val);
}

inline int32_t rand_i32() {
    return static_cast<int32_t>(rand_u32() - (1 << 31));
}

// have to do it this way because of weird linker things
template <typename T>
inline T xfade(T a, T b, float blend);
#include "utils_impl.h"

inline float interpolate(const float* table, float index, float size) {
    index *= size - 1;
    MAKE_INTEGRAL_FRACTIONAL(index)
    float a = table[index_integral];
    float b = table[index_integral + 1];
    return a + (b - a) * index_fractional;
}

inline float pow2f(float x) {
    uint8_t neg = x < 0;
    if (neg) x = -x;
    MAKE_INTEGRAL_FRACTIONAL(x);
    float lut_val = interpolate(pow2f_lut, x_fractional, 1024.0);
    lut_val *= static_cast<float>(1 << x_integral);
    return neg? 1.0f/lut_val : lut_val;
}

inline float sin2pif(float x) {
    while (x > 1.0f) x -= 1.0f;
    x *= 4.0f;
    if (x < 1.0f) return  interpolate(sin2pif_lut, x,        1024.0);
    if (x < 2.0f) return  interpolate(sin2pif_lut, 2.0f - x, 1024.0);
    if (x < 3.0f) return -interpolate(sin2pif_lut, x - 2.0f, 1024.0);
    else          return -interpolate(sin2pif_lut, 4.0f - x, 1024.0);
}

inline float cos2pif(float x) {
    x += 0.25f;
    if (x >= 1.0f) x -= 1.0f;
    return sin2pif(x);
}

inline float sign(float x) {
    return (x > 0)? 1.0f : -1.0f;
}

inline float soft_limit(float x) {
    if (x >= 3.0) return 1.0;
    if (x <= -3.0) return -1.0;
    return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
}

#define ONE_POLE_LPF(out, in, coefficient) out += (in - out) * coefficient

// normalized frequency is freq (Hz) / sample rate (Hz)
inline float ewma_filter_coefficient(float normalized_freq) {
    return normalized_freq / (1.0f + normalized_freq);
}

// from Emilie Gillet
inline float this_blep(float t) {
    return 0.5f * t * t;
}

inline float next_blep(float t) {
    t = 1.0f - t;
    return -0.5f * t * t;
}

inline float next_iblep(float t) {
    const float t1 = 0.5f * t;
    const float t2 = t1 * t1;
    const float t4 = t2 * t2;
    return 0.1875f - t1 + 1.5f * t2 - t4;
}

inline float this_iblep(float t) {
    return next_iblep(1.0f - t);
}

#endif  // UTILS_H_