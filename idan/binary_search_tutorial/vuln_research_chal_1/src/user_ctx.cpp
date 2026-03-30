#include "user_ctx.h"

#include <random>
using namespace std;


user_ctx::user_ctx(pass_sz_t _pass_sz, arr_sz_t _arr_sz) {
    if (_pass_sz < pass_min_sz) throw runtime_error("invalid pass sz");
    if (_arr_sz < arr_min_sz)  throw runtime_error("invalid arr sz");
    pass_sz = _pass_sz;
    arr_sz = _arr_sz;

    // create map
    size_t map_sz = size();
    if (auto new_res = new byte[map_sz]; new_res != nullptr) map_addr = (uintptr_t) new_res;
    else throw runtime_error("couldn't create map");

    // init pass
    random_device rd;
    for (pass_sz_t i = 0; i < pass_sz; i++)
        at_pass(i) = static_cast<byte>(rd() & 0xff);
    pass_check_status() = 0;
}

user_ctx::~user_ctx() {
    delete[] (byte*) map_addr;
}


void user_ctx::feed_pass(vector<byte> const& pass) {
    if (pass.size() != pass_sz) throw runtime_error("bogus pass sz");

    pass_check_status() = 0;
    bool match = true;
    for (pass_sz_t i = 0; i < pass_sz; i++) {
        if (at_pass(i) != pass[i]) match = false;
        
        if (match)
            ++pass_check_status();
    }
    
}

bool user_ctx::try_authorize() {
    if (pass_check_status() != pass_sz) {
        cout << "can't authorize" << endl;
        ++no_failed_attempts;

        if (no_failed_attempts > max_no_failed_attempts) {
            throw runtime_error("too many failed authorize attempts");
        } else {
            cout << "try again" << endl;
            return false;
        }

    }

    return true;
}

void user_ctx::set_val(byte val, unsigned idx) {
    if (!(idx < arr_sz)) throw runtime_error("idx of out bounds");

    at_arr(idx) = val;
}

void user_ctx::set_arr(vector<byte> const& arr) {
    if (arr.size() != arr_sz) throw runtime_error("bogus arr sz");

    for (arr_sz_t i = 0; i < arr_sz; i++)
        at_arr(i) = arr[i];
}


void user_ctx::leak_pass(pass_sz_t idx) {
    if (!(no_pass_leaks < max_no_pass_leaks)) throw runtime_error("too many pass leaks");

    if (!(idx < pass_sz)) throw runtime_error("idx out of bounds");

    for (arr_sz_t i = 0; i < arr_sz; i += 2)
        at_arr(i) = at_pass(idx);

    ++no_pass_leaks;
}

static byte lower_bound(byte const* arr, arr_sz_t arr_sz, byte val) {
    arr_sz_t l = 0;
    arr_sz_t r = arr_sz;
    while (l + 1 < r) {
        arr_sz_t mid = (l + r) / 2;

        if (arr[mid] < val) l = mid + 1;
        else r = mid;
    }
    --r;

    // l and r are now the same, pointing to the lower bound element

    return arr[r];
}

void user_ctx::query_arr(byte val) {
    arr_query_status() = lower_bound((byte const*) (map_addr + arr_off()), arr_sz, val);
}
