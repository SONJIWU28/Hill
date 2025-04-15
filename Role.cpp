#include "Role.h"
#include "Ant.h"
#include "Enemy.h"
#include "Resource.h"
#include "Cemetery.h"

vector<Ant> ant;


void shock(Ant& ant) {
    ant.set_velocity(0, 0);
}

void escape_from_enemy(Ant& ant) {
    ant.set_target(Position(window_width / 2, window_height / 2));
}

bool enemy_nearby(Ant& ant, const vector<Enemy>& enemies, float detection_radius = 100.0f) {
    Position ant_pos = ant.get_pos();

    for (const auto& enemy : enemies) {
        if (!enemy.is_visible()) continue;
        Vector2f enemy_pos_vec = enemy.get_shape().getPosition();
        Position enemy_pos(enemy_pos_vec.x, enemy_pos_vec.y);
        float dx = ant_pos.x - enemy_pos.x;
        float dy = ant_pos.y - enemy_pos.y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance <= detection_radius) {
            return true;
        }
    }
    return false;
}

// ------------------- Baby (�������) -------------------

void Baby::work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies) {
    // ������� ������ �� ������, ��� ������ ����������
    // ���� ����� ������������� �������� ������
}

// ------------------- Sitter (�������) -------------------

void Sitter::work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies) {
    if (enemy_nearby(ant, enemies)) {
        emergency_response(ant, enemies);
        return;
    }
    care_for_babies(ant);
}

void Sitter::emergency_response(Ant& ant, const vector<Enemy>& enemies) {
    shock(ant);
    if (ant.has_valid_target() && ant.get_inventory() != no_res) escape_from_enemy(ant);
    else  evacuate_babies(ant);

}

void Sitter::evacuate_babies(Ant& ant) {
    // TODO: ����� ������� � ����������� �� � ���������� �����
    // � ������� ���������� ������ ������� � ����������
    escape_from_enemy(ant);
}

void Sitter::care_for_babies(Ant& ant) {
    if (!ant.has_valid_target()) {
        float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * 3.14159f;
        float distance = start_radius * 0.8f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float target_x = window_width / 2 + cos(angle) * distance;
        float target_y = window_height / 2 + sin(angle) * distance;

        // �������� ������ ������
        if (target_x < 0) target_x = 0;
        if (target_x > window_width) target_x = window_width;
        if (target_y < 0) target_y = 0;
        if (target_y > window_height) target_y = window_height;

        ant.set_target(Position(target_x, target_y));
    }
}

// ------------------- Collector (����������) -------------------

void Collector::work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies) {
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) {
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // ���� ������� �� ����� ��� - ���� ��
    if (ant.get_inventory() != food) {
        seek_food(resources, ant);
    }
    // ���� ������� ����� ��� � �� ����� ����, ���������� � ����������
    else if (!ant.has_valid_target()) {
        deliver_food_to_anthill(ant);
    }
}

void Collector::seek_food(std::vector<Resource>& resources, Ant& ant) {
    // ���� ������� ��� ���� ���-�� ��� ����� ����, �������
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // ������������� ��������� ������� � ������� ���
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != food || res.get_quantity() <= 0)
            continue;

        // ���������, ��������� �� ��� � ���� ������ �������
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // ������� ����� ��� - ��������� ����
            ant.set_target(resPos);
            break;
        }
    }
}

void Collector::deliver_food_to_anthill(Ant& ant) {
    // ���������� ������� � ������ �����������
    ant.set_target(Position(window_width / 2, window_height / 2));
}

// ------------------- Builder (���������) -------------------

void Builder::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // ��������� ������� ������ ����������
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) { // ���� 4 - ��� Soldier
        // ��������� �� ���������
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // ���� ������� �� ����� ����� - ���� ��
    if (ant.get_inventory() != stick) {
        seek_sticks(resources, ant);
    }
    // ���� ������� ����� ����� � �� ����� ����, ���������� � ����������
    else if (!ant.has_valid_target()) {
        deliver_sticks_to_anthill(ant);
    }
}

