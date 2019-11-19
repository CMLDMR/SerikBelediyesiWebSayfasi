#include "haritawidget.h"


const WLeafletMap::Coordinate HaritaWidget::SERIKBLD_COORDS(36.91623,31.104);

HaritaWidget::HaritaWidget()
{
    auto layout = setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());
    setHeight(400);
    map_ = layout->addWidget(Wt::cpp14::make_unique<Wt::WLeafletMap>(), 1);
    Wt::Json::Object options;
    options["maxZoom"] = 19;
    options["attribution"] = "&copy; <a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> contributors";
    map_->addTileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", options);
    map_->panTo(SERIKBLD_COORDS);
    setAttributeValue (Style::style,Style::background::color::color (Style::color::White::White));



}

void HaritaWidget::addSerikLogoMarker()
{
    auto emwebLogo = Wt::cpp14::make_unique<Wt::WImage>(
              Wt::WLink("img/serikbldcoor.png"));
        emwebLogo->setInline(false);
        emwebLogo->resize(64, 75);
        auto emwebMarker = Wt::cpp14::make_unique<Wt::WLeafletMap::WidgetMarker>(SERIKBLD_COORDS, std::move(emwebLogo));
        map_->addMarker(std::move(emwebMarker));
}

void HaritaWidget::setZoomLevel(const int &level)
{
    map_->setZoomLevel (level);
}
