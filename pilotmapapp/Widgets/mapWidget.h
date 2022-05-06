#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <vector>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QLineEdit>
#include <QPushButton>

namespace Esri{
    namespace ArcGISRuntime{
        class Map;
        class MapGraphicsView;
        class Viewpoint;
        class GraphicsOverlay;
        class Graphic;
    }
}

class mapWidget : public QWidget{
    Q_OBJECT

private:
    QVBoxLayout* vBoxLayout;
    
    Esri::ArcGISRuntime::MapGraphicsView* arcGISMapView = nullptr;    
    Esri::ArcGISRuntime::GraphicsOverlay* arcGISOverlay = nullptr;

    Esri::ArcGISRuntime::Graphic* boatPoint = nullptr;
    
    double boatLat = 0, boatLon = 0;
    
    QHBoxLayout* hBoxLayout;
    QLineEdit* addressLineEdit;
    QPushButton* addressUpdateButton;

    void renderBar();

    void setupMapFromMmpk();

    void renderGraphics(Esri::ArcGISRuntime::GraphicsOverlay* overlay, bool shouldOnlyRenderBoat = false);

    Esri::ArcGISRuntime::Graphic* drawPoint(Esri::ArcGISRuntime::GraphicsOverlay* overlay, double lat, double lon, QColor pointColor = Qt::blue, QColor outlineColor = Qt::white);

    std::vector<std::pair<double, double>> loadBuoyCoordinates();

public:
    mapWidget(QWidget* parent = nullptr);
    ~mapWidget();

public slots:
    void updateBoatLocation(double lat, double lon);
};

#endif