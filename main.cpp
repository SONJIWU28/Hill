#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "Ants.h"

// ���������� ��������� ��� ��������� ���������
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float ZOOM_FACTOR = 1.1f;
const float PAN_SPEED = 10.0f;

// ���������� �������� � �������
sf::Texture antTexture;
sf::Texture sitterTexture;
sf::Texture collectorTexture;
sf::Texture builderTexture;
sf::Texture soldierTexture;
sf::Texture shepherdTexture;
sf::Texture cleanerTexture;
sf::Texture foodTexture;
sf::Texture stickTexture;
sf::Texture aphidTexture;
sf::Texture enemyTexture;
sf::Texture bodyTexture;
sf::Font font;

// ������� ��� �������� ������� � ��������
bool loadResources() {
    // �������� ������� ��� ��������
    if (!antTexture.loadFromFile("resources/ant.png")) {
        std::cerr << "�� ������� ��������� �������� �������!" << std::endl;
        return false;
    }
    if (!sitterTexture.loadFromFile("resources/sitter.png")) {
        std::cerr << "�� ������� ��������� �������� ������!" << std::endl;
        return false;
    }

    if (!collectorTexture.loadFromFile("resources/collector.png")) {
        std::cerr << "�� ������� ��������� �������� ��������!" << std::endl;
        return false;
    }

    if (!builderTexture.loadFromFile("resources/builder.png")) {
        std::cerr << "�� ������� ��������� �������� ���������!" << std::endl;
        return false;
    }

    if (!soldierTexture.loadFromFile("resources/soldier.png")) {
        std::cerr << "�� ������� ��������� �������� �������!" << std::endl;
        return false;
    }

    if (!shepherdTexture.loadFromFile("resources/shepherd.png")) {
        std::cerr << "�� ������� ��������� �������� �������!" << std::endl;
        return false;
    }

    if (!cleanerTexture.loadFromFile("resources/cleaner.png")) {
        std::cerr << "�� ������� ��������� �������� ��������!" << std::endl;
        return false;
    }

    // �������� ������� ��� �������� � ������
    if (!foodTexture.loadFromFile("resources/food.png")) {
        std::cerr << "�� ������� ��������� �������� ���!" << std::endl;
        return false;
    }

    if (!stickTexture.loadFromFile("resources/stick.png")) {
        std::cerr << "�� ������� ��������� �������� �����!" << std::endl;
        return false;
    }

    if (!aphidTexture.loadFromFile("resources/aphid.png")) {
        std::cerr << "�� ������� ��������� �������� ���!" << std::endl;
        return false;
    }

    if (!enemyTexture.loadFromFile("resources/enemy.png")) {
        std::cerr << "�� ������� ��������� �������� �����!" << std::endl;
        return false;
    }

    if (!bodyTexture.loadFromFile("resources/body.png")) {
        std::cerr << "�� ������� ��������� �������� ����!" << std::endl;
        return false;
    }

    // �������� ������
    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cerr << "�� ������� ��������� �����!" << std::endl;
        return false;
    }

    return true;
}

// ������� ��� ��������� ������� ������� � ����������� �� ��� ����
void updateAntSprite(unique_ptr<Ant>& ant) {
    const char* role_name = ant->get_role_name();
    sf::Sprite sprite;

    if (strcmp(role_name, "Baby") == 0) {
        sprite.setTexture(antTexture);
        sprite.setScale(0.5f, 0.5f); // ��������� ������ ��� �������
    }
    else if (strcmp(role_name, "Sitter") == 0) {
        sprite.setTexture(sitterTexture);
        sprite.setScale(0.8f, 0.8f);
    }
    else if (strcmp(role_name, "Collector") == 0) {
        sprite.setTexture(collectorTexture);
        sprite.setScale(1.0f, 1.0f);
    }
    else if (strcmp(role_name, "Builder") == 0) {
        sprite.setTexture(builderTexture);
        sprite.setScale(1.0f, 1.0f);
    }
    else if (strcmp(role_name, "Soldier") == 0) {
        sprite.setTexture(soldierTexture);
        sprite.setScale(1.2f, 1.2f); // ������� ������� �������
    }
    else if (strcmp(role_name, "Shepherd") == 0) {
        sprite.setTexture(shepherdTexture);
        sprite.setScale(1.0f, 1.0f);
    }
    else if (strcmp(role_name, "Cleaner") == 0) {
        sprite.setTexture(cleanerTexture);
        sprite.setScale(0.9f, 0.9f);
    }
    else {
        sprite.setTexture(antTexture);
        sprite.setScale(1.0f, 1.0f);
    }

    // ������������� ����� �������
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);

    // ������������� ������� �������
    Position pos = ant->get_position();
    sprite.setPosition(pos.x, pos.y);

    // ������������� ������ �������
    ant->get_sprite() = sprite;
}

