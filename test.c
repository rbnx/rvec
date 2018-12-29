#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "rvec.h"


int main(int argc, const char *argv[]) {
    rvec_t(int) v1;
    rvec_init(v1);

    assert(rvec_capacity(v1) == 0);
    assert(rvec_size(v1) == 0);

    int el1 = 111, el2 = 222;

    rvec_push(v1,el1);
    rvec_push(v1,el2);

    assert(rvec_get(v1,1) == el2);
    assert(rvec_i(v1,1) == el2);
    assert(*rvec_begin(v1) == el1);
    assert(*(rvec_end(v1) - 1) == el2);

    assert(0 == rvec_push(v1,333));

    int popped = rvec_pop(v1);

    assert(popped == 333 && rvec_size(v1) == 2);

    for(size_t i = 0; i < 800; i++) {
        rvec_push(v1,i);
    }

    assert(rvec_size(v1) == 802 && rvec_capacity(v1) >= 802);

    rvec_resize(v1,100);
    assert(rvec_size(v1) == 100);

    rvec_set(v1,77,-999);

    rvec_t(int) v2;
    rvec_init(v2);

    rvec_copy(v2,v1);
    assert(rvec_size(v2) == rvec_size(v1));

    assert(rvec_i(v2,77) == -999);

    { // new scope
        rvec_t(double) v3;
        rvec_init(v3);
        rvec_push(v3,9.9);
    } // <-- v3 gets freed here. use valgrind to prove

    puts("OK");
    return 0;
}