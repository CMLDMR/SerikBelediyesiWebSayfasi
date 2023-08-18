#include "mainpage.h"
#include "mainpagecontroller.h"
#include "SerikBelediyesiWebSayfasi/src/body.h"
#include "SerikBelediyesiWebSayfasi/src/giriswidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/baskanwidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/externalWidget/nobetcieczanewidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/talepler/talepwidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/externalWidget/haritawidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/kvkk/kvkkcontainer.h"

#include "meclis/meclispage.h"
#include "meclis/meclispublicpage.h"

#include "personel/yonetimwidget.h"
#include "meclis/meclisuyesipublicwidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/nostserik.h"
#include "duyuruyonetim.h"



MainPage::MainPage(mongocxx::database *_db)
    :SerikBLDCore::DB (_db),_signal(this,"_signal")
{
    auto header = addWidget(cpp14::make_unique<HeaderPage>());
    header->setZIndex(12);
    header->addStyleClass("header");

    mContentWidget = addWidget(cpp14::make_unique<WContainerWidget>());

    if( !this->initMeclisCanliYayin() )
    {
        this->init();
    }





    header->ClickHaber().connect(this,&MainPage::initHaberler);
    header->ClickCalisma().connect(this,&MainPage::initCalismalar);
    header->ClickProjeler().connect(this,&MainPage::initProjeler);
    header->ClickEtkinlikler().connect(this,&MainPage::initEtkinlikler);
    header->ClickBilgiEdinme().connect(this,&MainPage::initBilgiEdinme);
    header->ClickGiris().connect(this,&MainPage::initGiris);
    header->ClickMeclis().connect(this,&MainPage::initMeclis);
    header->ClickHakkinda().connect(this,&MainPage::initHakkinda);
    header->ClickIletisim().connect(this,&MainPage::initIletisim);
    header->ClickBaskan().connect(this,&MainPage::initBaskan);
    header->ClickBaskanYrd ().connect(this,&MainPage::initBaskanYardimcilari);
    header->ClickMeclisUyeleri ().connect(this,&MainPage::initMeclisUyeleri);
    header->ClickMeclisUyeleri ().connect(this,&MainPage::initKVKK);

    footer = addWidget(cpp14::make_unique<Footer::Footer>());



    _signal.connect([=](int _width,int _height){

        double _w = static_cast<double>(_width);
        double _h = static_cast<double>(_height);
        footer->setwidth(_w);
        footer->setheight(_h);
        if( footer->getWidth() / footer->getHeight() > 1.433 ){
            footer->addStyleClass("footerStickAbsolute");
        }
    });

    header->ClickAnaSayfa().connect([=](){
        if( footer->getWidth() / footer->getHeight() >= 1.333 ){
            footer->removeStyleClass("footerStickAbsolute");
            footer->addStyleClass("footerStickAbsolute");
        }
        this->init();
    });

    doJavaScript("var w = window.innerWidth;"
                 "var h = window.innerHeight;"
                 "console.log(w);console.log(h);" + _signal.createCall({"w,h"}) + ";");


}

void MainPage::init()
{

    mContentWidget->clear();

    mContentWidget->setContentAlignment(AlignmentFlag::Center);

    auto rContainer = mContentWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setMaximumSize(1270,WLength::Auto);
    rContainer->setPadding(180,Side::Top);

    {
        auto controller = rContainer->addWidget(cpp14::make_unique<MainPageController>());
        //        controller->addStyleClass(Bootstrap::Grid::Visible::visible_lg);
        controller->addStyleClass(Bootstrap::Grid::col_full_12);
        controller->ClickHaber().connect(this,&MainPage::initHaberler);
        controller->ClickCalisma().connect(this,&MainPage::initCalismalar);
        controller->ClickProjeler().connect(this,&MainPage::initProjeler);
        controller->ClickEtkinlikler().connect(this,&MainPage::initEtkinlikler);
        controller->ClickBilgiEdinme().connect(this,&MainPage::initBilgiEdinme);
        controller->ClickGiris().connect(this,&MainPage::initGiris);
        controller->ClickMeclis().connect(this,&MainPage::initMeclis);
//        controller->ClickAnounce().connect(this,&MainPage::initAnounceDetail);
        controller->ClickDuyurular().connect(this,&MainPage::initAnounceList);
        controller->ClickHakkinda().connect(this,&MainPage::initHakkinda);
        controller->ClickIletisim().connect(this,&MainPage::initIletisim);
        controller->ClickDuyurular().connect(this,&MainPage::initAnounceList);
        controller->ClickNobetciEczane().connect(this,&MainPage::initNobetciEczane);
        controller->ClickNostSerik().connect(this,&MainPage::initNostSerik);
        controller->ClickYonetim().connect(this,&MainPage::initBaskanYardimcilari);

    }

}

