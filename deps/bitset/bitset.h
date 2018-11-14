#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BITSET_VERSION_MAJOR        0
#define BITSET_VERSION_MINOR        1
#define BITSET_VERSION_PATCH        0
#define BITSET_VERSION_SUFFIX       ""
#define BITSET_VERSION_IS_RELEASE   0
#define BITSET_VERSION_HEX          0x000100

extern bool bitset_isOn(uint64_t bitset, unsigned index);
extern bool bitset_isOff(uint64_t bitset, unsigned index);

extern uint64_t bitset_setOn(uint64_t bitset, unsigned index);
extern uint64_t bitset_setOff(uint64_t bitset, unsigned index);

#ifdef __cplusplus
};
#endif
