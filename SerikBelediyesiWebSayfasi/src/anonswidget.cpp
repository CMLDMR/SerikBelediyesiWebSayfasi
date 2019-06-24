#include "anonswidget.h"

#include <Wt/WGoogleMap.h>
#include <Wt/WApplication.h>


AnonsWidget::AnonsWidget(mongocxx::database* _db , bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db , _user , u8"Anons Cihazları")
{

    mMap = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mMap->setHeight(WLength(450,LengthUnit::Pixel));
    mMap->setWidth(WLength(100,LengthUnit::Percentage));
    mMap->setId("mapdiv");


    {
        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setContentAlignment(AlignmentFlag::Center);
        auto refreshBtn = container->addWidget(cpp14::make_unique<WPushButton>(WString::fromUTF8(u8"Tüm Cihazlar")));
        refreshBtn->addStyleClass(Bootstrap::Button::Primary);
        refreshBtn->clicked().connect([=](){
            this->initDevices();
        });
    }


    mDeviceListContainer = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mDeviceListContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_12);

    mDevicePropertiesContainer = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mDevicePropertiesContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_12);




    {
        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());

        auto text = container->addWidget(cpp14::make_unique<WText>(WString::fromUTF8(u8"B Türkçe Karakter Sorunu ÇŞĞÖİÜ").toUTF8().c_str()));
    }





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


            try {
                auto value = doc["durum"].get_utf8().value.to_string();
                deviceWidget->setDurum(value);
            } catch (bsoncxx::exception &e) {
                std::cout << "Line " << __LINE__ << "->in doc durum type is not " << "utf8() :"<< e.what() << std::endl;
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
                mMap->doJavaScript(script);
                this->DeviceProperties(deviceWidget->oid());
            });

            deviceWidget->PopupClick().connect([=](std::string str){
                std::cout << "POPUP Signal İmplement this About Anounce Devices: " << deviceWidget->mahalle() << std::endl;
                this->DeviceProperties(deviceWidget->oid());
            });

            count++;

        }

        objlist += "listdevice(objlist);";
        //        std::cout << objlist << std::endl;
        mMap->doJavaScript(objlist);

    } catch (mongocxx::exception &e) {

    }

    //    auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());

}

