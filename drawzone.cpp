#include <math.h>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QApplication>
#include <QColorDialog>
#include <QGraphicsDropShadowEffect>
#include "mainwindow.h"
#include "drawzone.h"

drawZone::drawZone(QWidget *parent) :
    QGraphicsView(parent)
{
    qDebug() << "init drawZone" ;
    setMouseTracking(true);
    scene = new QGraphicsScene(this);
    setScene(scene);
    focusPolicy();

/*
   QBrush greenBrush(Qt::green);
   QBrush blueBrush(Qt::blue);
   QPen outlinePen(Qt::black);
   outlinePen.setWidth(2);

   rectangle = scene->addRect(100, 0, 80, 100, outlinePen, blueBrush);
   rectangle->setAcceptHoverEvents( true );
   rectangle->setFlag(QGraphicsItem::ItemIsMovable);
   rectangle->setFlag(QGraphicsItem::ItemIsSelectable);
   //(x,y,w,h,pen,brush)
   ellipse = scene->addEllipse(0, -100, 300, 60, outlinePen, greenBrush);

   text = scene->addText("Test", QFont("Arial", 20) );
   */
}

QGraphicsScene* drawZone::getScene(){
    return scene;
}

void drawZone::setactualTool(Tool tool){
    setCursor(Qt::ArrowCursor);
    //Test sur l'outil précedemment sélectionné
    if(actualTool==POLYGON){
        polygons.append(*poly);
        polygon=nullptr;
        scene->removeItem(actualPoint);
        actualPoint=nullptr;
        poly= new QPolygonF();

    }
    if(actualTool==FREE){
        scene->removeItem(previewPoint);
        previewPoint=nullptr;
        if(SelItem!=nullptr)
            SelItem->setGraphicsEffect(0);
        SelItem=nullptr;
    }

    actualTool=tool;

    //Test sur l'outil actuellement sélectionné

    if(tool==FREE){
        setCursor(Qt::BlankCursor);
    }
}

void drawZone::setactualSize(int value){
    actualSize=value;
    if(actualTool==CURSOR&&SelItem!=nullptr){
        if(pathItem)
            pathItem->setPen(QPen(pathItem->pen().color(),actualSize));
        if(lineItem)
            lineItem->setPen(QPen(lineItem->pen().color(),actualSize));
        if(polygonItem)
            polygonItem->setPen(QPen(polygonItem->pen().color(),actualSize));
        if(rectItem)
            rectItem->setPen(QPen(rectItem->pen().color(),actualSize));
        if(elliItem)
            elliItem->setPen(QPen(elliItem->pen().color(),actualSize));
    }
}

void drawZone::setactualtextSize(int value){
    actualtextSize=value;
}

void drawZone::setactualtextFont(QFont font){
    actualtextFont=font;
}

void drawZone::setactualtextContent(QString text){
    actualtextContent=text;
}

void drawZone::setactualColor(QColor color){
    actualColor=color;
    //meme principe
    if(actualTool==CURSOR&&SelItem!=nullptr){
        if(pathItem)
            pathItem->setPen(QPen(actualColor,pathItem->pen().width()));
        if(lineItem)
            lineItem->setPen(QPen(actualColor,lineItem->pen().width()));
        if(polygonItem)
            polygonItem->setPen(QPen(actualColor,polygonItem->pen().width()));
        if(rectItem)
            rectItem->setPen(QPen(actualColor,rectItem->pen().width()));
        if(elliItem)
            elliItem->setPen(QPen(actualColor,elliItem->pen().width()));
    }
}

void drawZone::setactualColor2(QColor color){
    actualColor2=color;
    if(actualTool==CURSOR&&SelItem!=nullptr){
        if(polygonItem)
            polygonItem->setBrush(QBrush(actualColor2));
        if(rectItem)
            rectItem->setBrush(QBrush(actualColor2));
        if(elliItem)
            elliItem->setBrush(QBrush(actualColor2));
    }
}

