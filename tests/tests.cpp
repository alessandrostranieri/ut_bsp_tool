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
