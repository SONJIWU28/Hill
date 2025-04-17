#include "Ant.h"
#include "Anthill.h"
#include "Game.h"

void Ant::upd_role() {
    switch (role_id) {
    case 0: { role = roles[role_id++]; upd_color(); return; }
    case 1: {
        role_id = rand() % 2 + 2;
        role = roles[role_id];
        upd_color();
        return;
    }
    case 2:case 3: {
        if (max_hp <= 400) {
            role_id = 4;
            role = roles[role_id];
        }
        else {
            role_id = 5;
            role = roles[role_id];
        }
        upd_color();
        return;
    }
    case 4:case 5: {
        role_id = 6;
        role = roles[role_id];
        upd_color();
        return;
    }
    case 6: {
        role_id = -1;
        hp = 0;
        visible = 0;
        upd_color();
        return;
    }
    }
}
void Ant::move() {
    if (hp <= 0) return;

    if (has_valid_target()) {
        float dx = target.x - pos.x;
        float dy = target.y - pos.y;
        float dist = sqrt(dx * dx + dy * dy);

        float min_dist = 5.f;

        // Учитываем радиус ресурса, если он есть
        if (carrying_from_id != -1) {
            for (auto& res : Game::get_current()->resources) {
                if (res.get_id() == carrying_from_id) {
                    float r = res.get_shape().getRadius();
                    min_dist = std::max(5.f, r * 0.75f);
                    break;
                }
            }
        }

        if (dist > min_dist) {
            velocity.x = (dx / dist) * ant_speed;
            velocity.y = (dy / dist) * ant_speed;
        }
        else {
            pos = target;
            velocity = { 0, 0 };
            has_target = false;

            // ==== Пастух и тля ====
            if (role_id == 5 && inventory == no_res) {
                for (Aphid& aphid : Game::get_current()->aphids) {
                    float adx = aphid.get_pos().x - pos.x;
                    float ady = aphid.get_pos().y - pos.y;
                    float adist = sqrt(adx * adx + ady * ady);
                    if (adist < 5.f) {
                        int honey = aphid.produce_honey();
                        for (int i = 0; i < honey; ++i)
                            Anthill::add_food();
                        has_collected_honey = true;
                        going_home = true;
                        set_target(Position(window_width / 2, window_height / 2));
                        break;
                    }
                }
            }

            // ==== Доставка ресурсов ====
            if (inventory != no_res) {
                // 🏠 Муравейник
                if (pos.in_anthill() && role_id != 6) {
                    if (inventory == food) {
                        Anthill::add_food();
                    }
                    else if (inventory == stick) {
                        Anthill::add_stick();
                    }

                    // уменьшение количества (только еда/палка)
                    for (auto& res : Game::get_current()->resources) {
                        if (res.get_id() == carrying_from_id &&
                            (res.get_type() == food || res.get_type() == stick)) {
                            res.decrease_quantity(1);
                            break;
                        }
                    }

                    carrying_from_id = -1;
                    inventory = no_res;
                    going_home = false;
                }

                // 🪦 Кладбище (чистильщик)
                else if (role_id == 6) {
                    Cemetery* cemetery = Cemetery::get_current();
                    if (cemetery) {
                        if (inventory == body) cemetery->add_body();
                        else if (inventory == trash) cemetery->add_trash();
                    }

                    // вручную скрываем труп/мусор
                    for (auto& res : Game::get_current()->resources) {
                        if (res.get_id() == carrying_from_id) {
                            res.set_invisible(); // не через decrease_quantity!
                            break;
                        }
                    }

                    carrying_from_id = -1;
                    inventory = no_res;
                    going_home = false;
                }

                // 📦 Ещё не в anthill, нужно дойти
                else if (!pos.in_anthill()) {
                    set_target(Position(window_width / 2, window_height / 2));
                    going_home = true;
                }
            }
        }

        pos.x += velocity.x;
        pos.y += velocity.y;
        shape.setPosition(pos.x, pos.y);

    }
    else {
        // === Случайное движение (вне целей) ===
        if (role_id == 0) return;

        if (role_id != 1) {
            if (age % velocity_changing_period == 0)
                set_velocity(randomise_velocity() * ant_speed, randomise_velocity() * ant_speed);
            if (pos.x + velocity.x < 0 || pos.x + velocity.x > window_width) velocity.x = -velocity.x;
            if (pos.y + velocity.y < 0 || pos.y + velocity.y > window_height) velocity.y = -velocity.y;
        }
        else {
            if (age % velocity_changing_period == 0)
                set_velocity(randomise_velocity() * ant_speed, randomise_velocity() * ant_speed);
            if (pos.x + velocity.x < window_width / 2 - 2 * start_radius || pos.x + velocity.x > window_width / 2 + 2 * start_radius)
                velocity.x = -velocity.x;
            if (pos.y + velocity.y < window_height / 2 - 2 * start_radius || pos.y + velocity.y > window_height / 2 + 2 * start_radius)
                velocity.y = -velocity.y;
        }

        pos.x += velocity.x;
        pos.y += velocity.y;
        shape.setPosition(pos.x, pos.y);
    }
}





