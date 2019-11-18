#include "mainapplication.h"
#include "SerikBelediyesiWebSayfasi/src/header.h"
#include "SerikBelediyesiWebSayfasi/src/footer.h"

#include "serikbeltr.h"

#include "SerikBelediyesiWebSayfasi/srcV2/mainpage.h"

#include <Wt/WOverlayLoadingIndicator.h>
#include "SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekceview.h"

#include "SerikBelediyesiWebSayfasi/srcV2/talepler/talepview.h"


#include "../url.h"


MainApplication::MainApplication(const Wt::WEnvironment &env)
    :WApplication(env)
{

    wApp->addMetaHeader(MetaHeaderType::Meta,"Content-Type","text/html; charset=windows-1254");

    wApp->addMetaHeader("description","Serik Belediyesi Resmi Web Sayfası","text/html; charset=utf-8");


    try {
        mClient = new mongocxx::client(mongocxx::uri(_url));
    } catch (mongocxx::exception& e) {
        std::cout << "MongoDB Connection Error: " << e.what() << std::endl;
        root()->addWidget(cpp14::make_unique<WText>("Driver Yüklenemedi!"));
        return;
    }

    Wt::WApplication *app = Wt::WApplication::instance();
    app->setLoadingIndicator(std::make_unique<WOverlayLoadingIndicator>());
    app->loadingIndicator()->setMessage("Yükleniyor...");


    db = mClient->database(SBLDKeys::DB);
    Bucket = db.gridfs_bucket();


    p_wtTheme = std::make_shared<Wt::WBootstrapTheme>();

    p_wtTheme->setVersion(Wt::WBootstrapTheme::Version::v3);

    p_wtTheme.get()->setResponsive(true);

    Wt::WApplication::instance()->setTheme(p_wtTheme);

    Wt::WApplication::instance()->setTitle("Serik Belediyesi Resmi Web Sayfası");
    Wt::WApplication::instance()->addMetaLink("icon/20180126113337667.ico","shortcut icon","","","image/x-icon","16x16",false);

    WApplication::useStyleSheet(WLink("css/mainPage.css"));
    WApplication::useStyleSheet(WLink("css/slider.css"));
    WApplication::useStyleSheet(WLink("css/eventWidget.css"));
    WApplication::useStyleSheet(WLink("css/header.css"));

    //    WApplication::useStyleSheet(WLink("css/test.css"));

    Wt::WApplication::instance()->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
    Wt::WApplication::require("script/Script.js");
    //    Wt::WApplication::require("script/testscript.js");


    Wt::WApplication::require("script/jszip/jszip.min.js");
    Wt::WApplication::require("script/jszip/jszip.js");
    //FileSaver.js
    Wt::WApplication::require("script/jszip/FileSaver.js");






    WApplication::instance()->require("https://www.googletagmanager.com/gtag/js?id=UA-56529726-1");

    WApplication::instance()->require("https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.1.4/Chart.min.js");


    WApplication::instance()->doJavaScript("window.dataLayer = window.dataLayer || [];"
                                           "function gtag(){dataLayer.push(arguments);}"
                                           "gtag('js', new Date());"
                                           "gtag('config', 'UA-56529726-1');");


    //    <!-- Global site tag (gtag.js) - Google Analytics -->
    //    <script async src="https://www.googletagmanager.com/gtag/js?id=UA-56529726-1"></script>
    //    <script>
    //      window.dataLayer = window.dataLayer || [];
    //      function gtag(){dataLayer.push(arguments);}
    //      gtag('js', new Date());

    //      gtag('config', 'UA-56529726-1');
    //    </script>




    //    Wt::WApplication::require("ViewerJS/compatibility.js");
    //    Wt::WApplication::require("ViewerJS/pdf.js");
    //    Wt::WApplication::require("ViewerJS/pdf.worker.js");
    //    Wt::WApplication::require("ViewerJS/pdfjsversion.js");
    //    Wt::WApplication::require("ViewerJS/text_layer_builder.js");
    //    Wt::WApplication::require("ViewerJS/ui_utils.js");
    //    Wt::WApplication::require("ViewerJS/webodf.js");



    //    Wt::WApplication::instance()->setBodyClass("introMain");

    WApplication::instance()->addMetaHeader("viewport","width=device-width, initial-scale=1.0");
    wApp->require("http://www.openlayers.org/api/OpenLayers.js");




    QMap<QString,QString> mapList;
    //    test Link : http://192.168.0.31:8080/?type=dilekce&_id=5daee97a6435000043002489 cevaplanmis
    // http://192.168.0.31:8080/?type=dilekce&_id=5daeebd8222400005d0005dc Cevaplanmamis
    for (auto str : env.getParameterMap() ) {
        //        std::cout << "First: " << str.first << " " << str.second.size() << " " << str.second.back() << std::endl;
        for( auto sec : str.second )
        {
            mapList[str.first.c_str ()] = QString::fromStdString (sec);
            std::cout << "Parameter: " << str.first << " Value: " << sec << std::endl;
        }
    }


    bool showSpecLink = false;

    if( mapList.contains ("type") )
    {
        if( mapList["type"] == "dilekce" )
        {
            auto oid = mapList["_id"];
            showSpecLink = this->loadDilekce (oid.toStdString ());
        }

        if( mapList["type"] == "talep" )
        {
            auto oid = mapList["_id"];
            showSpecLink = this->loadTalep (oid.toStdString ());
        }
    }



    if( !showSpecLink )
    {
        this->init();
    }





    auto device0 = root()->addWidget(cpp14::make_unique<WText>("Serik Belediyesi © 2019"));
    auto device1 = root()->addWidget(cpp14::make_unique<WText>("Serik Belediyesi © 2019"));
    auto device2 = root()->addWidget(cpp14::make_unique<WText>("Serik Belediyesi © 2019"));
    auto device3 = root()->addWidget(cpp14::make_unique<WText>("Serik Belediyesi © 2019"));
    auto device4 = root()->addWidget(cpp14::make_unique<WText>("Serik Belediyesi © 2019"));

    device0->addStyleClass("device0");
    device1->addStyleClass("device1");
    device2->addStyleClass("device2");
    device3->addStyleClass("device3");
    device4->addStyleClass("device4");



}

