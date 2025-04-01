#include "Ants.h"

// Реализация методов класса Baby
void Baby::work(Ant& ant) {
    // Личинки просто растут
    cout << "Baby is growing" << endl;
}

void Baby::role_up(Ant& ant) {
    ant.set_role(unique_ptr<Role>(new Sitter()));
    cout << "Baby has grown into a Sitter" << endl;
}

// Реализация методов класса Sitter
void Sitter::work(Ant& ant) {
    cout << "Sitter is taking care of babies" << endl;

    // Проверяем, есть ли враги поблизости
    Anthill* home = ant.get_home();
    const vector<Enemy>& enemies = home->get_enemies();

    for (const Enemy& enemy : enemies) {
        // Если враг рядом с муравейником, няньки помогают защищать
        if (enemy.get_position().distance(home->get_position()) < 50.0f) {
            // Информирование всех о нападении
            home->get_enemy_informer().notify(enemy.get_position());
            cout << "Sitter warns about enemy near anthill!" << endl;
            break;
        }
    }
}

void Sitter::role_up(Ant& ant) {
    int n = rand() % 100;

    // Отписываемся от старого информера
    ant.unsubscribe_from_informer(enemy_info);

    if (n < 50) {
        ant.set_role(unique_ptr<Role>(new Collector()));
        // Подписываемся на новый информер
        ant.subscribe_to_informer(food_info);
        cout << "Sitter has become a Collector" << endl;
    }
    else {
        ant.set_role(unique_ptr<Role>(new Builder()));
        // Подписываемся на новый информер
        ant.subscribe_to_informer(stick_info);
        cout << "Sitter has become a Builder" << endl;
    }
}

// Реализация методов класса Collector
void Collector::work(Ant& ant) {
    Anthill* home = ant.get_home();
    Position ant_pos = ant.get_position();

    // Если муравей несет еду, вернуть ее в муравейник
    if (has_food) {
        if (ant_pos.distance(home->get_position()) < 30.0f) {
            // Доставили еду в муравейник
            home->add_food(1);
            has_food = false;
            ant.set_returning_home(false);
            ant.clear_target();
            cout << "Collector delivered food to anthill" << endl;

            // Оставляем феромон для других сборщиков
            ant.leave_pheromone(food_info);
        }
        else {
            // Несем еду домой
            ant.set_returning_home(true);
            ant.set_target(home->get_position());
            cout << "Collector is carrying food to anthill" << endl;
        }
    }
    else {
        // Ищем еду поблизости
        const vector<Resource>& resources = home->get_resources();

        for (const Resource& resource : resources) {
            if (resource.get_type() == food && !resource.is_collected() &&
                ant.can_see(resource, 50.0f)) {

                ant.set_target(resource.get_position());
                cout << "Collector found food" << endl;

                // Если ресурс большой, оповещаем других коллекторов
                if (resource.get_size() > 1) {
                    home->get_food_informer().notify(resource.get_position(), resource.get_size());
                    cout << "Collector informs others about large food source!" << endl;
                }

                if (ant_pos.distance(resource.get_position()) < 5.0f) {
                    // Собираем еду
                    has_food = true;
                    cout << "Collector picked up food" << endl;

                    // Оставляем феромон
                    ant.leave_pheromone(food_info);
                    break;
                }
            }
        }

        // Если нет видимой еды, проверяем феромоны других муравьев
        if (!has_food && !ant.has_target) {
            const vector<Pheromone>& pheromones = home->get_pheromones();

            for (const Pheromone& pheromone : pheromones) {
                if (pheromone.get_type() == food_info && pheromone.is_active() &&
                    ant.can_see(pheromone, 60.0f)) {

                    ant.set_target(pheromone.get_position());
                    cout << "Collector follows food pheromone" << endl;
                    break;
                }
            }
        }
    }
}

