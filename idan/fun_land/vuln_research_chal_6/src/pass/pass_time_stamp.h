#pragma once

#include <cstdint>
#include <cstddef>
using namespace std;


struct pass_time_stamp_t {
    uint8_t mins_0 = 0;
    uint8_t mins_1 = 0;
    uint8_t hours_0 = 0;
    uint8_t hours_1 = 0;

    size_t get_as_epoch() const {
        return
            mins_0
            + mins_1 * 10
            + hours_0 * 60
            + hours_1 * 10 * 60
        ;
    }

    void set_from_epoch(size_t epoch) {
        mins_0 = (epoch % 60) % 10;
        mins_1 = (epoch % 60) / 10;

        hours_0 = (epoch / 60) % 10;
        hours_1 = (epoch / 60) / 10;
    }
};

bool pass_time_stamp_advance(pass_time_stamp_t& time, size_t add_mins);

bool pass_time_stamp_less(pass_time_stamp_t const& l, pass_time_stamp_t const& r);
size_t pass_time_stamp_diff(pass_time_stamp_t const& small, pass_time_stamp_t const& big);

pass_time_stamp_t get_pass_time_stamp();
void pass_time_stamp_print(pass_time_stamp_t const& time);