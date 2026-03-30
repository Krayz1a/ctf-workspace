#pragma once

#include "pass/pass.h"

#include <set>
#include <cstddef>
using namespace std;

struct visitor_t {
    pass_t pass;

    set<size_t> visited_nodes;

    size_t max_no_failed_realizations;
    size_t no_failed_realizations;

    size_t max_no_cancels;
    size_t no_cancels;
};

extern visitor_t visitor;
void init_visitor();
