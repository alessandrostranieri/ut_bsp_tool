#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsLineItem>
#include <QMainWindow>
#include <memory>

// FORWARD DECLARATIONS
class World;
class BspNode;
class PartitionLine;

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
    std::vector<PartitionLine*> partitionLines_;
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

    void initAlgorithmView();

    void createPartitionLines();

    void extendPartitionLines();

    void clipPartitionLines();

    void addPartitionLinesToScene();

    void walkBspPartitionLines(std::shared_ptr<BspNode> bsp_tree, std::vector<PartitionLine*>& partitionLines, PartitionLine* parentLine = nullptr);

    void createRenderLines();

    void clearWorldView();

    std::tuple<size_t, size_t> countVisible();
};

#endif // MAINWINDOW_H
