#include <gmock/gmock.h>

#include "bsp/bsp.h"
#include "bsp/worldline.h"

using namespace testing;

TEST(BspTests, TestPointOnSide_01) {
  auto line = WorldLine{QLine{QPoint{0, 0}, QPoint{3, 2}}, QPoint{2, -3}};
  auto point = QPoint{3, 1};

  auto side = PointOnSide(point, line);

  EXPECT_EQ(side, PointWrtLine::FRONT);
}

TEST(BspTests, TestLineOnSide_Front) {
  auto line = WorldLine{QLine{QPoint{4, 2}, QPoint{7, 1}}, QPoint{}};
  auto dividing_line =
      WorldLine{QLine{QPoint{1, 1}, QPoint{5, 5}}, QPoint{1, -1}};

  auto side = LineOnSide(line, dividing_line);

  EXPECT_EQ(side, LineWrtLine::FRONT);
}

TEST(BspTests, TestLineOnSide_Back) {
  auto line = WorldLine{QLine{QPoint{3, 4}, QPoint{1, 5}}, QPoint{}};
  auto dividing_line =
      WorldLine{QLine{QPoint{1, 1}, QPoint{5, 5}}, QPoint{1, -1}};

  auto side = LineOnSide(line, dividing_line);

  EXPECT_EQ(side, LineWrtLine::BACK);
}

TEST(BspTests, TestLineOnSide_Split) {
  auto line = WorldLine{QLine{QPoint{8, 2}, QPoint{7, 5}}, QPoint{}};
  auto dividing_line =
      WorldLine{QLine{QPoint{1, 1}, QPoint{6, 3}}, QPoint{-2, 5}};

  auto side = LineOnSide(line, dividing_line);

  EXPECT_EQ(side, LineWrtLine::INTERSECT);
}

TEST(BspTests, TestBestSplit) {
  // A
  auto line_01 = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};
  auto line_02 = WorldLine{QLine{QPoint{8, 8}, QPoint{8, 10}}, QPoint{1, 0}};
  auto line_03 = WorldLine{QLine{QPoint{8, 10}, QPoint{2, 10}}, QPoint{0, 1}};
  auto line_04 = WorldLine{QLine{QPoint{2, 10}, QPoint{2, 8}}, QPoint{-1, 0}};
  // B
  auto line_05 = WorldLine{QLine{QPoint{2, 0}, QPoint{6, 4}}, QPoint{1, -1}};
  auto line_06 = WorldLine{QLine{QPoint{6, 4}, QPoint{6, 6}}, QPoint{1, 0}};
  auto line_07 = WorldLine{QLine{QPoint{6, 6}, QPoint{2, 2}}, QPoint{-1, 1}};
  auto line_08 = WorldLine{QLine{QPoint{2, 2}, QPoint{2, 0}}, QPoint{-1, 0}};

  const std::vector<WorldLine> lines{line_01, line_02, line_03, line_04,
                                     line_05, line_06, line_07, line_08};
  auto best_splitter = SelectBestSplitter(lines);
  EXPECT_EQ(best_splitter, std::optional<size_t>{0});
}
