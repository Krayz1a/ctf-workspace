#include "auth_tag.h"
#include "hash.h"

#include <cstdlib>
#include <iostream>
#include <random>
using namespace std;


static size_t constexpr ptr_no_bits = sizeof(uintptr_t) * 8;
static size_t constexpr tag_no_bits = 10;
static size_t constexpr val_no_bits = ptr_no_bits - tag_no_bits;

static size_t constexpr secret_sz = 100;
static vector<byte> secret;

static uintptr_t auth_tag_extract_tag(uintptr_t ptr) {
    return ptr & ~((1ull << val_no_bits) - 1);
}

static uintptr_t auth_tag_extract_val(uintptr_t ptr) {
    return ptr & ((1ull << val_no_bits) - 1);
}

void auth_tag_init() {
    secret.resize(secret_sz);

    random_device rd;
    for (size_t i = 0; i < secret_sz; i++)
        secret[i] = static_cast<byte>(rd() & 0xff);
}

uintptr_t auth_tag_sign(uintptr_t ptr) {
    if (auth_tag_has_tag(ptr)) {
        cout << "invalid ptr to sign" << endl;
        abort();
    }

    vector<byte> in;
    for (size_t i = 0; i < sizeof(uintptr_t); i++) in.push_back(((byte const*) &ptr)[i]);
    for (size_t i = 0; i < secret_sz; i++) in.push_back(secret[i]);

    auto out = hash_calc(in);

    uintptr_t tag = *(uintptr_t*)out.data();

    return auth_tag_extract_val(ptr) | auth_tag_extract_tag(tag << val_no_bits);
}

bool auth_tag_has_tag(uintptr_t ptr) {
    return auth_tag_extract_tag(ptr) != 0;
}

bool auth_tag_is_tag_valid(uintptr_t ptr) {
    return ptr == auth_tag_sign(auth_tag_extract_val(ptr));
}

uintptr_t auth_tag_strip_tag(uintptr_t ptr) {
    return auth_tag_extract_val(ptr);
}

uintptr_t auth_tag_set_val(uintptr_t ptr, uintptr_t val) {
    if (auth_tag_extract_tag(val) != 0) {
        cout << "invalid val" << endl;
        abort();
    }

    return val + auth_tag_extract_tag(ptr);
}

uintptr_t auth_tag_set_tag(uintptr_t ptr, uintptr_t tag) {
    if (auth_tag_extract_val(tag) != 0) {
        cout << "invalid tag" << endl;
        abort();
    }

    return auth_tag_extract_val(ptr) + tag;
}
