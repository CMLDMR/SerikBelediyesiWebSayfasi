#include "mainapplication.h"
#include "SerikBelediyesiWebSayfasi/src/header.h"
#include "SerikBelediyesiWebSayfasi/src/footer.h"

#include "serikbeltr.h"

#include "SerikBelediyesiWebSayfasi/srcV2/mainpage.h"

#include <Wt/WOverlayLoadingIndicator.h>
#include "SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekceview.h"

#include "SerikBelediyesiWebSayfasi/srcV2/talepler/talepview.h"
#include "SerikBelediyesiWebSayfasi/srcV2/meclis/meclisitempage.h"


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
    WApplication::useStyleSheet(WLink("css/style.css"));
    WApplication::useStyleSheet(WLink("css/v2.css"));

//    WApplication::useStyleSheet (WLink("https://www.w3schools.com/w3css/4/w3.css"));

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
//            std::cout << "Parameter: " << str.first << " Value: " << sec << std::endl;
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

        if( mapList["type"] == "deviceStatus" )
        {
            this->LoadDeviceStatus ();
            showSpecLink = true;
        }

        if( mapList["type"] == "talep" )
        {
            auto oid = mapList["_id"];
            showSpecLink = this->loadTalep (oid.toStdString ());
        }

        if( mapList["type"] == "gundem" )
        {
            auto oid = mapList["_id"];
            showSpecLink = this->loadGundem (oid.toStdString ());
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

//    if( QDate(2021,1,31).toJulianDay () > QDate::currentDate ().toJulianDay () ){
//        this->showYapilandirmaEmlak ();
//    }

    root()->addStyleClass("rootBody");

//     Afis Ön Görsel
        if( false ){

            auto container = root()->addWidget(cpp14::make_unique<WContainerWidget>());

            container->setWidth(WLength("100%"));
            container->setHeight(WLength("100%"));
            container->setPositionScheme(PositionScheme::Fixed);
            container->setAttributeValue(Style::style,Style::background::color::rgba(255,255,255,0.75));
            container->setZIndex(1000);

            auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->setWidth(WLength("100%"));
            _container->setHeight(WLength("100%"));
            _container->setPositionScheme(PositionScheme::Fixed);

            _container->setZIndex(1000);

            WTimer::singleShot(std::chrono::milliseconds(27000),[=](){
                           root()->removeWidget(container);
                        });

            {
                _container->setContentAlignment (AlignmentFlag::Center);
                _container->setPadding (50,Side::Top);
                auto mContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                mContainer->setPositionScheme (PositionScheme::Relative);
                mContainer->setContentAlignment(AlignmentFlag::Center);
                mContainer->setMaximumSize(1024,WLength::Auto);
                mContainer->setHeight (512);
                mContainer->addStyleClass ("boxShadow boxRadius");
                {
                    QList<std::string> linkList;

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/37HcNTWQ0xs?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/LyUlmFfGPoY?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/_S4n2GeZgBw?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }


                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/NCzo3hoc1CI?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/JgCz1_oezRk?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/v7fmVvbBIQE?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/6EH7J3W4axs?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/FZkFQGY6O5s?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/TP6qml_HGQc?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    {
                        auto link = "<iframe src=\"https://www.youtube.com/embed/jtHfbs3RKXY?autoplay=1&showinfo=0&controls=0\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                                 "allow=\"accelerometer; autoplay=true; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";
                        linkList.push_back(link);
                    }

                    std::random_device rd;
                    std::mt19937 mt(rd());
                    std::uniform_int_distribution<int> dist(0,9);
                    auto dice_roll = dist(mt);

                    std::cout << "\nDice ve Roll: " << dice_roll << "\n";

                    auto link = linkList.at(dice_roll);


                    auto text = mContainer->addWidget(cpp14::make_unique<WText>(link,TextFormat::UnsafeXHTML));
                    text->setMaximumSize(1280,WLength::Auto);
                }

                {
                    auto skipContainer = mContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    skipContainer->setPositionScheme (PositionScheme::Absolute);
                    skipContainer->setOffsets (0,Side::Right|Side::Top);
                    skipContainer->setWidth (170);
                    skipContainer->setHeight (60);
                    skipContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::White));
                    skipContainer->decorationStyle ().setCursor (Cursor::PointingHand);

                    auto layout = skipContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
                    auto text = layout->addWidget (cpp14::make_unique<WText>("Ana SAYFA"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                    text->setAttributeValue (Style::style,Style::font::weight::lighter+Style::font::size::s14px+Style::color::color (Style::color::Grey::Black));

                    skipContainer->clicked ().connect ([=](){
                        root()->removeWidget(container);
                    });

                }
            }
        }


