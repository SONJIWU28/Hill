#pragma once
#include "Game Settings.h"
#include "Position.h"

class Cemetery {
private:
    RectangleShape shape;
    Position pos;
    int bodies_count;
    int trash_count;
public:
    static Cemetery* current_cemetery;
    Cemetery() : bodies_count(0), trash_count(0) {
        pos.x = window_width / 2 + 4 * start_radius;
        pos.y = window_height / 2 - start_radius / 2;
        shape.setSize(Vector2f(100, 100));
        shape.setPosition(pos.x, pos.y);
        shape.setFillColor(Color(100, 100, 100, 120));
        shape.setOutlineColor(Color(50, 50, 50));
        shape.setOutlineThickness(2.0f);
    }

    bool in_cemetery(const Position& position) const;
    Position get_center() const { return pos; }

    void add_body() { bodies_count++; }
    void add_trash() { trash_count++; }

    int get_bodies_count() const { return bodies_count; }
    int get_trash_count() const { return trash_count; }

    const RectangleShape& get_shape() const { return shape; }
    static void set_current(Cemetery* cemetery) { current_cemetery = cemetery; }
    static Cemetery* get_current() { return current_cemetery; }
};