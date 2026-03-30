#include "sign_oracle.h"
#include "auth_tag.h"

#include <set>
using namespace std;


// static size_t no_signings_left;
static set<uintptr_t> signed_ptrs;

void signing_oracle_init() {
    // no_signings_left = 1000;
    signed_ptrs = {};
}

uintptr_t signing_oracle_sign(uintptr_t ptr_raw) {
    // if (no_signings_left == 0) throw runtime_error("too many signing");

    // sign
    uintptr_t ptr_tagged = auth_tag_sign(ptr_raw);

    // add the raw ptr to the signed_ptrs list
    signed_ptrs.insert(ptr_raw);

    // decrement no_signings_left
    // --no_signings_left;

    return ptr_tagged;
}

bool signing_oracle_was_signed(uintptr_t ptr_raw) {
    return signed_ptrs.count(ptr_raw);
}
