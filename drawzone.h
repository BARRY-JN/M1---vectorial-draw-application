#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QGraphicsView>

class drawZone : public QGraphicsView
{
public:
    explicit drawZone(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *ev) override;
};

#endif // DRAWZONE_H
