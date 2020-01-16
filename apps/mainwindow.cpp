#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bsp/player.h"
#include "bsp/polygon.h"
#include "bsp/world.h"
#include "bsp/bsp.h"
#include "treenode.h"
#include "crosshairgraphicsitem.h"
#include "partitionline.h"

#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRect>
#include <QVector2D>

#include <algorithm>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), world_(nullptr)
{
    ui->setupUi(this);

    world_ = std::make_unique<World>();

    QGraphicsScene* worldScene = new QGraphicsScene;
    ui->world_gv->setScene(worldScene);
    worldScene->setBackgroundBrush(Qt::black);

    QGraphicsScene* algorithmScene = new QGraphicsScene;
    ui->algorithm_gv->setScene(algorithmScene);
    algorithmScene->setBackgroundBrush(Qt::black);
}

void MainWindow::init()
{
    render_world();

    bspTree_ = BuildBspTree(world_->all_lines());

    createPartitionLines();

    createRenderLines();

    ui->nextStepButton->setEnabled(true);
    ui->previousStepButton->setEnabled(false);

    ui->buildBspButton->click();
    initAlgorithmView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

template <typename G>
void toggleNextVisible(std::vector<G*>& lines){
    auto found = std::find_if_not(lines.begin(), lines.end(), [](const auto& gl){return gl->isVisible();});
    if(found != lines.end()){
        (*found)->setVisible(true);
    }
}

void MainWindow::nextStep()
{
    qDebug("Next Step");
    switch(algorithmView_){
    case AlgorithmView::BUILD_BSP:
        toggleNextVisible(partitionLines_);
        break;
    case AlgorithmView::WALK_BSP:
        toggleNextVisible(renderLines_);
    }
    updateStepButton();
}

template <typename G>
void togglePreviousVisible(std::vector<G*>& lines){
    auto found = std::find_if(lines.rbegin(), lines.rend(), [](const auto& gl){return gl->isVisible();});
    if(found != lines.rend()){
        (*found)->setVisible(false);
    }
}

void MainWindow::previousStep()
{
    qDebug("Previous Step");
    switch(algorithmView_){
    case AlgorithmView::BUILD_BSP:
        togglePreviousVisible(partitionLines_);
        break;
    case AlgorithmView::WALK_BSP:
        togglePreviousVisible(renderLines_);
    }
    updateStepButton();
}

std::tuple<size_t, size_t> MainWindow::countVisible(){
    if (algorithmView_ == AlgorithmView::BUILD_BSP){
        const auto count_visible = static_cast<size_t>(std::count_if(partitionLines_.begin(),
                                                                     partitionLines_.end(),
                                                                     [](const auto& gl){ return gl->isVisible();}));
        const auto num_lines = partitionLines_.size();
        return std::make_tuple(count_visible, num_lines);
    } else {
        const auto count_visible = static_cast<size_t>(std::count_if(renderLines_.begin(),
                                                                     renderLines_.end(),
                                                                     [](const auto& gl){ return gl->isVisible();}));
        const auto num_lines = renderLines_.size();
        return std::make_tuple(count_visible, num_lines);
    }
}

void MainWindow::updateStepButton()
{
    auto [count_visible, num_lines] = countVisible();
    if (count_visible == 0) {
        ui->nextStepButton->setEnabled(true);
        ui->previousStepButton->setEnabled(false);
    } else if(count_visible == num_lines){
        ui->nextStepButton->setEnabled(false);
        ui->previousStepButton->setEnabled(true);
    } else {
        ui->nextStepButton->setEnabled(true);
        ui->previousStepButton->setEnabled(true);
    }
}

void MainWindow::toggleBuildView()
{
    algorithmView_ = AlgorithmView::BUILD_BSP;
    qDebug() << to_string(algorithmView_);
    playerMarker_->setVisible(false);
    clearWorldView();
    updateStepButton();
}

void MainWindow::toggleWalkView()
{
    algorithmView_ = AlgorithmView::WALK_BSP;
    qDebug() << to_string(algorithmView_);
    playerMarker_->setVisible(true);
    clearWorldView();
    updateStepButton();
}

void MainWindow::render_world()
{
    // OPEN FILE
    QFile in_file(QStringLiteral(":/worlds/world_01.json"));
    if (!in_file.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open world file.");
    }

    QByteArray worldData = in_file.readAll();
    const QJsonDocument loadDoc(QJsonDocument::fromJson(worldData));
    const QJsonObject world_json = loadDoc.object();
    world_->read(world_json);

    // RENDER PLAYER
    playerMarker_ = new QGraphicsEllipseItem(QRect{world_->get_player().position() - QPoint{5, 5}, world_->get_player().position() + QPoint{5, 5}});
    playerMarker_->setPen(QPen(Qt::yellow));
    playerMarker_->setVisible(false);
    playerMarker_->setBrush(QBrush(Qt::yellow));
    ui->world_gv->scene()->addItem(playerMarker_);

    // RENDER LINES
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(4);
    for (const auto& polygon : world_->get_polygons())
    {
        const auto& lines = polygon.lines();
        for (auto line : lines)
        {
            ui->world_gv->scene()->addLine(QLine(line.p0(), line.p1()), pen);
        }
    }
}

void MainWindow::initAlgorithmView()
{
    auto* crosshairItem = new CrosshairGraphicsItem(QPointF{0, 0});
    ui->algorithm_gv->scene()->addItem(crosshairItem);
}

void MainWindow::createPartitionLines()
{
    auto xMin = ui->world_gv->sceneRect().x();
    auto yMin = ui->world_gv->sceneRect().y();
    auto xMax = xMin + ui->world_gv->sceneRect().width();
    auto yMax = yMin + ui->world_gv->sceneRect().height();
    QLineF top{{xMin,yMin}, {xMax,yMin}};
    QLineF bottom{{xMin,yMax}, {xMax,yMax}};
    QLineF left{{xMin, yMax}, {xMin, yMin}};
    QLineF right{{xMax, yMax}, {xMax, yMin}};

    PartitionLine* topPartition = new PartitionLine(top, nullptr);
    PartitionLine* bottomPartition = new PartitionLine(bottom, topPartition);
    PartitionLine* leftPartition = new PartitionLine(left, bottomPartition);
    PartitionLine* rightPartition = new PartitionLine(right, leftPartition);

    qDebug() << "Partition lines creation started";
    walkBspPartitionLines(bspTree_, partitionLines_, rightPartition);
    qDebug() << "Partition lines creation ended";

    extendPartitionLines();

    clipPartitionLines();

    qDebug() << "Adding partition lines to scene ";
    addPartitionLinesToScene();
    qDebug() << "Partition lines added to scene";
}

void MainWindow::extendPartitionLines()
{
//    auto xMin = ui->world_gv->sceneRect().x();
//    auto yMin = ui->world_gv->sceneRect().y();
//    auto xMax = xMin + ui->world_gv->sceneRect().width();
//    auto yMax = yMin + ui->world_gv->sceneRect().height();
//    QLineF top{{xMin,yMin}, {xMax,yMin}};
//    QLineF bottom{{xMin,yMax}, {xMax,yMax}};
//    QLineF left{{xMin, yMax}, {xMin, yMin}};
//    QLineF right{{xMax, yMax}, {xMax, yMin}};

//    for(auto partition_line: partitionLines_){
//        QLineF line = partition_line->line();
//        qreal angle = line.angle();
//        if(angle == 90.0 || angle == 270.0){
//            QPointF topIntersection;
//            line.intersect(top, &topIntersection);
//            QPointF bottomIntersection;
//            line.intersect(bottom, &bottomIntersection);
//            partition_line->setLine({topIntersection, bottomIntersection});
//        } else {
//            QPointF leftIntersection;
//            line.intersect(left, &leftIntersection);
//            QPointF rightIntersection;
//            line.intersect(right, &rightIntersection);
//            partition_line->setLine({leftIntersection, rightIntersection});
//        }
//    }

//    PartitionLine* topPartition = new PartitionLine(top, nullptr);
//    PartitionLine* bottomPartition = new PartitionLine(bottom, topPartition);
//    PartitionLine* leftPartition = new PartitionLine(left, bottomPartition);
//    PartitionLine* rightPartition = new PartitionLine(right, leftPartition);
}

void MainWindow::clipPartitionLines()
{
    for(auto& partitionLine : partitionLines_){
        QVector2D p1Vec{partitionLine->line().p1()};
        QVector2D p2Vec{partitionLine->line().p2()};
        PartitionLine* parentLine = partitionLine->getParentLine();
        float distGlobalP1 = 1000.0;
        float distGlobalP2 = 1000.0;
        while(parentLine != nullptr){
            // CALCULATE INTERSECTION
            QPointF intersectionPoint;
            auto intersection = partitionLine->line().intersect(parentLine->line(), &intersectionPoint);
            if(intersection != QLineF::IntersectType::NoIntersection){
                auto distP1 = p1Vec.distanceToPoint(QVector2D{intersectionPoint});
                auto distP2 = p2Vec.distanceToPoint(QVector2D{intersectionPoint});
                if (distP1 < distP2 && distP1 < distGlobalP1){
                    distGlobalP1 = distP1;
                    QLineF newLine{intersectionPoint, partitionLine->line().p2()};
                    partitionLine->setLine(newLine);
                } else if (distP2 < distP1 && distP2 < distGlobalP2) {
                    distGlobalP2 = distP2;
                    QLineF newLine{partitionLine->line().p1(), intersectionPoint};
                    partitionLine->setLine(newLine);
                }
            }

            // NEXT
            parentLine = parentLine->getParentLine();
        }
    }
}

void MainWindow::addPartitionLinesToScene()
{
    for(auto partition_line: partitionLines_){
        ui->world_gv->scene()->addItem(partition_line);
        partition_line->setVisible(false);
    }
}

void MainWindow::walkBspPartitionLines(std::shared_ptr<BspNode> bsp_tree,
                                       std::vector<PartitionLine *>& partitionLines,
                                       PartitionLine* parentLine)
{
    if (bsp_tree == nullptr)
    {
        return;
    }
    QPen pen(Qt::green);
    pen.setWidth(2);
    // LEAF
    if (!bsp_tree->split_line.has_value() && !bsp_tree->lines.empty())
    {
        const auto& line = bsp_tree->lines[0];
        PartitionLine* partitionLine = new PartitionLine(QLine(line.p0(), line.p1()), parentLine);
        partitionLine->setPen(pen);
        partitionLines.push_back(partitionLine);
    }
    // SPLIT NODE
    else
    {
        const auto& split_line = bsp_tree->split_line.value();
        const auto line = QLine(split_line.p0(), split_line.p1());
        PartitionLine* partitionLine = new PartitionLine(line, parentLine);
        partitionLine->setPen(pen);
        partitionLines.push_back(partitionLine);
        walkBspPartitionLines(bsp_tree->front_node, partitionLines, partitionLine);
        walkBspPartitionLines(bsp_tree->back_node, partitionLines, partitionLine);
    }
}

void MainWindow::createRenderLines()
{
    std::vector<WorldLine> ordered_lines;
    WalkBspTree(world_->get_player(), bspTree_, ordered_lines);

    QPen pen(Qt::red);
    pen.setWidth(4);
    for(const auto& line: ordered_lines){
        auto* renderLine = new QGraphicsLineItem({line.p0(), line.p1()});
        renderLine->setPen(pen);
        renderLines_.push_back(renderLine);
    }
    qDebug() << "Number of render lines: " << renderLines_.size();

    for(const auto renderLine: renderLines_){
        ui->world_gv->scene()->addItem(renderLine);
        renderLine->setVisible(false);
    }
}

void MainWindow::clearWorldView()
{
    for(auto& partitionLine: partitionLines_){
        partitionLine->setVisible(false);
    }
    for(auto& renderLine: renderLines_){
        renderLine->setVisible(false);
    }
}

QString to_string(const AlgorithmView& algorithmView)
{
    switch (algorithmView)
    {
    case (AlgorithmView::BUILD_BSP):
        return QString("Build BSP");
    case (AlgorithmView::WALK_BSP):
        return QString("Walk BSP");
    default:
        abort();
    }
}
