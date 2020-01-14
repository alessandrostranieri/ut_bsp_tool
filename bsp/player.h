//
// Created by astranieri on 09/01/2020.
//

#ifndef UT_BSP_TOOL_PLAYER_H
#define UT_BSP_TOOL_PLAYER_H

#include <QJsonObject>
#include <QPoint>

class Player
{

  public:
    Player()
    {
    }

    Player(QPoint position) : position_(position)
    {
    }

    QPoint position() const;

    void read(const QJsonObject& json_object);

    void write(QJsonObject& json_object) const;

  private:
    QPoint position_;
};

#endif // UT_BSP_TOOL_PLAYER_H
