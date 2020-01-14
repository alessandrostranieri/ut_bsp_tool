//
// Created by astranieri on 09/01/2020.
//

#include "player.h"

QPoint Player::position() const
{
    return position_;
}

void Player::read(const QJsonObject& json_object)
{
    position_.setX(json_object["x"].toInt());
    position_.setY(json_object["y"].toInt());
}

void Player::write(QJsonObject& json_object) const
{
    json_object["x"] = position_.x();
    json_object["y"] = position_.y();
}
