#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsLineItem>
#include <QMainWindow>
#include <memory>

// FORWARD DECLARATIONS
class World;
class BspNode;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

enum class AlgorithmView
{
    BUILD_BSP,
    WALK_BSP
};

QString to_string(const AlgorithmView& algorithmView);

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    void init();

    ~MainWindow();

  private:
    Ui::MainWindow* ui;
    std::unique_ptr<World> world_;
    std::shared_ptr<BspNode> bspTree_;
    AlgorithmView algorithmView_ = AlgorithmView::BUILD_BSP;
    std::vector<QGraphicsLineItem*> partitionLines_;
    std::vector<QGraphicsLineItem*> renderLines_;
    QGraphicsEllipseItem* playerMarker_;

  private slots:

    void nextStep();

    void previousStep();

    void updateStepButton();

    void toggleBuildView();

    void toggleWalkView();

  private:
    void render_world();

    void createPartitionLines();

    void extendPartitionLines();

    void addPartitionLinesToScene();

    void walkBspPartitionLines(std::shared_ptr<BspNode> bsp_tree, std::vector<QGraphicsLineItem*>& partitionLines);

    void createRenderLines();

    void clearWorldView();
};

#endif // MAINWINDOW_H
