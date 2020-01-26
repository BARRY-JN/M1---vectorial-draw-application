#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QGraphicsView>

class drawZone : public QGraphicsView
{
public:
    explicit drawZone(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *);
};

#endif // DRAWZONE_H
