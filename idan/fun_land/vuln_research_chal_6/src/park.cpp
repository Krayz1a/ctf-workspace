#include "park.h"

#include <random>
#include <iostream>
using namespace std;

park_t park;
void init_park() {
    random_device rd;

    // pens
    park.no_pen_types = uniform_int_distribution<size_t>(park_t::min_no_pen_types,park_t::max_no_pen_types)(rd);
    cout << "no_pen_types: " << park.no_pen_types << endl;

    for (size_t i = 0; i < park.no_pen_types; i++)
        park.pens_magics.push_back(static_cast<byte>(rd() & 0xff));

    // nodes
    park.no_nodes = uniform_int_distribution<size_t>(park_t::min_no_nodes, park_t::max_no_nodes)(rd);
    cout << "no_nodes: " << park.no_nodes << endl;

    for (size_t i = 0; i < park.no_nodes; i++) {
        park_t::node_t node;

        node.pen_type = uniform_int_distribution<size_t>(0, park.no_pen_types - 1)(rd);
        cout << "node[" << i << "].pen_type: " << node.pen_type << endl;

        node.handwrite_magic = static_cast<byte>(rd() & 0xff);

        park.nodes.push_back(node);
    }

    park.curr_time = {};
}