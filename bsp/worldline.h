#ifndef WORLDLINE_H
#define WORLDLINE_H

#include <QLine>
#include <QPoint>

class WorldLine
{
public:
  WorldLine(QLine line, QPoint normal);

  const QPoint p0() const;
  const QPoint p1() const;
  const QPoint normal() const;

private:
  QLine line_;
  QPoint normal_;
};

#endif // WORLDLINE_H
