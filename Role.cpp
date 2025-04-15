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

// ------------------- Baby (Личинка) -------------------

void Baby::work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies) {
    // Личинка ничего не делает, она просто существует
    // Этот метод преднамеренно оставлен пустым
}

// ------------------- Sitter (Сиделка) -------------------

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
    // TODO: Найти личинок и переместить их в безопасное место
    // В текущей реализации просто убегаем в муравейник
    escape_from_enemy(ant);
}

void Sitter::care_for_babies(Ant& ant) {
    if (!ant.has_valid_target()) {
        float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * 3.14159f;
        float distance = start_radius * 0.8f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float target_x = window_width / 2 + cos(angle) * distance;
        float target_y = window_height / 2 + sin(angle) * distance;

        // Контроль границ экрана
        if (target_x < 0) target_x = 0;
        if (target_x > window_width) target_x = window_width;
        if (target_y < 0) target_y = 0;
        if (target_y > window_height) target_y = window_height;

        ant.set_target(Position(target_x, target_y));
    }
}

// ------------------- Collector (Собиратель) -------------------

void Collector::work(Ant& ant, vector<Resource>& resources, vector<Enemy>& enemies) {
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) {
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // Если муравей не несет еду - ищем ее
    if (ant.get_inventory() != food) {
        seek_food(resources, ant);
    }
    // Если муравей несет еду и не имеет цели, направляем в муравейник
    else if (!ant.has_valid_target()) {
        deliver_food_to_anthill(ant);
    }
}

void Collector::seek_food(std::vector<Resource>& resources, Ant& ant) {
    // Если муравей уже несёт что-то или имеет цель, выходим
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // Просматриваем доступные ресурсы в поисках еды
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != food || res.get_quantity() <= 0)
            continue;

        // Проверяем, находится ли еда в поле зрения муравья
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // Муравей нашёл еду - установим цель
            ant.set_target(resPos);
            break;
        }
    }
}

void Collector::deliver_food_to_anthill(Ant& ant) {
    // Направляем муравья к центру муравейника
    ant.set_target(Position(window_width / 2, window_height / 2));
}

// ------------------- Builder (Строитель) -------------------

void Builder::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // Проверяем наличие врагов поблизости
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) { // Роль 4 - это Soldier
        // Реагируем на опасность
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // Если муравей не несет палки - ищем их
    if (ant.get_inventory() != stick) {
        seek_sticks(resources, ant);
    }
    // Если муравей несет палки и не имеет цели, направляем в муравейник
    else if (!ant.has_valid_target()) {
        deliver_sticks_to_anthill(ant);
    }
}

void Builder::seek_sticks(std::vector<Resource>& resources, Ant& ant) {
    // Если муравей уже несёт что-то или имеет цель, выходим
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // Просматриваем доступные ресурсы в поисках палок
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != stick || res.get_quantity() <= 0)
            continue;

        // Проверяем, находится ли палка в поле зрения муравья
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // Муравей нашёл палку - установим цель
            ant.set_target(resPos);
            break;
        }
    }
}

void Builder::deliver_sticks_to_anthill(Ant& ant) {
    // Направляем муравья к центру муравейника
    ant.set_target(Position(window_width / 2, window_height / 2));
}

// ------------------- Soldier (Солдат) -------------------

Soldier::Soldier() {
    damage = rand() % 21 + 30;
}

void Soldier::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // Солдаты не боятся врагов, они ищут их и атакуют

    // Если у солдата уже есть цель, мы продолжаем следовать ей
    if (ant.has_valid_target()) {
        return;
    }

    // Проверяем наличие врагов
    if (!enemies.empty()) {
        // Найти ближайшего врага
        Enemy* closest_enemy = find_closest_enemy(ant, enemies);
        if (closest_enemy) {
            // Атакуем найденного врага
            attack(ant, *closest_enemy);
            return;
        }
    }

    // Если врагов нет, патрулируем территорию вокруг муравейника
    patrol_around_anthill(ant);
}

Enemy* Soldier::find_closest_enemy(Ant& ant, vector<Enemy>& enemies) {
    Position ant_pos = ant.get_pos();
    float min_distance = 9999999.0f; // Заменяем std::numeric_limits на большое число
    Enemy* closest_enemy = nullptr;

    for (auto& enemy : enemies) {
        if (!enemy.is_visible()) continue;

        sf::Vector2f enemy_pos_vec = enemy.get_shape().getPosition();
        Position enemy_pos(enemy_pos_vec.x, enemy_pos_vec.y);
        float dx = ant_pos.x - enemy_pos.x;
        float dy = ant_pos.y - enemy_pos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        // Проверяем, находится ли враг в зоне видимости
        if (distance <= ant.get_vision_circle().getRadius() && distance < min_distance) {
            min_distance = distance;
            closest_enemy = &enemy;
        }
    }

    return closest_enemy;
}

void Soldier::attack(Ant& ant, Enemy& enemy) {
    // Направляем солдата к врагу
    sf::Vector2f enemy_pos_vec = enemy.get_shape().getPosition();
    Position enemy_pos(enemy_pos_vec.x, enemy_pos_vec.y);
    ant.set_target(Position(enemy_pos.x, enemy_pos.y));

    // Логика движения и проверка границ уже реализована в методе Ant::move()
}

