#include <QDebug>
#include <QMouseEvent>
#include "drawzone.h"

drawZone::drawZone(QWidget *parent) :
    QGraphicsView(parent)
{
    qDebug() << "init drawZone" ;
    setMouseTracking(true);
}

void drawZone::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << "x= " << ev->x() << " y= " << ev->y();
    qDebug() << this->parent()->parent()->objectName();
}
