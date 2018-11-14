#include <stdio.h>
#include <ordering.h>

static enum Ordering intCmp(const void *sut, const void *other);

int main() {
    int sut = 6, other = 8;

    const struct Eq *eq = Eq_instance(intCmp);
    printf("6 == 8? %s\n", (Eq_equal(eq, &sut, &other)) ? "true" : "false");
    printf("6 != 8? %s\n", (Eq_notEqual(eq, &sut, &other)) ? "true" : "false");

    puts("");

    const struct Ord *ord = Ord_instance(intCmp);
    printf("6 < 8? %s\n", (Ord_less(ord, &sut, &other)) ? "true" : "false");
    printf("6 <= 8? %s\n", (Ord_lessEqual(ord, &sut, &other)) ? "true" : "false");
    printf("6 == 8? %s\n", (Ord_equal(ord, &sut, &other)) ? "true" : "false");
    printf("6 != 8? %s\n", (Ord_notEqual(ord, &sut, &other)) ? "true" : "false");
    printf("6 > 8? %s\n", (Ord_greater(ord, &sut, &other)) ? "true" : "false");
    printf("6 >= 8? %s\n", (Ord_greaterEqual(ord, &sut, &other)) ? "true" : "false");
    printf("max(6, 8)? %d\n", *(int *) Ord_max(ord, &sut, &other));
    printf("min(6, 8)? %d\n", *(int *) Ord_min(ord, &sut, &other));
    switch (Ord_compare(ord, &sut, &other)) {
        case ORDERING_LESS:
            puts("compare(6, 8)? 6 < 8");
            break;
        case ORDERING_EQUAL:
            puts("compare(6, 8)? 6 == 8");
            break;
        case ORDERING_GREATER:
            puts("compare(6, 8)? 6 > 8");
            break;
        default:
            return -1;
    }

    puts("");

    eq = Ord_getEq(ord);
    printf("6 == 8? %s\n", (Eq_equal(eq, &sut, &other)) ? "true" : "false");
    printf("6 != 8? %s\n", (Eq_notEqual(eq, &sut, &other)) ? "true" : "false");

    return 0;
}

/*
 *
 */
enum Ordering intCmp(const void *const sut, const void *const other) {
    int x = (NULL == sut) ? 0 : *(int *) sut, y = (NULL == other) ? 0 : *(int *) other;
    if (x == y) {
        return ORDERING_EQUAL;
    } else if (x > y) {
        return ORDERING_GREATER;
    } else {
        return ORDERING_LESS;
    }
}
