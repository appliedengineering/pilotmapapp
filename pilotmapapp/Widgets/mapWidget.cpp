#include "mapWidget.h"

#include "../Backend/Utilities/utilities.h"

#include <QDir>
#include <QFile>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

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

    hBoxLayout = new QHBoxLayout();

    const int hBoxLayoutPadding = 10;

    hBoxLayout->setContentsMargins(hBoxLayoutPadding, hBoxLayoutPadding, hBoxLayoutPadding, hBoxLayoutPadding);
    hBoxLayout->setSpacing(hBoxLayoutPadding);

    vBoxLayout->addLayout(hBoxLayout, 100 - arcGISMapViewStretchFactor);

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

mapWidget::~mapWidget(){
}

//

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

void mapWidget::renderGraphics(Esri::ArcGISRuntime::GraphicsOverlay* overlay, bool shouldOnlyRenderBoat = false){

}

Esri::ArcGISRuntime::Graphic* drawPoint(Esri::ArcGISRuntime::GraphicsOverlay* overlay, double lat, double lon, QColor pointColor, QColor outlineColor){

}

std::vector<std::pair<double, double>> loadBuoyCoordinates(){
    
}






