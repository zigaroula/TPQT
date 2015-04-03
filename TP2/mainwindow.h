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

public slots:
    void openFile();
    void saveFile();
    void quitApp();
    void style(QAction*);
    void color(QAction*);
    void width(QAction*);
    void deleteShape(QAction*);
    void changeShape(QAction*);
};

#endif // MAINWINDOW_H