MainApplication::~MainApplication()
{
    std::cout << "--------------Destroy Session-------------" << wApp->sessionId() << std::endl;
    if( mClient )
    {
        delete  mClient;
    }
}

void MainApplication::init()
{

    root()->clear();

    root()->addStyleClass("rootBody");

    //    std::cout << "Julian Day: " << QDate::currentDate().toJulianDay() << std::endl;

    // Afis Ön Görsel
    //    if( true ){

    //        auto container = root()->addWidget(cpp14::make_unique<WContainerWidget>());

    //        container->setWidth(WLength("100%"));
    //        container->setHeight(WLength("100%"));
    //        container->setPositionScheme(PositionScheme::Fixed);
    //        container->setAttributeValue(Style::style,Style::background::color::rgba(25,25,25));
    //        container->setZIndex(1000);

    //        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
    //        _container->setWidth(WLength("100%"));
    //        _container->setHeight(WLength("100%"));
    //        _container->setPositionScheme(PositionScheme::Fixed);
    //        _container->setAttributeValue(Style::style,Style::background::url("v2/img/serik-spor.jpg")
    //                                     +Style::background::size::contain
    //                                      +Style::background::position::center_center
    //                                      +Style::background::repeat::norepeat);
    //        _container->setZIndex(1000);

    //        _container->clicked().connect([=](){
    //           root()->removeWidget(container);
    //        });
    //    }


    {
        auto container = root()->addWidget(cpp14::make_unique<WContainerWidget>());

        container->setWidth(WLength("100%"));
        container->setHeight(WLength("100%"));
        container->setPositionScheme(PositionScheme::Fixed);
        container->addStyleClass("backanimation");
        container->setZIndex(-100);


        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setWidth(WLength("100%"));
        _container->setHeight(WLength("100%"));
        _container->setPositionScheme(PositionScheme::Absolute);
        _container->setAttributeValue(Style::style,Style::background::url("v2/slide/Slidefilter.png")
                                      +Style::background::repeat::repeat);
        _container->setZIndex(-99);
    }


    if( db )
    {
        MainPage* mPage = root()->addWidget(cpp14::make_unique<MainPage>(&db));
    }else{
        std::cout << "DATABASE is Not Valid" << std::endl;
    }








    //    Header::Header* header = root()->addWidget(cpp14::make_unique<Header::Header>());
    //    Body::Body* body = root()->addWidget(cpp14::make_unique<Body::Body>(&db,&Bucket));
    //    Footer::Footer* footer = root()->addWidget(cpp14::make_unique<Footer::Footer>());
    //    _signal.connect([=](int _width,int _height){

    //       double _w = static_cast<double>(_width);
    //       double _h = static_cast<double>(_height);

    //       std::cout << " - VALUE: " << _width << " - " << _height <<  "R: " << _w / _h << std::endl;

    //       if( _w / _h > 1.433 ){
    //           footer->setPositionScheme(PositionScheme::Fixed);
    //           footer->setOffsets(0,Side::Bottom);
    //           footer->setWidth(WLength("100%"));
    //       }


    //    });

    //    root()->doJavaScript("var w = window.innerWidth;"
    //                         "var h = window.innerHeight;"
    //                         "console.log(w);console.log(h);" + _signal.createCall({"w,h"}) + ";");








}

