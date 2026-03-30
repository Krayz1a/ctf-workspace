#pragma once

#include <cstdint>
#include <string>
using namespace std;


struct registered_func {
    uintptr_t addr;
    string desc;

    bool operator==(registered_func const& other) const {
        return addr == other.addr;
    }

    bool operator!=(registered_func const& other) const {
        return !(*this == other);
    }

    bool operator<(registered_func const& other) const {
        return addr < other.addr;
    }

};

size_t fast_pow(size_t a, size_t b);

size_t cnt_no_bits(uint32_t val);

void access_flag();

void registered_funcs_print();

bool registered_funcs_has_func(uintptr_t ptr_raw);
