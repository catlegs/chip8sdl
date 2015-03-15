#ifndef CHIP8COMMON
#define CHIP8COMMON

#include <cinttypes>
#include <functional>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;

// invokes a passed in function at the specified times per second until it returns false
void invokeFuncAtInterval(std::function<bool()> func, long timesPerSecond);

#endif // CHIP8COMMON