#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ORDERING_VERSION_MAJOR          0
#define ORDERING_VERSION_MINOR          1
#define ORDERING_VERSION_PATCH          0
#define ORDERING_VERSION_SUFFIX         ""
#define ORDERING_VERSION_IS_RELEASE     0
#define ORDERING_VERSION_HEX            0x000100

/*
 *
 */
enum Ordering {
    ORDERING_LESS,
    ORDERING_EQUAL,
    ORDERING_GREATER
};

/*
 *
 */
struct Eq;

extern const struct Eq *Eq_instance(enum Ordering compare(const void *sut, const void *other));

extern bool Eq_equal(const struct Eq *self, const void *sut, const void *other);
extern bool Eq_notEqual(const struct Eq *self, const void *sut, const void *other);

/*
 *
 */
struct Ord;

extern const struct Ord *Ord_instance(enum Ordering compare(const void *sut, const void *other));

extern const struct Eq *Ord_getEq(const struct Ord *self);

extern bool Ord_less(const struct Ord *self, const void *sut, const void *other);
extern bool Ord_lessEqual(const struct Ord *self, const void *sut, const void *other);
extern bool Ord_equal(const struct Ord *self, const void *sut, const void *other);
extern bool Ord_notEqual(const struct Ord *self, const void *sut, const void *other);
extern bool Ord_greater(const struct Ord *self, const void *sut, const void *other);
extern bool Ord_greaterEqual(const struct Ord *self, const void *sut, const void *other);

extern const void *Ord_max(const struct Ord *self, const void *sut, const void *other);
extern const void *Ord_min(const struct Ord *self, const void *sut, const void *other);

extern enum Ordering Ord_compare(const struct Ord *self, const void *sut, const void *other);

#ifdef __cplusplus
};
#endif
