#pragma once

#include "pass_ctr_stamp.h"
#include "pass_time_stamp.h"

#include <cstdint>
#include <cstddef>
#include <vector>
#include <optional>
using namespace std;


struct pass_entry_t {
    // node
    size_t node_id;
    vector<byte> node_id_mac;

    // issue time
    pass_time_stamp_t issue_time;
    vector<byte> issue_time_mac;

    // yield time
    pass_time_stamp_t yield_time;
    vector<byte> yield_time_mac;

    // ctr stamp
    pass_ctr_stamp_t ctr_stamp;
    vector<byte> ctr_stamp_mac;
};

struct pass_t {
    // next ctr_stamp to use
    pass_ctr_stamp_t next_ctr_stamp = {};

    // curr entry and its expected ctr_stamp
    bool has_entry = false;
    pass_entry_t curr_entry;
    pass_ctr_stamp_t curr_ctr_stamp;

    // last realization time
    optional<pass_time_stamp_t> last_realization_time = {};
};

bool pass_create_entry(
    pass_t& pass,
    size_t node_id, pass_time_stamp_t issue_time, pass_time_stamp_t yield_time,
    byte const& pen_secret, byte const& handwrite_secret
);

bool pass_forge_entry(
    pass_t& pass,
    pass_entry_t const& forged_entry
);

bool pass_realize_entry(
    pass_t& pass,
    size_t node_id, pass_time_stamp_t curr_time,
    byte const& pen_secret, byte const& handwrite_secret
);

bool pass_cancel_entry(pass_t& pass);

void pass_print(pass_t& pass);