void drawZone::mouseMoveEvent(QMouseEvent *ev)
{

    //QPointF point = mapToScene(ev->x(), ev->y());
    int x,y;
    x = ev->x();
    y = ev->y();
    //qDebug() << x << ", " << y;
    //QPointF point = mapToScene(x, y);
    QPointF point = mapToScene(ev->pos());
    MainWindow::setCursorLabelCoord(ev);

    if(actualTool==FREE){
        if(ev->buttons().testFlag(Qt::LeftButton)){
            if(count%4==0){
                path->lineTo(point);
                /*
                On efface le précédent tracé puisqu'un nouveau point a été rajouté
                */
                if(pathitem!=nullptr){
                    scene->removeItem(pathitem);
                }
                pathitem=scene->addPath(*path,QPen(actualColor, actualSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                pathitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
            count++;
        }
        if(previewPoint!=nullptr)
            scene->removeItem(previewPoint);
        previewPoint=scene->addEllipse(point.x()-actualSize/2,point.y()-actualSize/2,actualSize,actualSize, QPen(QColor(Qt::black),2), QColor(Qt::transparent));
    }

    if(actualTool==CIRCLE){
        if(previewcircle!=nullptr)
            scene->removeItem(previewcircle);
        previewcircle = scene->addEllipse(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor2);
    }

    if(actualTool==LINE){
        if(previewcircle!=nullptr)
            scene->removeItem(previewcircle);
        previewcircle = scene->addEllipse(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor);

        if(PointActuel==1){
            if(previewline!=nullptr)
                scene->removeItem(previewline);
            QPen pen;  // creates a default pen
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(actualSize);
            pen.setBrush(actualColor);
            pen.setCapStyle(Qt::RoundCap);
            previewline = scene->addLine(PreviousPoint.x(),PreviousPoint.y(),point.x(),point.y(),pen);
        }
    }

    if(actualTool==RECTANGLE){
        if(previewrectangle!=nullptr)
            scene->removeItem(previewrectangle);
        previewrectangle = scene->addRect(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor2);
    }

    if(actualTool==TRIANGLE){
        if(previewtriangle!=nullptr)
            scene->removeItem(previewtriangle);
        QPolygonF poly;
        poly << QPointF(point.x()-actualSize, point.y()) << QPointF(point.x()+actualSize, point.y()) << QPointF(point.x(), point.y()-actualSize );
        previewtriangle = scene->addPolygon(poly,actualColor, actualColor2);
    }

    if(actualTool==TEXT){
        if(previewtext!=nullptr)
            scene->removeItem(previewtext);
        actualtextFont.setPointSize(actualtextSize);
        previewtext = scene->addText(actualtextContent, actualtextFont);
        previewtext->setPos(point);

    }

    if(actualTool==CURSOR){
        if(SelItem){
            if(ev->buttons().testFlag(Qt::LeftButton)){
                    //int x= SelItem->shape().boundingRect().x();
                    //int y= SelItem->shape().boundingRect().y();
                    //SelItem->setPos(point.x()-x-w/2, point.y()-y-h/2);
                    SelItem->setPos(point.x()-initX,point.y()-initY);

            }
            if(doRotate){
                int a1 = SelItem->shape().boundingRect().x()-point.x();
                int a2 = SelItem->shape().boundingRect().y()-point.y();
                SelItem->setRotation(atan2(a1,a2) * -180/M_PI);
            }
        }
    }

}

void drawZone::keyPressEvent( QKeyEvent *ev){
    if(SelItem){
        if(ev->key() == Qt::Key_R ){
            if(!centerPointSet){
                int x= SelItem->shape().boundingRect().x();
                int y= SelItem->shape().boundingRect().y();
                int w= SelItem->shape().boundingRect().width();
                int h= SelItem->shape().boundingRect().height();
                SelItem->setTransformOriginPoint(x+w/2,y+h/2);
                centerPointSet=true;
            }
            doRotate=!doRotate;
            qDebug()<<"touche appuyée";
        }
    }

}

void drawZone::addpicture(QPixmap pixmap){
        scene->addPixmap(pixmap);
        scene->setSceneRect(scene->sceneRect());
    }

bool drawZone::saveFile(const QString &fileName){
    qDebug() << __FUNCTION__ <<"sauvegarde";

    QPixmap *tosave = new QPixmap(scene->sceneRect().width(),scene->sceneRect().height());
    QPainter painter(tosave);
    scene->setBackgroundBrush(Qt::white);
    scene->render(&painter);

    painter.end();


    if (!fileName.isEmpty())
    {
      tosave->save(fileName);
      delete tosave;
      return true;
    }
    delete tosave;
    return false;
}

void drawZone::mouseReleaseEvent(QMouseEvent *event){
    if(actualTool==FREE){
        pathitem=nullptr;
        paths.append(*path);
        path=new QPainterPath();
        scene->removeItem(previewPoint);
        previewPoint=nullptr;
    }
}

void drawZone::leaveEvent(QEvent * e)
{
    scene->removeItem(previewPoint);
    previewPoint=nullptr;

    scene->removeItem(previewcircle);
    previewcircle=nullptr;

    scene->removeItem(previewline);
    previewline=nullptr;

    scene->removeItem(previewtriangle);
    previewtriangle=nullptr;

    scene->removeItem(previewtext);
    previewtext=nullptr;

    scene->removeItem(previewrectangle);
    previewrectangle=nullptr;

    MainWindow::leaveDrawZone();
}

void drawZone::clearScene()
{

    scene->clear();
}


void drawZone::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y()
    << ev->button();
    switch(ev->button()) {
    case Qt::RightButton :
        showcontextmenu();
        break;

    case Qt::LeftButton :
    {
        //PointActuel=0;
        int x,y,x2,y2;
        x2=0;
        y2=0;
        x = ev->x();
        y = ev->y();
        qDebug() << x << ", " << y;
        QPointF point = mapToScene(x, y);

        switch(actualTool){
            case(CIRCLE):
            {
                QGraphicsEllipseItem *circle;
                circle = scene->addEllipse(point.x()-actualSize/2,point.y()-actualSize/2,actualSize,actualSize,actualColor, actualColor2);
                circle->setFlag(QGraphicsEllipseItem::ItemIsSelectable);
                circle->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                circle->setAcceptDrops(true);
                //circle->setPos(point.x()-(actualSize/2),point.y()-(actualSize/2));

                break;
            }
            case(LINE):
            {
                if(PointActuel==0){
                    QGraphicsEllipseItem *ellipse;
                    ellipse = scene->addEllipse(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor);
                    ellipse->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                    PreviousPoint=point;
                    PointActuel=1;
                    return;
                }

                if(PointActuel==1){
                    QGraphicsLineItem *line;
                    QPen pen;  // creates a default pen

                    pen.setStyle(Qt::SolidLine);
                    pen.setWidth(actualSize);
                    pen.setBrush(actualColor);
                    pen.setCapStyle(Qt::RoundCap);

                    line = scene->addLine(PreviousPoint.x(),PreviousPoint.y(),point.x(),point.y(),pen);
                    line->setFlag(QGraphicsLineItem::ItemIsSelectable);
                    line->setFlag(QGraphicsLineItem::ItemIsMovable);
                    PointActuel=0;
                    return;
                }
                break;
            }
            case(FREE):
                //points.append(point);
                path->moveTo(point);
                break;
            case(POLYGON):
            {
                if(polygon!=nullptr){
                    scene->removeItem(polygon);
                    scene->removeItem(actualPoint);
                }
                poly->append(point);
                //polygon->setPolygon(*poly);
                polygon=scene->addPolygon(*poly,QPen(actualColor,actualSize),actualColor2);
                actualPoint=scene->addEllipse(point.x()-(actualSize+5)/2,point.y()-(actualSize+5)/2,actualSize+10,actualSize+10, QColor(Qt::red), QColor(Qt::red));

                break;
            }

            case(RECTANGLE):
            {
                QGraphicsRectItem *rectangle;
                //rectangle = scene->addRect(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor2);
                rectangle = scene->addRect(point.x()-actualSize/2,point.y()-actualSize/2,actualSize,actualSize,actualColor, actualColor2);
                rectangle->setFlag(QGraphicsRectItem::ItemIsSelectable);
                rectangle->setFlag(QGraphicsRectItem::ItemIsMovable);
                //rectangle->setPos(point);
                break;
            }

            case(TRIANGLE):
            {
                QGraphicsPolygonItem *polygon;
                QPolygonF poly;
                //poly << QPointF(point.x()-actualSize, point.y()) << QPointF(point.x()+actualSize, point.y()) << QPointF(point.x(), point.y()-actualSize );
                poly << QPointF(point.x()-actualSize, point.y()) << QPointF(actualSize+point.x(), point.y()) << QPointF(point.x(), point.y()-actualSize );
                polygon=scene->addPolygon(poly,actualColor, actualColor2);
                polygon->setFlag(QGraphicsPolygonItem::ItemIsSelectable);
                polygon->setFlag(QGraphicsPolygonItem::ItemIsMovable);
                break;
            }

            case(TEXT):
            {
                QGraphicsTextItem *text;
                actualtextFont.setPointSize(actualtextSize);
                text = scene->addText(actualtextContent, actualtextFont);
                text->setPos(point);
                text->setFlag(QGraphicsTextItem::ItemIsSelectable);
                text->setFlag(QGraphicsTextItem::ItemIsMovable);
                text->setFlag(QGraphicsTextItem::ItemIsFocusable);
                break;
            }
            case(CURSOR):
            {
                bool somethingSelected=false;
                QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
                effect->setBlurRadius(10);

                foreach (QGraphicsItem *item, scene->items()) {
                    if(item->contains(item->mapFromScene(QPointF(point.x(),point.y())))){
                        qDebug()<<"OUI";
                        somethingSelected=true;
                         if(SelItem!=nullptr){
                             centerPointSet=false;
                             SelItem->setGraphicsEffect(0);
                         }
                         SelItem=item; 
                         item->setGraphicsEffect(effect);
                         initX=point.x()-item->x();
                         initY=point.y()-item->y();


                         //Pour détecter le type de l'item sélectionné et afficher les caractéristiques correspondantes
                         pathItem = dynamic_cast<QGraphicsPathItem*>(item);
                         lineItem = dynamic_cast<QGraphicsLineItem*>(item);

                         //affiche les caractéristiques de l'outil sélectionné
                         if(pathItem||lineItem)
                             emit actualToolShowProperty(LINE);

                         polygonItem = dynamic_cast<QGraphicsPolygonItem*>(item);
                         rectItem = dynamic_cast<QGraphicsRectItem*>(item);
                         elliItem = dynamic_cast<QGraphicsEllipseItem*>(item);
                         if(polygonItem||rectItem||elliItem)
                             emit actualToolShowProperty(RECTANGLE);

                         textItem = dynamic_cast<QGraphicsTextItem*>(item);

                         if(textItem)
                             emit actualToolShowProperty(TEXT);


                         if(pathItem){
                             emit  setStrokeColor(pathItem->pen().color());
                             emit  setStrokeSize(pathItem->pen().width());
                         }
                         if(lineItem){
                             emit  setStrokeColor(lineItem->pen().color());
                             emit  setStrokeSize(lineItem->pen().width());
                         }

                         if(polygonItem){
                             emit  setStrokeColor(polygonItem->pen().color());
                             emit  setStrokeSize(polygonItem->pen().width());
                             emit  setFillColor(polygonItem->brush().color());
                         }

                         if(rectItem){
                             emit  setStrokeColor(rectItem->pen().color());
                             emit  setStrokeSize(rectItem->pen().width());
                             emit  setFillColor(rectItem->brush().color());
                         }

                         if(elliItem){
                             emit  setStrokeColor(elliItem->pen().color());
                             emit  setStrokeSize(elliItem->pen().width());
                             emit  setFillColor(elliItem->brush().color());
                         }
                        break;
                     }
                }

                if(!somethingSelected)
                    if(SelItem!=nullptr){
                        SelItem->setGraphicsEffect(0);
                        SelItem=nullptr;
                        doRotate=false;
                    }
                break;
            }
            default:
            break;
        }
    break;
    }
    default:
        break;
    }
}

void drawZone::showcontextmenu()
{
    QMenu menu(this);
    menu.addAction(new QAction("Copier", this));
    menu.addAction(new QAction("Coller", this));
    menu.addAction(new QAction("Supprimer", this));
    menu.exec(QCursor::pos());
}

void drawZone::selectNothing(){
    SelItem=nullptr;
}

drawZone::~drawZone(){
    delete scene;
    delete rectangle;
    delete text;
    delete line;
    delete pixmap;
    delete simpletext;
    delete painter;
    delete pathitem;
    delete path;
    delete previewPoint;
    delete polygon;
    delete poly;
    delete actualPoint;
    delete SelItem;
    delete pathItem;
    delete lineItem;
    delete polygonItem;
    delete rectItem;
    delete elliItem;
    delete textItem;
    delete previewcircle;
    delete previewline;
    delete previewtriangle;
    delete previewrectangle;
    delete previewtext;
    scene=nullptr;
    text=nullptr;
    line=nullptr;
    pixmap=nullptr;
    simpletext=nullptr;
    painter=nullptr;
    pathitem=nullptr;
    path=nullptr;
    previewPoint=nullptr;
    polygon=nullptr;
    poly=nullptr;
    actualPoint=nullptr;
    SelItem=nullptr;
    pathItem=nullptr;
    lineItem=nullptr;
    polygonItem=nullptr;
    rectItem=nullptr;
    elliItem=nullptr;
    textItem=nullptr;
    previewcircle=nullptr;
    previewline=nullptr;
    previewtriangle=nullptr;
    previewrectangle=nullptr;
    previewtext=nullptr;

}
