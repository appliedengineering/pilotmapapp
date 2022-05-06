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

    QHBoxLayout* hBoxLayout;
    QLineEdit* addressLineEdit;
    QPushButton* addressUpdateButton;

    void setupMapFromMmpk();

    void renderGraphics(Esri::ArcGISRuntime::GraphicsOverlay* overlay, bool shouldOnlyRenderBoat = false);

    Esri::ArcGISRuntime::Graphic* drawPoint(Esri::ArcGISRuntime::GraphicsOverlay* overlay, double lat, double lon, QColor pointColor = Qt::blue, QColor outlineColor = Qt::white);

    std::vector<std::pair<double, double>> loadBuoyCoordinates();

public:
    mapWidget(QWidget* parent = nullptr);
    ~mapWidget();
};

#endif