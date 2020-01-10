#ifndef BSP_H
#define BSP_H

#include "worldline.h"

enum class PointWrtLine { FRONT, BACK, COLINEAR };

PointWrtLine PointOnSide(const QPoint& point, const WorldLine& word_line);

enum class LineWrtLine { FRONT, BACK, INTERSECT };

LineWrtLine LineOnSide(const WorldLine& line, const WorldLine& dividing_line);

#endif // BSP_H
