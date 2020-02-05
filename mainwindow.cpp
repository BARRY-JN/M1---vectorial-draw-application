#include <QMouseEvent>
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QAction>
#include <QImage>
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

    actualTool = CURSOR;
    mouse_coord->x=0;
    mouse_coord->y=0;
    initStatusBar();

    ui->drawzone->setactualSize(ui->horizontalSlider->value());
    ui->drawzone->setactualColor(ui->strokeColorButton2->palette().color(ui->strokeColorButton2->backgroundRole()));
    ui->drawzone->setactualColor2(ui->fillColorButton->palette().color(ui->fillColorButton->backgroundRole()));

    connect(ui->drawzone, SIGNAL(actualToolShowProperty(Tool)),SLOT(actualToolChangeProperty(Tool)));
    connect(ui->drawzone, SIGNAL(setStrokeColor(QColor)),SLOT(changeStrokeColor(QColor)));
    connect(ui->drawzone, SIGNAL(setFillColor(QColor)),SLOT(changeFillColor(QColor)));
    connect(ui->drawzone, SIGNAL(setStrokeSize(int)),SLOT(changeStrokeSize(int)));
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
    connect(ui->freeDrawButton, SIGNAL(clicked()),this,SLOT(lineToolSelected()));
    connect(ui->textButton, SIGNAL(clicked()),this,SLOT(textToolSelected()));

    connect(ui->pictureButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));
    connect(ui->cursorButton, SIGNAL(clicked()),this,SLOT(noPropertyToolSelected()));

    connect(ui->lineButton, SIGNAL(toggled(bool)),this,SLOT(lineChecked(bool)));
    connect(ui->freeDrawButton, SIGNAL(toggled(bool)),this,SLOT(lineChecked(bool)));
}

