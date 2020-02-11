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
    Q_OBJECT
public:
    explicit drawZone(QWidget *parent = 0);
    ~drawZone() override;
    static int getWidth();
    static int getHeight();
    void setWidht(int w);
    void setHeight(int h);
    void setactualTool(Tool);
    void setactualColor(QColor);
    void setactualColor2(QColor);
    void setactualSize(int);
    void setactualtextSize(int);
    void setactualtextFont(QFont);
    void setactualtextContent(QString);
    void clearScene();
    void saveScene();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool saveAsFile();
    void addpicture(QPixmap pixmap);
    QGraphicsItem* getActItem();
    QGraphicsScene *getScene();
    void selectNothing();

protected:
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void leaveEvent(QEvent*) override;
    void keyPressEvent( QKeyEvent*) override;
    void showcontextmenu();

private slots:
    void deleteItem();
    void CopyItem();

private :
    QGraphicsScene *scene;
    QPointF origPoint;
    Tool actualTool;
    QColor actualColor;
    QColor actualColor2;
    int actualSize;
    int actualtextSize;
    QFont actualtextFont;
    QString actualtextContent;
    int PointActuel=0;
    int count=0;
    bool first=true;
    QPointF point_init;
    QPainter *painter;
    QMenu *menu;
    QAction *qduplicate, *qdelete;

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

     //pour le deplacement
     double initX,initY;

     //pour la rotation
     bool centerPointSet=false;
     bool doRotate=false;
     int angle=0;

     //Pour les previews des différentes formes
     QGraphicsEllipseItem *previewcircle=nullptr;
     QGraphicsLineItem* previewline=nullptr ;
     QGraphicsPolygonItem* previewtriangle=nullptr ;
     QGraphicsRectItem* previewrectangle=nullptr;
     QGraphicsTextItem* previewtext=nullptr;

     QPointF PreviousPoint;


signals:
    void actualToolShowProperty(Tool);
    void setStrokeColor(QColor);
    void setFillColor(QColor);
    void setStrokeSize(int);
    void setTextFont(QFont);
    void setTextContent(QString);


};

#endif // DRAWZONE_H
