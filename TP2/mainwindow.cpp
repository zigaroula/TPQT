#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    colorDialog = new QColorDialog();
    QMenuBar * menuBar = this->menuBar( );
    QMenu * fileMenu = menuBar->addMenu( tr ("&Fichier") );
    QAction * open = new QAction( QIcon(":/icone/open.png"), tr("&Open..."), this);
    QAction * save = new QAction( QIcon(":/icone/save.png"), tr("&Save..."), this);
    QAction * quit = new QAction( QIcon(":/icone/quit.png"), tr("&Quit..."), this);
    //QToolBar * toolBar = this->addToolBar( tr("Fichier") );
    drawZone = new DrawZone(this);

    setCentralWidget(drawZone);

    // Suppression
    QActionGroup * deleteGroup = new QActionGroup(this);
    connect(deleteGroup, SIGNAL(triggered(QAction*)), this, SLOT(deleteShape(QAction*)));
    delete1 = deleteGroup->addAction(QIcon(":/icone/back.png"), tr("Retour"));
    nouveau = deleteGroup->addAction(QIcon(":/icone/open.png"), tr("Nouveau"));

    // Boutons generaux
    open->setShortcut( tr("Ctrl+O"));
    save->setShortcut( tr("Ctrl+S"));
    quit->setShortcut( tr("Ctrl+Q"));
    open->setToolTip( tr("Open a file"));
    save->setToolTip( tr("Save a file"));
    quit->setToolTip( tr("Quit"));
    open->setStatusTip( tr("Open a file"));
    save->setStatusTip( tr("Save a file"));
    quit->setStatusTip( tr("Quit"));
    fileMenu->addAction(nouveau);
    fileMenu->addAction(open);
    fileMenu->addAction(save);
    fileMenu->addAction(delete1);
    fileMenu->addAction(quit);
    //toolBar->addAction(open);
    //toolBar->addAction(save);
    //toolBar->addAction(quit);
    connect(open, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(save, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quit, SIGNAL(triggered()), this, SLOT(quitApp()));

    // Style
    QMenu * style = menuBar->addMenu(tr("&Style"));
    QActionGroup * styleGroup = new QActionGroup(this);
    connect(styleGroup, SIGNAL(triggered(QAction*)), this, SLOT(style(QAction*)));
    style1 = styleGroup->addAction(tr("&SolidLine"));
    style2 = styleGroup->addAction(tr("&DashLine"));
    style3 = styleGroup->addAction(tr("D&otLine"));
    style->addAction(style1);
    style->addAction(style2);
    style->addAction(style3);
    style1->setCheckable(true);
    style2->setCheckable(true);
    style3->setCheckable(true);
    style1->setChecked(true);

    // Couleur
    QMenu * color = menuBar->addMenu( tr("&Couleur") );
    QActionGroup * colorGroup = new QActionGroup(this);
    connect(colorGroup, SIGNAL(triggered(QAction*)), this, SLOT(color(QAction*)));
    rouge = colorGroup->addAction(tr("&Rouge"));
    bleu = colorGroup->addAction(tr("&Bleu"));
    vert = colorGroup->addAction(tr("&Vert"));
    noir = colorGroup->addAction(tr("&Noir"));
    color->addAction(rouge);
    color->addAction(bleu);
    color->addAction(vert);
    color->addAction(noir);
    rouge->setCheckable(true);
    bleu->setCheckable(true);
    vert->setCheckable(true);
    noir->setCheckable(true);
    noir->setChecked(true);

    // Taille
    QMenu * width = menuBar->addMenu(tr("&Largeur"));
    QActionGroup * widthGroup = new QActionGroup(this);
    connect(widthGroup, SIGNAL(triggered(QAction*)), this, SLOT(width(QAction*)));
    width1 = widthGroup->addAction(tr("&0"));
    width2 = widthGroup->addAction(tr("&3"));
    width3 = widthGroup->addAction(tr("&6"));
    width->addAction(width1);
    width->addAction(width2);
    width->addAction(width3);
    width1->setCheckable(true);
    width2->setCheckable(true);
    width3->setCheckable(true);
    width1->setChecked(true);

    // Forme
    QMenu * shape = menuBar->addMenu(tr("&Forme"));
    QActionGroup * shapeGroup = new QActionGroup(this);
    connect(shapeGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeShape(QAction*)));
    trait = shapeGroup->addAction(QIcon(":/icone/trait.png"), tr("&Trait"));
    rectangle = shapeGroup->addAction(QIcon(":/icone/rectangle.png"), tr("&Rectangle"));
    ellipse = shapeGroup->addAction(QIcon(":/icone/ellipse.png"), tr("&Ellipse"));
    polyline = shapeGroup->addAction(QIcon(":/icone/polyline.png"), tr("&Polyline"));
    polygone = shapeGroup->addAction(QIcon(":/icone/polygone.png"), tr("Poly&gone"));
    shape->addAction(trait);
    shape->addAction(rectangle);
    shape->addAction(ellipse);
    shape->addAction(polyline);
    shape->addAction(polygone);
    //toolBar->addAction(trait);
    //toolBar->addAction(rectangle);
    //toolBar->addAction(ellipse);
    //toolBar->addAction(polyline);
    //toolBar->addAction(polygone);

    // Selection
    selection = shapeGroup->addAction(QIcon(":/icone/cursor.png"), tr("&Selection"));
    shape->addAction(selection);
    //toolBar->addAction(selection);

    // Slider
    QSlider * slider = new QSlider();
    slider->setRange(0, 10);
    slider->setOrientation(Qt::Horizontal);
    ui->toolBar_2->addWidget(slider);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(widthUI(int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../TP2/files", tr("All Files (*)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    int taille, i;
    stream >> taille;
    for (i = 0 ; i < taille ; i++) {
        QPainterPath savedPath;
        stream >> savedPath;
        QPen savedPen;
        stream >> savedPen;
        drawZone->setShape(savedPath, savedPen);
    }
    qDebug() << "Opened the file " << qPrintable(fileName) << endl;
}

void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "../TP2/files", tr("All Files (*)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    QList<Shape> liste = drawZone->getList();
    stream << liste.size();
    QList<Shape>::iterator i;
    for (i = liste.begin(); i != liste.end(); i++) {
        stream << (*i).path << (*i).pen;
    }
    qDebug() << "Saved the file " << qPrintable(fileName) << endl;
}

void MainWindow::quitApp() {
    int reponse = QMessageBox::question(this, "Quit", "Voulez-vous vraiment quitter ?", QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes){
        qDebug() << "Quit" << endl;
        QApplication::quit();
    }
}

void MainWindow::style(QAction* sender) {
    if (sender == style1){
        drawZone->style(SOLIDLINE);
    }
    else if (sender == style2) {
        drawZone->style(DASHLINE);
    }
    else if (sender == style3) {
        drawZone->style(DOTLINE);
    }
}

void MainWindow::color(QAction* sender) {
    if (sender == rouge){
        drawZone->color(ROUGE);
    }
    else if (sender == bleu) {
        drawZone->color(BLEU);
    }
    else if (sender == vert) {
        drawZone->color(VERT);
    }
    else if (sender == noir) {
        drawZone->color(NOIR);
    }
}

void MainWindow::width(QAction* sender) {
    if (sender == width1){
        drawZone->width(WIDTH1);
    }
    else if (sender == width2) {
        drawZone->width(WIDTH2);
    }
    else if (sender == width3) {
        drawZone->width(WIDTH3);
    }
}

void MainWindow::deleteShape(QAction * sender) {
    if (sender == delete1 || sender == ui->actionRetour){
        drawZone->deleteShape(DELETE1);
    }
    else if (sender == nouveau || sender == ui->actionNouveau) {
        drawZone->deleteShape(DELETEALL);
    }
}

void MainWindow::changeShape(QAction * sender) {
    if (sender == trait || sender == ui->actionTrait) {
        drawZone->changeShape(TRAIT);
    }
    else if (sender == rectangle || sender == ui->actionRectangle) {
        drawZone->changeShape(RECTANGLE);
    }
    else if (sender == ellipse || sender == ui->actionEllipse) {
        drawZone->changeShape(ELLIPSE);
    }
    else if (sender == polyline || sender == ui->actionPolyline) {
        drawZone->changeShape(POLYLINE);
    }
    else if (sender == polygone || sender == ui->actionPolygone) {
        drawZone->changeShape(POLYGONE);
    }
    else if (sender == selection || sender == ui->actionSelection) {
        drawZone->changeShape(SELECTION);
    }
}

void MainWindow::colorUI() {
    colorDialog->open(this, SLOT(selectColor()));
}

void MainWindow::selectColor() {
    QColor couleur;
    couleur = colorDialog->selectedColor();
    drawZone->setCouleur(couleur);
}

void MainWindow::widthUI(int width) {
    drawZone->setWidth(width);
}