void AnonsWidget::DeviceProperties(std::string deviceOid)
{

    mDevicePropertiesContainer->clear();


    auto filter = document{};

    try {
        filter.append(kvp("_id",bsoncxx::oid(deviceOid))) ;
    } catch (bsoncxx::exception &e) {
        std::cout << "Line " << __LINE__ << "-> filter." << "_id :"<< e.what() << std::endl;
    }



    try {

        auto val = this->db()->collection("AnonsCihazlari").find_one(filter.view());

        if( val )
        {
            auto doc = val.value().view();

            auto fContainer = mDevicePropertiesContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            fContainer->addStyleClass(Bootstrap::Grid::col_full_12);

            auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            rContainer->addStyleClass(Bootstrap::Grid::row);

            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                adContainer->setHeight(50);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                auto text = vLayout->addWidget(cpp14::make_unique<WText>(doc["cihazadi"].get_utf8().value.to_string()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
            }

            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                adContainer->setHeight(50);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                auto text = vLayout->addWidget(cpp14::make_unique<WText>(doc["mahalle"].get_utf8().value.to_string()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
            }

            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                adContainer->setHeight(50);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center|AlignmentFlag::Middle);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Koordinatlar"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
                }

                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(std::to_string(doc["longtitude"].get_double().value) +
                                                   " - " + std::to_string(doc["latitute"].get_double().value)),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
                }
            }

            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                adContainer->setHeight(50);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                auto text = vLayout->addWidget(cpp14::make_unique<WText>(doc["durum"].get_utf8().value.to_string()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
            }

            // Fotoğraflar
            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                adContainer->setHeight(50);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                auto fotoBtn = vLayout->addWidget(cpp14::make_unique<WPushButton>(u8"Fotoğraflar"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                fotoBtn->addStyleClass(Bootstrap::Button::Success);

                fotoBtn->clicked().connect([=](){
                    this->showFoto(deviceOid);
                });

            }

            // Açıklama Ekle Bölümü
            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                adContainer->setHeight(50);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                auto aciklamaEkle = vLayout->addWidget(cpp14::make_unique<WPushButton>(u8"Açıklama Ekle"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                aciklamaEkle->addStyleClass(Bootstrap::Button::Primary);

                if( this->User().view()["Birimi"].get_utf8().value.to_string() != u8"Bilgi İşlem Müdürlüğü" )
                {
                    aciklamaEkle->setEnabled(false);
                }

                aciklamaEkle->clicked().connect([=](){
                    this->addAciklama(deviceOid);
                });


            }


            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                //                adContainer->setHeight(50);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(u8"Adını Değiştir"),0,AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
                }
                auto selectBtn = vLayout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
                selectBtn->setText(doc["cihazadi"].get_utf8().value.to_string());

                auto degistir = vLayout->addWidget(cpp14::make_unique<WPushButton>(u8"Değiştir"),0,AlignmentFlag::Center);
                degistir->addStyleClass(Bootstrap::Button::Primary);

                if( this->User().view()["Birimi"].get_utf8().value.to_string() != u8"Bilgi İşlem Müdürlüğü" )
                {
                    degistir->setEnabled(false);
                }

                degistir->clicked().connect([=](){

                    auto filter = document{};

                    try {
                        filter.append(kvp("_id",bsoncxx::oid{deviceOid}));
                    } catch (bsoncxx::exception &e) {

                    }


                    auto setDoc = document{};

                    try {
                        setDoc.append(kvp("$set",make_document(kvp("cihazadi",selectBtn->text().toUTF8().c_str())))) ;
                    } catch (bsoncxx::exception &e) {
                        std::cout << "Line " << __LINE__ << "-> setDoc." << "$set :"<< e.what() << std::endl;
                    }

                    try {
                        auto upt = this->db()->collection("AnonsCihazlari").update_one(filter.view(),setDoc.view());

                        if( upt )
                        {
                            if( upt.value().modified_count() )
                            {
                                this->showMessage(u8"Başarılı",u8"Cihaz Adı Değiştirildi","Tamam");
                            }else{
                                this->showMessage(u8"Uyarı",u8"Cihaz Adı Değiştirilemedi","Tamam");
                            }
                        }else{
                            this->showMessage(u8"Uyarı",u8"Cihaz Adı Değiştirilemedi","Tamam");
                        }

                    } catch (mongocxx::exception &e) {
                        std::cout << "Line: " << __LINE__ << "  ->" <<e.what() << std::endl;
                        this->showMessage(u8"Error",u8"Line: " + std::to_string(__LINE__) + " -> " + e.what(),"Tamam");
                    }


                });

            }

            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
                adContainer->setContentAlignment(AlignmentFlag::Center);
                adContainer->setHeight(115);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(u8"Durum Değiştir"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
                }
                auto selectBtn = vLayout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                selectBtn->addItem(u8"Arızalı");
                selectBtn->addItem(u8"Çalışıyor");
                selectBtn->addItem(u8"Kapalı");
                selectBtn->addItem(u8"Bilinmiyor");

                //Bilgi İşlem Müdürlüğü

                if( this->User().view()["Birimi"].get_utf8().value.to_string() != u8"Bilgi İşlem Müdürlüğü" )
                {
                    selectBtn->setEnabled(false);
                }
                std::string mDurum;

                try {
                    mDurum = doc["durum"].get_utf8().value.to_string();
                } catch (bsoncxx::exception &e) {
                    std::cout << "Line " << __LINE__ << "->in doc durum type is not " << "utf8() :"<< e.what() << std::endl;
                }


                if( mDurum == u8"Arızalı" )
                {
                    selectBtn->setCurrentIndex(0);
                }else if (mDurum == u8"Çalışıyor") {
                    selectBtn->setCurrentIndex(1);
                }else if (mDurum == u8"Kapalı") {
                    selectBtn->setCurrentIndex(2);
                }else{
                    selectBtn->setCurrentIndex(3);
                }


                selectBtn->changed().connect([=](){

                    auto filter = document{};

                    try {
                        filter.append(kvp("_id",bsoncxx::oid{deviceOid}));
                    } catch (bsoncxx::exception &e) {

                    }


                    auto setDoc = document{};

                    try {
                        setDoc.append(kvp("$set",make_document(kvp("durum",selectBtn->currentText().toUTF8().c_str())))) ;
                    } catch (bsoncxx::exception &e) {
                        std::cout << "Line " << __LINE__ << "-> setDoc." << "$set :"<< e.what() << std::endl;
                    }

                    try {
                        auto upt = this->db()->collection("AnonsCihazlari").update_one(filter.view(),setDoc.view());

                        if( upt )
                        {
                            if( upt.value().modified_count() )
                            {
                                this->showMessage(u8"Başarılı",u8"Cihaz Durumu Değiştirildi","Tamam");
                            }else{
                                this->showMessage(u8"Uyarı",u8"Cihaz Durumu Değiştirilemedi","Tamam");
                            }
                        }else{
                            this->showMessage(u8"Uyarı",u8"Cihaz Durumu Değiştirilemedi","Tamam");
                        }

                    } catch (mongocxx::exception &e) {
                        std::cout << "Line: " << __LINE__ << "  ->" <<e.what() << std::endl;
                        this->showMessage(u8"Error",u8"Line: " + std::to_string(__LINE__) + " -> " + e.what(),"Tamam");
                    }

                });

            }

            {
                auto adContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                adContainer->setMargin(25,Side::Top);
                adContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                adContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25),this->getRandom(125),this->getRandom(175)));
                adContainer->setContentAlignment(AlignmentFlag::Center);
                adContainer->addStyleClass(Bootstrap::ImageShape::img_rounded);

                auto vLayout = adContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(u8"Açıklamalar"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
                }

                auto aciklamaArray = bsoncxx::builder::basic::array{}.view();

                try {
                    aciklamaArray = doc["aciklamalar"].get_array().value;
                } catch (bsoncxx::exception &e) {
                    std::cout << "Line " << __LINE__ << "->in doc aciklamalar type is not " << "arr :"<< e.what() << std::endl;
                }

                for( auto item : aciklamaArray){

                    auto aciklamaContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                    aciklamaContainer->addStyleClass(Bootstrap::Grid::container_fluid);

                    auto aciklamaRContainer = aciklamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    aciklamaRContainer->addStyleClass(Bootstrap::Grid::row);

                    {
                        auto itemContainer = aciklamaRContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        itemContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail+Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                        auto itemLayout = itemContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        try {
                            auto value = item.get_document().view()["julianDate"].get_int64().value;
                            itemLayout->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(value).toString().toStdString()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                        } catch (bsoncxx::exception &e) {
                            std::cout << "Line " << __LINE__ << "->in item.get_document().view() julianDate type is not " << "int64 :"<< e.what() << std::endl;
                        }
                    }
                    {
                        auto itemContainer = aciklamaRContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        itemContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail+Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                        auto itemLayout = itemContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        try {
                            auto value = item.get_document().view()["saat"].get_utf8().value.to_string();
                            itemLayout->addWidget( cpp14::make_unique<WText>( value , TextFormat::XHTML ) , 0 , AlignmentFlag::Center|AlignmentFlag::Middle);
                        } catch (bsoncxx::exception &e) {
                            std::cout << "Line " << __LINE__ << "->in item.get_document().v saat type is not " << "utf8() :"<< e.what() << std::endl;
                        }
                    }
                    {
                        auto itemContainer = aciklamaRContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        itemContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail+Bootstrap::Grid::col_full_12);
                        auto itemLayout = itemContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        try {
                            auto value = item.get_document().view()["aciklama"].get_utf8().value.to_string();
                            itemLayout->addWidget(cpp14::make_unique<WText>(value,TextFormat::XHTML),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                        } catch (bsoncxx::exception &e) {
                            std::cout << "Line " << __LINE__ << "->in item.get_document().v aciklama type is not " << "utf8() :"<< e.what() << std::endl;
                        }
                    }

                    vLayout->addSpacing(10);
                }

            }

        }


    } catch (mongocxx::exception &e) {
        std::cout << "Line: " << __LINE__ << "  ->" <<e.what() << std::endl;
    }




}

