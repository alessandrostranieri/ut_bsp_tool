#include "worldline.h"

#include <QJsonArray>

WorldLine::WorldLine(QLine line, QPoint normal)
    : line_(line), normal_(normal) {}

const QPoint WorldLine::p0() const { return line_.p1(); }

const QPoint WorldLine::p1() const { return line_.p2(); }

const QPoint WorldLine::normal() const { return normal_; }

bool operator==(const WorldLine& lhs, const WorldLine& rhs) {
  return lhs.line_ == rhs.line_ && lhs.normal_ == rhs.normal_;
}

bool operator!=(const WorldLine& lhs, const WorldLine& rhs) {
  return lhs.line_ != rhs.line_ || lhs.normal_ != rhs.normal_;
}

void WorldLine::read(const QJsonObject &json) {
    id_ = json["id"].toInt();
    auto from = json["from"].toArray();
    line_.setP1({from[0].toInt(), from[1].toInt()});
    auto to = json["to"].toArray();
    line_.setP2({to[0].toInt(), to[1].toInt()});
    auto normal = json["normal"].toArray();
    normal_.setX(normal[0].toInt());
    normal_.setY(normal[1].toInt());
}

std::uint32_t WorldLine::id() const {
    return id_;
}
