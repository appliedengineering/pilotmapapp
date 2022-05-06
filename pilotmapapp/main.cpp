#include "mainwindow.h"

#include "Backend/Communication/communicationManager.h"
#include "Backend/Communication/communicationThread.h"

#include <QApplication>
#include <ArcGISRuntimeEnvironment.h>
#include <QFile>

#include <thread>
#include <chrono>
#include <future>

void setupArcGISRuntimeEnvironment(){
    QFile licenseKeyFile(":ArcGISLicenseKey.txt");
    if (licenseKeyFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString key(licenseKeyFile.readAll());

        qInfo() << "Loaded ArcGIS License Key";

        Esri::ArcGISRuntime::ArcGISRuntimeEnvironment::setLicense(key); 
    }
    else{
        qFatal("No ArcGIS License Key file found");
    }

    licenseKeyFile.close();
}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    setupArcGISRuntimeEnvironment();

    communicationManager::getInstance();

    auto commThread = std::async(std::launch::async, [&] {
        communicationThread::getInstance()->start();
    });

    MainWindow w;
    w.show();
    return a.exec();
}
