#pragma once
#include "Game Settings.h"

class Ant;
class Resource;
class Enemy;
class Cemetery;
class Game;

class Role {
public:
    virtual ~Role() = default;
    virtual void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) = 0;
};

class Baby : public Role {
public:
    Baby() = default;
    void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) override;
};

class Sitter : public Role {
public:
    Sitter() = default;
    void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) override;
    void emergency_response(Ant& ant, const vector<Enemy>& enemies);
    void evacuate_babies(Ant& ant);
    void care_for_babies(Ant& ant);
};

class Collector : public Role {
public:
    Collector() = default;
    void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) override;
    void seek_food(std::vector<Resource>& resources, Ant& ant);
    void deliver_food_to_anthill(Ant& ant);
};

class Builder : public Role {
public:
    Builder() = default;
    void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) override;
    void seek_sticks(std::vector<Resource>& resources, Ant& ant);
    void deliver_sticks_to_anthill(Ant& ant);
};

class Soldier : public Role {
public:
    Soldier();
    void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) override;
    void attack(Ant& ant, Enemy& enemy);
    void patrol_around_anthill(Ant& ant);
    Enemy* find_closest_enemy(Ant& ant, vector<Enemy>& enemies);
private:
    int damage;
};

class Shepperd : public Role {
public:
    Shepperd() = default;
    void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) override;
    void find_and_tend_aphids(Ant& ant, std::vector<Resource>& resources);
    void deliver_honey_to_anthill(Ant& ant);
};

class Cleaner : public Role {
public:
    Cleaner() = default;
    void work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) override;
    void seek_bodies(std::vector<Resource>& resources, Ant& ant);
    void seek_trash(std::vector<Resource>& resources, Ant& ant);
    bool is_carrying_waste(const Ant& ant) const;
    void deliver_to_cemetery(Ant& ant);
};