#include "worldline.h"

WorldLine::WorldLine(QLine line, QPoint normal)
    : line_(line), normal_(normal) {}

const QPoint WorldLine::p0() const { return line_.p1(); }

const QPoint WorldLine::p1() const { return line_.p2(); }

const QPoint WorldLine::normal() const { return normal_; }
