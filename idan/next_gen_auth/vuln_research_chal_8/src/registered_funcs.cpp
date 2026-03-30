#include "registered_funcs.h"

#include <set>
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
using namespace std;


size_t fast_pow(size_t a, size_t b) {
    size_t res = 1;

    size_t curr_pow = a;
    while (b > 0) {
        if (b & 0b1)
            res *= curr_pow;

        curr_pow = curr_pow * curr_pow;
        b /= 2;
    }

    return res;
}

size_t cnt_no_bits(uint32_t val) {
    size_t no_bits = 0;

    while (val > 0) {
        ++no_bits;
        val &= (val - 1);
    }

    return no_bits;
}

static void print_flag() {
    ifstream flag_file("flag.txt");
    if (!flag_file.is_open()) throw runtime_error("can't open flag file");
    else cout << flag_file.rdbuf() << endl;
}

static size_t constexpr access_flag_no_byte = 100;
void access_flag() {
    random_device rd;

    bool success = true;
    for (size_t i = 0; i < access_flag_no_byte; i++) {
        uint8_t val = static_cast<uint8_t>(rd() & 0xff);

        cout << "choose val" << endl;
        uint8_t in;
        cin >> in;

        if (val != in) {
            success = false;
            break;
        }
    }

    if (success) {
        cout << "access_flag success" << endl;
        print_flag();
    }
    else {
        cout << "access_flag fail" << endl;
    }
}

static set<registered_func> const registered_funcs = {
    {(uintptr_t) ctime, "get time"},
    {(uintptr_t) expf, "exp with float"},
    {(uintptr_t) bsearch, "binary search"},
    {(uintptr_t) fast_pow, "fast power"},
    {(uintptr_t) cnt_no_bits, "count number of \"1\" bits in the binary representation"},
    {(uintptr_t) access_flag, "access the flag"},
    {(uintptr_t) malloc, "alloc"}
};

void registered_funcs_print() {
    size_t i = 0;
    for (auto& registered_func : registered_funcs) {
        printf("registered_func[%zu]: %s, %zx\n", i, registered_func.desc.c_str(), registered_func.addr);
        ++i;
    }
}

bool registered_funcs_has_func(uintptr_t ptr_raw) {
    return registered_funcs.count(registered_func{ptr_raw, ""});
}