//    std::cout << "\n" << QDate(2021,9,31).toJulianDay() << " " << QDate::currentDate().toJulianDay() << "\n";



//    if( QDate(2021,9,31).toJulianDay() < QDate::currentDate().toJulianDay() ){
//        auto container = root()->addWidget(cpp14::make_unique<WContainerWidget>());

//        container->setWidth(WLength("100%"));
//        container->setHeight(WLength("100%"));
//        container->setPositionScheme(PositionScheme::Fixed);

//        container->addStyleClass("backanimation");

//        {
//            std::random_device rd;
//            std::mt19937 mt(rd());
//            std::uniform_int_distribution<int> dist(1,35);
//            auto index = dist(mt);
//            std::string photoUrl = "v2/slide/"+std::to_string (index)+".jpg";
//            container->setAttributeValue (Style::style,Style::background::url (photoUrl)+
//                                          Style::background::size::cover+
//                                          Style::background::repeat::norepeat);
//        }

//        container->setZIndex(9999);


//        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        _container->setWidth(WLength("100%"));
//        _container->setHeight(WLength("100%"));
//        _container->setPositionScheme(PositionScheme::Absolute);
//        _container->setAttributeValue(Style::style,Style::background::color::rgba(15,15,15)
//                                      +Style::background::repeat::repeat);
//        _container->setZIndex(9999);


//        _container->setContentAlignment(AlignmentFlag::Center);

//        auto iframe = _container->addWidget(cpp14::make_unique<WContainerWidget>());
//        iframe->setHtmlTagName("iframe");
//        iframe->setWidth(848);
//        iframe->setHeight(480);
//        iframe->setAttributeValue("src","https://www.youtube.com/embed/zoVUZK0sI9w?&rel=0&autohide=1&autoplay=1&controls=0");
//        _container->addWidget(cpp14::make_unique<WBreak>());

//        auto iframe1 = _container->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto dvmBtn = _container->addWidget(cpp14::make_unique<WPushButton>("Bu Sayfayı Atla"));
//        dvmBtn->addStyleClass(Bootstrap::Button::Warning);
//        dvmBtn->clicked().connect([=](){
//            root()->removeWidget(container);
//        });
//    }



    // Arkaplan Resimleri Slaytı
    {
        auto container = root()->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass (Bootstrap::Grid::Hidden::hidden_xs+Bootstrap::Grid::Hidden::hidden_sm);

        container->setWidth(WLength("100%"));
        container->setHeight(WLength("100%"));
        container->setPositionScheme(PositionScheme::Fixed);

        container->addStyleClass("backanimation");

        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(1,35);
            auto index = dist(mt);
            std::string photoUrl = "v2/slide/"+std::to_string (index)+".jpg";
            container->setAttributeValue (Style::style,Style::background::url (photoUrl)+
                                          Style::background::size::cover+
                                          Style::background::repeat::norepeat);
        }

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

}

void MainApplication::showYapilandirmaEmlak()
{


    auto container = root ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->setId ("yonlendirme");

    container->setPositionScheme (PositionScheme::Fixed);
    container->setWidth (WLength("100%"));
    container->setHeight (WLength("100%"));
    container->setAttributeValue (Style::style,Style::background::color::rgba (25,25,25,0.5));
    container->setZIndex (1000);


    auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

    // Create an anchor that links to a URL through clickable text.
    Wt::WLink link = Wt::WLink("http://webportal.serik.bel.tr:9195/home");
    link.setTarget(Wt::LinkTarget::NewWindow);

    std::unique_ptr<Wt::WAnchor> anchor =
            Wt::cpp14::make_unique<Wt::WAnchor>(link,
                            "7256 Sayılı Yasa İle Yapılandırma için Tıklayınız");

    {
        auto linkContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify|AlignmentFlag::Middle);
        linkContainer->setContentAlignment (AlignmentFlag::Center);
        linkContainer->setPadding (15,AllSides);
        linkContainer->setAttributeValue (Style::style,Style::background::color::rgba (255,255,255,1));

        linkContainer->addWidget (std::move(anchor));

        linkContainer->addWidget (cpp14::make_unique<WBreak>());
        auto btn = linkContainer->addWidget (cpp14::make_unique<WPushButton>("Ana Sayfa İçin Tıklayınız"));
        btn->setMargin (15,Side::Top);
        btn->addStyleClass (Bootstrap::Button::Primary);
        btn->clicked ().connect ([=](){
           root ()->removeWidget(container);
        });
    }


