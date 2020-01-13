#ifndef WORLDLINE_H
#define WORLDLINE_H

#include <QLine>
#include <QPoint>
#include <QJsonObject>

class WorldLine
{
public:
  WorldLine() = default;

  WorldLine(QLine line, QPoint normal);

  std::uint32_t id() const;
  const QPoint p0() const;
  const QPoint p1() const;
  const QPoint normal() const;

  void read(const QJsonObject& json);

  friend bool operator==(const WorldLine& lhs, const WorldLine& rhs);
  friend bool operator!=(const WorldLine& lhs, const WorldLine& rhs);

private:
  std::uint32_t id_;
  QLine line_;
  QPoint normal_;
};

#endif // WORLDLINE_H
