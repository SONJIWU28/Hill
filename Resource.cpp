#include "Resource.h"
int Resource::count = 0;

void create_cluster(vector<Resource>& resources, float x, float y, res_type type)
{
    int res_in_cluster = rand() % max_resource_in_claster + 5;
    for (int i = 0; i < res_in_cluster; ++i) {
        float shift_x = (rand() % dist_btw_res) - dist_btw_res / 2;
        float shift_y = (rand() % dist_btw_res) - dist_btw_res / 2;
        int n = rand() % 100;
        res_size size;
        if (n < probability_of_small_resources) { size = small; }
        else if (n < probability_of_small_resources + probability_of_medium_resources) { size = medium; }
        else { size = big; }
        Resource res(type, size);
        res.set_posit(x + shift_x, y + shift_y);
        res.set_color(type);
        res.set_shape_size(size);
        resources.push_back(res);
    }
}

void Resource::set_color(res_type type)
{
    switch (type) {
    case food:shape.setFillColor(Color(130, 200, 130)); break;
    case stick:shape.setFillColor(Color(139, 90, 50)); break;
    case body:shape.setFillColor(Color(180, 180, 180)); break;
    case trash:shape.setFillColor(Color(170, 170, 100)); break;
    }
}

void Resource::decrease_quantity(int amount) {
    // только для палок и еды!
    if (type != food && type != stick) return;

    quantity -= amount;
    if (quantity < 0) quantity = 0;

    float radius = 0.0f;
    float k = 1.0f;
    if (size == small) radius = small_resource_size;
    else if (size == medium) {
        radius = madium_resource_size;
        k = static_cast<float>(quantity) / 3.0f;
    }
    else if (size == big) {
        radius = big_resource_size;
        k = static_cast<float>(quantity) / 7.0f;
    }

    shape.setRadius(std::max(1.f, k * radius));
    shape.setPosition(pos.x, pos.y);

    if (quantity == 0) set_invisible();
}