void Collector::role_up(Ant& ant) {
    int h = ant.get_max_hp();

    // Отписываемся от старого информера
    ant.unsubscribe_from_informer(food_info);

    if (h >= 10) {
        ant.set_role(unique_ptr<Role>(new Soldier()));
        // Подписываемся на новый информер
        ant.subscribe_to_informer(enemy_info);
        cout << "Collector has become a Soldier" << endl;
    }
    else {
        ant.set_role(unique_ptr<Role>(new Shepherd()));
        // Подписываемся на новый информер
        cout << "Collector has become a Shepherd" << endl;
    }
}

// Реализация методов класса Builder
void Builder::work(Ant& ant) {
    Anthill* home = ant.get_home();
    Position ant_pos = ant.get_position();

    // Если муравей несет палку, вернуть ее в муравейник
    if (has_stick) {
        if (ant_pos.distance(home->get_position()) < 30.0f) {
            // Доставили палку в муравейник
            home->add_sticks(1);
            has_stick = false;
            ant.set_returning_home(false);
            ant.clear_target();
            cout << "Builder delivered stick to anthill" << endl;

            // Оставляем феромон для других строителей
            ant.leave_pheromone(stick_info);
        }
        else {
            // Несем палку домой
            ant.set_returning_home(true);
            ant.set_target(home->get_position());
            cout << "Builder is carrying stick to anthill" << endl;
        }
    }
    else {
        // Ищем палки поблизости
        const vector<Resource>& resources = home->get_resources();

        for (const Resource& resource : resources) {
            if (resource.get_type() == stick && !resource.is_collected() &&
                ant.can_see(resource, 50.0f)) {

                ant.set_target(resource.get_position());
                cout << "Builder found stick" << endl;

                // Если ресурс большой, оповещаем других строителей
                if (resource.get_size() > 1) {
                    home->get_stick_informer().notify(resource.get_position(), resource.get_size());
                    cout << "Builder informs others about large stick!" << endl;
                }

                if (ant_pos.distance(resource.get_position()) < 5.0f) {
                    // Собираем палку
                    has_stick = true;
                    cout << "Builder picked up stick" << endl;

                    // Оставляем феромон
                    ant.leave_pheromone(stick_info);
                    break;
                }
            }
        }

        // Если нет видимых палок, проверяем феромоны других муравьев
        if (!has_stick && !ant.has_target) {
            const vector<Pheromone>& pheromones = home->get_pheromones();

            for (const Pheromone& pheromone : pheromones) {
                if (pheromone.get_type() == stick_info && pheromone.is_active() &&
                    ant.can_see(pheromone, 60.0f)) {

                    ant.set_target(pheromone.get_position());
                    cout << "Builder follows stick pheromone" << endl;
                    break;
                }
            }
        }
    }
}

void Builder::role_up(Ant& ant) {
    int h = ant.get_max_hp();

    // Отписываемся от старого информера
    ant.unsubscribe_from_informer(stick_info);

    if (h >= 10) {
        ant.set_role(unique_ptr<Role>(new Soldier()));
        // Подписываемся на новый информер
        ant.subscribe_to_informer(enemy_info);
        cout << "Builder has become a Soldier" << endl;
    }
    else {
        ant.set_role(unique_ptr<Role>(new Shepherd()));
        cout << "Builder has become a Shepherd" << endl;
    }
}

// Реализация методов класса Shepherd
void Shepherd::work(Ant& ant) {
    cout << "Shepherd is guiding ants to work efficiently" << endl;

    Anthill* home = ant.get_home();
    Position ant_pos = ant.get_position();

    // Пастухи ищут и заботятся о тле
    const vector<Resource>& resources = home->get_resources();

    for (const Resource& resource : resources) {
        if (resource.get_type() == aphid && ant.can_see(resource, 50.0f)) {
            ant.set_target(resource.get_position());
            cout << "Shepherd found aphid" << endl;

            if (ant_pos.distance(resource.get_position()) < 10.0f) {
                // Заботимся о тле
                cout << "Shepherd is caring for aphid" << endl;

                // Оставляем феромон для других пастухов
                ant.leave_pheromone(food_info);
                break;
            }
        }
    }
}

