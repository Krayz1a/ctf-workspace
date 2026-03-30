#include "room_addr.h"
#include "room_info.h"

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <limits>
using namespace std;


void get_uint8_t(uint8_t& val) {
    int tmp;
    cin >> dec >> tmp;
    val = tmp;
}


static size_t constexpr min_group_sz = 50;
static size_t constexpr max_group_sz = 350;

static size_t constexpr min_budget_per_person = 2000;
static size_t constexpr max_budget_per_person = 4000;


static size_t group_sz;
static map<room_addr_t, room_info_t> rooms;

void alloc_rooms() {
    vector<room_addr_builder> used_addrs;
    auto is_room_used = [&](room_addr_builder const& builder) -> bool {
        for (auto const& used_addr : used_addrs)
            if (used_addr == builder)
                return true;

        return false;
    };

    for (size_t person_id = 0; person_id < group_sz; person_id++) {
        cout << "choose type" << endl;
        int type;
        cin >> type;

        switch (type) {
            // choose room addr manually
            case 0: {
                while (true) {
                    room_addr_builder builder;

                    cout << "choose zone" << endl;
                    get_uint8_t(builder.zone);

                    cout << "choose building" << endl;
                    get_uint8_t(builder.building);

                    cout << "choose floor" << endl;
                    get_uint8_t(builder.floor);

                    cout << "choose room" << endl;
                    get_uint8_t(builder.room);

                    if (!(min_zone <= builder.zone && builder.zone <= max_zone)) throw runtime_error("invalid zone");
                    if (!(min_building <= builder.building && builder.building <= max_building)) throw runtime_error("invalid building");
                    if (!(min_floor <= builder.floor && builder.floor <= max_floor)) throw runtime_error("invalid floor");
                    if (!(min_room <= builder.room && builder.room <= max_room)) throw runtime_error("invalid room");
                    
                    if (is_room_used(builder)) {
                        cout << "room is already in used" << endl;
                        continue;
                    }

                    // add to used addrs
                    used_addrs.push_back(builder);

                    // construct the room
                    rooms.insert({room_addr_serialize(builder), room_info_t()});

                    break;
                }

                break;
            }
            // choose room addr randomaly
            case 1: {
                room_addr_builder builder;
                random_device rd;

                while (true) {
                    builder.zone = static_cast<uint8_t>(rd() & 0xff);
                    builder.building = static_cast<uint8_t>(rd() & 0xff);
                    builder.floor = static_cast<uint8_t>(rd() & 0xff);
                    builder.room = static_cast<uint8_t>(rd() & 0xff);

                    if (!(min_zone <= builder.zone && builder.zone <= max_zone)) continue;
                    if (!(min_building <= builder.building && builder.building <= max_building)) continue;
                    if (!(min_floor <= builder.floor && builder.floor <= max_floor)) continue;
                    if (!(min_room <= builder.room && builder.room <= max_room)) continue;
                    
                    if (is_room_used(builder)) continue;

                    break;
                }
            
                // add to used addrs
                used_addrs.push_back(builder);

                // construct the room
                rooms.insert({room_addr_serialize(builder), room_info_t()});

                break;
            }
            // choose room addr near another person
            case 2: {
                while (true) {
                    size_t other_person;
                    cout << "choose other_person" << endl;
                    cin >> other_person;

                    if (!(other_person < person_id)) throw runtime_error("other_person doesn't have a room yet");

                    // the other_person addr builder
                    room_addr_builder builder = used_addrs[person_id];

                    // forward or backward preference
                    bool forward;
                    {
                        string preference;
                        cout << "do you prefer being closer to the gym or to the pool" << endl;
                        cin >> preference;

                        if (preference == "gym") forward = true; // the gyms are at the end of each zone
                        else if (preference == "pool") forward = false; // the pools are at the start of each zone
                        else throw runtime_error("invalid choice");
                    }

                    if (forward) {
                        bool found = false;
                        for (; builder.building <= max_building; builder.building++) {
                            for (uint8_t floor = min_floor; floor <= max_floor; floor++) {
                                builder.floor = floor;
                                for (uint8_t room = min_room; room <= max_room; room++) {
                                    builder.room = room;

                                    if (!is_room_used(builder)) {
                                        found = true;
                                        goto found_room;
                                    }
                                }
                            }
                        }
                        found_room: {}

                        if (!found) {
                            cout << "no suitable room in this zone" << endl;
                            continue;
                        }

                        // add to used addrs
                        used_addrs.push_back(builder);

                        // construct the room
                        rooms.insert({room_addr_serialize(builder), room_info_t()});

                        break;

                    } else {
                        bool found = false;
                        for (; builder.building >= min_building; builder.building--) {
                            for (uint8_t floor = min_floor; floor <= max_floor; floor++) {
                                builder.floor = floor;
                                for (uint8_t room = min_room; room <= max_room; room++) {
                                    builder.room = room;
                                    
                                    if (!is_room_used(builder)) {
                                        found = true;
                                        goto found_room_2;
                                    }
                                }
                            }
                        }
                        found_room_2: {}

                        if (!found) {
                            cout << "no suitable room in this zone" << endl;
                            continue;
                        }

                        // add to used addrs
                        used_addrs.push_back(builder);

                        // construct the room
                        rooms.insert({room_addr_serialize(builder), room_info_t()});

                        break;
                    }
                }

                break;
            }

            default: throw runtime_error("invalid type");
        }
    }

}

