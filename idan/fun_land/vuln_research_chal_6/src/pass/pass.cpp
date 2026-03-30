#include "pass.h"

#include "hash.h"

#include <iostream>
#include <random>
using namespace std;


static vector<byte> mac_calc_node_id(
    size_t node_id,
    byte const& pen_secret, byte const& handwrite_secret
) {
    vector<byte> mac;

    for (size_t i = 0; i < sizeof(node_id); i++) {
        mac.push_back(
            hash_calc({byte(
                ((uint8_t) ((byte const*) &node_id)[i] ^ (uint8_t) pen_secret ^ (uint8_t) handwrite_secret)
            )})[0]
        );
    }

    return mac;
}

static bool mac_validate_node_id(
    size_t node_id,
    vector<byte> const& mac,
    byte const& pen_secret, byte const& handwrite_secret
) {
    auto other_mac = mac_calc_node_id(node_id, pen_secret, handwrite_secret);

    if (other_mac.size() != mac.size()) return false;

    for (size_t i = 0; i < other_mac.size(); i++)
        if (other_mac[i] != mac[i]) return false;

    return true;
}

static vector<byte> mac_calc_time(
    pass_time_stamp_t time,
    byte const& pen_secret
) {
    vector<byte> mac;

    mac.push_back(
        hash_calc({byte(time.mins_0 ^ (uint8_t) pen_secret)})[0]
    );
    mac.push_back(
        hash_calc({byte(time.mins_1 ^ (uint8_t) pen_secret)})[0]
    );
    mac.push_back(
        hash_calc({byte(time.hours_0 ^ (uint8_t) pen_secret)})[0]
    );
    mac.push_back(
        hash_calc({byte(time.hours_1 ^ (uint8_t) pen_secret)})[0]
    );

    return mac;
}

static bool mac_validate_time(
    pass_time_stamp_t time,
    vector<byte> const& mac,
    byte const& pen_secret
) {
    auto other_mac = mac_calc_time(time, pen_secret);

    if (other_mac.size() != mac.size()) return false;

    for (size_t i = 0; i < other_mac.size(); i++)
        if (other_mac[i] != mac[i]) return false;

    return true;
}

static vector<byte> mac_calc_ctr_stamp(
    pass_ctr_stamp_t ctr_stamp,
    byte const& pen_secret
) {
    vector<byte> mac;
    for (size_t i = 0; i < pass_ctr_stamp_no_digits; i++) {
        mac.push_back(
            hash_calc({byte(
                ctr_stamp.digits[i] ^ (uint8_t) pen_secret)
            })[0]
        );
    }

    return mac;
}

static bool mac_validate_ctr_stamp(
    pass_ctr_stamp_t ctr_stamp,
    vector<byte> const& mac,
    byte const& pen_secret
) {
    auto other_mac = mac_calc_ctr_stamp(ctr_stamp, pen_secret);

    if (other_mac.size() != mac.size()) return false;

    for (size_t i = 0; i < other_mac.size(); i++)
        if (other_mac[i] != mac[i]) return false;

    return true;
}


bool pass_create_entry(
    pass_t& pass,
    size_t node_id, pass_time_stamp_t issue_time, pass_time_stamp_t yield_time,
    byte const& pen_secret, byte const& handwrite_secret
) {
    if (pass.has_entry) return false;

    pass_entry_t entry;

    // node id
    entry.node_id = node_id;
    entry.node_id_mac = mac_calc_node_id(node_id, pen_secret, handwrite_secret);

    // issue time
    entry.issue_time = issue_time;
    entry.issue_time_mac = mac_calc_time(issue_time, pen_secret);

    // yield time
    entry.yield_time = yield_time;
    entry.yield_time_mac = mac_calc_time(yield_time, pen_secret);

    // ctr stamp
    if (pass_ctr_stamp_is_max(pass.next_ctr_stamp)) throw runtime_error("can't create more entries");
    entry.ctr_stamp = pass.next_ctr_stamp;
    pass_ctr_stamp_increment(pass.next_ctr_stamp);
    entry.ctr_stamp_mac = mac_calc_ctr_stamp(entry.ctr_stamp, pen_secret);


    pass.has_entry = true;
    pass.curr_entry = entry;
    pass.curr_ctr_stamp = entry.ctr_stamp;

    return true;
}

bool pass_forge_entry(
    pass_t& pass,
    pass_entry_t const& forged_entry
) {
    if (pass.has_entry) return false;

    pass.has_entry = true;
    pass.curr_entry = forged_entry;

    return true;
}