void Shepherd::role_up(Ant& ant) {
    ant.set_role(unique_ptr<Role>(new Cleaner()));
    cout << "Shepherd has become a Cleaner" << endl;
}
// Продолжение реализации методов классов из Ants.cpp
// Начало файла должно включать уже существующие реализации

// Реализация методов класса Soldier
void Soldier::work(Ant& ant) {
    Anthill* home = ant.get_home();
    Position ant_pos = ant.get_position();

    // Проверяем, сражается ли солдат
    if (is_fighting) {
        // Если солдат сражается, он продолжает атаковать врага
        cout << "Soldier is fighting an enemy" << endl;
        return;
    }

    // Солдаты ищут врагов поблизости
    const vector<Enemy>& enemies = home->get_enemies();

    for (size_t i = 0; i < enemies.size(); i++) {
        const Enemy& enemy = enemies[i];
        if (!enemy.isDead() && ant.can_see(enemy, 60.0f)) {
            // Обнаружен враг
            ant.set_target(enemy.get_position());
            cout << "Soldier found an enemy!" << endl;

            // Оповещаем других солдат
            home->get_enemy_informer().notify(enemy.get_position());

            if (ant_pos.distance(enemy.get_position()) < 10.0f) {
                // Атакуем врага
                is_fighting = true;
                cout << "Soldier is attacking enemy with damage " << damage << endl;

                // Оставляем феромон опасности
                ant.leave_pheromone(enemy_info);
                break;
            }
        }
    }

    // Если нет видимых врагов, проверяем феромоны опасности
    if (!is_fighting && !ant.has_target) {
        const vector<Pheromone>& pheromones = home->get_pheromones();

        for (const Pheromone& pheromone : pheromones) {
            if (pheromone.get_type() == enemy_info && pheromone.is_active() &&
                ant.can_see(pheromone, 80.0f)) {

                ant.set_target(pheromone.get_position());
                cout << "Soldier follows enemy pheromone" << endl;
                break;
            }
        }
    }

    // Если нет целей, патрулируем территорию
    if (!is_fighting && !ant.has_target) {
        // Патрулирование вокруг муравейника
        if (rand() % 100 < 5) {  // 5% шанс сменить направление патрулирования
            Position anthill_pos = home->get_position();
            float patrol_radius = 100.0f + (rand() % 50);
            float angle = (rand() % 360) * 3.14159f / 180.0f;

            Position patrol_pos;
            patrol_pos.x = anthill_pos.x + cos(angle) * patrol_radius;
            patrol_pos.y = anthill_pos.y + sin(angle) * patrol_radius;

            ant.set_target(patrol_pos);
            cout << "Soldier is patrolling" << endl;
        }
    }
}

void Soldier::role_up(Ant& ant) {
    // Отписываемся от старого информера
    ant.unsubscribe_from_informer(enemy_info);

    // Солдаты со временем становятся уборщиками
    ant.set_role(unique_ptr<Role>(new Cleaner()));
    cout << "Soldier has become a Cleaner" << endl;
}

