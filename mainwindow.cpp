#include <QMouseEvent>
#include <QDebug>
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

    connect(ui->actionAide, SIGNAL(triggered()),this, SLOT(helpButtonClicked()));
    connect(ui->actionOutils, SIGNAL(triggered()),this, SLOT(toolButtonClicked()));
    connect(ui->actionProprietes, SIGNAL(triggered()),this, SLOT(propertyButtonClicked()));

    connect(ui->squareButton, SIGNAL(clicked()),this,SLOT(shapeToolSelected()));
    connect(ui->triangleButton, SIGNAL(clicked()),this,SLOT(shapeToolSelected()));
    connect(ui->circleButton, SIGNAL(clicked()),this,SLOT(shapeToolSelected()));

    connect(ui->lineButton, SIGNAL(clicked()),this,SLOT(lineToolSelected()));
    connect(ui->textButton, SIGNAL(clicked()),this,SLOT(textToolSelected()));

    connect(ui->pictureButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->moveButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->rotateButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));


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

void MainWindow::shapeToolSelected(){
    ui->actualProperty->setCurrentIndex(0);
}

void MainWindow::lineToolSelected(){
    ui->actualProperty->setCurrentIndex(1);
}
void MainWindow::textToolSelected(){
    ui->actualProperty->setCurrentIndex(2);
}
void MainWindow::noPropertyToolSelected(){
    ui->actualProperty->setCurrentIndex(3);
}

void MainWindow::helpButtonClicked(){
    if(!ui->helpWidget->isHidden())
        ui->helpWidget->hide();
    else {
        ui->helpWidget->show();
    }
    /*
    ui->helpWidget->show();
    ui->actionAide->setEnabled(false);
    */

}
void MainWindow::toolButtonClicked(){
    if(!ui->toolWidget->isHidden())
        ui->toolWidget->hide();
    else {
        ui->toolWidget->show();
    }
}
void MainWindow::propertyButtonClicked(){
    if(!ui->propertyWidget->isHidden())
        ui->propertyWidget->hide();
    else {
        ui->propertyWidget->show();
    }
}