bool Ant::look_around(vector<Resource>& resources, vector<Aphid>& aphids) {
    if (hp <= 0 || has_target || inventory != no_res || going_home) return false;

    for (auto& res : resources) {
        if (!res.is_visible()) continue;
        float dx = res.get_posit().x - pos.x;
        float dy = res.get_posit().y - pos.y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < radius_vision) {
            if ((res.get_type() == food && role_id == 2) ||
                (res.get_type() == stick && role_id == 3) ||
                ((res.get_type() == body || res.get_type() == trash) && role_id == 6)) {
                set_inventory(res.get_type());
                set_target(res.get_posit());
                carrying_from_id = res.get_id();
                return true;
            }
        }
    }
    if (role_id == 5) {
        for (Aphid& aphid : aphids) {
            if (!aphid.is_visible()) continue;
            float dx = aphid.get_pos().x - pos.x;
            float dy = aphid.get_pos().y - pos.y;
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < radius_vision) {
                set_target(aphid.get_pos());
                return true;
            }
        }
    }

    return false;
}



CircleShape Ant::get_vision_circle() const {
    CircleShape vision(radius_vision);
    vision.setOrigin(radius_vision, radius_vision); // ÑÐµÐ½ÑÑ ÐºÑÑÐ³Ð°
    vision.setPosition(pos.x, pos.y);
    vision.setFillColor(Color(255, 255, 255, 15));  // Ð±ÐµÐ»ÑÐ¹, Ð¿Ð¾ÑÑÐ¸ Ð¿ÑÐ¾Ð·ÑÐ°ÑÐ½ÑÐ¹
    vision.setOutlineColor(Color(0, 0, 255, 20)); // Ð¿Ð¾Ð»ÑÐ¿ÑÐ¾Ð·ÑÐ°ÑÐ½ÑÐ¹ ÐºÐ¾Ð½ÑÑÑ
    vision.setOutlineThickness(1.f);
    return vision;
}

bool Ant::pick(Resource& res) {
    if (!res.is_visible()) return 0;
    if ((res.get_type() == food && role_id == 3) || (res.get_type() == stick && role_id == 2) || ((res.get_type() == body || res.get_type() == trash) && role_id == 6)) return 1;
}

void Ant::upd_color()
{
    if (hp <= 0 || already_dead) {
        shape.setFillColor(Color(134, 138, 142));
        return;
    }
    switch (role_id) {
    case 0: { shape.setFillColor(Color::White); return; }
    case 1: { shape.setFillColor(Color(255, 102, 178)); return; }
    case 2: { shape.setFillColor(Color(255, 128, 0)); return; }
    case 3: { shape.setFillColor(Color::Yellow); return; }
    case 4: { shape.setFillColor(Color::Black); return; }
    case 5: { shape.setFillColor(Color(0, 0, 204)); return; }
    case 6: { shape.setFillColor(Color(102, 51, 0)); return; }
    }
}
void Ant::dead(vector<Resource>& resources)
{
    if (already_dead) return;
    Resource res(body, small);
    res.set_posit(pos.x, pos.y);
    res.set_color(body);
    res.set_shape_size(small);
    resources.push_back(res);
    hp = 0;
    velocity.x = 0;
    velocity.y = 0;
    has_target = false;
    inventory = no_res;
    already_dead = true;
}
float randomise_velocity()
{
    int n = rand() % 3;
    switch (n) {
    case 0: return -1.0f;
    case 1: return 0.0f;
    case 2: return 1.0f;
    }
}