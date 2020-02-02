#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <QMenu>
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

drawZone::~drawZone(){
    delete path;
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
    QPointF point = mapToScene(x, y);
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
        point_init = mapToScene(x, y);
        QPointF point = mapToScene(x, y);
        QPointF point2;

        switch(actualTool){
            case(CIRCLE):
            {
                QGraphicsEllipseItem *circle;
                circle = scene->addEllipse(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor2);
                circle->setFlag(QGraphicsItem::ItemIsSelectable);
                circle->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }
            case(LINE):
            {
                if(PointActuel==0){
                    QGraphicsEllipseItem *ellipse;
                    ellipse = scene->addEllipse(point.x(),point.y(),actualSize,actualSize,actualColor, actualColor2);
                    ellipse->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                    x2=point.x();
                    y2=point.y();
                    PointActuel=1;
                    return;
                }

                if(PointActuel==1){
                    QGraphicsLineItem *line;
                    line = scene->addLine(x2,y2,point.x(),point.y(),actualColor);
                    line->setFlag(QGraphicsItem::ItemIsSelectable);
                    line->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                    //PointActuel=0;
                    return;
                }
                break;
            }
            case(FREE):
                //points.append(point_init);
                path->moveTo(point_init);
                break;
            case(POLYGON):
            {
                if(polygon!=nullptr){
                    scene->removeItem(polygon);
                    scene->removeItem(actualPoint);
                }
                poly->append(point_init);
                //polygon->setPolygon(*poly);
                polygon=scene->addPolygon(*poly,QPen(actualColor,actualSize),actualColor2);
                actualPoint=scene->addEllipse(point_init.x()-(actualSize+5)/2,point_init.y()-(actualSize+5)/2,actualSize+10,actualSize+10, QColor(Qt::red), QColor(Qt::red));

                break;
            }

            case(RECTANGLE):
            {
                QGraphicsRectItem *rectangle;
                rectangle = scene->addRect(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor2);
                rectangle->setFlag(QGraphicsItem::ItemIsSelectable);
                rectangle->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }

            case(TRIANGLE):
            {
                QGraphicsPolygonItem *polygon;
                QPolygonF poly;
                poly << QPointF(point.x()-actualSize, point.y()) << QPointF(point.x()+actualSize, point.y()) << QPointF(point.x(), point.y()-actualSize );
                polygon=scene->addPolygon(poly,actualColor, actualColor2);
                polygon->setFlag(QGraphicsItem::ItemIsSelectable);
                polygon->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }

            case(TEXT):
            {
                QGraphicsTextItem *text;
                QFont serifFont("Times", actualSize, QFont::Bold);
                text = scene->addText(QString("Bonjour !"), QFont(serifFont));
                text->setPos(point);
                text->setFlag(QGraphicsItem::ItemIsSelectable);
                text->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }
            case(CURSOR):
            {
                bool somethingSelected=false;
                QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
                effect->setBlurRadius(50);

                foreach (QGraphicsItem *item, scene->items()) {
                    if(item->contains(QPointF(point.x(),point.y()))){
                        somethingSelected=true;
                         if(SelItem!=nullptr){
                             SelItem->setGraphicsEffect(0);
                         }
                         SelItem=item;
                         item->setGraphicsEffect(effect);

                         //Pour détecter le type de l'item sélectionné et afficher les caractéristiques correspondantes
                         pathItem = dynamic_cast<QGraphicsPathItem*>(item);
                         lineItem = dynamic_cast<QGraphicsLineItem*>(item);
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
                             emit  setFillColor(rectItem->brush().color());
                         }
                        break;
                     }
                }


                if(!somethingSelected)
                    if(SelItem!=nullptr){
                        SelItem->setGraphicsEffect(0);
                        SelItem=nullptr;
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
