#include "anonswidget.h"

#include <Wt/WGoogleMap.h>
#include <Wt/WApplication.h>


AnonsWidget::AnonsWidget(mongocxx::database* _db , bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db , _user , "Anons Sitemi")
{
    wApp->require("http://www.openlayers.org/api/OpenLayers.js");
//    wApp->require("OpenLayer/ol.js");


    mMap = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mMap->setHeight(WLength(450,LengthUnit::Pixel));
    mMap->setWidth(WLength(100,LengthUnit::Percentage));
    mMap->setId("mapdiv");



    mDeviceListContainer = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mDeviceListContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_12);

    mDevicePropertiesContainer = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mDevicePropertiesContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_12);






    {
        auto container = this->getFooterRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setContentAlignment(AlignmentFlag::Center);
        auto refreshBtn = container->addWidget(cpp14::make_unique<WPushButton>(WString::fromUTF8("Cihazları Listele")));

        refreshBtn->clicked().connect([=](){
            this->initDevices();
        });
    }


//    {
//        auto text = this->getFooterRowContainer()->addWidget(cpp14::make_unique<WText>("Uygulamayı indirin"));
////        text->addStyleClass(Bootstrap::Grid::col_full_12);
//    }


    this->initDevices();


}

void AnonsWidget::initDevices()
{

    mDeviceListContainer->clear();

    auto fContainer = mDeviceListContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);


    auto filter = document{};

    std::string objlist = "var objlist = new Array;";

    try {
        auto cursor = this->db()->collection("AnonsCihazlari").find(filter.view());

        int count = 0 ;

        for( auto doc : cursor )
        {

            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());

            container->addStyleClass(Bootstrap::Grid::col_full_12);

            container->setContentAlignment(AlignmentFlag::Center);



            auto deviceWidget = container->addWidget(cpp14::make_unique<Device>(doc["_id"].get_oid().value.to_string()));


            std::string objstring = "var obj"+ std::to_string(count) +" = new Object;";

            try {
                auto value = doc["cihazadi"].get_utf8().value.to_string();
                deviceWidget->setDeviceName(value);
                objstring += "obj"+std::to_string(count)+"[\"name\"] = \"" + deviceWidget->deviceName()+"\";";

            } catch (bsoncxx::exception &e) {
//                std::cout << "Line " << __LINE__ << "->in doc cihazadi type is not " << "utf8 :"<< e.what() << std::endl;
                deviceWidget->setDeviceName(e.what());
            }

            try {
                auto value = doc["mahalle"].get_utf8().value.to_string();
                deviceWidget->setMahalle(value);
                objstring += "obj"+std::to_string(count)+"[\"mahalle\"] = \"" + value+"\";";
            } catch (bsoncxx::exception &e) {
//                std::cout << "Line " << __LINE__ << "->in doc mahalle type is not " << "utf8() :"<< e.what() << std::endl;
                deviceWidget->setMahalle(e.what());
            }

            try {
                auto value = doc["longtitude"].get_double().value;

                deviceWidget->setLongtitute(value);
                objstring += "obj"+std::to_string(count)+"[\"longtitude\"] = " + std::to_string(value)+";";


            } catch (bsoncxx::exception &e) {
//                std::cout << "Line " << __LINE__ << "->in doc longtitude type is not " << "double :"<< e.what() << std::endl;
            }

            try {
                auto value = doc["latitute"].get_double().value;
                deviceWidget->setLatitute(value);
                objstring += "obj"+std::to_string(count)+"[\"latitute\"] = " + std::to_string(value)+";";


            } catch (bsoncxx::exception &e) {
//                std::cout << "Line " << __LINE__ << "->in doc latitute type is not " << "double() :"<< e.what() << std::endl;
            }



            objstring += "obj"+std::to_string(count)+"[\"oid\"] = \"" + (deviceWidget->oid())+"\";";

            objstring += "obj"+std::to_string(count)+"[\"signal\"] = \"" + (deviceWidget->PopupClick().createCall({}))+"\";";

            objlist += objstring+"objlist.push(obj"+std::to_string(count)+");";

            deviceWidget->clicked().connect([=]{
                std::string script = "iconPixel(\""+std::to_string(deviceWidget->longtitute())+
                        "\",\""+std::to_string(deviceWidget->latitute())+
                        "\",\""+deviceWidget->deviceName()+
                        "\",\""+deviceWidget->mahalle()+
                        "\",\""+ deviceWidget->PopupClick().createCall({}) + "\");";
//                std::cout << "Script : " << script << std::endl;
                mMap->doJavaScript(script);
            });

            deviceWidget->PopupClick().connect([=](std::string str){
                std::cout << "POPUP Signal İmplement this About Anounce Devices: " << deviceWidget->mahalle() << std::endl;
            });



            count++;

        }

        objlist += "listdevice(objlist);";
        std::cout << objlist << std::endl;
        mMap->doJavaScript(objlist);



    } catch (mongocxx::exception &e) {

    }



//    auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());



}

Device::Device(std::string oid)
    :WContainerWidget () , _mPopupClick(this,oid)
{
    setOid(oid);
    setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::DarkSlateBlue));
    setMargin(3,Side::Top|Side::Bottom);
    setMargin(0,Side::Left|Side::Right);
    decorationStyle().setCursor(Cursor::PointingHand);

    auto vLayout = setLayout(cpp14::make_unique<WVBoxLayout>());

    mDeviceText = vLayout->addWidget(cpp14::make_unique<WText>("Device Name"),0,AlignmentFlag::Middle|AlignmentFlag::Center);

    mDeviceText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);

    mMahalleText = vLayout->addWidget(cpp14::make_unique<WText>("Device Mahalle"),0,AlignmentFlag::Middle|AlignmentFlag::Center);

    mMahalleText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s10px+Style::font::weight::normal);
}

Signal<std::string> &Device::ClickOid()
{
    return _mClickOid;
}

double Device::longtitute() const
{
    return mLongtitute;
}

void Device::setLongtitute(double longtitute)
{
    mLongtitute = longtitute;
}

double Device::latitute() const
{
    return mLatitute;
}

void Device::setLatitute(double latitute)
{
    mLatitute = latitute;
}

std::string Device::deviceName() const
{
    return mDeviceName;
}

void Device::setDeviceName(const std::string &deviceName)
{
    mDeviceName = deviceName;
    mDeviceText->setText(mDeviceName);
}

std::string Device::oid() const
{
    return mOid;
}

void Device::setOid(const std::string &oid)
{
    mOid = oid;
}

std::string Device::mahalle() const
{
    return mMahalle;
}

void Device::setMahalle(const std::string &mahalle)
{
    mMahalle = mahalle;
    mMahalleText->setText(mMahalle);
}

JSignal<std::string> &Device::PopupClick()
{
    return _mPopupClick;
}

