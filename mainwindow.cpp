#include <QMouseEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"

struct Point
{
   int x, y;
};

Point *mouse_coord;
QLabel *labMouseCoord;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->page->setMouseTracking(true);
    connect(ui->drawZone, SIGNAL(mouseMoveEvent(QMouseEvent*)),this, SLOT(mouseMoveEvent(QMouseEvent*)));
    mouse_coord = new Point();
    labMouseCoord = new QLabel("",statusBar());
    mouse_coord->x=0;
    mouse_coord->y=0;
    createStatusBar();
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    mouse_coord->x = ev->x();
    mouse_coord->y = ev->y();
    labMouseCoord->setText("( " + QString::number(mouse_coord->x) + ", "+QString::number(mouse_coord->y)+" )");
    statusBar()->repaint();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createStatusBar()
{
    QLabel *labIcon = new QLabel("",statusBar());
    QIcon *green = new QIcon(":/Icons/ressources/Pointer.png");
    labIcon->setPixmap( green->pixmap(24,24) );


    statusBar()->addWidget(labIcon,0);
    statusBar()->addWidget(labMouseCoord,0);
}
