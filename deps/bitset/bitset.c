#include <assert.h>
#include "bitset.h"

bool bitset_isOn(const uint64_t bitset, const unsigned index) {
    assert(index < 64);
    return (bitset & (1UL << index)) > 0;
}

bool bitset_isOff(const uint64_t bitset, const unsigned index) {
    assert(index < 64);
    return (bitset & (1UL << index)) == 0;
}

uint64_t bitset_setOn(const uint64_t bitset, const unsigned index) {
    assert(index < 64);
    return bitset | (1UL << index);
}

uint64_t bitset_setOff(const uint64_t bitset, const unsigned index) {
    assert(index < 64);
    return bitset & ~(1UL << index);
}
