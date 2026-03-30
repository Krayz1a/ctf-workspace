#include "user_ctx.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


void get_byte(byte& val) {
    int tmp;
    cin >> dec >> tmp;
    val = byte(tmp);
}

void print_byte(byte val) {
    int tmp = (int) val;
    cout << dec << tmp;
}

shared_ptr<user_ctx> global_ctx;

void init() {
    // init global ctx
    cout << "choose pass sz" << endl;
    pass_sz_t pass_sz;
    cin >> pass_sz;

    cout << "choose arr sz" << endl;
    arr_sz_t arr_sz;
    cin >> arr_sz;

    global_ctx = make_shared<user_ctx>(pass_sz, arr_sz);

    // init no_failed_attempts
    global_ctx->max_no_failed_attempts = 3;
    global_ctx->no_failed_attempts = 0;

    // init pass leaks
    global_ctx->max_no_pass_leaks = global_ctx->pass_sz - 1;
    global_ctx->no_pass_leaks = 0;
}

bool handle_opt() {
    cout << "choose opt" << endl;
    int opt;
    cin >> opt;

    switch (opt) {
        /* populate the arr */
        case 0: {
            cout << "choose arr" << endl;
            vector<byte> arr(global_ctx->arr_sz);
            for (arr_sz_t i = 0; i < arr.size(); i++)
                get_byte(arr[i]);

            global_ctx->set_arr(arr);

            break;
        }
        /* populate a single val */
        case 1: {
            cout << "choose val" << endl;
            byte val;
            get_byte(val);

            cout << "choose idx" << endl;
            arr_sz_t idx;
            cin >> idx;

            global_ctx->set_val(val, idx);

            break;
        }
        /* leak pass */
        case 2: {
            cout << "choose idx" << endl;
            pass_sz_t idx;
            cin >> idx;

            global_ctx->leak_pass(idx);

            break;
        }
        /* query arr */
        case 3: {
            cout << "choose val" << endl;
            byte val;
            get_byte(val);

            global_ctx->query_arr(val);

            cout << "the lower bound: ";
            print_byte(global_ctx->arr_query_status());
            cout << endl;

            break;
        }
        /* feed pass */
        case 4: {
            cout << "choose pass" << endl;
            vector<byte> pass(global_ctx->pass_sz);
            for (pass_sz_t i = 0; i < pass.size(); i++)
                get_byte(pass[i]);

            global_ctx->feed_pass(pass);

            break;
        }
        /* attempt to authorize */
        case 5: {
            if (global_ctx->try_authorize()) {
                cout << "authorize succecss" << endl;
                return true;
            } else {
                cout << "authorize failed" << endl;
            }

            break;
        }
        default: throw runtime_error("invalid opt");
    }

    return false;
}

void chal() {
    init();

    while (true)
        if (handle_opt())
            return;
}

void print_flag() {
    ifstream flag_file("flag.txt");
    if (!flag_file.is_open()) throw runtime_error("can't open flag file");
    else cout << flag_file.rdbuf() << endl;
}

size_t constexpr no_tests = 10;
void chal_tester() {
    for (size_t i = 0; i < no_tests; i++) {
        cout << "starting challenge " << i << endl;
        chal();
    }

    cout << "done" << endl;

    print_flag();
}

int main() {
    try {
        chal_tester();
    } catch (exception const& e) {
        cout << "err: " << e.what() << endl;
        abort();
    }
}
