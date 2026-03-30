#include "auth_tag.h"
#include "sign_oracle.h"
#include "entries.h"
#include "registered_funcs.h"

#include <cmath>
#include <set>
#include <iostream>
#include <fstream>
using namespace std;


bool handle_opt() {
    cout << "choose opt" << endl;
    int opt;
    cin >> opt;

    switch (opt) {
        /* sign oracle */
        case 0: {
            cout << "choose ptr_raw" << endl;
            uintptr_t ptr_raw;
            cin >> ptr_raw;

            uintptr_t ptr_tagged = signing_oracle_sign(ptr_raw);
            cout << "ptr_tagged: " << hex << ptr_tagged << endl;

            break;
        }
        /* entries_set_from_registered */
        case 1: {
            cout << "choose idx" << endl;
            size_t idx;
            cin >> idx;

            cout << "choose ptr_raw" << endl;
            uintptr_t ptr_raw;
            cin >> ptr_raw;

            entries_set_from_registered(idx, ptr_raw);

            break;
        }
        /* entries_set_custom */
        case 2: {
            cout << "choose idx" << endl;
            size_t idx;
            cin >> idx;

            cout << "choose ptr_raw" << endl;
            uintptr_t ptr_raw;
            cin >> ptr_raw;

            cout << "choose ptr_tagged" << endl;
            uintptr_t ptr_tagged;
            cin >> ptr_tagged;

            entries_set_custom(idx, ptr_raw, ptr_tagged);

            break;
        }
        /* entries_exec */
        case 3: {
            cout << "choose idx" << endl;
            size_t idx;
            cin >> idx;

            entries_exec(idx);

            break;
        }
        
        default: throw runtime_error("invalid opt");
    }

    return false;
}

void chal() {
    auth_tag_init();
    signing_oracle_init();
    entries_init();
    registered_funcs_print();

    while (true)
        if (handle_opt())
            return;
}

int main() {
    try {
        chal();
    } catch (exception const& e) {
        cout << "err: " << e.what() << endl;
        abort();
    }
}