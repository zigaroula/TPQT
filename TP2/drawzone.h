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
    void style(int); // Change le style du trait courant ou selectionne
    void color(int); // Change la couleur du trait courant ou selectionne
    void width(int); // Change l'epaisseur du trait courant ou selectionne
    void deleteShape(int); // Supprime une ou plusieurs formes de la QList
    void changeShape(int); // Change l'etat de la machine a etats
    QList<Shape> getList(); // Renvoie la liste des formes (sert notemment a la serialisation)
    void setShape(QPainterPath, QPen); // Change la forme (sert notemment a la serialisation)
    void setCouleur(QColor); // Change la couleur courante (pop-up)
    void setWidth(int); // Change l'epaisseur courante (slider)

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
    void setDebut(); // Debut de toute forme
    void moveTrait(); // Appele quand on déplace la souris
    void moveRectangle(); // Appele quand on déplace la souris
    void moveEllipse(); // Appele quand on déplace la souris
    void endTrait(); // Appele quand on termine la forme
    void endRectangle(); // Appele quand on termine la forme
    void endEllipse(); // Appele quand on termine la forme
    void movePoly(); // Appele quand on déplace la souris
    void newLine(); // Appele quand on clique
    void endPolyline(); // Appele quand on termine la forme
    void endPolygone(); // Appele quand on termine la forme
    void debutSelection(); // Appele quand on appuie sur le bouton
    void moveSelection(); // Appele quand on déplace la souris
    void quitSelection(); // Appele quand on relache la souris
};

#endif // DRAWZONE_H
