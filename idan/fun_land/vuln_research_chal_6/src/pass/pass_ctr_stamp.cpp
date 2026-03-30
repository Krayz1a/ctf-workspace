#include "pass_ctr_stamp.h"

#include <iostream>
using namespace std;

bool pass_ctr_stamp_is_same(pass_ctr_stamp_t const& l, pass_ctr_stamp_t const& r) {
    for (size_t i = 0; i < pass_ctr_stamp_no_digits; i++)
        if (l.digits[i] != r.digits[i])
            return false;
    
    return true;
}

void pass_ctr_stamp_increment(pass_ctr_stamp_t& ctr_stamp) {
    for (size_t i = 0; i < pass_ctr_stamp_no_digits; i++) {
        if (ctr_stamp.digits[i] == 9) {
            ctr_stamp.digits[i] = 0;

        } else {
            ++ctr_stamp.digits[i];
            break;
        }
    }
}

bool pass_ctr_stamp_is_max(pass_ctr_stamp_t const& ctr_stamp) {
    for (size_t i = 0; i < pass_ctr_stamp_no_digits; i++)
        if (ctr_stamp.digits[i] != 9) return false;

    return true;
}

pass_ctr_stamp_t get_pass_ctr_stamp() {
    pass_ctr_stamp_t res;

    string str;
    cin >> str;

    if (str.size() != pass_ctr_stamp_no_digits) throw runtime_error("invalid pass_ctr_stamp");

    for (size_t i = 0; i < pass_ctr_stamp_no_digits; i++) {
        try {
            res.digits[i] = stoi(str.substr(str.size() - 1 - i, 1));
            if (!(0 <= res.digits[i] && res.digits[i] <= 9)) throw;
            
        } catch (...) {
            throw runtime_error("invalid pass_ctr_stamp");
        }
    }

    return res;
}

void pass_ctr_stamp_print(pass_ctr_stamp_t const& ctr_stamp) {
    size_t i = pass_ctr_stamp_no_digits;
    do {
        --i;
        cout << (int) ctr_stamp.digits[i];
    } while (i != 0);
}