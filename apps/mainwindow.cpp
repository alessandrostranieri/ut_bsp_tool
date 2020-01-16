#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bsp/player.h"
#include "bsp/polygon.h"
#include "bsp/world.h"
#include "bsp/bsp.h"

#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRect>

#include <algorithm>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), world_(nullptr)
{
    ui->setupUi(this);

    world_ = std::make_unique<World>();

    QGraphicsScene* scene = new QGraphicsScene;
    ui->world_gv->setScene(scene);
    scene->setBackgroundBrush(Qt::black);
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextStep()
{
    qDebug("Next Step");
    const std::vector<QGraphicsLineItem*>& lines = algorithmView_ == AlgorithmView::BUILD_BSP ? partitionLines_ : renderLines_;
    auto found = std::find_if_not(lines.begin(), lines.end(), [](const auto& gl){return gl->isVisible();});
    if(found != lines.end()){
        (*found)->setVisible(true);
    }
    updateStepButton();
}

void MainWindow::previousStep()
{
    qDebug("Previous Step");
    const std::vector<QGraphicsLineItem*>& lines = algorithmView_ == AlgorithmView::BUILD_BSP ? partitionLines_ : renderLines_;
    auto found = std::find_if(lines.rbegin(), lines.rend(), [](const auto& gl){return gl->isVisible();});
    if(found != lines.rend()){
        (*found)->setVisible(false);
    }
    updateStepButton();
}

void MainWindow::updateStepButton()
{
    const std::vector<QGraphicsLineItem*>& lines = algorithmView_ == AlgorithmView::BUILD_BSP ? partitionLines_ : renderLines_;
    const auto count_visible = static_cast<size_t>(std::count_if(lines.begin(),
                                                                 lines.end(),
                                                                 [](const auto& gl){ return gl->isVisible();}));
    const auto num_lines = lines.size();
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

void MainWindow::createPartitionLines()
{
    qDebug() << "Partition lines creation started";
    walkBspPartitionLines(bspTree_, partitionLines_);
    qDebug() << "Partition lines creation ended";

    extendPartitionLines();

    qDebug() << "Adding partition lines to scene ";
    addPartitionLinesToScene();
    qDebug() << "Partition lines added to scene";
}

void MainWindow::extendPartitionLines()
{
    auto xMin = ui->world_gv->sceneRect().x();
    auto yMin = ui->world_gv->sceneRect().y();
    auto xMax = xMin + ui->world_gv->sceneRect().width();
    auto yMax = yMin + ui->world_gv->sceneRect().height();
    QLineF top{{xMin,yMin}, {xMax,yMin}};
    QLineF bottom{{xMin,yMax}, {xMax,yMax}};
    QLineF left{{xMin, yMax}, {xMin, yMin}};
    QLineF right{{xMax, yMax}, {xMax, yMin}};

    for(auto partition_line: partitionLines_){
        QLineF line = partition_line->line();
        qreal angle = line.angle();
        if(angle == 90.0 || angle == 270.0){
            QPointF topIntersection;
            line.intersect(top, &topIntersection);
            QPointF bottomIntersection;
            line.intersect(bottom, &bottomIntersection);
            partition_line->setLine({topIntersection, bottomIntersection});
        } else {
            QPointF leftIntersection;
            line.intersect(left, &leftIntersection);
            QPointF rightIntersection;
            line.intersect(right, &rightIntersection);
            partition_line->setLine({leftIntersection, rightIntersection});
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
                                       std::vector<QGraphicsLineItem *>& partitionLines)
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
        QGraphicsLineItem* partitionLine = new QGraphicsLineItem(QLine(line.p0(), line.p1()));
        partitionLine->setPen(pen);
        partitionLines.push_back(partitionLine);
    }
    // SPLIT NODE
    else
    {
        const auto& split_line = bsp_tree->split_line.value();
        const auto line = QLine(split_line.p0(), split_line.p1());
        QGraphicsLineItem* partitionLine = new QGraphicsLineItem(line);
        partitionLine->setPen(pen);
        partitionLines.push_back(partitionLine);
        walkBspPartitionLines(bsp_tree->front_node, partitionLines);
        walkBspPartitionLines(bsp_tree->back_node, partitionLines);
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
