#pragma once
#include "Game Settings.h"
#include "Cemetery.h"
#include "Tlya.h"

class Ant;
class Enemy;
class Resource;

class Role {
public:
    virtual ~Role() = default;
    virtual void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids) = 0;
};

class Baby : public Role {
public:
    Baby() = default;
    void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids)  override {};
};

class Sitter : public Role {
public:
    Sitter() = default;
    void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids)  override {};
};

class Collector : public Role {
public:
    Collector() = default;
    void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids)  override;
};

class Builder : public Role {
public:
    Builder() = default;
    void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids)  override;
};

class Soldier : public Role {
public:
    Soldier() = default;
    void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids)  override {};
    /*void attack(Ant& ant);
    void escape_from_enemy(Ant& ant);
    void set_on_enemy(Ant& ant);*/
    //private:
    //    int damage;
};

class Shepperd : public Role {
public:
    Shepperd() = default;
    void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids)  override ;
};

class Cleaner : public Role {
public:
    Cleaner() = default;
    void work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies, vector<Aphid>& aphids)  override;
    //void seek_bodies(vector<Resource>& resources, Ant& ant);
};