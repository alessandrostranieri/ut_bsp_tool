#include <gmock/gmock.h>

#include "bsp/bsp.h"
#include "bsp/world.h"
#include "bsp/worldline.h"

#include <QFile>
#include <QJsonDocument>

using namespace testing;

TEST(BspTests, TestPointOnSide_01)
{
    auto line = WorldLine{QLine{QPoint{0, 0}, QPoint{3, 2}}, QPoint{2, -3}};
    auto point = QPoint{3, 1};

    auto side = PointOnSide(point, line);

    EXPECT_EQ(side, PointWrtLine::FRONT);
}

TEST(BspTests, TestLineOnSide_Front)
{
    auto line = WorldLine{QLine{QPoint{4, 2}, QPoint{7, 1}}, QPoint{}};
    auto dividing_line = WorldLine{QLine{QPoint{1, 1}, QPoint{5, 5}}, QPoint{1, -1}};

    auto side = LineOnSide(line, dividing_line);

    EXPECT_EQ(side, LineWrtLine::FRONT);
}

TEST(BspTests, TestLineOnSide_Back)
{
    auto line = WorldLine{QLine{QPoint{3, 4}, QPoint{1, 5}}, QPoint{}};
    auto dividing_line = WorldLine{QLine{QPoint{1, 1}, QPoint{5, 5}}, QPoint{1, -1}};

    auto side = LineOnSide(line, dividing_line);

    EXPECT_EQ(side, LineWrtLine::BACK);
}

TEST(BspTests, TestLineOnSide_JoinedLines_01)
{
    auto line = WorldLine{QLine{QPoint{2, 8}, QPoint{2, 10}}, QPoint{-1, 0}};
    auto dividing_line = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};

    auto side = LineOnSide(line, dividing_line);

    EXPECT_EQ(side, LineWrtLine::BACK);
}

TEST(BspTests, TestLineOnSide_JoinedLines_02)
{
    auto line = WorldLine{QLine{QPoint{8, 8}, QPoint{8, 10}}, QPoint{1, 0}};
    auto dividing_line = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};

    auto side = LineOnSide(line, dividing_line);

    EXPECT_EQ(side, LineWrtLine::BACK);
}

TEST(BspTests, TestLineOnSide_Split)
{
    auto line = WorldLine{QLine{QPoint{8, 2}, QPoint{7, 5}}, QPoint{}};
    auto dividing_line = WorldLine{QLine{QPoint{1, 1}, QPoint{6, 3}}, QPoint{-2, 5}};

    auto side = LineOnSide(line, dividing_line);

    EXPECT_EQ(side, LineWrtLine::INTERSECT);
}

TEST(BspTests, TestBestSplit_NormalCase)
{
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

    const std::vector<WorldLine> lines{line_01, line_02, line_03, line_04, line_05, line_06, line_07, line_08};
    auto best_splitter = SelectBestSplitter(lines);
    EXPECT_EQ(best_splitter, std::optional<size_t>{0});
}

TEST(BspTests, TestBestSplit_OneLine)
{
    auto line = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};

    const std::vector<WorldLine> lines{line};
    auto best_splitter = SelectBestSplitter(lines);
    EXPECT_EQ(best_splitter, std::optional<size_t>{});
}

TEST(BspTests, TestCutLine_P0_in_front)
{
    auto line = WorldLine{QLine{QPoint{8, 2}, QPoint{7, 5}}, QPoint{3, 1}};
    auto dividing_line = WorldLine{QLine{QPoint{1, 1}, QPoint{6, 3}}, QPoint{2, -5}};

    SplitResult result = SplitLine(line, dividing_line);
    EXPECT_EQ(result.front, WorldLine(QLine(QPoint(8, 2), QPoint(7, 4)), QPoint(3, 1)));
    EXPECT_EQ(result.back, WorldLine(QLine(QPoint(7, 4), QPoint(7, 5)), QPoint(3, 1)));
}

TEST(BspTests, TestCutLine_P1_in_front)
{
    auto line = WorldLine{QLine{QPoint{8, 2}, QPoint{7, 5}}, QPoint{3, 1}};
    auto dividing_line = WorldLine{QLine{QPoint{1, 1}, QPoint{6, 3}}, QPoint{-2, 5}};

    SplitResult result = SplitLine(line, dividing_line);
    EXPECT_EQ(result.front, WorldLine(QLine(QPoint(7, 4), QPoint(7, 5)), QPoint(3, 1)));
    EXPECT_EQ(result.back, WorldLine(QLine(QPoint(8, 2), QPoint(7, 4)), QPoint(3, 1)));
}

TEST(BspTests, TestPartitionLines)
{
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
    const std::vector<WorldLine> lines{line_01, line_02, line_03, line_04, line_05, line_06, line_07, line_08};
    const auto splitter = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};
    const PartitionResult partition_result = PartitionLines(lines, splitter);

    EXPECT_THAT(partition_result.back, UnorderedElementsAreArray({line_02, line_03, line_04}));
    EXPECT_THAT(partition_result.front, UnorderedElementsAreArray({line_05, line_06, line_07, line_08}));
}

TEST(BspTests, TestBuildBspTree_NoLines)
{
    auto bspNode = BuildBspTree(std::vector<WorldLine>{});

    EXPECT_THAT(bspNode, IsNull());
}

TEST(BspTests, TestBuildBspTree_OneLine)
{

    auto line_01 = WorldLine{QLine{QPoint{2, 8}, QPoint{8, 8}}, QPoint{0, -1}};
    const std::vector<WorldLine> lines{line_01};

    auto bspNode = BuildBspTree(lines);

    EXPECT_FALSE(bspNode->split_line.has_value());
    EXPECT_EQ(bspNode->lines, lines);
    EXPECT_THAT(bspNode->front_node, IsNull());
    EXPECT_THAT(bspNode->back_node, IsNull());
}

