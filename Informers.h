#pragma once

#include "Ant.h"
#include "Resource.h"
#include "Position.h"
#include <vector>
#include <cmath>

// Класс Informers реализован как набор статических функций,
// работающих с уже существующими коллекциями объектов типа Ant и Resource.
// Здесь мы не храним никаких указателей или reference_wrapper – используем обычные вектора по значению.
// Важно: Изменения, внесённые в объекты через ссылки (из элементов вектора), будут отражаться
// на тех объектах, которые находятся в этих векторах (при условии, что вектора не копируются).
class Informers {
public:
    // Функция update() проходит по всем ресурсам и для каждого, если он виден и имеет ненулевое количество,
    // определяет требуемую роль для его сбора (например, food → роль 2, stick → роль 3, body/trash → роль 6).
    // Для каждого муравья с требуемой ролью, если он свободен (без цели) и находится в зоне видения ресурса,
    // назначается задание (цель = позиция ресурса). Если ресурс «большой» (его количество больше get_request_ants()),
    // дополнительно ищется еще один свободный муравей для его сбора.
    static void update(std::vector<Ant>& ants, std::vector<Resource>& resources) {
        for (Resource& res : resources) {
            if (!res.is_visible() || res.get_quantity() <= 0)
                continue;

            // Определяем, какая роль нужна для сбора данного ресурса.
            int requiredRole = -1;
            if (res.get_type() == food)
                requiredRole = 2;
            else if (res.get_type() == stick)
                requiredRole = 3;
            else if (res.get_type() == body || res.get_type() == trash)
                requiredRole = 6;
            if (requiredRole == -1)
                continue;

            // Перебираем всех муравьёв.
            for (Ant& ant : ants) {
                // Если роль муравья соответствует требуемой и у муравья ещё нет задачи.
                if (ant.get_role() != requiredRole || ant.has_valid_target())
                    continue;

                // Вычисляем расстояние между муравьём и ресурсом.
                Position antPos = ant.get_pos();
                Position resPos = res.get_posit();
                float dx = antPos.x - resPos.x;
                float dy = antPos.y - resPos.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                float vision = ant.get_vision_circle().getRadius();

                if (distance <= vision) {
                    // Если муравей видит ресурс, назначаем ему задачу – собираем ресурс.
                    ant.set_target(res.get_posit());

                    // Если ресурс "большой" (его количество больше порогового значения),
                    // пробуем назначить дополнительного муравья (помощника) из числа свободных.
                    if (res.get_quantity() > res.get_request_ants()) {
                        for (Ant& helper : ants) {
                            if (&helper == &ant)
                                continue;
                            if (helper.get_role() == requiredRole && !helper.has_valid_target()) {
                                Position hPos = helper.get_pos();
                                float hdx = hPos.x - resPos.x;
                                float hdy = hPos.y - resPos.y;
                                float hdist = std::sqrt(hdx * hdx + hdy * hdy);
                                if (hdist <= helper.get_vision_circle().getRadius()) {
                                    helper.set_target(res.get_posit());
                                    break; // Назначаем только одного помощника.
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};
