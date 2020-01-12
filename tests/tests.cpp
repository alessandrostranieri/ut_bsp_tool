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

TEST(BspTests, TestLineOnSide_JoinedLines_01) {
  auto line = WorldLine{QLine{QPoint{2, 8}, QPoint{2, 10}}, QPoint{-1, 0}};
  auto dividing_line =
      WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};

  auto side = LineOnSide(line, dividing_line);

  EXPECT_EQ(side, LineWrtLine::BACK);
}

TEST(BspTests, TestLineOnSide_JoinedLines_02) {
  auto line = WorldLine{QLine{QPoint{8, 8}, QPoint{8, 10}}, QPoint{1, 0}};
  auto dividing_line =
      WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};

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

TEST(BspTests, TestBestSplit_NormalCase) {
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

TEST(BspTests, TestBestSplit_OneLine) {
  auto line = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};

  const std::vector<WorldLine> lines{line};
  auto best_splitter = SelectBestSplitter(lines);
  EXPECT_EQ(best_splitter, std::optional<size_t>{});
}

TEST(BspTests, TestCutLine_P0_in_front) {
  auto line = WorldLine{QLine{QPoint{8, 2}, QPoint{7, 5}}, QPoint{3, 1}};
  auto dividing_line =
      WorldLine{QLine{QPoint{1, 1}, QPoint{6, 3}}, QPoint{2, -5}};

  SplitResult result = SplitLine(line, dividing_line);
  EXPECT_EQ(result.front,
            WorldLine(QLine(QPoint(8, 2), QPoint(7, 4)), QPoint(3, 1)));
  EXPECT_EQ(result.back,
            WorldLine(QLine(QPoint(7, 4), QPoint(7, 5)), QPoint(3, 1)));
}

TEST(BspTests, TestCutLine_P1_in_front) {
  auto line = WorldLine{QLine{QPoint{8, 2}, QPoint{7, 5}}, QPoint{3, 1}};
  auto dividing_line =
      WorldLine{QLine{QPoint{1, 1}, QPoint{6, 3}}, QPoint{-2, 5}};

  SplitResult result = SplitLine(line, dividing_line);
  EXPECT_EQ(result.front,
            WorldLine(QLine(QPoint(7, 4), QPoint(7, 5)), QPoint(3, 1)));
  EXPECT_EQ(result.back,
            WorldLine(QLine(QPoint(8, 2), QPoint(7, 4)), QPoint(3, 1)));
}

TEST(BspTests, TestPartitionLines) {
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
  // LIST OF LINES
  const std::vector<WorldLine> lines{line_01, line_02, line_03, line_04,
                                     line_05, line_06, line_07, line_08};
  const auto splitter =
      WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};
  const PartitionResult partition_result = PartitionLines(lines, splitter);

  EXPECT_THAT(partition_result.back,
              UnorderedElementsAreArray({line_02, line_03, line_04}));
  EXPECT_THAT(partition_result.front,
              UnorderedElementsAreArray({line_05, line_06, line_07, line_08}));
}

TEST(BspTests, TestBuildBspTree_NoLines) {
  auto bspNode = BuildBspTree(std::vector<WorldLine>{});

  EXPECT_THAT(bspNode, IsNull());
}

TEST(BspTests, TestBuildBspTree_OneLine) {

  auto line_01 = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};
  const std::vector<WorldLine> lines{line_01};

  auto bspNode = BuildBspTree(lines);

  EXPECT_FALSE(bspNode->split_line.has_value());
  EXPECT_EQ(bspNode->lines, lines);
  EXPECT_THAT(bspNode->front_node, IsNull());
  EXPECT_THAT(bspNode->back_node, IsNull());
}

TEST(BspTests, TestBuildBspTree_TwoLines) {
  // A
  auto line_01 = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};
  auto line_02 = WorldLine{QLine{QPoint{8, 8}, QPoint{8, 10}}, QPoint{1, 0}};
  // LIST OF LINES
  const std::vector<WorldLine> lines{line_01, line_02};

  auto bspNode = BuildBspTree(lines);

  EXPECT_EQ(bspNode->split_line, std::optional<WorldLine>(line_01));
  EXPECT_TRUE(bspNode->lines.empty());
  EXPECT_THAT(bspNode->front_node, IsNull());
  EXPECT_THAT(bspNode->back_node, NotNull());

  const BspNode& back_01 = *bspNode->back_node;
  EXPECT_EQ(back_01.split_line, std::optional<WorldLine>{});
  EXPECT_EQ(back_01.lines, std::vector<WorldLine>{line_02});
  EXPECT_THAT(back_01.front_node, IsNull());
  EXPECT_THAT(back_01.back_node, IsNull());
}

