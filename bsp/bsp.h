#ifndef BSP_H
#define BSP_H

#include "worldline.h"

enum class PointWrtLine { FRONT, BACK, COLINEAR };

PointWrtLine PointOnSide(const WorldLine& word_line, const QPoint point);

#endif // BSP_H
