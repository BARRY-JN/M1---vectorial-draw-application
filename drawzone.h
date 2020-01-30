#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtGui>
#include <QWidget>
#include <QColor>
#include "mainwindow.h"

class drawZone : public QGraphicsView
{
public:
    explicit drawZone(QWidget *parent = 0);
    ~drawZone() override;
    void setactualTool(Tool);
    void setactualColor(QColor);
    void setactualColor2(QColor);
    void setactualSize(int);
    void clearScene();
    void saveScene();
    //-----------------------------------------
//    QVariant item_to_variant(QGraphicsItem* item);
//    QGraphicsItem* item_from_variant(QVariant v);
//    void save_state();
//    void restore_state();

    //---------------------------------------------

protected:
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void leaveEvent(QEvent*) override;
    void showcontextmenu();

private :
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    QGraphicsTextItem *text;
    QGraphicsLineItem *line;
    QGraphicsPathItem *pathItem;
    QGraphicsPixmapItem *pixmap;
    QGraphicsPolygonItem *polygon;
    QGraphicsSimpleTextItem *simpletext;
    QPointF origPoint;
    Tool actualTool;
    QColor actualColor;
    QColor actualColor2;
    int actualSize;


signals:

public slots:

};

#endif // DRAWZONE_H
