//
// Created by astranieri on 13/01/2020.
//

#ifndef UT_BSP_TOOL_WORLD_H
#define UT_BSP_TOOL_WORLD_H

#include "player.h"
#include "polygon.h"

#include <vector>
#include <QJsonObject>

class World {
public:
    World();

    const Player& get_player() const;
    const std::vector<Polygon>& get_polygons() const;

    std::vector<WorldLine> all_lines() const;

    void read(const QJsonObject& json);
private:
    Player player_;
    std::vector<Polygon> polygons_;
};


#endif //UT_BSP_TOOL_WORLD_H
