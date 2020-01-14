#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bsp/player.h"
#include "bsp/polygon.h"
#include "bsp/world.h"

#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRect>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene* scene = new QGraphicsScene;
    ui->world_gv->setScene(scene);
    scene->setBackgroundBrush(Qt::black);

    render_world();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextStep()
{
    qDebug("Next Step");
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
    World world;
    world.read(world_json);

    // RENDER PLAYER
    ui->world_gv->scene()->addEllipse(
        QRect{world.get_player().position() - QPoint{5, 5}, world.get_player().position() + QPoint{5, 5}},
        QPen{Qt::red});

    // RENDER LINES
    for (const auto& polygon : world.get_polygons())
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
