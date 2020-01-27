#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include "mainwindow.h"
#include "drawzone.h"

int PointActuel=0;

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

void drawZone::mouseMoveEvent(QMouseEvent *ev)
{
    MainWindow::setCursorLabelCoord(ev);
}

void drawZone::leaveEvent(QEvent * e)
{
    MainWindow::leaveDrawZone();
}

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
        QPointF point = mapToScene(x, y);
        QPointF point2;

        switch(MainWindow::getactualTool()){
            case(CIRCLE):
            {
                QGraphicsEllipseItem *circle;
                circle = scene->addEllipse(point.x(),point.y(),25,25,QPen(Qt::blue), QBrush(Qt::blue));
                circle->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }
            case(LINE):
            {
                if(PointActuel==0){
                    QGraphicsEllipseItem *ellipse;
                    ellipse = scene->addEllipse(point.x(),point.y(),5,5,QPen(Qt::red), QBrush(Qt::red));
                    ellipse->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                    x2=point.x();
                    y2=point.y();
                    PointActuel=1;
                    return;
                }

                if(PointActuel==1){
                    QGraphicsLineItem *line;
                    line = scene->addLine(x2,y2,point.x(),point.y(),QPen(Qt::red));
                    //PointActuel=0;
                    return;
                }
                break;
            }

            case(RECTANGLE):
            {
                QGraphicsRectItem *rectangle;
                rectangle = scene->addRect(point.x(),point.y(),25,25,QPen(Qt::red), QBrush(Qt::red));
                rectangle->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }

            case(TRIANGLE):
            {
                QGraphicsPolygonItem *polygon;
                QPolygonF poly;
                poly << QPointF(point.x()-15, point.y()) << QPointF(point.x()+15, point.y()) << QPointF(point.x(), point.y()-25 );
                polygon=scene->addPolygon(poly,QPen(Qt::green),QBrush(Qt::green));
                break;
            }

            case(TEXT):
            {
                QGraphicsTextItem *text;
                QFont serifFont("Times", 10, QFont::Bold);
                text = scene->addText(QString("Bonjour !"), QFont(serifFont));
                text->setPos(point);
                text->setFlag(QGraphicsEllipseItem::ItemIsMovable);
                break;
            }

            case(IMAGE):
                break;
            case(MOVE):
                break;
            case(ROTATE):
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
