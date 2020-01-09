#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QGraphicsScene>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

void write_polygons_to_file(){
    // CREATE A POLYGON
    QPoint p0(0, 0);
    QPoint p1(0, 10);
    QPoint p2(10, 10);
    QPoint p3(10, 0);
    QPolygon polygon{QVector{p0, p1, p2, p3}};

    // STORE INTO JSON
    QJsonArray polygon_json;
    const auto p_size = polygon.size();
    for(auto i = 0; i < p_size-1; i++){
        QJsonObject line;
        line["from"] = QJsonArray{polygon[i].x(), polygon[i].y()};
        line["to"] = QJsonArray{polygon[i+1].x(), polygon[i+1].y()};
        polygon_json.append(line);
    }

    QJsonObject line;
    line["from"] = QJsonArray{polygon[p_size-1].x(), polygon[p_size-1].y()};
    line["to"] = QJsonArray{polygon[0].x(), polygon[0].y()};
    polygon_json.append(line);

    QJsonArray world_json;
    world_json.append(polygon_json);

    // SAVE TO FILE
    QFile out_file(QStringLiteral("../ut_bsp_tool/worlds/world_01.json"));
    if (!out_file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open world file.");
    } else {
        QJsonDocument doc(world_json);
        out_file.write(doc.toJson());
        qDebug("File written successfully");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene* scene = new QGraphicsScene;
    ui->world_gv->setScene(scene);
    scene->setBackgroundBrush(Qt::black);

    write_polygons_to_file();

    render_polygons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::render_polygons()
{
    // OPEN FILE
    QFile in_file(QStringLiteral(":/worlds/world_01.json"));
    if (!in_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open world file.");
    }

    QByteArray saveData = in_file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QVector<QLine> lines;
    QJsonArray polygons = loadDoc.array();
    for (auto polygon : polygons) {
      QJsonArray polygon_lines = polygon.toArray();
      for (auto polygon_line : polygon_lines) {
        QJsonObject line_json = polygon_line.toObject();
        QJsonArray from_json = line_json["from"].toArray();
        QJsonArray to_json = line_json["to"].toArray();
        lines.push_back(
            QLine{QPoint{from_json[0].toInt(), from_json[1].toInt()},
                  QPoint{to_json[0].toInt(), to_json[1].toInt()}});
      }
    }

    for (auto line : lines) {
      auto scene = ui->world_gv->scene();

      scene->addLine(line, QPen(Qt::white));
    }
}
