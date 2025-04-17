#include "Game.h"

Game* Game::current = nullptr;
void Game::update(Font& font)
{
    Game::set_current(this);
    Cemetery::set_current(&cemetery);
    ticks++;
    anthill.upd_anthill(ticks, resources);
    if (anthill.colony.size() > 0) has_started_colony = 1;
    statsLines.clear();
    add_stats(font);
    update_aphids();
    if (get_ticks() % enemy_wave_period == 0) raid.spawn_raid();
}

void Game::add_stats(Font& font) {
    int line = 0;
    auto makeText = [&](const string& text, Color color) {
        Text t(text, font, 18);
        t.setFillColor(color);
        t.setPosition(10, 10 + line * 20);
        statsLines.push_back(t);
        line++;
        };
    // Подсчет ресурсов типа "body" (трупы)
    int bodyResourcesCount = 0;
    for (const auto& res : resources) {
        if (res.is_visible() && res.get_type() == body) {
            bodyResourcesCount++;
        }
    }
    makeText("Ants: " + to_K(anthill.get_ant_count()) + " (" + to_K(anthill.get_max_ants()) + ")", Color::White);
    makeText("Enemies: " + to_string(raid.get_size()), Color::Red);
    makeText("Aphids: " + to_string(aphids.size()), Color(75, 0, 130));
    makeText("Food: " + to_K(anthill.get_food_count()) + " (" + to_K(anthill.get_max_food()) + ")", Color(0, 255, 0));
    makeText("Bodies: " + to_string(bodyResourcesCount), Color(134, 138, 142));
    makeText("Sticks: " + to_K(anthill.get_stick_count()) + " (FU: " + to_K(anthill.get_for_upd() - anthill.get_stick_count()) + ")", Color(139, 69, 19));
    makeText("---------------", Color(0, 0, 0));
    makeText("Babies: " + to_string(anthill.get_baby_count()), Color::White);
    makeText("Sitters: " + to_string(anthill.get_sitter_count()), Color(255, 102, 178));
    makeText("Collectors: " + to_string(anthill.get_collector_count()), Color(255, 128, 0));
    makeText("Builders: " + to_string(anthill.get_builder_count()), Color::Yellow);
    makeText("Soldiers: " + to_string(anthill.get_soldier_count()), Color::Black);
    makeText("Shepherds: " + to_string(anthill.get_shepherd_count()), Color(0, 0, 204));
    makeText("Cleaners: " + to_string(anthill.get_cleaner_count()), Color(102, 51, 0));
}



void Game::reset() {
    anthill = Anthill();
    raid.crowd.clear();
    resources.clear();
    aphids.clear();
    statsLines.clear();
    ticks = 0;
    spawn_res();
}


void Game::spawn_res()
{
    for (int i = 0; i <= stick_claster_count + food_cluster_count; ++i) {
        int x, y;
        do {
            x = rand() % (window_width - 2 * dist_btw_res) + dist_btw_res;
            y = rand() % (window_height - 2 * dist_btw_res) + dist_btw_res;
        } while ((x > window_width / 2 - 3 * start_radius && x < window_width / 2 + 3 * start_radius) ||
            (y > window_height / 2 - 3 * start_radius && y < window_height / 2 + 3 * start_radius) || ((x < 0.2 * window_width) && (y < 0.3 * window_height)));
        if (i <= food_cluster_count) create_cluster(resources, x, y, food);
        else create_cluster(resources, x, y, stick);
    }
}

void Game::spawn_body()
{
    vector<size_t> ants_dead;
    for (size_t i = 0; i < anthill.colony.size(); i++) {
        if (anthill.colony[i].get_hp() <= 0 && !anthill.colony[i].is_already_dead()) {
            anthill.colony[i].dead(resources);
            ants_dead.push_back(i);
        }
    }
    for (int i = ants_dead.size() - 1; i >= 0; i--) {
        anthill.colony.erase(anthill.colony.begin() + ants_dead[i]);
    }
}

void Game::spawn_aphids() {
    for (int i = 0; i < aphid_cluster_count; ++i) {
        int x, y;
        do {
            x = rand() % (window_width - 2 * dist_btw_res) + dist_btw_res;
            y = rand() % (window_height - 2 * dist_btw_res) + dist_btw_res;
        } while ((x > window_width / 2 - 3 * start_radius && x < window_width / 2 + 3 * start_radius) ||
            (y > window_height / 2 - 3 * start_radius && y < window_height / 2 + 3 * start_radius) ||
            ((x < 0.2 * window_width) && (y < 0.3 * window_height)));

        int count = rand() % max_aphids_in_cluster + 1;
        create_aphid_cluster(aphids, x, y, count);
    }
}


