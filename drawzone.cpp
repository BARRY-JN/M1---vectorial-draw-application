#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QColorDialog>
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
    if(actualTool==POLYGON){
        polygons.append(*poly);
        polygon=nullptr;
        scene->removeItem(ellipse);
        ellipse=nullptr;
        poly= new QPolygonF();

    }

    actualTool=tool;
}

void drawZone::setactualSize(int value){
    actualSize=value;
}


void drawZone::setactualColor(QColor color){
    actualColor=color;
}

void drawZone::setactualColor2(QColor color){
    actualColor2=color;
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
        //scene->addLine(0,0,point.x(),point.y(),QPen(Qt::red));
        if(ev->buttons().testFlag(Qt::LeftButton)){
            if(count%2==0){
                path->lineTo(point);

                /*
                On efface le précédent tracé puisqu'un nouveau point a été rajouté
                */
                if(pathItem!=nullptr){
                    scene->removeItem(pathItem);
                }
                pathItem=scene->addPath(*path,QPen(actualColor, actualSize));

            }
            count++;
        }
    }

}

void drawZone::mouseReleaseEvent(QMouseEvent *event){
    if(actualTool==FREE){
        pathItem=nullptr;
        paths.append(*path);
        path=new QPainterPath();
    }
}

void drawZone::leaveEvent(QEvent * e)
{
    MainWindow::leaveDrawZone();
}

void drawZone::clearScene()
{

    scene->clear();
}

//------------Code ci dessous insipreré de-------------------------------------------------------
// ------- https://stackoverflow.com/questions/30678877/save-load-items-from-a-qgraphicsscene ---



//QVariant drawZone::item_to_variant(QGraphicsItem* item)
//{
//  QVariantHash data;
//  //save all needed attributes
//  data["pos"] = item->pos();
//  data["rotation"] = item->rotation();
//  if(QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item))
//  {
//    data["type"] = "pixmap";
//    data["pixmap"] = pixmap_item->pixmap();
//  } else { /*...*/ }
// //...
//  return data;
//}

//QGraphicsItem* drawZone::item_from_variant(QVariant v)
//{
//  QVariantHash data = v.toHash();
//  QGraphicsItem* result;
//  if (data["type"].toString() == "pixmap") {
//    result = new QGraphicsPixmapItem();
//    result->setPixmap(data["pixmap"].value<QPixmap>());
//  } else { /*...*/ }
//  result->setPos(data["pos"].toPointF());
//  result->setRotation(data["rotation"].toDouble());
//  //...
//  return result;
//}

//void drawZone::save_state()
//{
//  QVariantList data_list;
//  foreach(QGraphicsItem* item, items_list)
//  {
//    data_list << item_to_variant(item);
//  }
//  QSettings settings;
//  settings.setValue("items", data_list);
//}

//void drawZone::restore_state()
//{
//  QSettings settings;
//  foreach(QVariant data, settings.value("items").toList()) {
//    QGraphicsItem* item = item_from_variant(data);
//    scene->addItem(item);
//    items_list << item;
//  }

//}


//----------------------------------------------------------------------------------------


void drawZone::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y()
    << ev->button();
    switch(ev->button()) {
    case Qt::RightButton :
        showcontextmenu();
        break;
    default:
    break;
    }
    if(ev->buttons().testFlag(Qt::LeftButton))
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
                    scene->removeItem(ellipse);
                }
                poly->append(point_init);
                //polygon->setPolygon(*poly);
                polygon=scene->addPolygon(*poly,actualColor,actualColor2);
                ellipse=scene->addEllipse(point_init.x()-5,point_init.y()-5,10,10, QColor(Qt::red), QColor(Qt::red));

                break;
            }

            case(RECTANGLE):
            {
                QGraphicsRectItem *rectangle;
                rectangle = scene->addRect(point.x()-(actualSize/2),point.y()-(actualSize/2),actualSize,actualSize,actualColor, actualColor2);
                rectangle->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }

            case(TRIANGLE):
            {
                QGraphicsPolygonItem *polygon;
                QPolygonF poly;
                poly << QPointF(point.x()-actualSize, point.y()) << QPointF(point.x()+actualSize, point.y()) << QPointF(point.x(), point.y()-actualSize );
                polygon=scene->addPolygon(poly,actualColor, actualColor2);
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

            case(IMAGE):
                break;
            case(MOVE):
                break;
            case(ROTATE):
                break;
            case(NONE):
                break;
        }


    }
// call base class
QGraphicsView::mousePressEvent(ev);

}

void drawZone::showcontextmenu()
{
    QMenu menu(this);
    menu.addAction(new QAction("Copier", this));
    menu.addAction(new QAction("Coller", this));
    menu.addAction(new QAction("Supprimer", this));
    menu.exec(QCursor::pos());
}
