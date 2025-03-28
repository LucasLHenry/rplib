#ifndef RPLIB_DSP_BLEP_H_
#define RPLIB_DSP_BLEP_H_

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

#endif  // RPLIB_DSP_BLEP_H_