// Реализация методов класса Cleaner
void Cleaner::work(Ant& ant) {
    Anthill* home = ant.get_home();
    Position ant_pos = ant.get_position();

    // Если муравей несет мусор, вернуть его в муравейник
    if (carrying_trash) {
        if (ant_pos.distance(home->get_position()) < 30.0f) {
            // Доставили мусор в муравейник (переработка)
            carrying_trash = false;
            ant.set_returning_home(false);
            ant.clear_target();
            cout << "Cleaner delivered trash to anthill for recycling" << endl;
        }
        else {
            // Несем мусор домой
            ant.set_returning_home(true);
            ant.set_target(home->get_position());
            cout << "Cleaner is carrying trash to anthill" << endl;
        }
    }
    else {
        // Ищем мусор или трупы поблизости
        const vector<Resource>& resources = home->get_resources();

        for (const Resource& resource : resources) {
            if ((resource.get_type() == body || resource.get_type() == trash) &&
                !resource.is_collected() && ant.can_see(resource, 50.0f)) {

                ant.set_target(resource.get_position());
                cout << "Cleaner found " << (resource.get_type() == body ? "body" : "trash") << endl;

                if (ant_pos.distance(resource.get_position()) < 5.0f) {
                    // Подбираем мусор
                    carrying_trash = true;
                    cout << "Cleaner picked up " << (resource.get_type() == body ? "body" : "trash") << endl;
                    break;
                }
            }
        }
    }

    // Уборщики также очищают неактивные феромоны
    const vector<Pheromone>& pheromones = home->get_pheromones();

    // Если феромон старый и с низкой интенсивностью, "очищаем" его
    // Реальная очистка происходит в методе update_pheromones() муравейника
    for (const Pheromone& pheromone : pheromones) {
        if (pheromone.is_active() && pheromone.get_intensity() < 20 &&
            ant.can_see(pheromone, 20.0f) && !carrying_trash) {

            ant.set_target(pheromone.get_position());
            cout << "Cleaner is cleaning old pheromone" << endl;
            break;
        }
    }
}

void Cleaner::role_up(Ant& ant) {
    // Уборщики возвращаются обратно в няньки (замыкая жизненный цикл)
    ant.set_role(unique_ptr<Role>(new Sitter()));
    cout << "Cleaner has become a Sitter again" << endl;
}

// Реализация конструктора муравья
Ant::Ant(Anthill* _home, Position start_pos)
    : hp(1), max_hp(1), age(0), speed(3),
    charact(passive), role(nullptr), pos(start_pos), home(_home),
    carrying_resource(false), resource_type(no_res),
    current_dir(none), returning_home(false), has_target(false),
    vision_radius(50.0f), is_dead(false), tick_counter(0)
{
    // Устанавливаем начальную роль как Baby (личинка)
    role = make_unique<Baby>();

    // Инициализация спрайта произойдет через set_sprite в main.cpp
}

void Ant::update() {
    if (is_dead) return;

    tick_counter++;

    // Каждые 100 тактов увеличиваем возраст
    if (tick_counter % 100 == 0) {
        age_up();
    }

    // Работа согласно роли
    work();

    // Перемещение муравья
    move();
}

void Ant::work() {
    if (role && !is_dead) {
        role->work(*this);
    }
}

void Ant::move() {
    if (is_dead) return;

    // Если у муравья есть цель, двигаемся к ней
    if (has_target) {
        // Направление к цели
        float dx = target.x - pos.x;
        float dy = target.y - pos.y;
        float distance = sqrt(dx * dx + dy * dy);

        // Если достигли цели, убираем её
        if (distance < 3.0f) {
            has_target = false;
            return;
        }

        // Нормализация вектора направления
        dx /= distance;
        dy /= distance;

        // Движение к цели
        pos.x += dx * speed;
        pos.y += dy * speed;
    }
    else {
        // Случайное движение, если нет цели
        if (rand() % 100 < 5) {  // 5% шанс сменить направление
            current_dir = static_cast<direction>(rand() % 4);
        }

        // Применяем текущее направление
        switch (current_dir) {
        case direction::up:    pos.y -= speed * 0.5f; break;
        case direction::down:  pos.y += speed * 0.5f; break;
        case direction::left:  pos.x -= speed * 0.5f; break;
        case direction::right: pos.x += speed * 0.5f; break;
        default: break;
        }

        // Проверка на выход за пределы карты (предположим 800x600)
        const float border = 20.0f;
        if (pos.x < border) { pos.x = border; current_dir = direction::right; }
        if (pos.y < border) { pos.y = border; current_dir = down; }
        if (pos.x > 800 - border) { pos.x = 800 - border; current_dir = direction::left; }
        if (pos.y > 600 - border) { pos.y = 600 - border; current_dir = up; }
    }

    // Обновляем позицию спрайта
    sprite.setPosition(pos.x, pos.y);
}

