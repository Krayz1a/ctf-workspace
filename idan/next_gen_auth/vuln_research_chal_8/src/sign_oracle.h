#pragma once

#include <cstdint>
using namespace std;

void signing_oracle_init();

uintptr_t signing_oracle_sign(uintptr_t ptr_raw);

bool signing_oracle_was_signed(uintptr_t ptr_raw);
