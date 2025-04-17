#pragma once
#include "Game Settings.h"
#include "Position.h"

class Entity {
protected:
    int hp, age;
    Position pos, target;
    Vector2f velocity;
    CircleShape shape;
    bool has_target, visible;
    float radius_vision;
public:
    Entity(float x = 0, float y = 0, float radius = ant_size, Color color = Color::White) : pos(x, y), velocity(0, 0), age(0), has_target(false), radius_vision(80.f), target(0, 0), visible(true)
    {
        hp = rand() % 201 + 300;
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setPosition(pos.x, pos.y);
        shape.setPosition(x, y);
    }

    virtual ~Entity() = default;
    virtual void move() = 0;
    void set_velocity(float vx, float vy);
    void set_hp(int x) { if (hp > 0) hp = x;  else hp = 0; }
    void set_target(const Position& p) { target = p; has_target = true; }
    void clear_target() { has_target = false; }
    void up() { age++; }
    void set_invisible() { visible = 0; }

    bool is_visible() const { return visible; }
    bool check_pos() { if (pos.in_anthill()) return 1; return 0; }
    bool has_valid_target() const { return has_target; }
    const CircleShape& get_shape() const { return shape; }
    Vector2f get_velocity() const { return velocity; }
    Position get_pos() const { return pos; }
    int get_hp() const { return hp; }
    int get_age() { return age; }
};