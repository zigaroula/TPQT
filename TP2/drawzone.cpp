#include "drawzone.h"

DrawZone::DrawZone(QWidget *parent) : QWidget(parent) {
    this->setMouseTracking(true);
    this->setMinimumSize(800, 800);
    lineStyle = Qt::SolidLine;
    lineWidth = 0;
    lineColor = QColor("#000000");

    // Etat parent forme
    forme = new QState();

    // Etat trait
    trait = new QState(forme);
    traitClick = new QState(trait);
    traitMove = new QState(trait);
    addMouseTrans(traitClick, traitMove, this, QEvent::MouseButtonPress, Qt::LeftButton, this, SLOT(setDebut()));
    addMouseTrans(traitMove, traitMove, this, QEvent::MouseMove, Qt::NoButton, this, SLOT(moveTrait()));
    addMouseTrans(traitMove, traitClick, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(endTrait()));
    trait->setInitialState(traitClick);

    // Etat rectangle
    rectangle = new QState(forme);
    rectangleClick = new QState(rectangle);
    rectangleMove = new QState(rectangle);
    addMouseTrans(rectangleClick, rectangleMove, this, QEvent::MouseButtonPress, Qt::LeftButton, this, SLOT(setDebut()));
    addMouseTrans(rectangleMove, rectangleMove, this, QEvent::MouseMove, Qt::NoButton, this, SLOT(moveRectangle()));
    addMouseTrans(rectangleMove, rectangleClick, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(endRectangle()));
    rectangle->setInitialState(rectangleClick);

    // Etat ellipse
    ellipse = new QState(forme);
    ellipseClick = new QState(ellipse);
    ellipseMove = new QState(ellipse);
    addMouseTrans(ellipseClick, ellipseMove, this, QEvent::MouseButtonPress, Qt::LeftButton, this, SLOT(setDebut()));
    addMouseTrans(ellipseMove, ellipseMove, this, QEvent::MouseMove, Qt::NoButton, this, SLOT(moveEllipse()));
    addMouseTrans(ellipseMove, ellipseClick, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(endEllipse()));
    ellipse->setInitialState(ellipseClick);

    // Etat polyline
    polyline = new QState(forme);
    polylineClick = new QState(polyline);
    polylineMove = new QState(polyline);
    addMouseTrans(polylineClick, polylineMove, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(setDebut()));
    addMouseTrans(polylineMove, polylineMove, this, QEvent::MouseMove, Qt::NoButton, this, SLOT(movePoly()));
    addMouseTrans(polylineMove, polylineMove, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(newLine()));
    addMouseTrans(polylineMove, polylineClick, this, QEvent::MouseButtonRelease, Qt::RightButton, this, SLOT(endPolyline()));
    polyline->setInitialState(polylineClick);

    // Etat polygone
    polygone = new QState(forme);
    polygoneClick = new QState(polygone);
    polygoneMove = new QState(polygone);
    addMouseTrans(polygoneClick, polygoneMove, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(setDebut()));
    addMouseTrans(polygoneMove, polygoneMove, this, QEvent::MouseMove, Qt::NoButton, this, SLOT(movePoly()));
    addMouseTrans(polygoneMove, polygoneMove, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(newLine()));
    addMouseTrans(polygoneMove, polygoneClick, this, QEvent::MouseButtonRelease, Qt::RightButton, this, SLOT(endPolygone()));
    polygone->setInitialState(polygoneClick);

    // Etat selection
    selection = new QState(forme);
    selectionClick = new QState(selection);
    selectionMove = new QState(selection);
    addMouseTrans(selectionClick, selectionMove, this, QEvent::MouseButtonPress, Qt::LeftButton, this, SLOT(debutSelection()));
    addMouseTrans(selectionMove, selectionMove, this, QEvent::MouseMove, Qt::NoButton, this, SLOT(moveSelection()));
    addMouseTrans(selectionMove, selectionClick, this, QEvent::MouseButtonRelease, Qt::LeftButton, this, SLOT(endSelection()));
    selection->setInitialState(selectionClick);

    // Machine
    machine = new QStateMachine();
    forme->setInitialState(trait);
    machine->addState(forme);
    machine->setInitialState(forme);
    addCustomTrans(forme, trait, TRAIT);
    addCustomTrans(forme, rectangle, RECTANGLE);
    addCustomTrans(forme, ellipse, ELLIPSE);
    addCustomTrans(forme, polyline, POLYLINE);
    addCustomTrans(forme, polygone, POLYGONE);
    addCustomTrans(forme, selection, SELECTION);
    QObject::connect(selection, SIGNAL(exited()), this, SLOT(quitSelection()));
    machine->start();
}

