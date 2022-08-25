#include "mainpagecontroller.h"
#include "SerikBelediyesiWebSayfasi/src/body.h"

MainPageController::MainPageController(mongocxx::database *_db)
    :DataBaseWidget (_db)
{

    setContentAlignment(AlignmentFlag::Center);

    auto rContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setMaximumSize(1270,WLength::Auto);
    auto contentContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    contentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);



    {

        auto bContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        bContainer->addStyleClass(Bootstrap::Grid::row+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
        bContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,250),this->getRandom(200,250),this->getRandom(200,250),0.5));

        auto bootsrapString = Bootstrap::Grid::Large::col_lg_3 + Bootstrap::Grid::Medium::col_md_3 + Bootstrap::Grid::Small::col_sm_4 + Bootstrap::Grid::ExtraSmall::col_xs_4;

//        if(false){   // HABERLER.
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->setContentAlignment(AlignmentFlag::Center);


//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(225,250),
//                                                                               this->getRandom(225,250),
//                                                                               this->getRandom(225,250),0.95));

//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail+CSSStyle::Shadows::shadow8px);

//            btn->decorationStyle().setCursor(Cursor::PointingHand);



//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());

//            // Create an anchor that links to a URL through clickable text.
//            Wt::WLink link = Wt::WLink("http://webportal.serik.bel.tr:9195/home");
//            link.setTarget(Wt::LinkTarget::NewWindow);

//            std::unique_ptr<Wt::WAnchor> anchor =
//                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
//                                    "<b>7326 Yapılandırma Başvurusu</b>");


//            layout->addWidget(std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//        }



        {   // HABERLER
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);


            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));

            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail+CSSStyle::Shadows::shadow8px);

            btn->decorationStyle().setCursor(Cursor::PointingHand);

            btn->clicked().connect([=](){
              _Haber.emit(NoClass());
            });

            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("HABERLER"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }

//        {   // ÇALIŞMALAR
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,50),
//                                                                               this->getRandom(25,50),
//                                                                               this->getRandom(25,50)));
//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

//            btn->decorationStyle().setCursor(Cursor::PointingHand);

//            btn->clicked().connect([=](){
//              _Calisma.emit(NoClass());
//            });

//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
//            auto text = layout->addWidget(cpp14::make_unique<WText>("ÇALIŞMALAR"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
//        }

//        {   // PROJELER
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,50),
//                                                                               this->getRandom(25,50),
//                                                                               this->getRandom(25,50)));
//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

//            btn->decorationStyle().setCursor(Cursor::PointingHand);
//            btn->clicked().connect([=](){
//              _Projeler.emit(NoClass());
//            });

//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
//            auto text = layout->addWidget(cpp14::make_unique<WText>("PROJELER"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
//        }

//        {   // ETKİNLİKLER
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
//                                                                               this->getRandom(25,100),
//                                                                               this->getRandom(25,100)));
//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

//            btn->decorationStyle().setCursor(Cursor::PointingHand);
//            btn->clicked().connect([=](){
//              _Etkinlikler.emit(NoClass());
//            });

//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
//            auto text = layout->addWidget(cpp14::make_unique<WText>("ETKİNLİKLER"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
//        }

        {   // MECLİS
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _Meclis.emit(NoClass());
            });

            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("MECLİS"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }

        {   // DUYURULAR
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _duyurular.emit(NoClass());
            });


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("DUYURULAR"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }

//        {   // KURUMSAL
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,50),
//                                                                               this->getRandom(25,50),
//                                                                               this->getRandom(25,50)));
//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
//            auto text = layout->addWidget(cpp14::make_unique<WText>("KURUMSAL"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
//        }

//        {   // GALERİ
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,50),
//                                                                               this->getRandom(25,50),
//                                                                               this->getRandom(25,50)));
//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
//            auto text = layout->addWidget(cpp14::make_unique<WText>("GALERİ"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
//        }

        {   // BİLGİ EDİNME
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _BilgiEdinme.emit(NoClass());
            });


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("BİLGİ EDİNME"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }

        {   // e-BELEDİYE
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());

            Wt::WLink link = Wt::WLink("https://webportal.serik.bel.tr/web/guest/2");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "e-Belediye");

            auto text = layout->addWidget(std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }

        {   // GİRİŞ
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
//                std::cout << "Giriş Butonu Clicked" << std::endl;
              _Giris.emit(NoClass());
            });


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("GİRİŞ"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }


        {   // Nöbetçi Eczaneler
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::url("v2/img/eczane.png")+
                                   Style::background::size::contain+
                                   Style::background::repeat::norepeat+
                                   Style::background::position::center_center);
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _NobetciEczane.emit(NoClass());
            });
        }


        {   // BİR ZAMANLAR SERİK
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::url("img/aspicon.jpg")+Style::background::size::cover
                                   +Style::background::position::center_center
                                   +Style::background::repeat::norepeat+Style::Border::border("3px solid white"));

            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            btn->setPositionScheme(PositionScheme::Relative);
            btn->addStyleClass("galeriBtn");

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
//                std::cout << "Giriş Butonu Clicked" << std::endl;
              _NostSerik.emit(NoClass());
            });

            auto iconWidget = btn->addWidget(cpp14::make_unique<WContainerWidget>());
//            iconWidget->setPositionScheme(PositionScheme::Absolute);
            iconWidget->setWidth(WLength("100%"));
            iconWidget->setHeight(WLength("100%"));
//            iconWidget->setOffsets(-10,Side::Top|Side::Left);


