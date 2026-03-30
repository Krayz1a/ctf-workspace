#include "pass_time_stamp.h"

#include <iostream>
using namespace std;

bool pass_time_stamp_advance(pass_time_stamp_t& time, size_t add_mins) {
    size_t epoch = time.get_as_epoch();

    // out of the current day
    if (!(add_mins < 60 * 24 - epoch)) return false;

    epoch += add_mins;

    time.set_from_epoch(epoch);

    return true;
}

bool pass_time_stamp_less(pass_time_stamp_t const& l, pass_time_stamp_t const& r) {
    return l.get_as_epoch() < r.get_as_epoch();
}

size_t pass_time_stamp_diff(pass_time_stamp_t const& small, pass_time_stamp_t const& big) {
    return big.get_as_epoch() - small.get_as_epoch();
}

pass_time_stamp_t get_pass_time_stamp() {
    unsigned hours, mins;
    if (scanf("%u:%u", &hours, &mins) != 2) throw runtime_error("can't get input");

    if (!(0 <= hours && hours < 24) || !(0 <= mins && mins < 60)) throw runtime_error("invalid time");

    pass_time_stamp_t res;
    res.mins_0 = mins % 10;
    res.mins_1 = mins / 10;
    res.hours_0 = hours % 10;
    res.hours_1 = hours / 10;

    return res;
}

void pass_time_stamp_print(pass_time_stamp_t const& time) {
    printf("%u%u:%u%u", (unsigned) time.hours_1, (unsigned) time.hours_0, (unsigned) time.mins_1, (unsigned) time.mins_0);
}