#include "entries.h"
#include "auth_tag.h"
#include "sign_oracle.h"
#include "registered_funcs.h"

#include <cstddef>
#include <vector>
#include <cmath>
using namespace std;

static int cmp_int(void const* left, void const* right) {
    return *(int const*)left < *(int const*)right;
}


static size_t constexpr no_entries = 10;
vector<entry_t> entries;

void entries_init() {
    entries.resize(no_entries);
    for (size_t i = 0; i < no_entries; i++)
        entries[i] = entry_t{.ptr_tagged = 0, .ptr_raw = 0};
}

void entries_set_from_registered(size_t idx, uintptr_t ptr_raw) {
    if (!(idx < no_entries)) throw runtime_error("invalid entry idx");

    entries[idx].ptr_raw = ptr_raw;

    if (registered_funcs_has_func(ptr_raw)) {
        // sign the ptr
        entries[idx].ptr_tagged = auth_tag_sign(ptr_raw);
    } else {
        // not a registered func
        // invalidate the ptr
        entries[idx].ptr_tagged = auth_tag_set_tag(entries[idx].ptr_tagged, 0);
    }
}

void entries_set_custom(size_t idx, uintptr_t ptr_raw, uintptr_t ptr_tagged) {
    if (!(idx < no_entries)) throw runtime_error("invalid entry idx");

    entries[idx].ptr_tagged = ptr_tagged;

    if (!auth_tag_has_tag(entries[idx].ptr_tagged)) {
        // no tag at all
        // invalidate the ptr
        cout << "no tag at all, invalidating the ptr" << endl;
        entries[idx].ptr_tagged = auth_tag_set_val(entries[idx].ptr_tagged, 0);
        return;
    }

    if (!auth_tag_is_tag_valid(entries[idx].ptr_tagged)) {
        // ptr is invalid
        cout << "tag is invalid" << endl;
        return;
    }

    if (auth_tag_strip_tag(entries[idx].ptr_tagged) != ptr_raw) {
        // ptr_tagged doesn't match the ptr_raw
        // invalidate the ptr
        cout << "ptr_tagged doesn't match the ptr_raw, invalidate the ptr" << endl;
        entries[idx].ptr_tagged = auth_tag_set_val(entries[idx].ptr_tagged, 0);
        return;
    }

    entries[idx].ptr_raw = ptr_raw;
}

void entries_exec(size_t idx) {
    if (!(idx < no_entries)) throw runtime_error("invalid entry idx");

    if (entries[idx].ptr_tagged == 0) return;

    if (!auth_tag_is_tag_valid(entries[idx].ptr_tagged)) {
        // ptr is invalid
        cout << "ptr is invalid" << endl;
        return;
    }

    uintptr_t func = entries[idx].ptr_raw;

    if (signing_oracle_was_signed(func)) {
        // ptr was sign by the oracle
        cout << "ptr was signed by the oracle" << endl;
        return;
    }

    cout << "calling: " << hex << func << endl;
    if (func == (uintptr_t) ctime) {
        cout << "choose time" << endl;
        time_t time;
        cin >> time;

        cout << ((char*(*)(time_t const*)) func)(&time) << endl;

    } else if (func == (uintptr_t) expf) {
        cout << "choose val" << endl;
        float val;
        cin >> val;

        cout << ((float(*)(float)) func)(val) << endl;

    } else if (func == (uintptr_t) bsearch) {
        cout << "choose arr_len" << endl;
        size_t arr_len;
        cin >> arr_len;

        cout << "choose arr" << endl;
        vector<int> arr(arr_len);
        for (size_t i = 0; i < arr_len; i++)
            cin >> arr[i];

        cout << "choose val" << endl;
        int val;
        cin >> val;

        void* res = ((void*(*)(void const*, void const*, size_t, size_t, int(*)(void const*, void const*))) func)(&val, arr.data(), arr_len, sizeof(int), cmp_int);
        if (res == NULL) {
            cout << "no res" << endl;
        } else {
            cout << "res: " << dec << *(int*)res << endl;
        }

    } else if (func == (uintptr_t) fast_pow) {
        cout << "choose a" << endl;
        size_t a;
        cin >> a;

        cout << "choose b" << endl;
        size_t b;
        cin >> b;

        cout << "res: " << ((size_t(*)(size_t, size_t)) func)(a, b) << endl;

    } else if (func == (uintptr_t) cnt_no_bits) {
        cout << "choose val" << endl;
        uint32_t val;
        cin >> val;

        cout << "no_bits: " << ((size_t(*)(uint32_t)) func)(val) << endl;

    } else if (func == (uintptr_t) access_flag) {
        ((void(*)()) func)();
        
    } else { // func == (uintptr_t) malloc
        cout << "choose sz" << endl;
        size_t sz;
        cin >> sz;

        cout << "alloc: " << hex << ((void*(*)(size_t))func)(sz) << endl;
    }
}