void Ant::age_up() {
    age++;

    // Увеличиваем HP в зависимости от типа роли
    if (role) {
        const char* role_name = role->get_role_name();

        if (strcmp(role_name, "Baby") == 0) max_hp = baby;
        else if (strcmp(role_name, "Sitter") == 0) max_hp = sitter;
        else if (strcmp(role_name, "Collector") == 0) max_hp = collector;
        else if (strcmp(role_name, "Builder") == 0) max_hp = builder;
        else if (strcmp(role_name, "Soldier") == 0) max_hp = soldier;
        else if (strcmp(role_name, "Shepherd") == 0) max_hp = shepherd;
        else if (strcmp(role_name, "Cleaner") == 0) max_hp = cleaner;

        hp = max_hp;  // Восстанавливаем HP при взрослении
    }

    // Каждые 5 единиц возраста проверяем, нужно ли повысить роль
    if (age % 5 == 0 && role) {
        role->role_up(*this);
    }
}

void Ant::take_damage(int damage) {
    hp -= damage;
    if (hp <= 0) {
        kill();
    }
}

void Ant::set_role(unique_ptr<Role> new_role) {
    role = std::move(new_role);
}

void Ant::subscribe_to_informer(info_type type) {
    switch (type) {
    case food_info:
        home->get_food_informer().add_subscriber(this);
        break;
    case stick_info:
        home->get_stick_informer().add_subscriber(this);
        break;
    case enemy_info:
        home->get_enemy_informer().add_subscriber(this);
        break;
    }
}

void Ant::unsubscribe_from_informer(info_type type) {
    switch (type) {
    case food_info:
        home->get_food_informer().remove_subscriber(this);
        break;
    case stick_info:
        home->get_stick_informer().remove_subscriber(this);
        break;
    case enemy_info:
        home->get_enemy_informer().remove_subscriber(this);
        break;
    }
}

void Ant::food_found(const Position& food_pos, int size) {
    // Только коллекторы реагируют на информацию о еде
    const char* role_name = get_role_name();
    if (strcmp(role_name, "Collector") == 0 && !has_target && !is_carrying_resource()) {
        set_target(food_pos);
        cout << "Ant received info about food at (" << food_pos.x << "," << food_pos.y << ") with size " << size << endl;
    }
}

void Ant::stick_found(const Position& stick_pos, int size) {
    // Только строители реагируют на информацию о палках
    const char* role_name = get_role_name();
    if (strcmp(role_name, "Builder") == 0 && !has_target && !is_carrying_resource()) {
        set_target(stick_pos);
        cout << "Ant received info about stick at (" << stick_pos.x << "," << stick_pos.y << ") with size " << size << endl;
    }
}

void Ant::enemy_spotted(const Position& enemy_pos) {
    // Только солдаты реагируют на информацию о врагах
    const char* role_name = get_role_name();
    if (strcmp(role_name, "Soldier") == 0 && !has_target) {
        set_target(enemy_pos);
        cout << "Ant received info about enemy at (" << enemy_pos.x << "," << enemy_pos.y << ")" << endl;
    }
}

void Ant::pick_resource(res_type type) {
    carrying_resource = true;
    resource_type = type;
}

void Ant::drop_resource() {
    carrying_resource = false;
    resource_type = no_res;
}

void Ant::attack(Enemy* enemy) {
    // Определяем силу атаки в зависимости от роли
    int attack_power = 1;
    const char* role_name = get_role_name();

    if (strcmp(role_name, "Soldier") == 0) {
        Soldier* soldier_role = dynamic_cast<Soldier*>(role.get());
        if (soldier_role) {
            attack_power = soldier_role->get_damage();
        }
    }

    cout << "Ant attacks enemy with power " << attack_power << endl;
    enemy->take_damage(attack_power);
}

void Ant::defend() {
    cout << "Ant defends itself" << endl;
}

void Ant::leave_pheromone(info_type type) {
    Pheromone new_pheromone(pos, type);
    home->register_pheromone(new_pheromone);
    cout << "Ant left pheromone of type " << type << " at position (" << pos.x << "," << pos.y << ")" << endl;
}

