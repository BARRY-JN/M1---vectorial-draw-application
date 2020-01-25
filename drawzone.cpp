#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include "mainwindow.h"
#include "drawzone.h"

drawZone::drawZone(QWidget *parent) :
    QGraphicsView(parent)
{
    qDebug() << "init drawZone" ;
    setMouseTracking(true);
}

void drawZone::mouseMoveEvent(QMouseEvent *ev)
{
    MainWindow::setCursorLabelCoord(ev);
}