//    {
//        auto linkContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify|AlignmentFlag::Middle);
//        linkContainer->setContentAlignment (AlignmentFlag::Center);
//        linkContainer->setPadding (15,AllSides);
//        linkContainer->setAttributeValue (Style::style,Style::background::color::rgba (255,255,255,1));


//    }







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

bool MainApplication::loadGundem(const std::string &oid)
{

    SerikBLDCore::Meclis::MeclisManager* dManager = new SerikBLDCore::Meclis::MeclisManager(new SerikBLDCore::DB(&this->db));

    SerikBLDCore::Meclis::MeclisItem filter;
    filter.setOid (oid);
    auto meclisGundem = dManager->findOneItem (filter);

    if( meclisGundem )
    {
        auto item = SerikBLDCore::Meclis::MeclisItem();
        item.setDocumentView (meclisGundem.value ().view ());
        wApp->setTitle ("Serik Belediyesi Meclis Gündemi - " + std::to_string (item.yil ()) + " " + item.ay ().toStdString ());
        root()->clear();
        root()->addStyleClass("rootBody");
        root ()->setContentAlignment (AlignmentFlag::Center);
        auto rContainer = root ()->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);
        rContainer->setMaximumSize (WLength(1024),WLength::Auto);

        if( item.yayinda () )
        {
            rContainer->addWidget (cpp14::make_unique<v2::MeclisItemPublicPage>(new SerikBLDCore::DB(&this->db),item));
        }else{
            rContainer->addWidget (cpp14::make_unique<WText>("<h4>Bu Meclis Bilgileri Şuanda Kullanılabilir Değil</h4>",TextFormat::UnsafeXHTML));
        }
        return true;
    }

    return false;

}

void MainApplication::LoadDeviceStatus()
{

    DeviceServices::DeviceServicesManager* mDeviceManager = new DeviceServices::DeviceServicesManager(new SerikBLDCore::DB(&this->db));


    DeviceServices::DeviceServicesItem filter;

    auto cursor = mDeviceManager->GetList(filter,1000);


    root()->clear();
    root()->setContentAlignment(AlignmentFlag::Center);

    root()->addStyleClass("rootBody");
    root ()->setContentAlignment (AlignmentFlag::Center);
    auto rContainer = root ()->addWidget (cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass (Bootstrap::Grid::row);
    rContainer->setMaximumSize (WLength(1024),WLength::Auto);
    rContainer->setMargin(100,Side::Top|Side::Bottom);

    int row = 0;
    for( const auto &item : cursor ){

        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(2,Side::Top|Side::Bottom);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        if( item.getStatus() == "OK" ){
            container->addStyleClass(Bootstrap::Button::Primary);
        }else{
            container->addStyleClass(Bootstrap::Button::Danger);
        }

        auto gLayout = container->setLayout(cpp14::make_unique<WGridLayout>());

        int col = 0;

        gLayout->addWidget(cpp14::make_unique<WText>(item.getType()),row,col++,AlignmentFlag::Left);
        gLayout->addWidget(cpp14::make_unique<WText>(item.getName()),row,col++,AlignmentFlag::Left);
        gLayout->addWidget(cpp14::make_unique<WText>(item.getIpAddress()),row,col++,AlignmentFlag::Left);
        gLayout->addWidget(cpp14::make_unique<WText>(item.getStatus()),row,col++,AlignmentFlag::Left);
        gLayout->addWidget(cpp14::make_unique<WText>(QDateTime::fromMSecsSinceEpoch(item.getLastEventTime()).date().toString("dd/MM/yyyy").toStdString()),row,col++,AlignmentFlag::Left);
        gLayout->addWidget(cpp14::make_unique<WText>(QDateTime::fromMSecsSinceEpoch(item.getLastEventTime()).time().toString("hh:mm:ss").toStdString()),row,col++,AlignmentFlag::Left);
        row++;

    }

}


