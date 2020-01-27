#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtGui>
#include <QWidget>

class drawZone : public QGraphicsView
{
public:
    explicit drawZone(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *) override;
    void showcontextmenu();

private :
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    QGraphicsTextItem *text;
    QGraphicsLineItem *line;
    QGraphicsPathItem *path;
    QGraphicsPixmapItem *pixmap;
    QGraphicsPolygonItem *polygon;
    QGraphicsSimpleTextItem *simpletext;
    QPointF origPoint;

signals:

public slots:

};

#endif // DRAWZONE_H
