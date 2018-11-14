#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <assert.h>
#include <panic/panic.h>
#include <bitset/bitset.h>
#include "ordering.h"

static const struct Eq *EqPool_instance(enum Ordering compare(const void *, const void *));
static const struct Ord *OrdPool_instance(enum Ordering compare(const void *, const void *));

struct Eq {
    enum Ordering (*compare)(const void *, const void *);
};

const struct Eq *Eq_instance(enum Ordering (*const compare)(const void *, const void *)) {
    Panic_when(NULL == compare);
    return EqPool_instance(compare);
}

bool Eq_equal(const struct Eq *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return ORDERING_EQUAL == self->compare(sut, other);
}

bool Eq_notEqual(const struct Eq *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return ORDERING_EQUAL != self->compare(sut, other);
}

struct Ord {
    enum Ordering (*compare)(const void *, const void *);
};

const struct Ord *Ord_instance(enum Ordering (*const compare)(const void *, const void *)) {
    Panic_when(NULL == compare);
    return OrdPool_instance(compare);
}

const struct Eq *Ord_getEq(const struct Ord *const self) {
    Panic_when(NULL == self);
    return Eq_instance(self->compare);
}

bool Ord_less(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return ORDERING_LESS == self->compare(sut, other);
}

bool Ord_lessEqual(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    switch (self->compare(sut, other)) {
        case ORDERING_LESS:
        case ORDERING_EQUAL:
            return true;
        case ORDERING_GREATER:
            return false;
        default:
            Panic_terminate("%s", "This line should never be reached");
    }
}

bool Ord_equal(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return ORDERING_EQUAL == self->compare(sut, other);
}

bool Ord_notEqual(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return ORDERING_EQUAL != self->compare(sut, other);
}

bool Ord_greater(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return ORDERING_GREATER == self->compare(sut, other);
}

bool Ord_greaterEqual(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    switch (self->compare(sut, other)) {
        case ORDERING_LESS:
            return false;
        case ORDERING_EQUAL:
        case ORDERING_GREATER:
            return true;
        default:
            Panic_terminate("%s", "This line should never be reached");
    }
}

const void *Ord_max(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return (ORDERING_GREATER == self->compare(sut, other)) ? sut : other;
}

const void *Ord_min(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return (ORDERING_LESS == self->compare(sut, other)) ? sut : other;
}

enum Ordering Ord_compare(const struct Ord *const self, const void *const sut, const void *const other) {
    Panic_when(NULL == self);
    return self->compare(sut, other);
}

/*
 *
 */
struct EqPool {
    struct Eq objects[31];
    struct EqPool *next;
    uint32_t tags: 31;
    uint32_t unused: 1;
};

struct EqPool *eqPool = NULL;

static struct EqPool *EqPool_new(void) {
    struct EqPool *pool = calloc(1, sizeof(*pool));
    Panic_when(NULL == pool);
    return pool;
}

static void EqPool_onExit(void) {
    while (NULL != eqPool) {
        struct EqPool *next = eqPool->next;
        free(eqPool);
        eqPool = next;
    }
}

const struct Eq *EqPool_instance(enum Ordering (*const compare)(const void *, const void *)) {
    assert(NULL != compare);

    if (NULL == eqPool) {
        eqPool = EqPool_new();
        atexit(EqPool_onExit);
    }

    struct EqPool *pool = eqPool;
    const unsigned index = (unsigned) ((uintptr_t) compare % 31);

    do {
        if (bitset_isOn(pool->tags, index)) {
            struct Eq *object = &pool->objects[index];
            if (compare == object->compare) {
                return object;
            }
        } else {
            break;
        }
        pool = pool->next;
    } while (NULL != pool);

    if (NULL == pool) {
        // Add a new chunk to the atoms-table
        pool = EqPool_new();
        pool->next = eqPool;
        eqPool = pool;
    }

    // mark the object at the given position in the atoms-table as unavailable
    pool->tags = (uint32_t) bitset_setOn(pool->tags, index);

    // Initialize the object
    struct Eq *object = &pool->objects[index];
    object->compare = compare;

    return object;
}

/*
 *
 */
struct OrdPool {
    struct Ord objects[31];
    struct OrdPool *next;
    uint32_t tags: 31;
    uint32_t unused: 1;
};

struct OrdPool *ordPool = NULL;

static struct OrdPool *OrdPool_new(void) {
    struct OrdPool *pool = calloc(1, sizeof(*pool));
    Panic_when(NULL == pool);
    return pool;
}

static void OrdPool_onExit(void) {
    while (NULL != ordPool) {
        struct OrdPool *next = ordPool->next;
        free(ordPool);
        ordPool = next;
    }
}

const struct Ord *OrdPool_instance(enum Ordering (*const compare)(const void *, const void *)) {
    assert(NULL != compare);

    if (NULL == ordPool) {
        ordPool = OrdPool_new();
        atexit(OrdPool_onExit);
    }

    struct OrdPool *pool = ordPool;
    const unsigned index = (unsigned) ((uintptr_t) compare % 31);

    do {
        if (bitset_isOn(pool->tags, index)) {
            struct Ord *object = &pool->objects[index];
            if (compare == object->compare) {
                return object;
            }
        } else {
            break;
        }
        pool = pool->next;
    } while (NULL != pool);

    if (NULL == pool) {
        // Add a new chunk to the atoms-table
        pool = OrdPool_new();
        pool->next = ordPool;
        ordPool = pool;
    }

    // mark the object at the given position in the atoms-table as unavailable
    pool->tags = (uint32_t) bitset_setOn(pool->tags, index);

    // Initialize the object
    struct Ord *object = &pool->objects[index];
    object->compare = compare;

    return object;
}
