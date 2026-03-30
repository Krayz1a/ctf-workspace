#pragma once

#include <cstdint>
#include <cstddef>
using namespace std;


size_t constexpr room_price = 3200;
size_t constexpr spa_price = 200;
size_t constexpr meal_price = 100;
size_t constexpr minibar_price = 50;
size_t constexpr movie_price = 80;


struct room_info_t {
    size_t spa_orders = 0;
    size_t food_orders = 0;
    size_t minibar_refills = 0;
    size_t movie_orders = 0;
};
