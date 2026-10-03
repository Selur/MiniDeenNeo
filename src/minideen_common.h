#include <cstdint>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#define MINIDEEN_X86 1
#include <immintrin.h>
#else
#define MINIDEEN_X86 0
#endif

enum PathType {
  Slow, Fast
};

template <typename PixelType>
void minideen_C(const uint8_t *, uint8_t *, int, int, int, int, unsigned int, int);

#if MINIDEEN_X86
void minideen_SSE2_8(const uint8_t *, uint8_t *, int, int, int, int, unsigned int, int);
void minideen_SSE2_16(const uint8_t *, uint8_t *, int, int, int, int, unsigned int, int);

void minideen_AVX2_8(const uint8_t *, uint8_t *, int, int, int, int, unsigned int, int);
void minideen_AVX2_16(const uint8_t *, uint8_t *, int, int, int, int, unsigned int, int);
#endif
