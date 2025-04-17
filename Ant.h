#pragma once
#include "Game Settings.h"
#include "Entity.h"
#include "Role.h"
#include "Resource.h"
#include "Tlya.h"

static Role* roles[7] = { new Baby(), new Sitter(), new Collector(), new Builder(), new Soldier(), new Shepperd(), new Cleaner() };

class Ant : public Entity {
private:
    int max_hp, role_id;
    bool already_dead;
    bool has_collected_honey;
    Role* role;
    bool going_home;
    res_type inventory;
    int carrying_from_id;


public:
    Ant(float x = 0, float y = 0) :Entity(x, y), role_id(0), going_home(false), inventory(no_res), already_dead(false), has_collected_honey(false), carrying_from_id (-1) {
        max_hp = hp;
        role = roles[role_id];
        shape.setRadius(ant_size);
    };

    void move() override;
    void upd_role();
    void upd_color();
    bool look_around(vector<Resource>& resources, vector<Aphid>& aphids);
    void set_inventory(res_type type) { inventory = type; }
    bool pick(Resource& res);
    void work(vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids) {
        if (role != nullptr) {
            role->work(*this, resources, enemies, aphids);
        }
    }

    void dead(vector<Resource>& resources);
    bool is_already_dead() const { return already_dead; }
    void set_already_dead() { already_dead = true; }
    int get_role() const { return role_id; }
    res_type get_inventory() const { return inventory; }
    CircleShape get_vision_circle() const;
};

float randomise_velocity();