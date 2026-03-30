#include "park.h"
#include "visitor.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <fstream>
using namespace std;


void get_byte(byte& val) {
    int tmp;
    cin >> dec >> tmp;
    val = byte(tmp);
}

void get_uint8_t(uint8_t& val) {
    int tmp;
    cin >> dec >> tmp;
    val = tmp;
}

bool handle_opt() {
    cout << "choose opt" << endl;
    int opt;
    cin >> opt;

    switch (opt) {
        /* print curr time */
        case 0: {
            cout << "curr_time: ";
            pass_time_stamp_print(park.curr_time);
            cout << endl;

            break;
        }
        /* wait */
        case 1: {
            cout << "choose wait_amount" << endl;
            size_t wait_amount;
            cin >> wait_amount;

            if (!pass_time_stamp_advance(park.curr_time, wait_amount)) throw runtime_error("end");

            break;
        }
        /* create entry */
        case 2: {
            cout << "choose node_id" << endl;
            size_t node_id;
            cin >> node_id;

            if (!(node_id < park.no_nodes)) throw runtime_error("no such node");

            park_t::node_t const& node = park.nodes[node_id];

            // calc the yield time
            pass_time_stamp_t yield_time = park.curr_time;
            random_device rd;
            if (!pass_time_stamp_advance(yield_time, uniform_int_distribution<size_t>(25, 35)(rd))) throw runtime_error("end");
            
            // create the entry
            if (
                !pass_create_entry(
                    visitor.pass,
                    node_id, park.curr_time, yield_time,
                    park.pens_magics[node.pen_type], node.handwrite_magic
                )
            ) throw runtime_error("can't create entry");

            break;
        }
        /* forge entry */
        case 3: {
            struct pass_entry_t forged_entry;

            // node id
            cout << "choose node_id" << endl;
            cin >> forged_entry.node_id;

            cout << "choose node_id_mac_sz" << endl;
            size_t node_id_mac_sz;
            cin >> node_id_mac_sz;

            cout << "choose node_id_mac" << endl;
            forged_entry.node_id_mac = vector<byte>(node_id_mac_sz);
            for (size_t i = 0; i < node_id_mac_sz; i++)
                get_byte(forged_entry.node_id_mac[i]);

            // issue time
            cout << "choose issue_time" << endl;
            forged_entry.issue_time = get_pass_time_stamp();
            
            cout << "choose issue_time_mac_sz" << endl;
            size_t issue_time_mac_sz;
            cin >> issue_time_mac_sz;

            cout << "choose issue_time_mac" << endl;
            forged_entry.issue_time_mac = vector<byte>(issue_time_mac_sz);
            for (size_t i = 0; i < issue_time_mac_sz; i++)
                get_byte(forged_entry.issue_time_mac[i]);

            // yield time
            cout << "choose yield_time" << endl;
            forged_entry.yield_time = get_pass_time_stamp();
            
            cout << "choose yield_time_mac_sz" << endl;
            size_t yield_time_mac_sz;
            cin >> yield_time_mac_sz;

            cout << "choose yield_time_mac" << endl;
            forged_entry.yield_time_mac = vector<byte>(yield_time_mac_sz);
            for (size_t i = 0; i < yield_time_mac_sz; i++)
                get_byte(forged_entry.yield_time_mac[i]);

            // cnt stamp
            cout << "choose ctr_stamp" << endl;
            forged_entry.ctr_stamp = get_pass_ctr_stamp();
            
            cout << "choose ctr_stamp_mac_sz" << endl;
            size_t ctr_stamp_mac_sz;
            cin >> ctr_stamp_mac_sz;

            cout << "choose ctr_stamp_mac" << endl;
            forged_entry.ctr_stamp_mac = vector<byte>(ctr_stamp_mac_sz);
            for (size_t i = 0; i < ctr_stamp_mac_sz; i++)
                get_byte(forged_entry.ctr_stamp_mac[i]);

            // add the forged entry
            if (!pass_forge_entry(visitor.pass, forged_entry)) throw runtime_error("can't forge entry");

            break;
        }
        /* cancel entry */
        case 4: {
            if (visitor.no_cancels + 1 > visitor.max_no_cancels) throw runtime_error("too many entry cancels");

            if (!pass_cancel_entry(visitor.pass)) throw runtime_error("cancle err");
            ++visitor.no_cancels;

            break;
        }
        /* realize entry */
        case 5: {
            cout << "choose node_id" << endl;
            size_t node_id;
            cin >> node_id;

            if (!(node_id < park.no_nodes)) throw runtime_error("no such node");

            park_t::node_t const& node = park.nodes[node_id];

            if (pass_realize_entry(
                visitor.pass,
                node_id, park.curr_time,
                park.pens_magics[node.pen_type], node.handwrite_magic
            )) {
                cout << "realize success" << endl;
                visitor.visited_nodes.insert(node_id);
                pass_cancel_entry(visitor.pass);

            } else {
                cout << "realize failed" << endl;
                ++visitor.no_failed_realizations;

                if (visitor.no_failed_realizations > visitor.max_no_failed_realizations) throw runtime_error("too many failed realizations");
            }

            break;
        }
        /* print entry */
        case 6: {
            pass_print(visitor.pass);
            break;
        }
        /* leave */
        case 7: {
            bool seen_all = true;
            for (size_t i = 0; i < park.no_nodes; i++)
                if (!visitor.visited_nodes.count(i))
                    seen_all = false;

            if (seen_all) {
                cout << "seen all nodes, leaving" << endl;
                return true;
            }

            cout << "didn't see all nodes yet" << endl;
            break;
        }

        default: throw runtime_error("invalid opt");
    }

    return false;
}

void chal() {
    init_park();
    init_visitor();

    while (true)
        if (handle_opt())
            return;
}

void print_flag() {
    ifstream flag_file("flag.txt");
    if (!flag_file.is_open()) throw runtime_error("can't open flag file");
    else cout << flag_file.rdbuf() << endl;
}

void chal_tester() {
    chal();

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