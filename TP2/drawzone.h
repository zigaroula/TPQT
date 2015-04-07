#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QBrush>
#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAction>
#include <string>
#include <QList>
#include <QColorDialog>
#include "Transitions.h"

enum Param{ ROUGE, VERT, BLEU, NOIR,
            WIDTH1, WIDTH2, WIDTH3,
            SOLIDLINE, DASHLINE, DOTLINE,
            DELETE1, DELETEALL,
            TRAIT, RECTANGLE, ELLIPSE, POLYLINE, POLYGONE, SELECTION };

struct Shape {
    QPainterPath path;
    QPen pen;
};

class DrawZone : public QWidget {
    Q_OBJECT
public:
    explicit DrawZone(QWidget *parent = 0);
    ~DrawZone();
    void style(int);
    void color(int);
    void width(int);
    void deleteShape(int);
    void changeShape(int);
    QList<Shape> getList();
    void setShape(QPainterPath, QPen);
    void setCouleur(QColor);
    void setWidth(int);

protected:
    virtual void paintEvent ( QPaintEvent* );

private:
    QPointF debugCurrentPath;
    QPoint polyDebut;
    QPoint debut;
    QPoint fin;
    QColor lineColor;
    qreal lineWidth;
    Qt::PenStyle lineStyle;
    QList<Shape> displayList;
    QPainterPath currentPath;
    QLine tempPolyLine;
    QPainterPath selectionPath;
    QList<Shape>::iterator indexSelection;

    QStateMachine * machine;
    QState * forme;
    QState * trait,  * rectangle, * ellipse, * polyline, * polygone, * selection;
    QState * traitClick, * traitMove;
    QState * rectangleClick, * rectangleMove;
    QState * ellipseClick, * ellipseMove;
    QState * polylineClick, * polylineMove;
    QState * polygoneClick, * polygoneMove;
    QState * selectionClick, * selectionMove;

    bool selectionBool;

signals:

public slots:
    void setDebut();
    void moveTrait();
    void moveRectangle();
    void moveEllipse();
    void endTrait();
    void endRectangle();
    void endEllipse();
    void movePoly();
    void newLine();
    void endPolyline();
    void endPolygone();
    void debutSelection();
    void moveSelection();
    void quitSelection();
};

#endif // DRAWZONE_H
