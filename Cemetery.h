#pragma once
#pragma once
#include "Game Settings.h"
#include "Position.h"

class Cemetery {
private:
    RectangleShape shape;
    Position pos;
    int bodies_count;
    int trash_count;
    inline static Cemetery* current_cemetery = nullptr;

public:
    Cemetery(float x, float y, float width, float height) : bodies_count(0), trash_count(0) {
        pos.x = x;
        pos.y = y;
        shape.setSize(Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(Color(0, 0, 0, 180)); 
        shape.setOutlineColor(Color(50, 50, 50));
        shape.setOutlineThickness(2.0f);
    }

    bool in_cemetery(const Position& position) const {
        return position.x >= pos.x && position.x <= pos.x + shape.getSize().x &&
            position.y >= pos.y && position.y <= pos.y + shape.getSize().y;
    }
    Position get_center() const { return Position(pos.x + shape.getSize().x / 2, pos.y + shape.getSize().y / 2); }

    void add_body() { bodies_count++; }
    void add_trash() { trash_count++; }

    int get_bodies_count() const { return bodies_count; }
    int get_trash_count() const { return trash_count; }

    const RectangleShape& get_shape() const { return shape; }
    static void set_current(Cemetery* cemetery) {current_cemetery = cemetery;}
    static Cemetery* get_current() {return current_cemetery;}
};
