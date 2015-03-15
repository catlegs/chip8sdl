#ifndef CHIP8COMMON
#define CHIP8COMMON

#include <cinttypes>
#include <functional>

typedef uint_least8_t u8;
typedef int_least8_t s8;
typedef uint_least16_t u16;
typedef int_least16_t s16;
typedef uint_least32_t u32;
typedef int_least32_t s32;

// invokes a passed in function at the specified times per second until it returns false
void invokeFuncAtInterval(std::function<bool()> func, unsigned long timesPerSecond);

#endif // CHIP8COMMON