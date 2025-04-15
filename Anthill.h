#include "Game Settings.h"
#include "Ant.h"

class Anthill {
private:
    float rad;
    CircleShape shape;
    Position pos;
    int lvl, max_food, sticks_for_upd, max_ants;
    static int food_count, stick_count;
    int ants, babies, sitters, collectors, builders, soldiers, shepherds, cleaners;
public:
    vector<Ant> colony;
    Anthill();
    ~Anthill() = default;
    void up_lvl();
    void down_lvl();
    void feeding() { food_count -= ants; }
    void hunger();
    void born_baby();
    void upd_ant_stats();
    void upd_anthill(int ticks);
    void default_count() { ants = soldiers = builders = cleaners = sitters = babies = shepherds = collectors = 0; }
    void kill_colony() { colony.clear(); }
    static void add_food() { food_count++; }
    static void add_stick() { stick_count++; }
    static void del_food() { food_count--; }
    static void del_stick() { stick_count--; }

    const CircleShape& get_shape() const { return shape; }
    int get_soldier_count() const { return soldiers; }
    int get_builder_count() const { return builders; }
    int get_cleaner_count() const { return cleaners; }
    int get_sitter_count() const { return sitters; }
    int get_baby_count() const { return babies; }
    int get_shepherd_count() const { return shepherds; }
    int get_collector_count() const { return collectors; }
    int get_food_count() const { return food_count; }
    int get_stick_count() const { return stick_count; }
    int get_ant_count() const { return ants; }
    int get_max_ants() const { return max_ants; }
    int get_max_food() const { return max_food; }
    int get_for_upd() const { return sticks_for_upd; }
    int get_radius()const { return rad; }
};