bool pass_realize_entry(
    pass_t& pass,
    size_t node_id, pass_time_stamp_t curr_time,
    byte const& pen_secret, byte const& handwrite_secret
) {
    if (!pass.has_entry) return false;

    pass_entry_t const& entry = pass.curr_entry;

    // validate node id
    if (entry.node_id != node_id) {
        cout << "entry.node_id != node_id" << endl;
        return false;
    }
    if (!mac_validate_node_id(entry.node_id, entry.node_id_mac, pen_secret, handwrite_secret)) {
        cout << "!mac_validate_node_id(entry.node_id, entry.node_id_mac, pen_secret, handwrite_secret)" << endl;
        return false;
    }

    // validate issue time
    if (!mac_validate_time(entry.issue_time, entry.issue_time_mac, pen_secret)) {
        cout << "!mac_validate_time(entry.issue_time, entry.issue_time_mac, pen_secret)" << endl;
        return false;
    }

    // validate yield time
    if (!mac_validate_time(entry.yield_time, entry.yield_time_mac, pen_secret)) {
        cout << "!mac_validate_time(entry.yield_time, entry.yield_time_mac, pen_secret)" << endl;
        return false;
    }

    // validate ctr stamp
    if (!pass_ctr_stamp_is_same(entry.ctr_stamp, pass.curr_ctr_stamp)) {
        cout << "!pass_ctr_stamp_is_same(entry.ctr_stamp, pass.curr_ctr_stamp)" << endl;
        cout << "entry.ctr_stamp: ";
        pass_ctr_stamp_print(entry.ctr_stamp);
        cout << "pass.curr_ctr_stamp: ";
        pass_ctr_stamp_print(pass.curr_ctr_stamp);
        cout << endl;
        return false;
    }
    if (!mac_validate_ctr_stamp(entry.ctr_stamp, entry.ctr_stamp_mac, pen_secret)) {
        cout << "!mac_validate_ctr_stamp(entry.ctr_stamp, entry.ctr_stamp_mac, pen_secret)" << endl;
        return false;
    }


    // validate reasonable difference between the issue and yield times
    if (!(pass_time_stamp_less(entry.issue_time, entry.yield_time))) {
        cout << "!(pass_time_stamp_less(entry.issue_time, entry.yield_time))" << endl;
        return false;
    }
    if (auto diff = pass_time_stamp_diff(entry.issue_time, entry.yield_time); !(25 <= diff && diff <= 35)) {
        cout << "auto diff = pass_time_stamp_diff(entry.issue_time, entry.yield_time); !(25 <= diff && diff <= 35)" << endl;
        return false;
    }

    // validate yield time has reached
    if (pass_time_stamp_less(curr_time, entry.yield_time)) {
        cout << "pass_time_stamp_less(curr_time, entry.yield_time)" << endl;
        return false;
    }

    // validate yield time is not brefore the last realization time
    if (pass.last_realization_time.has_value() && pass_time_stamp_less(entry.yield_time, pass.last_realization_time.value())) {
        cout << "pass.last_realization_time.has_value() && pass_time_stamp_less(entry.yield_time, pass.last_realization_time.value())" << endl;
        cout << "entry.yield_time: ";
        pass_time_stamp_print(entry.yield_time);
        cout << "pass.last_realization_time.value(): ";
        pass_time_stamp_print(pass.last_realization_time.value());
        cout << endl;
        return false;
    }


    pass.has_entry = false;
    pass.last_realization_time = entry.yield_time;

    return true;
}

bool pass_cancel_entry(pass_t& pass) {
    if (!pass.has_entry) return false;

    pass.has_entry = false;

    return true;
}

void pass_print(pass_t& pass) {
    // next ctr stamp
    cout << "next_ctr_stamp: ";
    pass_ctr_stamp_print(pass.next_ctr_stamp);
    cout << endl;
    
    // last realization time
    if (pass.last_realization_time.has_value()) {
        cout << "last_realization_time: ";
        pass_time_stamp_print(pass.last_realization_time.value());
        cout << endl;
    } else cout << "no last_realization_time" << endl;

    if (pass.has_entry) {
        // curr ctr stamp
        cout << "curr_ctr_stamp: ";
        pass_ctr_stamp_print(pass.curr_ctr_stamp);
        cout << endl;

        // curr entry
        pass_entry_t const& entry = pass.curr_entry;

        cout << "curr_entry:" << endl;

        cout << "node_id: " << entry.node_id << endl;
        cout << "node_id_mac: ";
        hash_print(entry.node_id_mac);
        cout << endl;

        cout << "issue_time: ";
        pass_time_stamp_print(entry.issue_time);
        cout << endl;
        cout << "issue_time_mac: ";
        hash_print(entry.issue_time_mac);
        cout << endl;

        cout << "yield_time: ";
        pass_time_stamp_print(entry.yield_time);
        cout << endl;
        cout << "yield_time_mac: ";
        hash_print(entry.yield_time_mac);
        cout << endl;

        cout << "ctr_stamp: ";
        pass_ctr_stamp_print(entry.ctr_stamp);
        cout << endl;
        cout << "ctr_stamp_mac: ";
        hash_print(entry.ctr_stamp_mac);
        cout << endl;

    } else {
        cout << "no curr entry in the pass" << endl;
    }

}