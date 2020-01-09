#include "player.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPolygon>
#include <QVector>

void write_world_file() {
  // MAIN JSON
  QJsonObject world_json;

  // PLAYER
  Player player{QPoint{5, 5}};
  QJsonObject player_json;
  player.write(player_json);

  world_json["player"] = player_json;

  // POLYGONS
  // CREATE A POLYGON
  QPoint p0(0, 0);
  QPoint p1(0, 10);
  QPoint p2(10, 10);
  QPoint p3(10, 0);
  QPolygon polygon{QVector{p0, p1, p2, p3}};

  // STORE INTO JSON
  QJsonArray lines_json;
  const auto p_size = polygon.size();
  for (auto i = 0; i < p_size - 1; i++) {
    QJsonObject line_json;
    line_json["from"] = QJsonArray{polygon[i].x(), polygon[i].y()};
    line_json["to"] = QJsonArray{polygon[i + 1].x(), polygon[i + 1].y()};
    lines_json.append(line_json);
  }
  QJsonObject line_json;
  line_json["from"] =
      QJsonArray{polygon[p_size - 1].x(), polygon[p_size - 1].y()};
  line_json["to"] = QJsonArray{polygon[0].x(), polygon[0].y()};
  lines_json.append(line_json);

  QJsonArray polygons_json;
  polygons_json.append(lines_json);
  world_json["polygons"] = polygons_json;

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

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  write_world_file();

  return a.exec();
}