// Реализация методов муравейника
Anthill::Anthill(Position pos)
    : level(1), food_count(50), stick_count(50), capacity(20),
    position(pos), shape(30.f),
    food_informer(food_info), stick_informer(stick_info), enemy_informer(enemy_info),
    food_consumption_timer(0), spawn_timer(0), resource_spawn_timer(0), enemy_spawn_timer(0)
{
    // Создаем начальную колонию из 10 муравьев
    for (int i = 0; i < 10; i++) {
        add_ant();
    }
}

void Anthill::update() {
    // Обновляем всех муравьев
    for (auto& ant : colony) {
        ant->update();
    }

    // Обновляем феромоны
    update_pheromones();

    // Обновляем ресурсы
    update_resources();

    // Обновляем врагов
    update_enemies();

    // Удаляем мертвых муравьев
    remove_dead_ants();

    // Таймер потребления пищи
    food_consumption_timer++;
    if (food_consumption_timer >= 200) {  // Каждые 200 тактов
        food_consumption_timer = 0;
        consume_food();
    }

    // Таймер появления новых муравьев
    spawn_timer++;
    if (spawn_timer >= 300 && food_count > 10 && colony.size() < capacity) {  // Каждые 300 тактов
        spawn_timer = 0;
        add_ant();
        food_count -= 10;  // Создание муравья стоит 10 единиц еды
    }

    // Таймер появления новых ресурсов
    resource_spawn_timer++;
    if (resource_spawn_timer >= 500) {  // Каждые 500 тактов
        resource_spawn_timer = 0;

        // Создаем случайные ресурсы на карте
        for (int i = 0; i < 5; i++) {
            float x = (rand() % 700) + 50;
            float y = (rand() % 500) + 50;

            int type_roll = rand() % 100;
            res_type type;

            if (type_roll < 50) {
                type = food;
            }
            else if (type_roll < 90) {
                type = stick;
            }
            else {
                type = aphid;
            }

            int size = rand() % 5 + 1;

            Resource new_resource(type, size, Position(x, y));
            register_resource(new_resource);
        }
    }

    // Таймер появления врагов
    enemy_spawn_timer++;
    if (enemy_spawn_timer >= 800) {  // Каждые 800 тактов
        enemy_spawn_timer = 0;

        // Шанс появления врага зависит от уровня муравейника
        if (rand() % 100 < 20 + level * 5) {
            float x = (rand() % 700) + 50;
            float y = (rand() % 500) + 50;

            // Враг появляется подальше от муравейника
            Position enemy_pos(x, y);
            while (enemy_pos.distance(position) < 150.0f) {
                x = (rand() % 700) + 50;
                y = (rand() % 500) + 50;
                enemy_pos = Position(x, y);
            }

            Enemy new_enemy(enemy_pos);
            register_enemy(new_enemy);
        }
    }

    // Проверяем условия для повышения уровня
    leveling();
}

void Anthill::add_ant() {
    if (colony.size() < capacity) {
        Ant new_ant(this, position);
        colony.push_back(make_unique<Ant>(this, position));
        cout << "New ant born in anthill" << endl;
    }
}

void Anthill::remove_dead_ants() {
    colony.erase(
        std::remove_if(colony.begin(), colony.end(),
            [](const unique_ptr<Ant>& ant_ptr) { // Используем unique_ptr
                return ant_ptr->has_died(); // Доступ через ->
            }),
        colony.end()
    );
}
void Anthill::leveling() {
    // Для повышения уровня нужно достаточное количество строительных материалов
    int required_sticks = level * 50;

    if (stick_count >= required_sticks) {
        level++;
        stick_count -= required_sticks;
        capacity = 10 + level * 10;  // Увеличиваем вместимость муравейника

        // Обновляем размер муравейника на экране
        shape.setRadius(30.f + 10.f * level);
        shape.setPosition(position.x - shape.getRadius(), position.y - shape.getRadius());

        cout << "Anthill upgraded to level " << level << "! New capacity: " << capacity << endl;
    }
}

