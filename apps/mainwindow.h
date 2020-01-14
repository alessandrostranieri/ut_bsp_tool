#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class AlgorithmView { BUILD_BSP, WALK_BSP };

QString to_string(const AlgorithmView& algorithmView);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow* ui;
    AlgorithmView algorithmView_ = AlgorithmView::BUILD_BSP;

  private slots:
    void nextStep();
    void previousStep();
    void toggleBuildView();
    void toggleWalkView();

  private:
    void render_world();
    void toggleAlgorithmView();
};
#endif // MAINWINDOW_H
