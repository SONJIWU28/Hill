#pragma once
#include "Game Settings.h"
#include "Enemy.h"

class Raid {
public:
    vector<Enemy> crowd;
    Raid() :rad(start_radius) {};
    ~Raid() = default;
    void spawn_raid();

    int get_size() const { return crowd.size(); }
private:
    float rad;
};