void Anthill::add_food(int amount) {
    food_count += amount;
    cout << "Added " << amount << " food. Total food: " << food_count << endl;
}

void Anthill::add_sticks(int amount) {
    stick_count += amount;
    cout << "Added " << amount << " sticks. Total sticks: " << stick_count << endl;
}

void Anthill::consume_food() {
    // Колония потребляет пищу
    int required_food = colony.size() / 2 + 1;

    if (food_count >= required_food) {
        food_count -= required_food;
        cout << "Colony consumed " << required_food << " food. Remaining: " << food_count << endl;
    }
    else {
        // Недостаточно еды - муравьи голодают
        cout << "Colony is starving! Not enough food!" << endl;

        // Наносим урон муравьям из-за голода
        for (auto& ant : colony) {
            ant->take_damage(1);
        }
    }
}

void Anthill::register_pheromone(Pheromone pheromone) {
    pheromones.push_back(pheromone);
}

void Anthill::update_pheromones() {
    // Обновляем все феромоны
    for (auto& pheromone : pheromones) {
        pheromone.update();
    }

    // Удаляем неактивные феромоны
    pheromones.erase(
        std::remove_if(pheromones.begin(), pheromones.end(),
            [](const Pheromone& p) { return !p.is_active(); }),
        pheromones.end()
    );
}

void Anthill::register_resource(Resource resource) {
    resources.push_back(resource);
}

void Anthill::update_resources() {
    // Обновляем ресурсы (например, тлю)
    for (auto& resource : resources) {
        if (resource.get_type() == aphid) {
            // У нас нет прямого доступа к методам Aphid, поэтому представим, что тля производит пищу
            if (rand() % 100 < 5) {  // 5% шанс на такте
                // Создаем новый ресурс еды рядом с тлей
                Position aphid_pos = resource.get_position();
                Position food_pos(aphid_pos.x + (rand() % 20 - 10), aphid_pos.y + (rand() % 20 - 10));
                Resource new_food(food, 1, food_pos);
                resources.push_back(new_food);
            }
        }
    }

    // Удаляем полностью собранные ресурсы
    resources.erase(
        std::remove_if(resources.begin(), resources.end(),
            [](const Resource& r) { return r.is_collected() && r.get_size() <= 0; }),
        resources.end()
    );
}

void Anthill::register_enemy(Enemy enemy) {
    enemies.push_back(enemy);
    cout << "New enemy spotted near the anthill!" << endl;

    // Оповещаем всех солдат о появлении врага
    enemy_informer.notify(enemy.get_position());
}

void Anthill::update_enemies() {
    for (auto& enemy : enemies) {
        if (!enemy.isDead()) {
            enemy.move();

            // Проверяем, атакует ли враг муравьев
            Position enemy_pos = enemy.get_position();
            for (auto& ant : colony) {
                if (!ant->has_died() && enemy_pos.distance(ant->get_position()) < 15.0f) {
                    // Враг атакует муравья
                    ant->take_damage(enemy.get_damage());
                    cout << "Enemy attacks ant!" << endl;

                    // Муравей защищается и наносит ответный урон
                    ant->defend();

                    // Если муравей - солдат, он наносит больше урона
                    if (strcmp(ant->get_role_name(), "Soldier") == 0) {
                        enemy.take_damage(3);
                        cout << "Soldier counterattacks enemy!" << endl;
                    }
                    else {
                        enemy.take_damage(1);
                        cout << "Ant tries to defend itself!" << endl;
                    }
                    break;
                }
            }
        }
    }

    // Удаляем мертвых врагов и превращаем их в ресурсы (тела)
    for (auto it = enemies.begin(); it != enemies.end();) {
        if (it->isDead()) {
            // Создаем ресурс "тело" на месте погибшего врага
            Position body_pos = it->get_position();
            Resource body_resource(body, 3, body_pos);
            resources.push_back(body_resource);

            cout << "Enemy defeated! Its body can be processed by cleaners." << endl;
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}