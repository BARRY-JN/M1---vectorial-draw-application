#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtGui>
#include <QWidget>
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include "mainwindow.h"

class drawZone : public QGraphicsView
{
    Q_OBJECT;
public:
    explicit drawZone(QWidget *parent = 0);
    ~drawZone() override;
    void setactualTool(Tool);
    void setactualColor(QColor);
    void setactualColor2(QColor);
    void setactualSize(int);
    void clearScene();
    void saveScene();

protected:
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void leaveEvent(QEvent*) override;
    void showcontextmenu();

private :
    QGraphicsScene *scene;
    QGraphicsRectItem *rectangle;
    QGraphicsTextItem *text;
    QGraphicsLineItem *line;
    QGraphicsPixmapItem *pixmap;
    QGraphicsSimpleTextItem *simpletext;
    QPointF origPoint;
    Tool actualTool;
    QColor actualColor;
    QColor actualColor2;
    int actualSize;
    int PointActuel=0;
    int count=0;
    bool first=true;
    QPointF point_init;

    //Pour le dessin à main levée
    QGraphicsPathItem *pathitem=nullptr;
    QVector<QPainterPath> paths;
    QPainterPath *path=new QPainterPath();
    QGraphicsEllipseItem *previewPoint=nullptr;

    //Pour le dessin polygonal
     QGraphicsPolygonItem *polygon=nullptr;
     QPolygonF *poly=new QPolygonF();
     QGraphicsEllipseItem *actualPoint=nullptr;
     QVector<QPolygonF> polygons;

    //Pour la selection
     QGraphicsItem *SelItem=nullptr;
     QGraphicsPathItem* pathItem ;
     QGraphicsLineItem* lineItem ;
     QGraphicsPolygonItem* polygonItem ;
     QGraphicsRectItem* rectItem;
     QGraphicsEllipseItem* elliItem ;
     QGraphicsTextItem* textItem;


signals:
    void actualToolShowProperty(Tool);
    void setStrokeColor(QColor);
    void setFillColor(QColor);
    void setStrokeSize(int);


};

#endif // DRAWZONE_H