void Game::update_aphids() {
    for (auto& aphid : aphids) if (aphid.is_visible()) aphid.update();
    aphids.erase(
        remove_if(aphids.begin(), aphids.end(),
            [](const Aphid& aphid) { return !aphid.is_visible() || aphid.is_dead(); }),
        aphids.end()
    );
}

void Game::over(Font& font) {
    statsLines.clear();

    OVER.setFont(font);
    OVER.setString("GAME OVER");
    OVER.setCharacterSize(130);
    OVER.setFillColor(Color::Black);

    YOU.setFont(font);
    YOU.setString("YOU ARE NEXT");
    YOU.setCharacterSize(70);
    YOU.setFillColor(Color::Red);

    FloatRect textBounds = OVER.getLocalBounds();
    OVER.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    OVER.setPosition(window_width / 2.0f, window_height / 2.0f);

    FloatRect textBounds2 = YOU.getLocalBounds();
    YOU.setOrigin(textBounds2.left + textBounds2.width / 2.0f,
        textBounds2.top + textBounds2.height / 2.0f);
    YOU.setPosition(window_width / 2.0f + 100, window_height / 2.0f + 80);
}

string Game::to_K(int x)
{
    int y = (int)(x / 1000);
    if (y > 0) return to_string(y) + "." + to_string((x % 1000) / 100) + "K";
    return to_string(x);
}

void Game::update_ants() {
    for (auto& ant : anthill.colony) {
        //ant.look_around(resources);
        ant.work(resources, raid.crowd, aphids);
        ant.move();
        if (ant.get_hp() > 0) {
            ant.up();
            if (ant.get_age() % stage_time == 0 && ant.get_age())
                ant.upd_role();
        }
    }
}

void Game::update_enemies() {
    for (auto& enemy : raid.crowd) {
        if (enemy.get_hp() > 0) {
            enemy.move();
            enemy.up();
        }
    }
    raid.crowd.erase(remove_if(raid.crowd.begin(), raid.crowd.end(),
        [](const Enemy& enemy) {
            return enemy.get_hp() <= 0 || !enemy.is_visible();
        }), raid.crowd.end());
}

void Game::handle_collisions() {
    for (size_t i = 0; i < anthill.colony.size(); i++) {
        for (size_t j = i + 1; j < anthill.colony.size(); j++) {
            Vector2f pos1 = anthill.colony[i].get_shape().getPosition();
            Vector2f pos2 = anthill.colony[j].get_shape().getPosition();
            float dx = pos1.x - pos2.x;
            float dy = pos1.y - pos2.y;
            float distance = sqrt(dx * dx + dy * dy);
            float min_dist = ant_size * 2.0f;

            if (distance < min_dist && distance > 0.001f) {
                float overlap = (min_dist - distance) / 2.0f;
                float offsetX = (dx / distance) * overlap;
                float offsetY = (dy / distance) * overlap;

                auto& shape1 = const_cast<CircleShape&>(anthill.colony[i].get_shape());
                auto& shape2 = const_cast<CircleShape&>(anthill.colony[j].get_shape());
                shape1.setPosition(pos1.x + offsetX, pos1.y + offsetY);
                shape2.setPosition(pos2.x - offsetX, pos2.y - offsetY);
            }
        }
    }

    for (size_t i = 0; i < raid.crowd.size(); i++) {
        for (size_t j = i + 1; j < raid.crowd.size(); j++) {
            Vector2f pos1 = raid.crowd[i].get_shape().getPosition();
            Vector2f pos2 = raid.crowd[j].get_shape().getPosition();
            float dx = pos1.x - pos2.x;
            float dy = pos1.y - pos2.y;
            float distance = sqrt(dx * dx + dy * dy);

            float radius_sum = raid.crowd[i].get_shape().getRadius() + raid.crowd[j].get_shape().getRadius();
            if (distance < radius_sum && distance > 0.001f) {
                float overlap = (radius_sum - distance) / 2.0f;
                float offsetX = (dx / distance) * overlap;
                float offsetY = (dy / distance) * overlap;

                auto& shape1 = const_cast<CircleShape&>(raid.crowd[i].get_shape());
                auto& shape2 = const_cast<CircleShape&>(raid.crowd[j].get_shape());
                shape1.setPosition(pos1.x + offsetX, pos1.y + offsetY);
                shape2.setPosition(pos2.x - offsetX, pos2.y - offsetY);
            }
        }
    }
}


bool Game::check_game_over() {
    return ((has_started_colony && anthill.colony.empty()) ||
        anthill.get_shape().getRadius() <= 0.75 * start_radius);
}