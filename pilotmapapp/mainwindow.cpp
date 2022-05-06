#include "mainwindow.h"

#include "Backend/Communication/communicationThread.h"

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
    communicationThread::getInstance()->stop();
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent* event){
    closeApplication();
}