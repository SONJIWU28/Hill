#pragma once
#include "Game Settings.h"
#include "Position.h"
#include "Role.h"
#include "Resource.h"
static Role* roles[7] = { new Baby(), new Sitter(), new Collector(), new Builder(), new Soldier(), new Shepperd(), new Cleaner() };

class Ant {
private:
    int hp, age, max_hp;
    bool already_dead;
    Position pos, target;
    Vector2f velocity;
    Role* role;
    int role_id;
    CircleShape shape;
    bool has_target;
    bool going_home;
    res_type inventory;
    bool visible;
    float radius_vision;
public:
    Ant(float x = 0, float y = 0) :pos(x, y), role_id(0), age(0), hp(1), velocity(0, 0), target(0, 0), has_target(false), going_home(false), inventory(no_res), radius_vision(80.f), already_dead(false) {
        max_hp = rand() % 201 + 300;
        role = roles[role_id];
        hp = max_hp;
        shape.setRadius(ant_size);
        shape.setFillColor(Color::White);
        shape.setPosition(x, y);
        visible = true;
    }
    //Position
    bool check_pos() { if (pos.in_anthill()) return 1; return 0; }
    void move();
    void set_velocity(float vx, float vy) { velocity.x = vx; velocity.y = vy; }

    //Updating textures & role
    void upd_role();
    void upd_color();
    void set_invisible() { visible = 0; }
    void dead(vector<Resource>& resources);

    //Setting
    void set_hp(int x) { if (hp > 0) hp = x;  else hp = 0; }
    void set_inventory(res_type type) { inventory = type; }
    void up() { age++; }

    //Should be used
    bool pick(Resource& res);
    void work(std::vector<Resource>& resources, vector<Enemy>& enemies) {
        if (role != nullptr) {
            role->work(*this, resources, enemies);
        }
    }

    //Target
    void set_target(const Position& p) { target = p; has_target = true; }
    void look_around(vector<Resource>& resources);
    void clear_target() { has_target = false; }
    bool has_valid_target() const { return has_target; }

    int get_hp() const { return hp; }
    int get_age() const { return age; }
    bool is_already_dead() const { return already_dead; }
    void set_already_dead() { already_dead = true; }
    int get_role() const { return role_id; }
    bool is_visible() const { return visible; }
    Position get_pos() const { return pos; }
    res_type get_inventory() const { return inventory; }
    const CircleShape& get_shape() const { return shape; }
    CircleShape get_vision_circle() const;
};

float randomise_velocity();