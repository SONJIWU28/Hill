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

    
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("music2.ogg")) return -1;
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(60);
    backgroundMusic.play();

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
            game.update(font);
            game.update_ants();
            game.spawn_body();
            //ant.upd_color();
            //game.update_enemies();
            game.handle_collisions();
            if (game.get_ticks() % feeding_period == 0) {
                game.anthill.hunger(); // Ïðîâåðêà íà ãîëîä êàæäûé ïåðèîä êîðìëåíèÿ
            }
            if (game.check_game_over()) {
                game.over(font);

                window.clear(Color::White);
                window.draw(game.OVER);
                // window.draw(game.YOU);
                window.display();

                time.restart();
                while (time.getElapsedTime().asSeconds() < 3.0f) {
                    Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed)
                            window.close();
                    }
                }
                window.close();
                return 0;
            }

            //Informers::update(game.anthill.colony, game.resources);

            window.clear(Color(180, 240, 180));
            window.draw(game.anthill.get_shape());
            window.draw(enemy_hill_1);
            window.draw(enemy_hill_2);
            window.draw(enemy_hill_3);
            window.draw(game.cemetery.get_shape());

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
        }
    }
    return 0;
}
#endif