void AnonsWidget::addAciklama(std::string deviveOid)
{

    std::cout << "ACIKLAMA EKLEM . < " << deviveOid << std::endl;

    auto dialog_ = addChild(std::make_unique<Wt::WDialog>(u8"Açıklama Giriniz"));
    dialog_->webWidget()->setZIndex(1000);

    //      dialog_->contents()->addWidget(std::make_unique<Wt::WText>(u8"Açıklamanız: "));
    auto edit_ = dialog_->contents()->addWidget(std::make_unique<Wt::WTextEdit>());
    edit_->setHeight(350);
    dialog_->contents()->addWidget(std::make_unique<Wt::WBreak>());

    Wt::WPushButton *ok = dialog_->contents()->addWidget(std::make_unique<Wt::WPushButton>("Ekle"));
    ok->addStyleClass(Bootstrap::Button::Primary);
    Wt::WPushButton *close = dialog_->contents()->addWidget(std::make_unique<Wt::WPushButton>("Kapat"));
    // these events will accept() the Dialog
    edit_->enterPressed().connect(dialog_, &Wt::WDialog::accept);
    ok->clicked().connect(dialog_, &Wt::WDialog::accept);
    close->clicked().connect(dialog_, &Wt::WDialog::reject);

    dialog_->finished().connect([=](DialogCode code){
        if (code == Wt::WDialog::Code::Accepted){

            auto filter = document{};

            try {
                filter.append(kvp("_id",bsoncxx::oid{deviveOid})) ;
            } catch (bsoncxx::exception &e) {
                std::cout << "Line " << __LINE__ << "-> filter." << "_id :"<< e.what() << std::endl;
            }

            auto push = document{};

            try {
                push.append(kvp("saat",QTime::currentTime().toString("hh:mm").toStdString().c_str())) ;
            } catch (bsoncxx::exception &e) {
                std::cout << "Line " << __LINE__ << "-> push." << "saat :"<< e.what() << std::endl;
            }

            try {
                push.append(kvp("julianDate",QDate::currentDate().toJulianDay())) ;
            } catch (bsoncxx::exception &e) {
                std::cout << "Line " << __LINE__ << "-> push." << "tarih :"<< e.what() << std::endl;
            }

            try {
                push.append(kvp("aciklama",edit_->text().toUTF8().c_str())) ;
            } catch (bsoncxx::exception &e) {
                std::cout << "Line " << __LINE__ << "-> push." << "aciklama :"<< e.what() << std::endl;
            }



            auto pushDoc = document{};

            try {
                pushDoc.append(kvp("$push",make_document(kvp("aciklamalar",push)))) ;
            } catch (bsoncxx::exception &e) {
                std::cout << "Line " << __LINE__ << "-> pushDoc." << "$push :"<< e.what() << std::endl;
            }

            dialog_->removeFromParent();

            try {
                auto upt = this->db()->collection("AnonsCihazlari").update_one(filter.view(),pushDoc.view());
                if( upt )
                {
                    if( upt.value().modified_count() )
                    {
                        this->showMessage( u8"Başarılı" , u8"Açıklama Eklendi" , u8"Tamam" );
                        this->DeviceProperties(deviveOid);
                    }else{
                        this->showMessage( u8"Uyarı" , u8"Açıklama Eklenemedi" , u8"Tamam" );
                    }
                }else{
                    this->showMessage( u8"Uyarı" , u8"Açıklama Eklenemedi" , u8"Tamam" );
                }
            } catch (mongocxx::exception &e) {
                std::cout << "Line: " << __LINE__ << "  ->" <<e.what() << std::endl;
                this->showMessage( u8"Hata" , u8"Line: " + std::to_string(__LINE__) + " -> " +e.what() , u8"Tamam" );
            }


        }
    });
    dialog_->show();

}

