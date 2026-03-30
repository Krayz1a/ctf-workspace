#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <memory>
using namespace std;


using pass_sz_t = unsigned;
using pass_check_status_t = pass_sz_t;
using arr_sz_t = unsigned;

pass_sz_t constexpr pass_min_sz = 16;
arr_sz_t constexpr arr_min_sz = 255;

/*
user ctx mapping:
| pass (arr of bytes) | arr (of bytes) | last pass check status | last arr query status |
*/

struct user_ctx {
    uintptr_t map_addr = {};

    pass_sz_t pass_sz;
    arr_sz_t arr_sz;

    user_ctx(pass_sz_t pass_sz, arr_sz_t arr_sz);

    ~user_ctx();

    size_t pass_off() const {return (size_t) 0; }
    size_t arr_off() const {return pass_off() + (size_t) pass_sz; }
    size_t pass_check_status_off() const {return arr_off() + (size_t) arr_sz; }
    size_t arr_query_status_off() const {return pass_check_status_off() + sizeof(pass_check_status_t); }
    size_t size() const {return arr_query_status_off() + sizeof(byte); }

    /* pass */
private:
    byte& at_pass(pass_sz_t off) const {return *(byte*)(map_addr + pass_off() + off); }
    pass_check_status_t& pass_check_status() const {return *(pass_check_status_t*)(map_addr + pass_check_status_off()); }

public:
    void feed_pass(vector<byte> const& pass);

    size_t max_no_failed_attempts;
    size_t no_failed_attempts;
    bool try_authorize();

    /* arr */
private:
    byte& at_arr(arr_sz_t off) const {return *(byte*)(map_addr + arr_off() + off); }

public:
    byte& arr_query_status() const {return *(byte*)(map_addr + arr_query_status_off()); }

    void set_val(byte val, arr_sz_t idx);
    void set_arr(vector<byte> const& arr);

    size_t max_no_pass_leaks;
    size_t no_pass_leaks;
    void leak_pass(pass_sz_t idx);

    void query_arr(byte val);

};

