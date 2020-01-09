#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "player.h"
#include "polygon.h"

#include <QFile>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRect>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
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

void MainWindow::render_world() {
  // OPEN FILE
  QFile in_file(QStringLiteral(":/worlds/world_01.json"));
  if (!in_file.open(QIODevice::ReadOnly)) {
    qWarning("Couldn't open world file.");
  }

  QByteArray saveData = in_file.readAll();
  const QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
  const QJsonObject world_json = loadDoc.object();

  // RENDER PLAYER
  Player player;
  player.read(world_json["player"].toObject());

  ui->world_gv->scene()->addEllipse(QRect{QPoint{5, 5}, QPoint{15, 15}},
                                    QPen{Qt::red});

  std::vector<Polygon> polygons;
  const QJsonArray polygons_json = world_json["polygons"].toArray();

  for (auto i = 0; i < polygons_json.size(); i++) {
    const QJsonArray polygon_json = polygons_json[i].toArray();
    Polygon polygon;
    polygon.read(polygon_json);
    polygons.push_back(polygon);
  }

  for (const auto& polygon : polygons) {
    const auto& lines = polygon.lines();
    for (auto line : lines) {
      ui->world_gv->scene()->addLine(line, QPen{Qt::white});
    }
  }
}
