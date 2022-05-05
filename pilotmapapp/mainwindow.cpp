#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

    this->setMinimumSize(400, 240);

    //

    map = new mapWidget(this);

    setCentralWidget(map);
}

MainWindow::~MainWindow(){
}

//

void MainWindow::closeApplication(){
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent* event){
    closeApplication();
}