TEST(BspTests, TestBuildBspTree_ThreeLines) {
  // A
  auto line_01 = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};
  auto line_02 = WorldLine{QLine{QPoint{8, 8}, QPoint{8, 10}}, QPoint{1, 0}};
  auto line_03 = WorldLine{QLine{QPoint{8, 10}, QPoint{2, 10}}, QPoint{0, 1}};

  // LIST OF LINES
  const std::vector<WorldLine> lines{line_01, line_02, line_03};

  auto node_01 = BuildBspTree(lines);

  EXPECT_EQ(node_01->split_line, std::optional<WorldLine>(line_01));
  EXPECT_TRUE(node_01->lines.empty());
  EXPECT_THAT(node_01->front_node, IsNull());
  EXPECT_THAT(node_01->back_node, NotNull());

  const BspNode& node_02 = *node_01->back_node;
  EXPECT_EQ(node_02.split_line, std::optional<WorldLine>{line_03});
  EXPECT_THAT(node_02.lines, IsEmpty());
  EXPECT_THAT(node_02.front_node, IsNull());
  EXPECT_THAT(node_02.back_node, NotNull());

  const BspNode& node_05 = *node_02.back_node;
  EXPECT_THAT(node_05.split_line, Eq(std::optional<WorldLine>{}));
  EXPECT_THAT(node_05.lines, Contains(line_02));
  EXPECT_THAT(node_05.front_node, IsNull());
  EXPECT_THAT(node_05.back_node, IsNull());
}

TEST(BspTests, TestBuildBspTree_TwoShapes) {
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
  // LIST OF LINES
  const std::vector<WorldLine> lines{line_01, line_02, line_03, line_04,
                                     line_05, line_06, line_07, line_08};

  auto node_01 = BuildBspTree(lines);

  EXPECT_EQ(node_01->split_line, std::optional<WorldLine>(line_01));
  EXPECT_TRUE(node_01->lines.empty());
  EXPECT_THAT(node_01->front_node, NotNull());
  EXPECT_THAT(node_01->back_node, NotNull());

  const BspNode& node_02 = *node_01->front_node;
  EXPECT_EQ(node_02.split_line, std::optional<WorldLine>{line_08});
  EXPECT_THAT(node_02.lines, IsEmpty());
  EXPECT_THAT(node_02.front_node, IsNull());
  EXPECT_THAT(node_02.back_node, NotNull());

  const BspNode& node_03 = *node_01->back_node;
  EXPECT_THAT(node_03.split_line, Eq(std::optional<WorldLine>{line_04}));
  EXPECT_THAT(node_03.lines, IsEmpty());
  EXPECT_THAT(node_03.front_node, IsNull());
  EXPECT_THAT(node_03.back_node, NotNull());

  const BspNode& node_07 = *node_03.back_node;
  EXPECT_THAT(node_07.split_line, Eq(std::optional<WorldLine>{line_02}));
  EXPECT_THAT(node_07.lines, IsEmpty());
  EXPECT_THAT(node_07.front_node, IsNull());
  EXPECT_THAT(node_07.back_node, NotNull());

  const BspNode& node_15 = *node_07.back_node;
  EXPECT_THAT(node_15.split_line, Eq(std::optional<WorldLine>{}));
  EXPECT_THAT(node_15.lines, Contains(line_03));
  EXPECT_THAT(node_15.front_node, IsNull());
  EXPECT_THAT(node_15.back_node, IsNull());

  const BspNode& node_05 = *node_02.back_node;
  EXPECT_EQ(node_05.split_line, std::optional<WorldLine>{line_05});
  EXPECT_THAT(node_05.lines, IsEmpty());
  EXPECT_THAT(node_05.front_node, IsNull());
  EXPECT_THAT(node_05.back_node, NotNull());

  const BspNode& node_10 = *node_05.back_node;
  EXPECT_EQ(node_10.split_line, std::optional<WorldLine>{line_07});
  EXPECT_THAT(node_10.lines, IsEmpty());
  EXPECT_THAT(node_10.front_node, IsNull());
  EXPECT_THAT(node_10.back_node, NotNull());

  const BspNode& node_20 = *node_10.back_node;
  EXPECT_EQ(node_20.split_line, std::optional<WorldLine>{});
  EXPECT_THAT(node_20.lines, Contains(line_06));
  EXPECT_THAT(node_20.front_node, IsNull());
  EXPECT_THAT(node_20.back_node, IsNull());
}
