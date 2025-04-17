#include "Cemetery.h"
Cemetery* Cemetery::current_cemetery = nullptr;

bool Cemetery::in_cemetery(const Position& position) const
{
    return position.x >= pos.x && position.x <= pos.x + shape.getSize().x &&
        position.y >= pos.y && position.y <= pos.y + shape.getSize().y;
}