void leisure() {
    for (auto& room : rooms) {
        cout << "choose leisure activities for room " << string(room.first.begin(), room.first.end()) << endl;

        cout << "choose spa_orders" << endl;
        cin >> room.second.spa_orders;

        cout << "choose food_orders" << endl;
        cin >> room.second.food_orders;

        cout << "choose minibar_refills" << endl;
        cin >> room.second.minibar_refills;

        cout << "choose movie_orders" << endl;
        cin >> room.second.movie_orders;
    }
}

void chal() {
    /* choose group size */
    cout << "choose group_sz" << endl;
    cin >> group_sz;
    if (!(min_group_sz <= group_sz && group_sz <= max_group_sz)) throw runtime_error("weird group size for vacation");
    
    /* alloc rooms for everyone */
    alloc_rooms();

    /* leisures */
    leisure();

    /* choose budget */
    cout << "choose budget_per_person" << endl;
    size_t budget_per_person;
    cin >> budget_per_person;
    if (!(min_budget_per_person <= budget_per_person && budget_per_person <= max_budget_per_person)) throw runtime_error("insufficient/improbable amount of budget per person");
    size_t total_budget = budget_per_person * group_sz;

    /* calc final payment */
    size_t total_payment = 0;
    auto add_to_payment = [&](size_t val) {
        if (SIZE_MAX - total_payment < val) throw runtime_error("payment too large");
        total_payment += val;
    };

    size_t total_spa_orders = 0;
    size_t total_food_orders = 0;
    size_t total_minibar_refills = 0;
    size_t total_movie_orders = 0;
    for (auto const& [room_addr, room_info] : rooms) {
        // payment for the room itself
        add_to_payment(room_price);

        // paynent for the spa
        add_to_payment(room_info.spa_orders * spa_price);
        total_spa_orders += room_info.spa_orders;

        // paynent for the food
        add_to_payment(room_info.food_orders * meal_price);
        total_food_orders += room_info.food_orders;

        // paynent for the minibar
        add_to_payment(room_info.minibar_refills * minibar_price);
        total_minibar_refills += room_info.minibar_refills;

        // paynent for the movies
        add_to_payment(room_info.movie_orders * movie_price);
        total_movie_orders += room_info.movie_orders;

    }

    /* validate leisure */
    // spa
    if (total_spa_orders < group_sz * 2) throw runtime_error("the leisure activities are shady");
    if (group_sz * 5 < total_spa_orders) throw runtime_error("the leisure activities are shady");

    // food orders
    if (total_food_orders < group_sz * 3) throw runtime_error("the leisure activities are shady");
    if (group_sz * 5 < total_food_orders) throw runtime_error("the leisure activities are shady");

    // minibar refills
    if (total_minibar_refills < group_sz / 2) throw runtime_error("the leisure activities are shady");
    if (group_sz * 4 < total_minibar_refills) throw runtime_error("the leisure activities are shady");

    // movie orders
    if (total_movie_orders < group_sz) throw runtime_error("the leisure activities are shady");
    if (group_sz * 2 < total_movie_orders) throw runtime_error("the leisure activities are shady");

    /* perform payment */
    cout << "total payment is " << total_payment << endl;
    cout << "your budget is " << total_budget << endl;

    if (total_payment > total_budget) throw runtime_error("you spent too much money");

    cout << "payment success, you have " << total_budget - total_payment << " money left" << endl;
    cout << "see you next vacation" << endl;
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
    } catch(exception const& e) {
        cout << "err: " << e.what() << endl;
        abort();
    }
}