//            iconWidget->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(175,255),
//                                                                               this->getRandom(150,200),
//                                                                               this->getRandom(25,75),0.5));

            iconWidget->addStyleClass("galeriBtnSlide");


            auto layout = iconWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("BİR ZAMANLAR SERİK"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }




        {   // İLETİSİM
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Hidden::hidden_lg+Bootstrap::Grid::Hidden::hidden_md);
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _iletisim.emit(NoClass());
            });


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("İLETİŞİM"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }

        {   // HAKKINDA
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Hidden::hidden_lg+Bootstrap::Grid::Hidden::hidden_md);
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,100),
                                                                               this->getRandom(25,100),
                                                                               this->getRandom(25,100)));
            btn->setHeight(100);
            btn->setWidth(100);
            btn->setMargin(15,AllSides);
            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _hakkinda.emit(NoClass());
            });


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("HAKKINDA"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }





    }


    {
//        auto bContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//        bContainer->addStyleClass(Bootstrap::Grid::row);

////        auto bootsrapString = Bootstrap::Grid::Large::col_lg_3 + Bootstrap::Grid::Medium::col_md_3 + Bootstrap::Grid::Small::col_sm_4 + Bootstrap::Grid::ExtraSmall::col_xs_4;

//        {   // EL HİJYENİ
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(Bootstrap::Grid::col_full_12);
//            container->setContentAlignment(AlignmentFlag::Center);
//            container->setMargin(10,Side::Top|Side::Bottom);

////            container->setAttributeValue(Style::style,Style::background::color::rgb (this->getRandom (50,75),
////                                                                               this->getRandom (150,175),
////                                                                               this->getRandom (170,210)));

//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgb (this->getRandom (50,75),
//                                                                               this->getRandom (150,175),
//                                                                               this->getRandom (170,210)));
//            btn->setHeight(100);
//            btn->setWidth(450);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
//    //            btn->decorationStyle().setCursor(Cursor::PointingHand);
//    //            btn->clicked().connect([=](){
//    //              _NobetciEczane.emit(NoClass());
//    //            });

//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());

//            // Create an anchor that links to a URL through clickable text.
//            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/download/html/isgucu.htm");
//            link.setTarget(Wt::LinkTarget::NewWindow);

//            std::unique_ptr<Wt::WAnchor> anchor =
//                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
//                                    "NİTELİKLİ İŞ GÜCÜ KURSLARI İÇİN TIKLAYINIZ");
//            anchor->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);

//            layout->addWidget (std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//        }





//        {   // EL HİJYENİ
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgb (this->getRandom (50,75),
//                                                                               this->getRandom (170,215),
//                                                                               this->getRandom (140,170)));
//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());

//            // Create an anchor that links to a URL through clickable text.
//            Wt::WLink link = Wt::WLink("https://dosyamerkez.saglik.gov.tr/Eklenti/27596,saglik-elimizde-a-4-brosurpdf.pdf?0");
//            link.setTarget(Wt::LinkTarget::NewWindow);

//            std::unique_ptr<Wt::WAnchor> anchor =
//                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
//                                    "SAĞLIK ELİMİZDE");
//            anchor->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);

//            layout->addWidget (std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//        }

//        {   // EL HİJYENİ
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgb (this->getRandom (170,220),
//                                                                               this->getRandom (50,115),
//                                                                               this->getRandom (140,170)));
//            btn->setHeight(100);
//            btn->setWidth(100);
//            btn->setMargin(15,AllSides);
//            btn->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());

//            // Create an anchor that links to a URL through clickable text.
//            Wt::WLink link = Wt::WLink("https://www.youtube.com/embed/aVViU6SIkWE?autoplay=1");
//            link.setTarget(Wt::LinkTarget::NewWindow);

//            std::unique_ptr<Wt::WAnchor> anchor =
//                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
//                                    "EL HİJYENİ Video");
//            anchor->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);

//            layout->addWidget (std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//        }







    }






//    auto anounceContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    anounceContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3);

//    {
//        auto list = anounceContainer->addWidget(cpp14::make_unique<Body::NewsAnnounceContent::AnnouncePanel::AnnounceList>(this->getDB()));
//        list->mGetOid().connect([=](std::string mOid){
//           _Announce.emit(mOid);
//        });
//    }
}

Signal<NoClass> &MainPageController::ClickHaber()
{
    return _Haber;
}

Signal<NoClass> &MainPageController::ClickCalisma()
{
    return _Calisma;
}

Signal<NoClass> &MainPageController::ClickProjeler()
{
    return _Projeler;
}

Signal<NoClass> &MainPageController::ClickEtkinlikler()
{
    return _Etkinlikler;
}

Signal<NoClass> &MainPageController::ClickBilgiEdinme()
{
    return _BilgiEdinme;
}

Signal<NoClass> &MainPageController::ClickGiris()
{
    return _Giris;
}

Signal<NoClass> &MainPageController::ClickMeclis()
{
    return _Meclis;
}

Signal<std::string> &MainPageController::ClickAnounce()
{
    return _Announce;
}

Signal<NoClass> &MainPageController::ClickIletisim()
{
    return _iletisim;
}

Signal<NoClass> &MainPageController::ClickHakkinda()
{
    return _hakkinda;
}

Signal<NoClass> &MainPageController::ClickDuyurular()
{
    return _duyurular;
}

Signal<NoClass> &MainPageController::ClickNobetciEczane()
{
    return _NobetciEczane;
}

Signal<NoClass> &MainPageController::ClickNostSerik()
{
    return _NostSerik;
;

}

