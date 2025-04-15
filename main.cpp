#include "Game.h"

#define Colony
#ifdef Colony
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    Game game;
    Clock time;
    float last_time = 0;
    int x = 0, y = 0;

    game.spawn_res();
    game.spawn_aphids();

    //Anthil setting
    CircleShape enemy_hill_1(start_radius);
    CircleShape enemy_hill_2(start_radius);
    CircleShape enemy_hill_3(start_radius);

    enemy_hill_1.setPosition(Vector2f(-start_radius, window_height - start_radius));
    enemy_hill_2.setPosition(Vector2f(window_width - start_radius, -start_radius));
    enemy_hill_3.setPosition(Vector2f(window_width - start_radius, window_height - start_radius));

    enemy_hill_1.setFillColor(Color(0, 0, 0, 80));
    enemy_hill_2.setFillColor(Color(0, 0, 0, 80));
    enemy_hill_3.setFillColor(Color(0, 0, 0, 80));

    //Window setting
    RenderWindow window(VideoMode(window_width, window_height), L"Colony Simulator");
    Image icon;
    if (!icon.loadFromFile("anthill.png")) return -1;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //MUSIC
    /*Music backgroundMusic;
    if (!backgroundMusic.openFromFile("music.ogg")) return -1;
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(100);
    backgroundMusic.play();*/

    //Events and font
    Event event;
    Font font;
    if (!font.loadFromFile("Arial.ttf")) return -1;
    Text statsText("", font, 25);
    statsText.setFillColor(Color::White);
    statsText.setPosition(5, 5);
    game.add_stats(font);
    int FPS = fps;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || event.type == Event::KeyPressed && ((event.key.code == Keyboard::Space) || (event.key.code == Keyboard::Escape) || (event.key.code == Keyboard::BackSpace))) window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) { FPS -= 5; }
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) { FPS += 5; }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
                game.reset();
                last_time = time.getElapsedTime().asMilliseconds();
            }
        }

        if (time.getElapsedTime().asMilliseconds() - last_time >= FPS) {
            last_time = time.getElapsedTime().asMilliseconds();
            game.tick();
            game.update();
            if (game.get_ticks() % feeding_period == 0) {
                game.anthill.hunger(); // Проверка на голод каждый период кормления
            }

            game.statsLines.clear();
            game.add_stats(font);
            game.update_aphids();
            game.spawn_body();

            //Informers::update(game.anthill.colony, game.resources);

            //Spawn entities
            if (game.get_ticks() % enemy_wave_period == 0) game.raid.spawn_raid();

            for (auto& ant : game.anthill.colony) {
                ant.look_around(game.resources);
                ant.move();
                ant.upd_color();
                if (ant.get_hp() > 0) {
                    ant.up();
                    if (ant.get_age() % stage_time == 0 && ant.get_age())
                        ant.upd_role();
                }
            }

            //Enemy's update
            for (auto& enemy : game.raid.crowd) {
                if (enemy.get_hp() > 0) {
                    enemy.move();
                    enemy.aged();
                }
                if (enemy.get_robbed()) {
                    game.raid.crowd.clear();
                    break;
                }
            }

            for (size_t i = 0; i < game.anthill.colony.size(); i++) {
                for (size_t j = i + 1; j < game.anthill.colony.size(); j++) {
                    Vector2f pos1 = game.anthill.colony[i].get_shape().getPosition();
                    Vector2f pos2 = game.anthill.colony[j].get_shape().getPosition();

                    float dx = pos1.x - pos2.x;
                    float dy = pos1.y - pos2.y;
                    float distance = sqrt(dx * dx + dy * dy);
                    float min_dist = ant_size * 2.0f;

                    if (distance < min_dist && distance > 0.001f) {
                        float overlap = (min_dist - distance) / 2.0f;
                        float offsetX = (dx / distance) * overlap;
                        float offsetY = (dy / distance) * overlap;

                        CircleShape& shape1 = const_cast<CircleShape&>(game.anthill.colony[i].get_shape());
                        CircleShape& shape2 = const_cast<CircleShape&>(game.anthill.colony[j].get_shape());

                        shape1.setPosition(pos1.x + offsetX, pos1.y + offsetY);
                        shape2.setPosition(pos2.x - offsetX, pos2.y - offsetY);
                    }
                }
            }

            for (size_t i = 0; i < game.raid.get_size(); i++) {
                for (size_t j = i + 1; j < game.raid.get_size(); j++) {
                    Vector2f enemy_pos1 = game.raid.crowd[i].get_shape().getPosition();
                    Vector2f enemy_pos2 = game.raid.crowd[j].get_shape().getPosition();

                    float dx = enemy_pos1.x - enemy_pos2.x;
                    float dy = enemy_pos2.y - enemy_pos2.y;
                    float distance = sqrt(dx * dx + dy * dy);

                    float radius1 = game.raid.crowd[i].get_shape().getRadius();
                    float radius2 = game.raid.crowd[j].get_shape().getRadius();
                    float min_dist_enemy = radius1 + radius2;

                    if (distance < min_dist_enemy && distance > 0.001f) {
                        float overlap = (min_dist_enemy - distance) / 2.0f;
                        float offsetX = (dx / distance) * overlap;
                        float offsetY = (dy / distance) * overlap;

                        CircleShape& eShape1 = const_cast<CircleShape&>(game.raid.crowd[i].get_shape());
                        CircleShape& eShape2 = const_cast<CircleShape&>(game.raid.crowd[j].get_shape());

                        eShape1.setPosition(enemy_pos1.x + offsetX, enemy_pos1.y + offsetY);
                        eShape2.setPosition(enemy_pos2.x - offsetX, enemy_pos2.y - offsetY);
                    }
                }
            }
        }

        window.clear(Color(102, 204, 0));
        window.draw(game.anthill.get_shape());
        window.draw(enemy_hill_1);
        window.draw(enemy_hill_2);
        window.draw(enemy_hill_3);

        for (const auto& res : game.resources) if (res.is_visible()) window.draw(res.get_shape());
        for (const auto& ant : game.anthill.colony) {
            if (ant.is_visible()) {
                window.draw(ant.get_shape());
                if (vision_circle && ant.get_role() != 0 && ant.get_role() != 1) window.draw(ant.get_vision_circle());
            }
        }
        for (const auto& aphid : game.aphids) if (aphid.is_visible()) window.draw(aphid.get_shape());
        for (auto& enemy : game.raid.crowd) if (enemy.is_visible()) window.draw(enemy.get_shape());
        for (const auto& text : game.statsLines) window.draw(text);
        window.display();

        if (((game.get_ticks() % (10 * second) == 0) && (game.anthill.colony.size() == 0)) ||
            game.anthill.get_shape().getRadius() <= 0.75 * start_radius) {
            game.over(font);

            window.clear(Color::White);
            window.draw(game.OVER);
            window.draw(game.YOU);
            window.display();

            time.restart();
            while (time.getElapsedTime().asSeconds() < 3.0f) {
                Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }
            }
            window.close();
            return 0;
        }
    }
    return 0;
}
#endif