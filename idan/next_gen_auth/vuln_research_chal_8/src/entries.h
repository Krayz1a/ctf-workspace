#pragma once

#include <cstdint>
#include <cstddef>
using namespace std;


struct entry_t {
    uintptr_t ptr_tagged;
    uintptr_t ptr_raw;
};

void entries_init();

void entries_set_from_registered(size_t idx, uintptr_t ptr_raw);

void entries_set_custom(size_t idx, uintptr_t ptr_raw, uintptr_t ptr_tagged);

void entries_exec(size_t idx);