void AnonsWidget::showFoto(const std::string &deviceOid)
{

    std::cout << deviceOid << std::endl;


    auto filter = document{};

    try {
        filter.append(kvp("_id",bsoncxx::oid{deviceOid})) ;
    } catch (bsoncxx::exception &e) {
        std::cout << "Line " << __LINE__ << "-> filter." << "_id :"<< e.what() << std::endl;
    }


    bsoncxx::document::value val(document{}.view());

    bool exist = false;

    try {
        auto value = this->db()->collection("AnonsCihazlari").find_one(filter.view());
        if( value )
        {
            exist = true;
            val = value.value();
        }
    } catch (mongocxx::exception &e) {
        std::cout << "Line: " << __LINE__ << "  ->" <<e.what() << std::endl;
    }



    auto dialog_ = addChild(std::make_unique<Wt::WDialog>(u8"Fotoğraflar"));
    dialog_->webWidget()->setZIndex(1100);
    dialog_->setWidth(1000);

    auto container = dialog_->contents()->addWidget(std::make_unique<Wt::WContainerWidget>());
    container->setHeight(650);
    container->setWidth(WLength("100%"));
    container->setOverflow(Overflow::Scroll,Orientation::Vertical);

    auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

//    auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

    if( exist )
    {
        try {
            auto fotolist = val.view()["Fotolist"].get_array().value;
            auto bucket = this->db()->gridfs_bucket();
            for( auto fotoitem : fotolist )
            {
                auto imgurl = SBLDKeys::downloadifNotExist(&bucket,fotoitem.get_document().view()["fotooid"].get_oid().value.to_string());
                auto imgContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                imgContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
                imgContainer->setHeight(600);
                imgContainer->setAttributeValue(Style::style,Style::background::url(imgurl)
                                                +Style::background::size::contain
                                                +Style::background::repeat::norepeat
                                                +Style::background::position::center_center);

                auto vLayout = imgContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                vLayout->addStretch(1);
                vLayout->setContentsMargins(0,0,0,0);

                {
                    auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify|AlignmentFlag::Bottom);
                    container->setAttributeValue(Style::style,Style::background::color::rgba(0,0,0));
                    container->setContentAlignment(AlignmentFlag::Center);
                    {
                        auto text = container->addWidget(cpp14::make_unique<WText>(fotoitem.get_document().view()["saat"].get_utf8().value.to_string().c_str()));
                        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AntiqueWhite)
                                                +Style::font::size::s14px+Style::font::weight::bold);
                    }
                    container->setMargin(0,Side::Top|Side::Bottom);
                    container->addWidget(cpp14::make_unique<WBreak>());

                    {
                        auto text = container->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(fotoitem.get_document().view()["julianDate"].get_int64().value).toString("dddd dd/MM/yyyy").toStdString()));
                        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AntiqueWhite)
                                                +Style::font::size::s14px+Style::font::weight::bold);
                    }

                }