DrawZone::~DrawZone() {

}

void DrawZone::paintEvent(QPaintEvent * e) {
    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(lineColor, lineWidth, lineStyle, Qt::SquareCap, Qt::BevelJoin));

    painter.drawPath(currentPath);
    painter.drawLine(tempPolyLine);


    QList<Shape>::iterator i;
    for (i = displayList.begin(); i != displayList.end(); i++) {
        painter.setPen((*i).pen);
        painter.drawPath((*i).path);
    }
}

void DrawZone::setDebut() {
    polyDebut = cursorPos(this);
    tempPolyLine = QLine(0,0,0,0);
    debut = cursorPos(this);
    fin = cursorPos(this);
    update();
}

void DrawZone::moveTrait() {
    fin = cursorPos(this);
    currentPath = QPainterPath();
    currentPath.moveTo(debut.x(), debut.y());
    currentPath.lineTo(fin.x(), fin.y());
    update();
}

void DrawZone::movePoly() {
    fin = cursorPos(this);
    currentPath.moveTo(debut.x(), debut.y());
    tempPolyLine = QLine(debut, fin);
    update();
}

void DrawZone::moveRectangle() {
    fin = cursorPos(this);
    currentPath = QPainterPath();
    currentPath.addRect(debut.x(), debut.y(), fin.x()-debut.x(), fin.y()-debut.y());
    update();
}

void DrawZone::moveEllipse() {
    fin = cursorPos(this);
    currentPath = QPainterPath();
    currentPath.addEllipse(debut.x(), debut.y(), fin.x()-debut.x(), fin.y()-debut.y());
    update();
}

void DrawZone::endTrait() {
    currentPath = QPainterPath();
    QPainterPath painterPath;
    fin = cursorPos(this);
    painterPath.moveTo(debut.x(), debut.y());
    painterPath.lineTo(fin.x(), fin.y());
    struct Shape shape;
    shape.pen = QPen(lineColor, lineWidth, lineStyle, Qt::SquareCap, Qt::BevelJoin);
    shape.path = painterPath;
    displayList.append(shape);
    update();
}

void DrawZone::endRectangle() {
    currentPath = QPainterPath();
    QPainterPath painterPath;
    fin = cursorPos(this);
    painterPath.addRect(debut.x(), debut.y(), fin.x()-debut.x(), fin.y()-debut.y());
    struct Shape shape;
    shape.pen = QPen(lineColor, lineWidth, lineStyle, Qt::SquareCap, Qt::BevelJoin);
    shape.path = painterPath;
    displayList.append(shape);
    update();
}

void DrawZone::endEllipse() {
    currentPath = QPainterPath();
    QPainterPath painterPath;
    fin = cursorPos(this);
    painterPath.addEllipse(debut.x(), debut.y(), fin.x()-debut.x(), fin.y()-debut.y());
    struct Shape shape;
    shape.pen = QPen(lineColor, lineWidth, lineStyle, Qt::SquareCap, Qt::BevelJoin);
    shape.path = painterPath;
    displayList.append(shape);
    update();
}

