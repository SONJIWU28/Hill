#include "Anthill.h"
int Anthill::food_count = 0;
int Anthill::stick_count = 0;

Anthill::Anthill() :lvl(1), rad(start_radius) {
    shape.setRadius(rad);
    pos.x = window_width / 2 - start_radius;
    pos.y = window_height / 2 - start_radius;
    shape.setPosition(Vector2f(pos.x, pos.y));
    food_count = start_food_limit / 2;
    stick_count = (int)(start_stick_for_update * 0.75);
    default_count();
    max_ants = start_max_ant_count;
    sticks_for_upd = start_stick_for_update;
    max_food = start_food_limit;
    shape.setFillColor(Color(115, 66, 34));
}

void Anthill::up_lvl() {
    lvl++;
    stick_count /= 2;
    max_ants *= 1.5;
    max_food *= 1.5;
    sticks_for_upd *= 1.5;
    rad *= 1.2;
    shape.setRadius(rad);
    pos.x = window_width / 2 - rad;
    pos.y = window_height / 2 - rad;
    shape.setPosition(Vector2f(pos.x, pos.y));
}

void Anthill::down_lvl() {
    lvl--;
    rad *= 0.8;
    max_ants = ants;
    max_food *= 0.5;
    sticks_for_upd *= 0.5;
    if (food_count > max_food) food_count = max_food;
    if (stick_count > sticks_for_upd) stick_count = sticks_for_upd;
    shape.setRadius(rad);
    pos.x = window_width / 2 - rad;
    pos.y = window_height / 2 - rad;
    shape.setPosition(Vector2f(pos.x, pos.y));
}

void Anthill::born_baby() {
    int x, y;
    if (ants < max_ants) {
        do {
            x = rand() % window_width;
            y = rand() % window_height;
        } while ((x < window_width / 2 - start_radius / 2) || (x > window_width / 2 + start_radius / 2) ||
            (y < window_height / 2 - start_radius / 2) || (y > window_height / 2 + start_radius / 2));
        colony.emplace_back(x, y);
    }
}

void Anthill::upd_ant_stats()
{
    default_count();
    for (auto& ant : colony) {
        ants++;
        switch (ant.get_role()) {
        case 0:babies++; break;
        case 1:sitters++; break;
        case 2:collectors++; break;
        case 3:builders++; break;
        case 4:soldiers++; break;
        case 5:shepherds++; break;
        case 6:cleaners++; break;
        }
    }
}

void Anthill::upd_anthill(int ticks)
{
    upd_ant_stats();
    //if (ticks % feeding_period == 0) feeding();
    if (food_count > food_for_born && ticks % min_born_period == 0) {
        born_baby();
        food_count -= food_for_born;
    }
    if (stick_count == sticks_for_upd) up_lvl();
    else if (stick_count <= 0.25 * sticks_for_upd && ticks % min_downgrade_period == 0) down_lvl();
}

void Anthill::hunger() {
    int ant_count = colony.size();
    if (food_count < ant_count) {
        for (auto& ant : colony) {
            int new_hp = ant.get_hp() - hungry_damage;
            ant.set_hp(new_hp);
        }
    }
    else food_count -= ant_count;
}