MainWindow::~MainWindow()
{
    delete mouse_coord;
    delete labMouseCoord;
    delete labIcon;
    delete labMsg;
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

void MainWindow::actualToolChangeProperty(Tool tool){
    qDebug() << "Edition des caractéristiques de l'outil !";
    switch(tool){
        case LINE:
            lineChecked(true);
            ui->actualProperty->setCurrentIndex(0);
            break;
        case RECTANGLE:
            lineChecked(false);
            ui->actualProperty->setCurrentIndex(0);
            break;
        case TEXT:
            ui->actualProperty->setCurrentIndex(1);
            break;
        default:
            break;
    }

}

void MainWindow::newFile()
{
    isSaved = false;
    ui->drawzone->clearScene();
    ui->drawzone->show();
    ui->drawzone->selectNothing();
    ui->drawzone->setactualTool(CURSOR);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }
    QStringList mimeTypeFilters;
       const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
           ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
       for (const QByteArray &mimeTypeName : supportedMimeTypes)
           mimeTypeFilters.append(mimeTypeName);
       mimeTypeFilters.sort();
       dialog.setMimeTypeFilters(mimeTypeFilters);
       dialog.selectMimeTypeFilter("image/jpeg");
       if (acceptMode == QFileDialog::AcceptSave)
           dialog.setDefaultSuffix("jpg");
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    newFile();
    QTextStream flux(&file);
    QString ligne;

    while(! flux.atEnd())
    {
        //traitement de la ligne
        ligne = flux.readLine();
        qDebug()<<"truc";
        QStringList word= ligne.split(" ",QString::SkipEmptyParts);

         bool ipoly=false, ielli=false, ipath=false, irec=false, itext=false, iline=false;
         bool xSet=false,ySet=false,wSet=false,hSet=false,sSet=false,qaSet=false, qbSet=false, coordStartRead=false,coordEndRead=false,xRead=true,firstPointSet=false;
        int x=-1,y=-1,w=-1,h=-1,s=-1;
        QColor qa, qb;
        QString t;
        QPolygonF *poly=new QPolygonF();
        QPainterPath *path=new QPainterPath();
        QPointF coord;
/* ______________________________________________________________________________________________________________________________________________________ */
/* ______________________________________________________________________________________________________________________________________________________ */
/* ______________________________________________________________________________________________________________________________________________________ */

        //Lecture mot par mot
         for (auto src : word){

            /*traitement du rectangle et de l'éllipse
            on fait une succession de test pour savoir
            quelle propriété de la figure est on en train de lire */

            if(irec||ielli||iline){
                if(!xSet){
                    x=src.toInt();
                    xSet=true;
                }else{
                    if(!ySet){
                        y=src.toInt();
                        ySet=true;
                    }else{
                        if(!wSet){
                            w=src.toInt();
                            wSet=true;
                        }else{
                            if(!hSet){
                                h=src.toInt();
                                hSet=true;
                            }else{
                                if(!sSet){
                                    s=src.toInt();
                                    sSet=true;
                                }else{
                                    if(!qaSet){
                                            qa.setNamedColor(src);
                                            qaSet=true;
                                        }else{
                                            if(!qbSet){
                                                qb.setNamedColor(src);
                                                qbSet=true;
                                            }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            /*traitement du tracé libre et du dessin polygonal*/
            if(ipoly||ipath){
                if(coordStartRead){
                    if(xRead){
                        coord.setX(src.toInt());
                    }
                    if(!xRead){
                        coord.setY(src.toInt());
                        if(!firstPointSet){
                            poly->append(coord);
                            path->moveTo(coord);
                            firstPointSet=true;
                        }else{
                            poly->append(coord);
                            path->lineTo(coord);
                        }
                    }
                     xRead=!xRead;
                }
                if(coordEndRead){
                    if(!sSet){
                        s=src.toInt();
                        sSet=true;
                    }else{
                        if(!qaSet){
                                qa.setNamedColor(src);
                                qaSet=true;
                        }else{
                                if(!qbSet){
                                    qb.setNamedColor(src);
                                    qbSet=true;
                                }
                        }
                    }
                }

                if(src=="{")
                    coordStartRead=true;
                if(src=="}"){
                    coordEndRead=true;
                    coordStartRead=false;
                }
            }

            if(itext){
                if(!xSet){
                    x=src.toInt();
                    xSet=true;
                }else{
                    if(!ySet){
                        y=src.toInt();
                        ySet=true;
                    }else{
                        t.append(src);
                        t.append(" ");
                    }
                }
            }

            if(src=="rect")
                irec=true;
            if(src=="elli")
                ielli=true;
            if(src=="path")
                ipath=true;
            if(src=="poly")
                ipoly=true;
            if(src=="text")
                itext=true;
            if(src=="line")
                iline=true;
        }

         if(irec){
             ui->drawzone->getScene()->addRect(x,y,w,h,QPen(qa,s),QBrush(qb));
         }
         if(ielli){
             ui->drawzone->getScene()->addEllipse(x,y,w,h,QPen(qa,s),QBrush(qb));
         }
         if(ipath){
             ui->drawzone->getScene()->addPath(*path,QPen(qa, s, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
         }
         if(ipoly){
             ui->drawzone->getScene()->addPolygon(*poly,QPen(qa,s),QBrush(qb));
         }
         if(iline){
             ui->drawzone->getScene()->addLine(x,y,w,h,QPen(qa,s));
         }
         if(itext){
             QGraphicsTextItem *qti = ui->drawzone->getScene()->addText(t);
             qti->setPos(x,y);
         }

 /* ______________________________________________________________________________________________________________________________________________________ */
 /* ______________________________________________________________________________________________________________________________________________________ */
 /* ______________________________________________________________________________________________________________________________________________________ */

    }
    statusBar()->showMessage(tr("Fichier chargé"), 2000);
    return true;
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
    QString it, coord;
    foreach (QGraphicsItem *item, ui->drawzone->getScene()->items()){
        pathItem = dynamic_cast<QGraphicsPathItem*>(item);
        if(pathItem){
            coord.append("{ ");
            for(int i=0;i<pathItem->path().elementCount();i++){
                coord.append(QString::number(pathItem->path().elementAt(i).x)+" "+QString::number(pathItem->path().elementAt(i).y)+" ");
            }
            coord.append("}");
            it.append("path "+coord+" "+QString::number(pathItem->pen().width())+" "+pathItem->pen().color().name());
        }
        lineItem = dynamic_cast<QGraphicsLineItem*>(item);
        if(lineItem)
            it.append("line "+QString::number(lineItem->line().x1())+" "+QString::number(lineItem->line().y1())+" "+QString::number(lineItem->line().x2())+" "+QString::number(lineItem->line().y2())+" "+QString::number(lineItem->pen().width())+" "+lineItem->pen().color().name());

        polygonItem = dynamic_cast<QGraphicsPolygonItem*>(item);
        if(polygonItem){
            coord.append("{ ");
            for(int i=0;i<polygonItem->polygon().size();i++){
                coord.append(QString::number(polygonItem->polygon().value(i).x())+" "+QString::number(polygonItem->polygon().value(i).y())+" ");
            }
            coord.append("}");
            it.append("poly "+coord+" "+QString::number(polygonItem->pen().width())+" "+polygonItem->pen().color().name()+" "+polygonItem->brush().color().name());
        }

        rectItem = dynamic_cast<QGraphicsRectItem*>(item);
        if(rectItem)
            it.append("rect "+QString::number(rectItem->rect().x())+" "+QString::number(rectItem->rect().y())+" "+QString::number(rectItem->rect().width())+" "+QString::number(rectItem->rect().height())+" "+ QString::number(rectItem->pen().width())+" "+rectItem->pen().color().name()+" "+rectItem->brush().color().name());

        elliItem = dynamic_cast<QGraphicsEllipseItem*>(item);
        if(elliItem)
            it.append("elli "+QString::number(elliItem->rect().x())+" "+QString::number(elliItem->rect().y())+" "+QString::number(elliItem->rect().width())+" "+QString::number(elliItem->rect().height())+" "+ QString::number(elliItem->pen().width())+" "+elliItem->pen().color().name()+" "+elliItem->brush().color().name());

        textItem = dynamic_cast<QGraphicsTextItem*>(item);
        if(textItem)
            it.append("text " + QString::number(textItem->x())+" "+QString::number(textItem->y())+" "+ textItem->toPlainText().simplified());
        coord="";
        it.append("\n");
    }

    QTextStream outputStream(&file);
    outputStream << it;
    file.close();

    it="";

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
    ui->actualProperty->setCurrentIndex(2);
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

void MainWindow::on_strokeColorButton2_clicked()
{
    QColor color = QColorDialog::getColor(ui->strokeColorButton2->palette().color(ui->strokeColorButton2->backgroundRole()), this, "Choisir une couleur");
    if( color.isValid() ){
        int r=0, g=0, b=0;
        color.getRgb(&r,&g,&b);
        QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
        ui->strokeColorButton2->setStyleSheet(scolor);
        ui->drawzone->setactualColor(color);
    }
}

void MainWindow::on_fillColorButton_clicked()
{
    QColor color = QColorDialog::getColor(ui->fillColorButton->palette().color(ui->fillColorButton->backgroundRole()), this, "Choisir une couleur");
    if( color.isValid() ){
        int r=0, g=0, b=0;
        color.getRgb(&r,&g,&b);
        QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
        ui->fillColorButton->setStyleSheet(scolor);
        ui->drawzone->setactualColor2(color);
    }
}

void MainWindow::lineToolSelected(){
    ui->actualProperty->setCurrentIndex(0);
}
void MainWindow::textToolSelected(){
    ui->actualProperty->setCurrentIndex(1);
    ui->drawzone->setactualTool(TEXT);
}

void MainWindow::lineChecked(bool checked){
    if(checked){
        ui->label_3->setVisible(false);
        ui->fillColorButton->setVisible(false);
    }else{
        ui->label_3->setVisible(true);
        ui->fillColorButton->setVisible(true);
    }
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

void MainWindow::on_cursorButton_clicked()
{
    ui->drawzone->setactualTool(CURSOR);
}

void MainWindow::on_freeDrawButton_clicked()
{
    ui->drawzone->setactualTool(FREE);
}

void MainWindow::on_pointButton_clicked()
{
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
    QFileDialog dialog(this);
   dialog.setWindowModality(Qt::WindowModal);
   dialog.setAcceptMode(QFileDialog::AcceptSave);
   initializeImageFileDialog(dialog, QFileDialog::AcceptSave);
   while (dialog.exec() == QDialog::Accepted && !saveExportFile(dialog.selectedFiles().first())) {}
}

bool MainWindow::saveExportFile(const QString &fileName){
    qDebug() << __FUNCTION__ <<currentFile<<"save basique";
        if(ui->drawzone->saveFile(fileName)){
            setCurrentFile(fileName);
            isSaved = true;
            return true;
        }else{
            return false;
        }
}

void MainWindow::on_actionImporter_triggered()
{
    if(ui->drawzone->isHidden()){
                ui->drawzone->show();
        }

        isSaved = false;
        QFileDialog dialog(this, tr("Open File"));
        initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
        while (dialog.exec() == QDialog::Accepted && !loadImportedFile(dialog.selectedFiles().first())) {}
}

bool MainWindow::loadImportedFile(const QString &fileName){

    QPixmap pixm;
    pixm.load(fileName);
    if (pixm.isNull()){
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), "Not a picture"));
        return false;

    }
    else{
        //ui->drawzone->clearScene();
        ui->drawzone->addpicture(pixm);
        setCurrentFile(fileName);
        return true;
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->drawzone->setactualSize(value);
}

void MainWindow::on_lineButton_clicked()
{
     ui->drawzone->setactualTool(LINE);
}

void MainWindow::changeStrokeColor(QColor color){
    int r=0, g=0, b=0;
    color.getRgb(&r,&g,&b);
    QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
    ui->strokeColorButton2->setStyleSheet(scolor);
}
void MainWindow::changeFillColor(QColor color){
    int r=0, g=0, b=0;
    color.getRgb(&r,&g,&b);
    QString scolor("background-color: rgb(" + QString::number(r) + ", " + QString::number(g) + ", " + QString::number(b) + ");");
    ui->fillColorButton->setStyleSheet(scolor);
}
void MainWindow::changeStrokeSize(int value){
    ui->horizontalSlider->setValue(value);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->drawzone->setactualtextSize(arg1);
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
     ui->drawzone->setactualtextFont(f);
}

void MainWindow::on_textEdit_textChanged()
{
    ui->drawzone->setactualtextContent(ui->textEdit->toPlainText());
}