void MainApplication::ShowForeGround()
{

    auto BackGroundWidget = root()->addWidget(cpp14::make_unique<WContainerWidget>());

    BackGroundWidget->setPositionScheme(PositionScheme::Fixed);

    BackGroundWidget->setOffsets(0,AllSides);

    BackGroundWidget->setAttributeValue(Style::style,Style::background::color::rgba(0,0,0,0.75));

    BackGroundWidget->setContentAlignment(AlignmentFlag::Center|AlignmentFlag::Middle);

    BackGroundWidget->setPadding(0,AllSides);

    BackGroundWidget->setPadding(50,Side::Top);

    BackGroundWidget->setZIndex(101);

    BackGroundWidget->setMargin(WLength::Auto,Side::Top|Side::Bottom);

    auto imgWidget = BackGroundWidget->addWidget(cpp14::make_unique<WContainerWidget>());

    imgWidget->setAttributeValue(Style::style,Style::background::url("img/ramazan2018.jpeg")+Style::background::repeat::norepeat+Style::background::position::center_center+Style::background::size::contain);
    imgWidget->setMargin(20,AllSides);
    imgWidget->setHeight(WLength("80%"));
    BackGroundWidget->clicked().connect([=](){
        root()->removeWidget(BackGroundWidget);
    });

}

bool MainApplication::loadDilekce(const std::string &oid)
{



    SerikBLDCore::DilekceManager* dManager = new SerikBLDCore::DilekceManager(&this->db);

    auto dilekce = dManager->LoadDilekce (oid);

    if( dilekce )
    {
        root()->clear();
        root()->addStyleClass("rootBody");
        root ()->setContentAlignment (AlignmentFlag::Center);
        auto rContainer = root ()->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);
        rContainer->setMaximumSize (WLength(1024),WLength::Auto);

        rContainer->addWidget (cpp14::make_unique<DilekceView>(dilekce.get (),&this->db,nullptr,true));
        return true;
    }

    return false;

}

bool MainApplication::loadTalep(const std::string &oid)
{
    SerikBLDCore::TalepManager* dManager = new SerikBLDCore::TalepManager(&this->db);

    SerikBLDCore::Talep filter;
    filter.setOid (oid);
    auto talep = dManager->findOneTalep (filter);

    wApp->setTitle ("Serik Belediyesi Talep/Şikayet Yönetim Sistemi");
    if( talep )
    {
        root()->clear();
        root()->addStyleClass("rootBody");
        root ()->setContentAlignment (AlignmentFlag::Center);
        auto rContainer = root ()->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);
        rContainer->setMaximumSize (WLength(1024),WLength::Auto);

        rContainer->addWidget (cpp14::make_unique<TalepView>(std::move(talep),&this->db,nullptr,true));
        return true;
    }

    return false;
}