// ������� ��� �������� ������� �������
sf::Sprite createResourceSprite(const Resource& resource) {
    sf::Sprite sprite;
    res_type type = resource.get_type();

    switch (type) {
    case food:
        sprite.setTexture(foodTexture);
        break;
    case stick:
        sprite.setTexture(stickTexture);
        break;
    case aphid:
        sprite.setTexture(aphidTexture);
        break;
    case body:
        sprite.setTexture(bodyTexture);
        break;
    default:
        sprite.setTexture(foodTexture);
        break;
    }

    // ������������� ����� �������
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);

    // ������������� ������� �������
    Position pos = resource.get_position();
    sprite.setPosition(pos.x, pos.y);

    // ������ ������� ������� �� ������� �������
    float scale = 0.5f + (resource.get_size() * 0.1f);
    sprite.setScale(scale, scale);

    return sprite;
}

// ������� ��� �������� ������� �����
sf::Sprite createEnemySprite(const Enemy& enemy) {
    sf::Sprite sprite;
    sprite.setTexture(enemyTexture);

    // ������������� ����� �������
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);

    // ������������� ������� �������
    Position pos = enemy.get_position();
    sprite.setPosition(pos.x, pos.y);

    // ������ ������� ������� �� ���� �����
    float scale = 1.0f + (enemy.get_damage() * 0.1f);
    sprite.setScale(scale, scale);

    return sprite;
}

// ������� ��� �������� ����� ��������
sf::CircleShape createPheromoneShape(const Pheromone& pheromone) {
    info_type type = pheromone.get_type();
    float intensity = pheromone.get_intensity() / 100.0f;
    sf::CircleShape shape(3.0f + intensity * 5.0f);

    // ���� ������� �� ���� ��������
    switch (type) {
    case food_info:
        shape.setFillColor(sf::Color(0, 255, 0, static_cast<sf::Uint8>(100 * intensity))); // ������� ��� ����
        break;
    case stick_info:
        shape.setFillColor(sf::Color(139, 69, 19, static_cast<sf::Uint8>(100 * intensity))); // ���������� ��� �����
        break;
    case enemy_info:
        shape.setFillColor(sf::Color(255, 0, 0, static_cast<sf::Uint8>(100 * intensity))); // ������� ��� ������
        break;
    default:
        shape.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(100 * intensity))); // ����� �� ���������
        break;
    }

    // ������������� ������� ��������
    Position pos = pheromone.get_position();
    shape.setPosition(pos.x - shape.getRadius(), pos.y - shape.getRadius());

    return shape;
}

// ������� ��� ����������� ���������� � �����������
void drawAnthillInfo(sf::RenderWindow& window, const Anthill& anthill) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);

    std::string info = "Anthill Level: " + std::to_string(anthill.get_level()) +
        "\nFood: " + std::to_string(anthill.get_food_count()) +
        "\nSticks: " + std::to_string(anthill.get_stick_count()) +
        "\nAnts: " + std::to_string(anthill.get_ant_count()) +
        "\nCapacity: " + std::to_string(anthill.get_capacity());

    text.setString(info);
    text.setPosition(10, 10);

    window.draw(text);
}

// ������� ��� ����������� ��������� �� ����������
void drawControls(sf::RenderWindow& window) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(12);
    text.setFillColor(sf::Color(200, 200, 200));

    std::string controls = "Controls:\n"
        "  WASD - Pan Camera\n"
        "  Mouse Wheel - Zoom In/Out\n"
        "  Space - Pause/Resume\n"
        "  R - Reset Zoom\n"
        "  Esc - Exit";

    text.setString(controls);
    text.setPosition(10, WINDOW_HEIGHT - 90);

    window.draw(text);
}

