#pragma once

#include "pass/pass_time_stamp.h"

#include <cstdint>
#include <cstddef>
#include <vector>
using namespace std;

struct park_t {
    // pens
    static size_t constexpr min_no_pen_types = 2;
    static size_t constexpr max_no_pen_types = 4;

    size_t no_pen_types;
    vector<byte> pens_magics;

    // nodes
    static size_t constexpr min_no_nodes = 60;
    static size_t constexpr max_no_nodes = 80;

    size_t no_nodes;
    struct node_t {
        size_t pen_type;
        byte handwrite_magic;
    };
    vector<node_t> nodes;

    pass_time_stamp_t curr_time;
};

extern park_t park;
void init_park();