void Builder::seek_sticks(std::vector<Resource>& resources, Ant& ant) {
    // ���� ������� ��� ���� ���-�� ��� ����� ����, �������
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // ������������� ��������� ������� � ������� �����
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != stick || res.get_quantity() <= 0)
            continue;

        // ���������, ��������� �� ����� � ���� ������ �������
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // ������� ����� ����� - ��������� ����
            ant.set_target(resPos);
            break;
        }
    }
}

void Builder::deliver_sticks_to_anthill(Ant& ant) {
    // ���������� ������� � ������ �����������
    ant.set_target(Position(window_width / 2, window_height / 2));
}

// ------------------- Soldier (������) -------------------

Soldier::Soldier() {
    damage = rand() % 21 + 30;
}

void Soldier::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // ������� �� ������ ������, ��� ���� �� � �������

    // ���� � ������� ��� ���� ����, �� ���������� ��������� ��
    if (ant.has_valid_target()) {
        return;
    }

    // ��������� ������� ������
    if (!enemies.empty()) {
        // ����� ���������� �����
        Enemy* closest_enemy = find_closest_enemy(ant, enemies);
        if (closest_enemy) {
            // ������� ���������� �����
            attack(ant, *closest_enemy);
            return;
        }
    }

    // ���� ������ ���, ����������� ���������� ������ �����������
    patrol_around_anthill(ant);
}

Enemy* Soldier::find_closest_enemy(Ant& ant, vector<Enemy>& enemies) {
    Position ant_pos = ant.get_pos();
    float min_distance = 9999999.0f; // �������� std::numeric_limits �� ������� �����
    Enemy* closest_enemy = nullptr;

    for (auto& enemy : enemies) {
        if (!enemy.is_visible()) continue;

        sf::Vector2f enemy_pos_vec = enemy.get_shape().getPosition();
        Position enemy_pos(enemy_pos_vec.x, enemy_pos_vec.y);
        float dx = ant_pos.x - enemy_pos.x;
        float dy = ant_pos.y - enemy_pos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        // ���������, ��������� �� ���� � ���� ���������
        if (distance <= ant.get_vision_circle().getRadius() && distance < min_distance) {
            min_distance = distance;
            closest_enemy = &enemy;
        }
    }

    return closest_enemy;
}

void Soldier::attack(Ant& ant, Enemy& enemy) {
    // ���������� ������� � �����
    sf::Vector2f enemy_pos_vec = enemy.get_shape().getPosition();
    Position enemy_pos(enemy_pos_vec.x, enemy_pos_vec.y);
    ant.set_target(Position(enemy_pos.x, enemy_pos.y));

    // ������ �������� � �������� ������ ��� ����������� � ������ Ant::move()
}

void Soldier::patrol_around_anthill(Ant& ant) {
    // ����������� ���������� ������ �����������
    float patrol_radius = start_radius * 2.0f; // ������ ��������������
    float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * 3.14159f;

    // ��������� ������� �� ���������� ������ �����������
    float target_x = window_width / 2 + std::cos(angle) * patrol_radius;
    float target_y = window_height / 2 + std::sin(angle) * patrol_radius;

    // �������� ������ ������
    if (target_x < 0) target_x = 0;
    if (target_x > window_width) target_x = window_width;
    if (target_y < 0) target_y = 0;
    if (target_y > window_height) target_y = window_height;

    ant.set_target(Position(target_x, target_y));

    // ��������� �������� ����������� ��� � Ant.cpp
    if (rand() % velocity_changing_period == 0) {
        ant.set_velocity(randomise_velocity() * ant_speed, randomise_velocity() * ant_speed);
    }
}

// ------------------- Shepperd (������) -------------------

void Shepperd::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // ��������� ������� ������ ����������
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) { // ���� 4 - ��� Soldier
        // ��������� �� ���������
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // ���� ������� ����� ���, ���������� ��� � ����������
    if (ant.get_inventory() == food && !ant.has_valid_target()) {
        deliver_honey_to_anthill(ant);
        return;
    }

    // ���� � ������� ��� ����, ���� ��� ��� �������� � ��� ���������
    if (!ant.has_valid_target()) {
        find_and_tend_aphids(ant, resources);
    }
}