int main() {
    // ������������� ���������� ��������� �����
    srand(static_cast<unsigned int>(time(nullptr)));

    // ������� ����
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ant Colony Simulation");
    window.setFramerateLimit(60);

    // ��������� �������
    if (!loadResources()) {
        return -1;
    }

    // ������� ���������� � ������ ������
    Position anthill_pos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    Anthill anthill(anthill_pos);

    // ����������� ��� ��� ������
    sf::View view = window.getDefaultView();

    // ���� ��� �����
    bool paused = false;

    // ������� ���� ����������
    sf::Clock clock;
    float dt = 0.0f;

    while (window.isOpen()) {
        float elapsed = clock.restart().asSeconds();
        dt += elapsed;

        // ��������� �������
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Space) {
                    paused = !paused;
                    std::cout << (paused ? "Simulation paused" : "Simulation resumed") << std::endl;
                }
                else if (event.key.code == sf::Keyboard::R) {
                    // ����� ����
                    view = window.getDefaultView();
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                // ��� � ������� �������� ����
                if (event.mouseWheelScroll.delta > 0) {
                    view.zoom(1.0f / ZOOM_FACTOR);
                }
                else if (event.mouseWheelScroll.delta < 0) {
                    view.zoom(ZOOM_FACTOR);
                }
            }
        }

        // ����������� ������ � ����������
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            view.move(0, -PAN_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            view.move(0, PAN_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            view.move(-PAN_SPEED, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            view.move(PAN_SPEED, 0);
        }

        // ���������� ��������� � ������������� �����
        const float fixedStep = 1.0f / 60.0f;
        if (!paused && dt >= fixedStep) {
            dt -= fixedStep;

            // ��������� ����������
            anthill.update();

            // ��������� ������� ��������
            for (auto& ant : anthill.get_ants()) {
                updateAntSprite(ant);
            }
        }

        // ���������
        window.clear(sf::Color(50, 50, 50)); // �����-����� ���
        window.setView(view);

        // ������ ��������
        const std::vector<Pheromone>& pheromones = anthill.get_pheromones();
        for (const Pheromone& pheromone : pheromones) {
            if (pheromone.is_active()) {
                sf::CircleShape pheromone_shape = createPheromoneShape(pheromone);
                window.draw(pheromone_shape);
            }
        }

        // ������ �������
        const std::vector<Resource>& resources = anthill.get_resources();
        for (const Resource& resource : resources) {
            if (!resource.is_collected()) {
                sf::Sprite resource_sprite = createResourceSprite(resource);
                window.draw(resource_sprite);
            }
        }

        // ������ ������
        const std::vector<Enemy>& enemies = anthill.get_enemies();
        for (const Enemy& enemy : enemies) {
            if (!enemy.isDead()) {
                sf::Sprite enemy_sprite = createEnemySprite(enemy);
                window.draw(enemy_sprite);
            }
        }

        // ������ ����������
        window.draw(anthill.get_shape());

        // ������ ��������
        for (const auto& ant : anthill.get_ants()) {
            if (!ant->has_died()) {
                window.draw(ant->get_sprite());
            }
        }

        // ���������� ����������� ��� ��� UI
        window.setView(window.getDefaultView());

        // ������ ���������� � ����������� � ���������
        drawAnthillInfo(window, anthill);
        drawControls(window);

        // ���������� ��������� �����, ���� �����
        if (paused) {
            sf::Text pause_text;
            pause_text.setFont(font);
            pause_text.setCharacterSize(24);
            pause_text.setFillColor(sf::Color::Red);
            pause_text.setString("PAUSED");
            sf::FloatRect textRect = pause_text.getLocalBounds();
            pause_text.setOrigin(textRect.width / 2, textRect.height / 2);
            pause_text.setPosition(WINDOW_WIDTH / 2, 30);
            window.draw(pause_text);
        }

        // ���������� ��� ���������
        window.display();
    }

    return 0;
}