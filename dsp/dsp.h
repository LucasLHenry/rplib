#ifndef RPLIB_DSP_H_
#define RPLIB_DSP_H_

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

#endif  // RPLIB_DSP_H_