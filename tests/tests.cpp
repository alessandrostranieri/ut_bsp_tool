#include <gmock/gmock.h>

#include "bsp/bsp.h"
#include "bsp/worldline.h"

using namespace testing;

TEST(BspTests, TestPointOnSide_01) {
  auto line = WorldLine{QLine{QPoint{0, 0}, QPoint{3, 2}}, QPoint{2, -3}};
  auto point = QPoint{3, 1};

  auto side = PointOnSide(line, point);

  EXPECT_EQ(side, PointWrtLine::FRONT);
}
