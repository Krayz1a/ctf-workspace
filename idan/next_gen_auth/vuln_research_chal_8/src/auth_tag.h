#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <iostream>
using namespace std;


void auth_tag_init();

uintptr_t auth_tag_sign(uintptr_t ptr);

bool auth_tag_has_tag(uintptr_t ptr);

bool auth_tag_is_tag_valid(uintptr_t ptr);

uintptr_t auth_tag_strip_tag(uintptr_t ptr);

uintptr_t auth_tag_set_val(uintptr_t ptr, uintptr_t val);

uintptr_t auth_tag_set_tag(uintptr_t ptr, uintptr_t tag);
