//
// Created by astranieri on 13/01/2020.
//

#include "world.h"

World::World() {}

void World::read(const QJsonObject &json) {
    player_.read(json["player"].toObject());
    polygons_.clear();
    const auto json_array = json["polygons"].toArray();
    for(const auto& polygon_json: json_array){
        Polygon polygon;
        polygon.read(polygon_json.toArray());
        polygons_.push_back(polygon);
    }
}

std::vector<WorldLine> World::all_lines() const {
    std::vector<WorldLine> lines;

    for(const auto& polygon: polygons_){
        const auto& polygon_lines = polygon.lines();
        lines.insert(lines.end(), polygon_lines.begin(), polygon_lines.end());
    }

    return lines;
}

const Player &World::get_player() const {
    return player_;
}

const std::vector<Polygon>& World::get_polygons() const {
    return polygons_;
}
