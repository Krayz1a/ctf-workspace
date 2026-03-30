#include "visitor.h"
#include "park.h"

visitor_t visitor;
void init_visitor() {
    visitor.pass = pass_t();

    visitor.visited_nodes = {};

    visitor.max_no_failed_realizations = 5;
    visitor.no_failed_realizations = 0;

    visitor.max_no_cancels = park.no_nodes;
    visitor.no_cancels = 0;
}
