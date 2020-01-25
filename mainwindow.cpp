#include <QMouseEvent>
#include <QDebug>
#include <QColorDialog>
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
    qDebug() << "init MainWindow" ;
    ui->setupUi(this);
    //ui->page->setMouseTracking(true);

    dockWidgetInit();
    propertyWidgetInit();

    mouse_coord = new Point();
    labMouseCoord = new QLabel("( 0, 0 )",statusBar());
    mouse_coord->x=0;
    mouse_coord->y=0;
    initStatusBar();
}

void MainWindow::dockWidgetInit(){
    connect(ui->actionAide, SIGNAL(triggered()),this, SLOT(helpButtonClicked()));
    connect(ui->actionOutils, SIGNAL(triggered()),this, SLOT(toolButtonClicked()));
    connect(ui->actionProprietes, SIGNAL(triggered()),this, SLOT(propertyButtonClicked()));

}

void MainWindow::propertyWidgetInit(){
    connect(ui->squareButton, SIGNAL(clicked()),this,SLOT(shapeToolSelected()));
    connect(ui->triangleButton, SIGNAL(clicked()),this,SLOT(shapeToolSelected()));
    connect(ui->circleButton, SIGNAL(clicked()),this,SLOT(shapeToolSelected()));

    connect(ui->lineButton, SIGNAL(clicked()),this,SLOT(lineToolSelected()));
    connect(ui->textButton, SIGNAL(clicked()),this,SLOT(textToolSelected()));

    connect(ui->pictureButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->moveButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->rotateButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initStatusBar()
{
    QLabel *labIcon = new QLabel("",statusBar());
    QIcon *green = new QIcon(":/Icons/ressources/Pointer.png");
    labIcon->setPixmap( green->pixmap(24,24) );


    statusBar()->addWidget(labIcon,0);
    statusBar()->addWidget(labMouseCoord,0);
}

 void MainWindow::setCursorLabelCoord(QMouseEvent* ev){
    labMouseCoord->setText("( "+QString::number(ev->x())+", "+QString::number(ev->y())+" )");
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

void MainWindow::on_strokeColorButton_clicked()
{
    QColor couleur = QColorDialog::getColor(ui->strokeColorButton->palette().color(ui->strokeColorButton->backgroundRole()), this, "Choisir une couleur");
    int r=0, g=0, b=0;
    couleur.getRgb(&r,&g,&b);
    QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
    ui->strokeColorButton->setStyleSheet(scolor);
    ui->strokeColorButton2->setStyleSheet(scolor);
}

void MainWindow::on_strokeColorButton2_clicked()
{
    on_strokeColorButton_clicked();
}

void MainWindow::on_fillColorButton_clicked()
{
    QColor couleur = QColorDialog::getColor(ui->fillColorButton->palette().color(ui->fillColorButton->backgroundRole()), this, "Choisir une couleur");
    int r=0, g=0, b=0;
    couleur.getRgb(&r,&g,&b);
    QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
    ui->fillColorButton->setStyleSheet(scolor);
}