void Shepperd::find_and_tend_aphids(Ant& ant, std::vector<Resource>& resources) {
    // �������� ����� ���� �� ���
    // � ��������� ������������ ������ �������� ��� �� ���
    if (rand() % 100 < 10) { // 10% ���� �������� ���
        ant.set_inventory(food);
        deliver_honey_to_anthill(ant);
    }
    else {
        // ���� ��� �� �������, ���������� ����� ���
        // ��������� �����������
        float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * 3.14159f;
        float distance = 150.0f;

        float target_x = ant.get_pos().x + std::cos(angle) * distance;
        float target_y = ant.get_pos().y + std::sin(angle) * distance;

        // �������� ������ ������
        if (target_x < 0) target_x = 0;
        if (target_x > window_width) target_x = window_width;
        if (target_y < 0) target_y = 0;
        if (target_y > window_height) target_y = window_height;

        ant.set_target(Position(target_x, target_y));

        // ��������� ��������� �������� ��� � Ant.cpp
        if (rand() % velocity_changing_period == 0) {
            ant.set_velocity(randomise_velocity() * ant_speed, randomise_velocity() * ant_speed);

            // �������� ������ ��� ��������
            Position pos = ant.get_pos();
            Vector2f velocity(ant.get_shape().getPosition().x - pos.x, ant.get_shape().getPosition().y - pos.y);

            if (pos.x + velocity.x < 0 || pos.x + velocity.x > window_width)
                velocity.x = -velocity.x;
            if (pos.y + velocity.y < 0 || pos.y + velocity.y > window_height)
                velocity.y = -velocity.y;

            ant.set_velocity(velocity.x, velocity.y);
        }
    }
}

void Shepperd::deliver_honey_to_anthill(Ant& ant) {
    // ���������� ������� � ������ �����������
    ant.set_target(Position(window_width / 2, window_height / 2));
}

// ------------------- Cleaner (�������) -------------------

void Cleaner::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // ��������� ������� ������ ����������
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) { // ���� 4 - ��� Soldier
        // ��������� �� ���������
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // ���� ������� �� ����� ������ - ���� ����� ������
    if (ant.get_inventory() == no_res && !ant.has_valid_target()) {
        // ������ ������ ����������� � ������ look_around ������ Ant
        // ����� �� �� ������ ������ ���������������
    }
    // ���� ������� ��� ����� ������, �� � ���� ��� ����
    else if ((ant.get_inventory() == body || ant.get_inventory() == trash) && !ant.has_valid_target()) {
        deliver_to_cemetery(ant);
    }
}

void Cleaner::seek_bodies(std::vector<Resource>& resources, Ant& ant) {
    // ���� ������� ��� ���� ���-�� ��� ����� ����, �������
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // ������������� ��������� ������� � ������� ������
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != body)
            continue;
        ant.move();
        // ���������, ��������� �� ���� � ���� ������ �������
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // ������� ����� ���� - ��������� ���� � ���������
            ant.set_target(resPos);
            ant.set_inventory(body);
            res.set_invisible(); // �������� ������ ��� �����������
            break;
        }
    }
}

void Cleaner::seek_trash(std::vector<Resource>& resources, Ant& ant) {
    // ���� ������� ��� ���� ���-�� ��� ����� ����, �������
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // ������������� ��������� ������� � ������� ������
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != trash)
            continue;

        // ���������, ��������� �� ����� � ���� ������ �������
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // ������� ����� ����� - ��������� ���� � ���������
            ant.set_target(resPos);
            ant.set_inventory(trash);
            res.set_invisible(); // �������� ������ ��� �����������
            break;
        }
    }
}

bool Cleaner::is_carrying_waste(const Ant& ant) const {
    return ant.get_inventory() == body || ant.get_inventory() == trash;
}

void Cleaner::deliver_to_cemetery(Ant& ant) {
    // �������� ������� ��������
    Cemetery* cemetery = Cemetery::get_current();

    if (cemetery) {
        // ���� �������� ����������, ���������� ������� � ��� ������
        Position cemetery_center = cemetery->get_center();
        ant.set_target(cemetery_center);
    }
    else {
        // ���� �������� ���, ���������� ��������� � ����
        ant.set_inventory(no_res);
        ant.clear_target();
    }
}