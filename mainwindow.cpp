#include <QMouseEvent>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "mainwindow.h"

Point *mouse_coord;
QLabel *labMouseCoord;
QLabel *labIcon;
QLabel *labMsg;
Tool actualTool;

bool isSaved=false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "init MainWindow" ;
    ui->setupUi(this);
    //ui->page->setMouseTracking(true);

    ui->drawzone->hide();
    connect(ui->actionNouveau, SIGNAL(triggered()),this,SLOT(newFile()));
    connect(ui->actionOuvrir, SIGNAL(triggered()),this,SLOT(openFile()));
    connect(ui->actionEnregistrer, SIGNAL(triggered()),SLOT(save()));
    connect(ui->actionEnregistrer_sous, SIGNAL(triggered()),SLOT(saveAs()));

    ui->helpWidget->hide();
    dockWidgetInit();
    propertyWidgetInit();

    mouse_coord = new Point();
    labMouseCoord = new QLabel("",statusBar());
    labIcon = new QLabel("",statusBar());
    labIcon->hide();
    labMsg = new QLabel("",statusBar());

    actualTool = NONE;
    mouse_coord->x=0;
    mouse_coord->y=0;
    initStatusBar();

}

Tool MainWindow::getactualTool(){
    return actualTool;
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
    connect(ui->pointButton, SIGNAL(clicked()),this,SLOT(lineToolSelected()));
    connect(ui->textButton, SIGNAL(clicked()),this,SLOT(textToolSelected()));

    connect(ui->pictureButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->moveButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->rotateButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->cursorButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initStatusBar()
{
    QIcon *green = new QIcon(":/Icons/ressources/Pointer.png");
    labIcon->setPixmap( green->pixmap(24,24) );


    statusBar()->addWidget(labIcon,0);
    statusBar()->addWidget(labMouseCoord,0);
    statusBar()->addWidget(labMsg,0);
}

void MainWindow::showStatusMessage(const QString &msg){
    labMsg->setText(msg);
}

void MainWindow::newFile()
{
    ui->drawzone->clearScene();
    ui->drawzone->show();
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    statusBar()->showMessage(tr("Fichier chargé"), 2000);
}

void MainWindow::importFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    showStatusMessage(tr("Fichier importé"));
}

bool MainWindow::save()
{
    if (currentFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(currentFile);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    showStatusMessage("File saved");
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    currentFile = fileName;
    ui->drawzone->isWindowModified();
    setWindowModified(false);

    QString shownName = currentFile;
    if (currentFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

 void MainWindow::setCursorLabelCoord(QMouseEvent* ev){
    labMouseCoord->show();
    labIcon->show();
    labMouseCoord->setText("( "+QString::number(ev->x())+", "+QString::number(ev->y())+" )");
}

 void MainWindow::leaveDrawZone(){
    labMouseCoord->hide();
    labIcon->hide();

}

void MainWindow::shapeToolSelected(){
    ui->actualProperty->setCurrentIndex(0);
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
    QColor color = QColorDialog::getColor(ui->strokeColorButton->palette().color(ui->strokeColorButton->backgroundRole()), this, "Choisir une couleur");
    if( color.isValid() ){
        int r=0, g=0, b=0;
        color.getRgb(&r,&g,&b);
        QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
        ui->strokeColorButton->setStyleSheet(scolor);
        ui->strokeColorButton2->setStyleSheet(scolor);
    }
}

void MainWindow::on_strokeColorButton2_clicked()
{
    on_strokeColorButton_clicked();
}

void MainWindow::on_fillColorButton_clicked()
{
    QColor color = QColorDialog::getColor(ui->fillColorButton->palette().color(ui->fillColorButton->backgroundRole()), this, "Choisir une couleur");
    if( color.isValid() ){
        int r=0, g=0, b=0;
        color.getRgb(&r,&g,&b);
        QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
        ui->fillColorButton->setStyleSheet(scolor);
    }
}

void MainWindow::lineToolSelected(){
    ui->actualProperty->setCurrentIndex(1);
    ui->drawzone->setactualTool(LINE);
}
void MainWindow::textToolSelected(){
    ui->actualProperty->setCurrentIndex(2);
    ui->drawzone->setactualTool(TEXT);
}

void MainWindow::on_squareButton_clicked()
{
    ui->drawzone->setactualTool(RECTANGLE);
}

void MainWindow::on_circleButton_clicked()
{
    ui->drawzone->setactualTool(CIRCLE);
}

void MainWindow::on_triangleButton_clicked()
{
    ui->drawzone->setactualTool(TRIANGLE);
}

void MainWindow::on_pictureButton_clicked()
{
    ui->drawzone->setactualTool(IMAGE);
}

void MainWindow::on_moveButton_clicked()
{
    ui->drawzone->setactualTool(MOVE);
}

void MainWindow::on_rotateButton_clicked()
{
    ui->drawzone->setactualTool(ROTATE);
}

void MainWindow::on_cursorButton_clicked()
{
    ui->drawzone->setactualTool(NONE);
}

void MainWindow::on_freeDrawButton_clicked()
{
    ui->actualProperty->setCurrentIndex(1);
    ui->drawzone->setactualTool(FREE);
}

void MainWindow::on_pointButton_clicked()
{
    ui->actualProperty->setCurrentIndex(1);
    ui->drawzone->setactualTool(POLYGON);
}

void MainWindow::on_actionZoomPlus_triggered()
{
    ui->drawzone->scale(2.0,2.0);
}

void MainWindow::on_actionZoomMoins_triggered()
{
     ui->drawzone->scale(0.5,0.5);
}

void MainWindow::on_actionExporter_triggered()
{
    QMenu exportMenu(this);
    exportMenu.addAction(new QAction("SVG", this));
    exportMenu.addAction(new QAction("JPG", this));
    exportMenu.addAction(new QAction("PNG", this));
    exportMenu.addAction(new QAction("BMP", this));
    exportMenu.exec(QCursor::pos());
}

void MainWindow::on_actionImporter_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        importFile(fileName);
}
