#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QApplication>

#include "Widgets/mapWidget.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
private:
    mapWidget* map = nullptr;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent* event);

public slots:
    void closeApplication();
};
#endif // MAINWINDOW_H
