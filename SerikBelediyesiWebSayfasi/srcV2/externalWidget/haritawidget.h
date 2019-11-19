#ifndef HARITAWIDGET_H
#define HARITAWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"


class HaritaWidget : public ContainerWidget
{
public:
    HaritaWidget();

    void addSerikLogoMarker();

    void setZoomLevel( const int& level );

private:
    Wt::WLeafletMap *map_;


    static const Wt::WLeafletMap::Coordinate SERIKBLD_COORDS;


};

#endif // HARITAWIDGET_H