void DrawZone::newLine() {
    currentPath.lineTo(fin.x(), fin.y());
    debut = fin;
    update();
}

void DrawZone::endPolyline() {
    currentPath.lineTo(fin.x(), fin.y());
    struct Shape shape;
    shape.pen = QPen(lineColor, lineWidth, lineStyle, Qt::SquareCap, Qt::BevelJoin);
    shape.path = currentPath;
    displayList.append(shape);
    currentPath = QPainterPath();
    tempPolyLine = QLine(0,0,0,0);
    update();
}

void DrawZone::endPolygone() {
    currentPath.lineTo(fin.x(), fin.y());
    currentPath.moveTo(fin.x(), fin.y());
    currentPath.lineTo(polyDebut.x(), polyDebut.y());
    struct Shape shape;
    shape.pen = QPen(lineColor, lineWidth, lineStyle, Qt::SquareCap, Qt::BevelJoin);
    shape.path = currentPath;
    displayList.append(shape);
    currentPath = QPainterPath();
    tempPolyLine = QLine(0,0,0,0);
    update();
}

void DrawZone::style(int style) {
    switch(style){
    case SOLIDLINE: lineStyle = Qt::SolidLine; break;
    case DASHLINE: lineStyle = Qt::DashLine; break;
    case DOTLINE: lineStyle = Qt::DotLine; break;
    }
    if (selectionBool) {
        (*indexSelection).pen.setStyle(lineStyle);
    }
    update();
}

void DrawZone::color(int couleur) {
    switch(couleur){
    case ROUGE: lineColor = QColor("#ff0000"); break;
    case VERT: lineColor = QColor("#00ff00"); break;
    case BLEU: lineColor = QColor("#0000ff"); break;
    case NOIR: lineColor = QColor("#000000"); break;
    }
    if (selectionBool) {
        (*indexSelection).pen.setColor(lineColor);
    }
    update();
}

void DrawZone::width(int width) {
    switch(width){
    case WIDTH1: lineWidth = 0; break;
    case WIDTH2: lineWidth = 3; break;
    case WIDTH3: lineWidth = 6; break;
    }
    if (selectionBool) {
        (*indexSelection).pen.setWidth(lineWidth);
    }
    update();
}

void DrawZone::deleteShape(int deleteShape) {
    switch(deleteShape){
    case DELETE1:
        displayList.removeLast();
        debut = QPoint(0, 0);
        fin = QPoint(0, 0);
        update();
        break;
    case DELETEALL:
        displayList.clear();
        debut = QPoint(0, 0);
        fin = QPoint(0, 0);
        update();
        break;
    }
}

void DrawZone::changeShape(int changeShape){
    machine->postEvent(new CustomEvent(changeShape));
}

QList<Shape> DrawZone::getList() {
    return displayList;
}

void DrawZone::setShape(QPainterPath savedPath, QPen savedPen) {
    struct Shape shape;
    shape.path = savedPath;
    shape.pen = savedPen;
    displayList.append(shape);
}

void DrawZone::debutSelection() {
    debut = cursorPos(this);
    fin = cursorPos(this);
    selectionPath = QPainterPath();
    selectionPath.addEllipse(cursorPos(this).x()-5,cursorPos(this).y()-5,10,10);
    QList<Shape>::iterator i;
    for (i = displayList.begin(); i != displayList.end(); i++) {
        if (selectionPath.intersects((*i).path)) {
            indexSelection = i;
            selectionBool = true;
        }
    }
    update();
}

void DrawZone::moveSelection() {
    if (selectionBool) {
        fin = cursorPos(this);
        (*indexSelection).path.translate(fin.x()-debut.x(), fin.y()-debut.y());
        update();
        debut = cursorPos(this);
    }
}

void DrawZone::endSelection() {

}

void DrawZone::quitSelection() {
    indexSelection = QList<Shape>::iterator();
    selectionBool = false;
}