void MainPage::initHaberler()
{
    mContentWidget->clear();
    mContentWidget->addWidget(cpp14::make_unique<Body::Haber>(this->getDB()->db()));
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initCalismalar()
{
    mContentWidget->clear();
    mContentWidget->addWidget(cpp14::make_unique<Body::Calisma>(this->getDB()->db()));
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initProjeler()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Proje>(this->getDB()->db()));
    widget->setMaximumSize(1024,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initEtkinlikler()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Etkinlik>(this->getDB()->db()));
    widget->setMaximumSize(1024,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initBilgiEdinme()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::BilgiEdin::BilgiEdin>(this->getDB()->db()));
    widget->setMaximumSize(1024,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initGiris()
{

    std::cout << "init Giriş" << std::endl;

    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Giris::GirisWidget>(this->getDB()->db()));
    widget->setMaximumSize(1250,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initMeclis()
{

    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<v2::MeclisPublicPage>(new SerikBLDCore::DB(this->getDB())));
    widget->setMaximumSize(1280,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");


    //    mContentWidget->clear();


//    {
//        auto container = mContentWidget->addWidget (cpp14::make_unique<WContainerWidget>());
//        container->setMaximumSize (1280,WLength::Auto);
//        container->addStyleClass (Bootstrap::Grid::row);
//        container->setMargin (90,Side::Top|Side::Bottom);
//        container->setMaximumSize(1280,WLength::Auto);
//        container->setPadding (25,Side::Top|Side::Bottom);


//        auto oldMeclisContainer = container->addWidget(cpp14::make_unique<ContainerWidget>());
//        oldMeclisContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
//                                           Bootstrap::Grid::Medium::col_md_6+
//                                           Bootstrap::Grid::Small::col_sm_6+
//                                           Bootstrap::Grid::ExtraSmall::col_xs_12+
//                                           Bootstrap::ImageShape::img_thumbnail);
//        oldMeclisContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::Fuchsia)+
//                                               Style::color::color (Style::color::White::Snow));
//        oldMeclisContainer->addWidget (cpp14::make_unique<WText>("<h4>2020 Öncesi</h4>",TextFormat::UnsafeXHTML));
//        oldMeclisContainer->decorationStyle ().setCursor (Cursor::PointingHand);

//        oldMeclisContainer->clicked ().connect ([&](){
//            mContentWidget->clear();
//            auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Meclis>(this->getDB()));
//            widget->setMaximumSize(1280,WLength::Auto);
//            footer->removeStyleClass("footerStickAbsolute");
//        });





//        auto newMeclisContainer = container->addWidget(cpp14::make_unique<ContainerWidget>());
//        newMeclisContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
//                                           Bootstrap::Grid::Medium::col_md_6+
//                                           Bootstrap::Grid::Small::col_sm_6+
//                                           Bootstrap::Grid::ExtraSmall::col_xs_12+
//                                           Bootstrap::ImageShape::img_thumbnail);
//        newMeclisContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::SlateBlue)+
//                                               Style::color::color (Style::color::White::Snow));

//        newMeclisContainer->addWidget (cpp14::make_unique<WText>("<h4>2020 Sonrası</h4>",TextFormat::UnsafeXHTML));
//        newMeclisContainer->decorationStyle ().setCursor (Cursor::PointingHand);

//        newMeclisContainer->clicked ().connect ([&](){
//            mContentWidget->clear();
//            auto widget = mContentWidget->addWidget(cpp14::make_unique<v2::MeclisPublicPage>(new SerikBLDCore::DB(this->getDB())));
//            widget->setMaximumSize(1280,WLength::Auto);
//            footer->removeStyleClass("footerStickAbsolute");
//        });
//    }



    //    {
    //         auto widget = mContentWidget->addWidget(cpp14::make_unique<v2::MeclisPublicPage>(new SerikBLDCore::DB(this->getDB())));
    //         widget->setMaximumSize(1280,WLength::Auto);
    //    }

    //    {
    //        auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Meclis>(this->getDB()));
    //        widget->setMaximumSize(1280,WLength::Auto);
    //    }
    //    footer->removeStyleClass("footerStickAbsolute");

    // TODO: Meclis Karar Ekleme Sayfası Yapıldıktan sonra Çalıştırılacak.
    //mContentWidget->addWidget (cpp14::make_unique<v2::MeclisPage>(new SerikBLDCore::DB(this->getDB ())));


}

void MainPage::initHakkinda()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Serik::Hakkinda>(this->getDB()->db()));
    widget->setMaximumSize(1024,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initIletisim()
{

    mContentWidget->clear();

    mContentWidget->setContentAlignment(AlignmentFlag::Center);
    auto container = mContentWidget->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1024,WLength::Auto);
    container->setPadding(0,AllSides);
    container->addStyleClass(Bootstrap::Grid::row);

    {
        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        std::string path = "img/map1.jpg";

        img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(160);
        img->setPadding(0,AllSides);

        auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
        gradientContainer->setHeight(160);
        gradientContainer->addStyleClass("SliderDetailTextBackground");
        auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        auto title = layout->addWidget(cpp14::make_unique<WText>("İletişim"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
        title->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
    }


    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }


    {


        // Serik Belediye Binası Coor: 36.91623, 31.104
        {
            auto haritaWidget = container->addWidget (cpp14::make_unique<HaritaWidget>());
            haritaWidget->addStyleClass (Bootstrap::Grid::col_full_12);
            haritaWidget->setHeight (450);
            haritaWidget->addSerikLogoMarker ();
            haritaWidget->setZoomLevel (16);
        }




        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                         Style::background::color::color(Style::color::White::Snow));




        auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());



        Wt::WLink link = Wt::WLink("https://www.google.co.uk/maps/place/T.C.+Serik+Belediyesi/@36.9165609,31.1040117,16z/data=!4m5!3m4!1s0x14c36295881be76f:0xe1e49781ec99e7eb!8m2!3d36.9161357!4d31.1040598");
        link.setTarget(Wt::LinkTarget::NewWindow);

        std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                    "<h4>GoogleMap Link</h4>");

        layout->addWidget(std::move(anchor),0,AlignmentFlag::Top);


        layout->addWidget(cpp14::make_unique<WText>("<h2>Adres: Orta Mahalle, Kızderesi Cad No:25, 07500 Serik/Antalya</h2>",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
        layout->addWidget(cpp14::make_unique<WText>("İletişim: 444 9 722",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
        layout->addWidget(cpp14::make_unique<WText>("+90 242 710 55 55",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

        layout->addWidget(cpp14::make_unique<WText>("Fax: +90 242 722 19 68",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

        layout->addWidget(cpp14::make_unique<WText>("mail: serik@serik.bel.tr",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

    }


    //    {
    //        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
    //        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    //        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
    //                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    //    }



    // Yeni Talep Sayfası
    {
        auto talep = container->addWidget (cpp14::make_unique<TalepWidget::TalepWidget>(this->getDB ()));
        talep->addStyleClass (Bootstrap::Grid::col_full_12);
        talep->setMargin (20,Side::Top|Side::Bottom);
        talep->addStyleClass ("boxShadow");
    }

    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initAnounceList()
{
    mContentWidget->clear();
    auto mMainContainer = mContentWidget->addWidget(cpp14::make_unique<v2::Duyuru::Widget::PublicListView>(this->getDB()));
    mMainContainer->setPadding( 90 , Side::Top );
    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    footer->removeStyleClass("footerStickAbsolute");
}


void MainPage::initBaskan()
{

    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<BaskanWidget>(this->getDB()->db()));
    widget->setMaximumSize(1024,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");

}

bool MainPage::initMeclisCanliYayin()
{

    auto filter = document{};

    try {
        filter.append(kvp("mecliscanliyayin",bsoncxx::types::b_utf8{"mecliscanliyayin"}));
    } catch (bsoncxx::exception &e) {

    }

    std::string embedlink;
    std::string CanliYayinTitle = "Canlı Yayın";
    bool online = false;

    try {
        auto val = this->getDB()->db()->collection("mecliscanli").find_one(filter.view());
        if( val )
        {
            try {
                online = val.value().view()["online"].get_bool().value;
            } catch (bsoncxx::exception &e) {

            }
            if( online )
            {
                try {
                    embedlink = val.value().view()["link"].get_string().value.data()+std::string("?autoplay=1+");
                } catch (bsoncxx::exception &e) {

                }

                try {
                    CanliYayinTitle = val.value().view()["title"].get_string().value.data();
                } catch (bsoncxx::exception &e) {

                }
            }
        }
    } catch (mongocxx::exception &e) {

    }

    if( online )
    {
        auto mContainer = mContentWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        mContainer->addStyleClass(Bootstrap::Grid::container_fluid);
        mContainer->setContentAlignment(AlignmentFlag::Center);
        mContainer->setMaximumSize(1280,WLength::Auto);

        auto rContaier = mContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rContaier->addStyleClass(Bootstrap::Grid::row);
        rContaier->setContentAlignment(AlignmentFlag::Center);


        {
            auto link = "<iframe src=\"" +embedlink+ "\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\" frameborder=\"1\" marginheight=\"5px\" marginwidth=\"5px\" height=\"100%\" width=\"100%\" "
                                                     "allow=\"accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture\" allowfullscreen></iframe>";

            auto container = rContaier->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail+Bootstrap::Grid::col_full_12);
            container->addStyleClass("CanliYayin");
            container->setPadding(90,Side::Top);
            container->setContentAlignment(AlignmentFlag::Center);
            auto text = container->addWidget(cpp14::make_unique<WText>(link,TextFormat::UnsafeXHTML));
            text->setMaximumSize(1280,WLength::Auto);
        }

        {
            auto container = rContaier->addWidget(cpp14::make_unique<WContainerWidget>());
            //            container->setPadding(90,Side::Top);
            container->setContentAlignment(AlignmentFlag::Center);
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail+Bootstrap::Grid::col_full_12);
            auto text = container->addWidget(cpp14::make_unique<WText>("<h4>"+CanliYayinTitle+"</h4>",TextFormat::UnsafeXHTML));
            text->setAttributeValue(Style::style,Style::color::rgb("25,25,25"));
        }

        {
            auto container = rContaier->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setContentAlignment(AlignmentFlag::Center);
            container->setMargin(15,Side::Top|Side::Bottom);
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            auto mAnaSayfaBtn = container->addWidget(cpp14::make_unique<WPushButton>("Ana Sayfa için Tıklayınız",TextFormat::UnsafeXHTML));
            mAnaSayfaBtn->addStyleClass(Bootstrap::Button::Warning);
            mAnaSayfaBtn->clicked().connect(this,&MainPage::init);
        }

    }

    return online;

}

void MainPage::initNobetciEczane()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<NobetciEczaneWidget>());
    widget->setMaximumSize(1024,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initBaskanYardimcilari()
{
    mContentWidget->clear ();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<v2::YonetimWidget>(new SerikBLDCore::DB(this->getDB ())));
    widget->setMaximumSize(1024,WLength::Auto);
    widget->BaskanClicked().connect(this,&MainPage::initBaskan);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initMeclisUyeleri()
{
    mContentWidget->clear ();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<v2::MeclisUyesiPublicWidget>(new SerikBLDCore::DB(this->getDB ())));
    widget->setMaximumSize(1024,WLength::Auto);
    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initNostSerik()
{

//    mContentWidget->clear();
    [[maybe_unused]] auto widget = mContentWidget->addWidget(cpp14::make_unique<v2::NostSerik>(new SerikBLDCore::DB(this->getDB())));
//    widget->setMaximumSize(1024,WLength::Auto);
    //    footer->removeStyleClass("footerStickAbsolute");
}

void MainPage::initKVKK()
{
    mContentWidget->clear ();
    [[maybe_unused]] auto widget = mContentWidget->addWidget(cpp14::make_unique<v2::KVKK::KVKKContainer>());
//    widget->setMaximumSize(1024,WLength::Auto);
//    footer->removeStyleClass("footerStickAbsolute");
}