TEST(BspTests, TestBuildBspTree_TwoLines)
{
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

TEST(BspTests, TestBuildBspTree_ThreeLines)
{
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

TEST(BspTests, TestBuildBspTree_TwoShapes)
{
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
    const std::vector<WorldLine> lines{line_01, line_02, line_03, line_04, line_05, line_06, line_07, line_08};

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

TEST(BspTests, TestBuildBspTree_WithDemoWorld)
{
    // LOAD WORLD FROM FILE
    QFile in_file(QStringLiteral(":/worlds/world_01.json"));
    ASSERT_TRUE(in_file.open(QIODevice::ReadOnly));

    const QByteArray saveData = in_file.readAll();
    const QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    const QJsonObject world_json = loadDoc.object();
    World world;
    world.read(world_json);

    // COLLECT LINES
    std::vector<WorldLine> lines = world.all_lines();

    // TEST TREE
    auto node_01 = BuildBspTree(lines);

    EXPECT_THAT(node_01, NotNull());
    EXPECT_THAT(node_01->split_line->id(), Eq(7));

    auto& node_02 = node_01->front_node;
    EXPECT_THAT(node_02->split_line->id(), Eq(6));

    auto& node_03 = node_01->back_node;
    EXPECT_THAT(node_03->split_line->id(), Eq(10));

    auto& node_04 = node_02->front_node;
    EXPECT_THAT(node_04, IsNull());

    auto& node_05 = node_02->back_node;
    EXPECT_THAT(node_05->split_line->id(), Eq(1));

    auto& node_06 = node_03->front_node;
    EXPECT_THAT(node_06->split_line->id(), Eq(11));

    auto& node_07 = node_03->back_node;
    EXPECT_THAT(node_07->split_line->id(), Eq(8));

    auto& node_10 = node_05->front_node;
    EXPECT_THAT(node_10, IsNull());

    auto& node_11 = node_05->back_node;
    EXPECT_THAT(node_11->split_line->id(), Eq(4));

    auto& node_12 = node_06->front_node;
    EXPECT_THAT(node_12, IsNull());

    auto& node_13 = node_06->back_node;
    EXPECT_THAT(node_13->split_line->id(), Eq(16));

    auto& node_14 = node_07->front_node;
    EXPECT_THAT(node_14, IsNull());

    auto& node_15 = node_07->back_node;
    EXPECT_THAT(node_15->lines.front().id(), Eq(9));
    EXPECT_THAT(node_15->front_node, IsNull());
    EXPECT_THAT(node_15->back_node, IsNull());

    auto& node_22 = node_11->front_node;
    EXPECT_THAT(node_22->lines.front().id(), Eq(5));

    auto& node_23 = node_11->back_node;
    EXPECT_THAT(node_23->split_line->id(), Eq(2));

    auto& node_26 = node_13->front_node;
    EXPECT_THAT(node_26, IsNull());

    auto& node_27 = node_13->back_node;
    EXPECT_THAT(node_27->split_line->id(), Eq(13));

    auto& node_46 = node_23->front_node;
    EXPECT_THAT(node_46, IsNull());

    auto& node_47 = node_23->back_node;
    EXPECT_THAT(node_47->lines.front().id(), Eq(3));
    EXPECT_THAT(node_47->front_node, IsNull());
    EXPECT_THAT(node_47->back_node, IsNull());

    auto& node_54 = node_27->front_node;
    EXPECT_THAT(node_54->lines.front().id(), Eq(12));

    auto& node_55 = node_27->back_node;
    EXPECT_THAT(node_55->split_line->id(), Eq(15));

    auto& node_110 = node_55->front_node;
    EXPECT_THAT(node_110, IsNull());

    auto& node_111 = node_55->back_node;
    EXPECT_THAT(node_111->lines.front().id(), Eq(14));
    EXPECT_THAT(node_111->front_node, IsNull());
    EXPECT_THAT(node_111->back_node, IsNull());
}

TEST(BspTests, TestWalkBspTree_WithDemoWorld)
{
    // LOAD WORLD FROM FILE
    QFile in_file(QStringLiteral(":/worlds/world_01.json"));
    ASSERT_TRUE(in_file.open(QIODevice::ReadOnly));

    const QByteArray saveData = in_file.readAll();
    const QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    const QJsonObject world_json = loadDoc.object();
    World world;
    world.read(world_json);

    // COLLECT LINES
    std::vector<WorldLine> lines = world.all_lines();
    const auto node_01 = BuildBspTree(lines);
    const auto player = world.get_player();

    std::vector<WorldLine> ordered_lines;
    WalkBspTree(player, node_01, ordered_lines);

    // NODES 1 AND 13 ARE COLINEAR AND ARE NOT RENDERED
    EXPECT_EQ(ordered_lines[0].id(), 8);
    EXPECT_EQ(ordered_lines[1].id(), 9);
    EXPECT_EQ(ordered_lines[2].id(), 10);
    EXPECT_EQ(ordered_lines[3].id(), 11);
    EXPECT_EQ(ordered_lines[4].id(), 12);
    EXPECT_EQ(ordered_lines[5].id(), 15);
    EXPECT_EQ(ordered_lines[6].id(), 14);
    EXPECT_EQ(ordered_lines[7].id(), 16);
    EXPECT_EQ(ordered_lines[8].id(), 7);
    EXPECT_EQ(ordered_lines[9].id(), 2);
    EXPECT_EQ(ordered_lines[10].id(), 3);
    EXPECT_EQ(ordered_lines[11].id(), 4);
    EXPECT_EQ(ordered_lines[12].id(), 5);
    EXPECT_EQ(ordered_lines[13].id(), 6);
}
