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

    ui->nextStepButton->setEnabled(true);
    ui->previousStepButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextStep()
{
    qDebug("Next Step");
    auto found = std::find_if_not(partitionLines_.begin(), partitionLines_.end(), [](const auto& gl){return gl->isVisible();});
    if(found != partitionLines_.end()){
        (*found)->setVisible(true);
    } else {
        ui->nextStepButton->setEnabled(false);
    }
}

void MainWindow::previousStep()
{
    qDebug("Previous Step");
}

void MainWindow::toggleBuildView()
{
    algorithmView_ = AlgorithmView::BUILD_BSP;
    qDebug() << to_string(algorithmView_);
}

void MainWindow::toggleWalkView()
{
    algorithmView_ = AlgorithmView::WALK_BSP;
    qDebug() << to_string(algorithmView_);
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
    ui->world_gv->scene()->addEllipse(
        QRect{world_->get_player().position() - QPoint{5, 5}, world_->get_player().position() + QPoint{5, 5}},
        QPen{Qt::red});

    // RENDER LINES
    for (const auto& polygon : world_->get_polygons())
    {
        const auto& lines = polygon.lines();
        for (auto line : lines)
        {
            ui->world_gv->scene()->addLine(QLine(line.p0(), line.p1()), QPen{Qt::white});
        }
    }
}

void MainWindow::toggleAlgorithmView()
{
    if (algorithmView_ == AlgorithmView::BUILD_BSP)
    {
        algorithmView_ = AlgorithmView::WALK_BSP;
    }
    else
    {
        algorithmView_ = AlgorithmView::BUILD_BSP;
    }
}

void MainWindow::createPartitionLines()
{
    qDebug() << "Partition lines creation started";
    walkBspPartitionLines(bspTree_, partitionLines_);
    qDebug() << "Partition lines creation ended";

    qDebug() << "Adding partition lines to scene ";
    for(auto partition_line: partitionLines_){
        ui->world_gv->scene()->addItem(partition_line);
        partition_line->setVisible(false);
    }
    qDebug() << "Partition lines added to scene";
}

void MainWindow::walkBspPartitionLines(std::shared_ptr<BspNode> bsp_tree,
                                       std::vector<QGraphicsLineItem *>& partitionLines)
{
    if (bsp_tree == nullptr)
    {
        return;
    }
    // LEAF
    if (!bsp_tree->split_line.has_value() && !bsp_tree->lines.empty())
    {
        const auto& line = bsp_tree->lines[0];
        QGraphicsLineItem* partitionLine = new QGraphicsLineItem(QLine(line.p0(), line.p1()));
        partitionLine->setPen(QPen(Qt::green));
        partitionLines.push_back(partitionLine);
    }
    // SPLIT NODE
    else
    {
        const auto& split_line = bsp_tree->split_line.value();
        const auto line = QLine(split_line.p0(), split_line.p1());
        QGraphicsLineItem* partitionLine = new QGraphicsLineItem(line);
        partitionLine->setPen(QPen(Qt::green));
        partitionLines.push_back(partitionLine);
        walkBspPartitionLines(bsp_tree->front_node, partitionLines);
        walkBspPartitionLines(bsp_tree->back_node, partitionLines);
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