void Soldier::patrol_around_anthill(Ant& ant) {
    // Патрулируем территорию вокруг муравейника
    float patrol_radius = start_radius * 2.0f; // Радиус патрулирования
    float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * 3.14159f;

    // Вычисляем позицию на окружности вокруг муравейника
    float target_x = window_width / 2 + std::cos(angle) * patrol_radius;
    float target_y = window_height / 2 + std::sin(angle) * patrol_radius;

    // Контроль границ экрана
    if (target_x < 0) target_x = 0;
    if (target_x > window_width) target_x = window_width;
    if (target_y < 0) target_y = 0;
    if (target_y > window_height) target_y = window_height;

    ant.set_target(Position(target_x, target_y));

    // Случайная скорость перемещения как в Ant.cpp
    if (rand() % velocity_changing_period == 0) {
        ant.set_velocity(randomise_velocity() * ant_speed, randomise_velocity() * ant_speed);
    }
}

// ------------------- Shepperd (Пастух) -------------------

void Shepperd::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // Проверяем наличие врагов поблизости
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) { // Роль 4 - это Soldier
        // Реагируем на опасность
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // Если муравей несет мед, доставляем его в муравейник
    if (ant.get_inventory() == food && !ant.has_valid_target()) {
        deliver_honey_to_anthill(ant);
        return;
    }

    // Если у муравья нет цели, ищем тлю или работаем с уже найденной
    if (!ant.has_valid_target()) {
        find_and_tend_aphids(ant, resources);
    }
}

void Shepperd::find_and_tend_aphids(Ant& ant, std::vector<Resource>& resources) {
    // Имитация сбора меда от тли
    // С некоторой вероятностью пастух получает мед от тли
    if (rand() % 100 < 10) { // 10% шанс получить мед
        ant.set_inventory(food);
        deliver_honey_to_anthill(ant);
    }
    else {
        // Если мед не получен, продолжаем поиск тли
        // Случайное перемещение
        float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * 3.14159f;
        float distance = 150.0f;

        float target_x = ant.get_pos().x + std::cos(angle) * distance;
        float target_y = ant.get_pos().y + std::sin(angle) * distance;

        // Контроль границ экрана
        if (target_x < 0) target_x = 0;
        if (target_x > window_width) target_x = window_width;
        if (target_y < 0) target_y = 0;
        if (target_y > window_height) target_y = window_height;

        ant.set_target(Position(target_x, target_y));

        // Добавляем случайную скорость как в Ant.cpp
        if (rand() % velocity_changing_period == 0) {
            ant.set_velocity(randomise_velocity() * ant_speed, randomise_velocity() * ant_speed);

            // Проверка границ для скорости
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
    // Направляем муравья к центру муравейника
    ant.set_target(Position(window_width / 2, window_height / 2));
}

// ------------------- Cleaner (Уборщик) -------------------

void Cleaner::work(Ant& ant, std::vector<Resource>& resources, vector<Enemy>& enemies) {
    // Проверяем наличие врагов поблизости
    if (enemy_nearby(ant, enemies) && ant.get_role() != 4) { // Роль 4 - это Soldier
        // Реагируем на опасность
        shock(ant);
        escape_from_enemy(ant);
        return;
    }

    // Если муравей не несет отходы - ищем новые задачи
    if (ant.get_inventory() == no_res && !ant.has_valid_target()) {
        // Логика поиска реализована в методе look_around класса Ant
        // Здесь мы не делаем ничего дополнительного
    }
    // Если муравей уже несет отходы, но у него нет цели
    else if ((ant.get_inventory() == body || ant.get_inventory() == trash) && !ant.has_valid_target()) {
        deliver_to_cemetery(ant);
    }
}

void Cleaner::seek_bodies(std::vector<Resource>& resources, Ant& ant) {
    // Если муравей уже несёт что-то или имеет цель, выходим
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // Просматриваем доступные ресурсы в поисках трупов
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != body)
            continue;
        ant.move();
        // Проверяем, находится ли труп в поле зрения муравья
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // Муравей нашёл труп - установим цель и инвентарь
            ant.set_target(resPos);
            ant.set_inventory(body);
            res.set_invisible(); // Помечаем ресурс как подобранный
            break;
        }
    }
}

void Cleaner::seek_trash(std::vector<Resource>& resources, Ant& ant) {
    // Если муравей уже несёт что-то или имеет цель, выходим
    if (ant.has_valid_target() || ant.get_inventory() != no_res)
        return;

    // Просматриваем доступные ресурсы в поисках мусора
    for (auto& res : resources) {
        if (!res.is_visible() || res.get_type() != trash)
            continue;

        // Проверяем, находится ли мусор в поле зрения муравья
        Position antPos = ant.get_pos();
        Position resPos = res.get_posit();
        float dx = antPos.x - resPos.x;
        float dy = antPos.y - resPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= ant.get_vision_circle().getRadius()) {
            // Муравей нашёл мусор - установим цель и инвентарь
            ant.set_target(resPos);
            ant.set_inventory(trash);
            res.set_invisible(); // Помечаем ресурс как подобранный
            break;
        }
    }
}

bool Cleaner::is_carrying_waste(const Ant& ant) const {
    return ant.get_inventory() == body || ant.get_inventory() == trash;
}

void Cleaner::deliver_to_cemetery(Ant& ant) {
    // Получаем текущее кладбище
    Cemetery* cemetery = Cemetery::get_current();

    if (cemetery) {
        // Если кладбище существует, направляем муравья к его центру
        Position cemetery_center = cemetery->get_center();
        ant.set_target(cemetery_center);
    }
    else {
        // Если кладбища нет, сбрасываем инвентарь и цель
        ant.set_inventory(no_res);
        ant.clear_target();
    }
}