//                {
//                    auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify|AlignmentFlag::Bottom);
//                    container->setAttributeValue(Style::style,Style::background::color::rgba(0,0,0));
//                    container->setContentAlignment(AlignmentFlag::Center);
//                    auto text = container->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(fotoitem.get_document().view()["julianDate"].get_int64().value).toString("dddd dd/MM/yyyy").toStdString()));
//                    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AntiqueWhite)
//                                            +Style::font::size::s14px+Style::font::weight::bold);
//                    container->setMargin(0,Side::Top|Side::Bottom);
//                }

                imgContainer->setMargin(15,Side::Top|Side::Bottom);
            }
        } catch (bsoncxx::exception &e) {
            std::cout << "Line " << __LINE__ << "->in val.view() Fotolist type is not " << "array() :"<< e.what() << std::endl;
        }


    }else{
        auto imgContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->addStyleClass(Bootstrap::Grid::col_full_12);
        imgContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        imgContainer->setContentAlignment(AlignmentFlag::Center);
        imgContainer->addWidget(cpp14::make_unique<WText>("<h3>Bu Cihaza Ait Hiç Fotoğraf Yok</h3>"));
    }
//    for( int i = 0 ; i < 5 ; i++ )
//    {
//        auto imgContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//        imgContainer->addStyleClass(Bootstrap::Grid::col_full_12);
//        imgContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
////        imgContainer->setWidth(600);
//        imgContainer->setHeight(600);
//        std::string url = "img/"+std::to_string(i+1)+".jpg";
//        imgContainer->setAttributeValue(Style::style,Style::background::url(url)
//                                        +Style::background::size::contain
//                                        +Style::background::repeat::norepeat
//                                        +Style::background::position::center_center);
//    }

//    dialog_->contents()->addWidget(std::make_unique<Wt::WBreak>());

    Wt::WPushButton *ok = dialog_->footer()->addWidget(std::make_unique<Wt::WPushButton>("Tamam"));
    ok->addStyleClass(Bootstrap::Button::Primary);
    // these events will accept() the Dialog

    ok->clicked().connect(dialog_, &Wt::WDialog::accept);

    dialog_->finished().connect([=](DialogCode code){
        dialog_->removeFromParent();
    });
    dialog_->show();


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

std::string Device::durum() const
{
    return mDurum;
}

void Device::setDurum(const std::string &durum)
{

    mDurum = durum;

//    std::cout << "Durum: " << "u8"+mDurum << durum << std::endl;

    if( mDurum == u8"Arızalı" )
    {
        setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::FireBrick));
    }else if(mDurum == u8"Çalışıyor") {
        setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::DarkGreen));
    }else{
        setAttributeValue(Style::style,Style::background::color::color(Style::color::Orange::DarkOrange));
    }

}

