#pragma once

#include <string>
#include <cstdint>
#include <cstddef>
#include <array>
using namespace std;


size_t constexpr min_zone = 0;
size_t constexpr max_zone = 3;

size_t constexpr min_building = 0;
size_t constexpr max_building = 9;

size_t constexpr min_floor = 0;
size_t constexpr max_floor = 5;

size_t constexpr min_room = 10;
size_t constexpr max_room = 60;


size_t constexpr room_addr_sz = 5;
using room_addr_t = array<char, room_addr_sz>;

struct room_addr_builder {
    uint8_t zone;
    uint8_t building;
    uint8_t floor;
    uint8_t room;

    bool operator==(room_addr_builder const& other) const {
        return
            zone == other.zone
            && building == other.building
            && floor == other.floor
            && room == other.room
        ;
    }
};

room_addr_t room_addr_serialize(room_addr_builder const& builder);

room_addr_builder room_addr_deserialize(room_addr_t const& addr);
