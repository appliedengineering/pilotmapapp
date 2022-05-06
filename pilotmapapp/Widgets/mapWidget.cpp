#include "mapWidget.h"

#include "../Backend/Utilities/utilities.h"
#include "../Backend/boatKernel.h"
#include "../Backend/Communication/communicationManager.h"

#include <QDir>
#include <QFile>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <QFont>

#include <ArcGISRuntimeEnvironment.h>
#include <Map.h>
#include <Basemap.h>
#include <MapGraphicsView.h>
#include <Viewpoint.h>
#include <Point.h>
#include <MobileMapPackage.h>
#include <Error.h>

#include <GraphicsOverlay.h>
#include <PolylineBuilder.h>
#include <PolygonBuilder.h>
#include <SimpleMarkerSceneSymbol.h>
#include <SimpleFillSymbol.h>
#include <SimpleMarkerSymbol.h>

using namespace Esri::ArcGISRuntime;

mapWidget::mapWidget(QWidget* parent){
    utilities::setPaletteColor(this, QPalette::Background, Qt::white);

    //

    vBoxLayout = new QVBoxLayout(this);
    this->setLayout(vBoxLayout);
    
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);

    //

    const int arcGISMapViewStretchFactor = 90;
    arcGISMapView = new MapGraphicsView(this);

	setupMapFromMmpk();

    vBoxLayout->addWidget(arcGISMapView, arcGISMapViewStretchFactor);

	//

	arcGISOverlay = new GraphicsOverlay(this);
	renderGraphics(arcGISOverlay);
	arcGISMapView->graphicsOverlays()->append(arcGISOverlay);

    //

    hBoxLayout = new QHBoxLayout();

    const int hBoxLayoutPadding = 10;

    hBoxLayout->setContentsMargins(hBoxLayoutPadding, hBoxLayoutPadding, hBoxLayoutPadding, hBoxLayoutPadding);
    hBoxLayout->setSpacing(hBoxLayoutPadding);

    vBoxLayout->addLayout(hBoxLayout, 100 - arcGISMapViewStretchFactor);

    //

	renderBar();

	//

	connect(boatKernel::getInstance(), &boatKernel::locationUpdateSignal, this, &mapWidget::updateBoatLocation);
	connect(addressUpdateButton, &QPushButton::released, this, &mapWidget::updateConnectionAddress);
}

mapWidget::~mapWidget(){
	disconnect(boatKernel::getInstance(), &boatKernel::locationUpdateSignal, this, &mapWidget::updateBoatLocation);

}

//

void mapWidget::updateBoatLocation(double lat, double lon){
	boatLat = lat;
	boatLon = lon;
	
	renderGraphics(arcGISOverlay, true);
	updateBoatCoordinateLabel(lat, lon);
}

void mapWidget::updateBoatCoordinateLabel(double lat, double lon){
	const int numOfDecimalPlaces = 6;
	QString newBoatCoordinateLabelText = "(" + QString::number(utilities::roundDouble(boatLat, numOfDecimalPlaces)) + ", " + QString::number(utilities::roundDouble(boatLon, numOfDecimalPlaces)) + ")";
	boatCoordinateLabel->setText(newBoatCoordinateLabelText);
}

void mapWidget::updateConnectionAddress(){
	communicationManager::getInstance()->reconnect(communicationManager::getInstance()->getIPCSocket(), addressLineEdit->text().toStdString());
}

//

void mapWidget::renderBar(){

	boatCoordinateLabel = new QLabel(this);

	boatCoordinateLabel->setText("(0.000000, 0.000000)");
	boatCoordinateLabel->setFixedWidth(180);
	boatCoordinateLabel->setAlignment(Qt::AlignHCenter);
	utilities::setPaletteColor(boatCoordinateLabel, QPalette::Foreground, Qt::black, true);
	
	QFont boatCoordinateLabelFont = boatCoordinateLabel->font();
	boatCoordinateLabelFont.setPixelSize(15);
	boatCoordinateLabel->setFont(boatCoordinateLabelFont);

	hBoxLayout->addWidget(boatCoordinateLabel);

	//

   	addressLineEdit = new QLineEdit(this);

    addressLineEdit->setText("localhost");
    addressLineEdit->setAlignment(Qt::AlignHCenter);
    utilities::setPaletteColor(addressLineEdit, QPalette::Text, Qt::black, true);
    utilities::setPaletteColor(addressLineEdit, QPalette::Base, Qt::white);

    hBoxLayout->addWidget(addressLineEdit);

    //

    addressUpdateButton = new QPushButton(this);

    addressUpdateButton->setText("Update");
    utilities::setPaletteColor(addressUpdateButton, QPalette::ButtonText, Qt::black, true);

    hBoxLayout->addWidget(addressUpdateButton);
}

void mapWidget::setupMapFromMmpk(){
    QString mmpkPath = QDir::currentPath() + "/map.mmpk";

	MobileMapPackage* mappackage = new MobileMapPackage(mmpkPath, this);

	connect(mappackage, &MobileMapPackage::doneLoading, this, [mappackage, this](Error error){
		if (!error.isEmpty()){
			qDebug() << "Error: " << error.message() << error.additionalMessage();
			return;
		}

		arcGISMapView->setMap(mappackage->maps().at(0));
		
    });

	mappackage->load();
}

void mapWidget::renderGraphics(Esri::ArcGISRuntime::GraphicsOverlay* overlay, bool shouldOnlyRenderBoat){
    if (!shouldOnlyRenderBoat)
		overlay->graphics()->clear();

	//

	if (boatPoint != nullptr){
		//qInfo() << "existing boatPoint";
		for (int i = 0; i < overlay->graphics()->size(); i++){
			if (overlay->graphics()->at(i) == boatPoint){
				//qInfo() << "found boatPoint -> " << overlay->graphics()->size();
				overlay->graphics()->removeAt(i);
				break;
			}
		}
	}

	boatPoint = drawPoint(overlay, boatLat, boatLon);

	//

	if (!shouldOnlyRenderBoat){

		std::vector<std::pair<double, double>> c = loadBuoyCoordinates();

		for (std::pair<double, double> i : c)
			drawPoint(overlay, i.first, i.second, Qt::red, Qt::black);

		//qInfo() << "after loading buoys -> " << overlay->graphics()->size();

	}
}

Esri::ArcGISRuntime::Graphic* mapWidget::drawPoint(Esri::ArcGISRuntime::GraphicsOverlay* overlay, double lat, double lon, QColor pointColor, QColor outlineColor){
    Point p(lon, lat, SpatialReference::wgs84());

  	SimpleLineSymbol* pOutline = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, outlineColor, 7, this);
	SimpleMarkerSymbol* pSymb = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, pointColor, 10, this);
	pSymb->setOutline(pOutline);

	Graphic* point = new Graphic(p, pSymb, this);
	overlay->graphics()->append(point);
	return point;
}

std::vector<std::pair<double, double>> mapWidget::loadBuoyCoordinates(){
    std::ifstream coordinateFile("buoycoords.txt");

	std::vector<std::pair<double, double>> c;

	if (!coordinateFile.is_open()){
		qInfo() << "No buoy coordinate file";
		return c;
	}

	std::string fileLine;
	while (std::getline(coordinateFile, fileLine)){
		if (fileLine.empty())
			continue;
		
		std::istringstream iss(fileLine);
		double x, y;
		if (!(iss >> x >> y)){
			qDebug() << "error parsing line " << QString::fromStdString(fileLine) << " in buoycoords.txt"; 
			continue;
		}

		c.push_back({x, y});
	}

	return c;
}






