#include "room_addr.h"

#include <cstring>
using namespace std;

room_addr_t room_addr_serialize(room_addr_builder const& builder) {
    string str;
    str += to_string((int) builder.zone);
    str += to_string((int) builder.building);
    str += to_string((int) builder.floor);
    str += to_string((int) builder.room);

    room_addr_t res;
    memcpy(res.data(), str.data(), room_addr_sz);
    return res;
}

room_addr_builder room_addr_deserialize(room_addr_t const& addr) {
    room_addr_builder builder;

    builder.zone = stoi(string{addr[0]});
    builder.building = stoi(string{addr[1]});
    builder.floor = stoi(string{addr[2]});
    builder.room = stoi(string{addr[3], addr[4]});

    return builder;
}
