#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "drawzone.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent * event) { event->ignore(); quitApp(); }
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    DrawZone * drawZone;
    QAction * rouge, * vert, * bleu, * noir;
    QAction* width1, * width2, * width3;
    QAction* style1, * style2, * style3;
    QAction* delete1, * nouveau;
    QAction* trait, * rectangle, * ellipse, * polyline, * polygone, *selection;
    QColorDialog * colorDialog;

public slots:
    void openFile(); // Ouvre des formes serialisees
    void saveFile(); // Serialise des formes
    void quitApp(); // Quitte l'application
    void style(QAction*); // Change le style du trait
    void color(QAction*); // Change la couleur du trait (menu)
    void width(QAction*); // Change l'epaisseur du trait (menu)
    void deleteShape(QAction*); // Suppression d'une ou de toutes les formes
    void changeShape(QAction*); // Changement d'etat de la forme a tracer
    void colorUI(); // Ouvre le pop-up permettant de changer la couleur
    void selectColor(); // Change la couleur du trait (pop-up)
    void widthUI(int); // Change l'epaisseur du trait (slider)
};

#endif // MAINWINDOW_H
