#pragma once
#include "Game Settings.h"
#include "Entity.h"
class Enemy :public Entity {
private:
    Position home;
    bool has_target;
    bool robbed, going_home, yet_robbed;
public:
    Enemy(float x, float y) : Entity(), has_target(false), yet_robbed(0), home(x, y), going_home(false), robbed(false) {
        shape.setRadius(ant_size);
        shape.setFillColor(Color(220, 80, 80));
        shape.setPosition(x, y);
        pos = home;
    }
    void move() override;
    bool get_robbed() const { return yet_robbed; }
};