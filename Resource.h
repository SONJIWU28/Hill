#pragma once
#include "Game Settings.h"
#include "Position.h"
class Resource {
private:
    res_type type;
    res_size size;
    Position pos;
    CircleShape shape;
    int quantity;
    int id, ants_for_collecting;
    bool visible = true;
public:
    static int count;
    Resource(res_type t = no_res, res_size s = small, Position p = Position()) : quantity(1), id(count++), size(s), type(t), pos(p), visible(true) {
        shape.setPointCount(count_of_angles_in_form);
        ants_for_collecting = size * 2;
        shape.setRadius(small_resource_size);
        shape.setFillColor(Color(0, 153, 0));
        shape.setPosition(pos.x, pos.y);
        if (s == medium) quantity = 3;
        else if (s == big) quantity = 7;
    };
    void decrease_quantity(int amount);
    void set_posit(float a, float b) { pos.x = a; pos.y = b; shape.setPosition(a, b); }
    void set_invisible() { visible = false; }
    void set_color(res_type type);
    void set_shape_size(int x) { shape.setRadius(x); }

    const CircleShape& get_shape() const { return shape; }
    res_type get_type() const { return type; }
    res_size get_size() const { return size; }
    Position get_posit() const { return pos; }
    bool is_visible() const { return visible; }
    int get_quantity() const { return quantity; }
    int get_request_ants() const { return ants_for_collecting; }
    int get_id() const { return id; }
    int get_count() const { return count; }
};

void create_cluster(vector<Resource>& resources, float x, float y, res_type type);