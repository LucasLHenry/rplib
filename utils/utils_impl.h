template <typename T>
inline T xfade(T a, T b, float blend) {
    return static_cast<T>(a + (b-a)*blend);
}