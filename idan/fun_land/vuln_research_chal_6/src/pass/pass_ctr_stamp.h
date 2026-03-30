#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
using namespace std;


size_t constexpr pass_ctr_stamp_no_digits = 3;
struct pass_ctr_stamp_t {
    array<uint8_t, pass_ctr_stamp_no_digits> digits = {};
};

bool pass_ctr_stamp_is_same(pass_ctr_stamp_t const& l, pass_ctr_stamp_t const& r);

void pass_ctr_stamp_increment(pass_ctr_stamp_t& ctr_stamp);
bool pass_ctr_stamp_is_max(pass_ctr_stamp_t const& ctr_stamp);

pass_ctr_stamp_t get_pass_ctr_stamp();
void pass_ctr_stamp_print(pass_ctr_stamp_t const& ctr_stamp);