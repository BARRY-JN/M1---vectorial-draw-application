#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Point
{
   int x, y;
};

enum Tool { NONE, LINE, RECTANGLE, CIRCLE, TRIANGLE, TEXT, IMAGE, MOVE, ROTATE };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    static void setCursorLabelCoord(QMouseEvent*);
    static void leaveDrawZone();
    void loadFile(const QString &fileName);
    void importFile(const QString &fileName);
    void showStatusMessage(const QString &msg);
    static Tool getactualTool();

private slots:
    void newFile();
    void openFile();
    bool save();
    bool saveAs();
    void dockWidgetInit();
    void propertyWidgetInit();
    void shapeToolSelected();
    void lineToolSelected();
    void textToolSelected();
    void noPropertyToolSelected();
    void helpButtonClicked();
    void toolButtonClicked();
    void propertyButtonClicked();
    void on_strokeColorButton_clicked();
    void on_fillColorButton_clicked();
    void on_strokeColorButton2_clicked();
    void on_squareButton_clicked();
    void on_circleButton_clicked();
    void on_triangleButton_clicked();
    void on_pictureButton_clicked();
    void on_moveButton_clicked();
    void on_rotateButton_clicked();
    void on_actionZoomPlus_triggered();
    void on_actionZoomMoins_triggered();
    void on_actionExporter_triggered();
    void on_actionImporter_triggered();

private:
    Ui::MainWindow *ui;
    void initStatusBar();
    bool saveFile(const QString &filename);
    void setCurrentFile(const QString &fileName);

    QString currentFile;
};
#endif // MAINWINDOW_H
