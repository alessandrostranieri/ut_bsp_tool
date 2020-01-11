#ifndef BSP_H
#define BSP_H

#include "worldline.h"

#include <memory>
#include <optional>

enum class PointWrtLine { FRONT, BACK, COLINEAR };

PointWrtLine PointOnSide(const QPoint& point, const WorldLine& word_line);

enum class LineWrtLine { FRONT, BACK, INTERSECT };

LineWrtLine LineOnSide(const WorldLine& line, const WorldLine& dividing_line);

std::optional<size_t> SelectBestSplitter(const std::vector<WorldLine>& lines);

struct SplitResult {
  WorldLine front;
  WorldLine back;
};

SplitResult SplitLine(const WorldLine& line, const WorldLine& split_line);

struct BspNode {
  std::vector<WorldLine> lines;
  std::optional<WorldLine> split_line;
  std::unique_ptr<BspNode> left_node;
  std::unique_ptr<BspNode> right_node;
};

BspNode BuildBspTree(std::vector<WorldLine> lines);

struct PartitionResult {
  std::vector<WorldLine> front;
  std::vector<WorldLine> back;
};

PartitionResult PartitionLines(std::vector<WorldLine> lines,
                               const WorldLine& split_line);

#endif // BSP_H
