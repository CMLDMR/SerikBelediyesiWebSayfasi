#include "mainpagecontroller.h"
#include "SerikBelediyesiWebSayfasi/src/body.h"

MainPageController::MainPageController()
//    :DataBaseWidget (_db)
{

    this->clear();
    setContentAlignment(AlignmentFlag::Center);

    auto rContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white"));

    rContainer->setMaximumSize(1270,WLength::Auto);
    auto contentContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    contentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    contentContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,15),
                                                                               this->getRandom(50,150),
                                                                               this->getRandom(225,255),0.15));


    {

        bContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//        bContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(175,250),
//                                                                                   this->getRandom(210,250),
//                                                                                   this->getRandom(225,255),0.75));


        {
            auto btn = createButton("HABERLER");
            btn->clicked().connect([=](){
                _Haber.emit(NoClass());
            });
        }

        {
            auto btn = createButton("MECLİS");
            btn->clicked().connect([=](){
                _Meclis.emit(NoClass());
            });
        }

        {
            auto btn = createButton("DUYURULAR");
            btn->clicked().connect([=](){
               _duyurular.emit(NoClass());
            });
        }

        {
            auto btn = createButton("BİLGİ EDİNME");
            btn->clicked().connect([=](){
                _BilgiEdinme.emit(NoClass());
            });
        }

//        {
//            auto btn = createButton("BİLGİ EDİNME");
//            btn->clicked().connect([=](){
//                _BilgiEdinme.emit(NoClass());
//            });
//        }








        {   // Nöbetçi Eczaneler
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::url("v2/img/eczane1.png")+
                                   Style::background::size::contain+
                                   Style::background::repeat::norepeat+
                                   Style::background::position::center_center+Style::Border::border("1px solid white"));
            btn->setHeight(mHeight);
            btn->setWidth(mWidth);
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
                                   +Style::background::repeat::norepeat+Style::Border::border("1px solid white"));

            btn->setHeight(mHeight);
            btn->setWidth(mWidth);
            btn->setMargin(15,AllSides);
            btn->setPositionScheme(PositionScheme::Relative);
            btn->addStyleClass("galeriBtn");

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _NostSerik.emit(NoClass());
            });

            auto iconWidget = btn->addWidget(cpp14::make_unique<WContainerWidget>());
            iconWidget->setWidth(WLength("100%"));
            iconWidget->setHeight(WLength("100%"));
            iconWidget->addStyleClass("galeriBtnSlide");


            auto layout = iconWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("BİR ZAMANLAR SERİK"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }



//        {   // e-BELEDİYE
//            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(bootsrapString);
//            container->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_3+Bootstrap::Grid::Offset::Medium::col_md_3);
//            container->setContentAlignment(AlignmentFlag::Center);
//            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            btn->setAttributeValue(Style::style,Style::background::color::rgba(236,157,8,0.95)+Style::Border::border("1px solid white"));
//            btn->setHeight(mHeight);
//            btn->setWidth(mWidth);
//            btn->setMargin(15,AllSides);
//            //            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
//            btn->addStyleClass(CSSStyle::Shadows::shadow8px);


//            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());

//            Wt::WLink link = Wt::WLink("https://webportal.serik.bel.tr/web/guest/2");
//            link.setTarget(Wt::LinkTarget::NewWindow);

//            std::unique_ptr<Wt::WAnchor> anchor =
//                Wt::cpp14::make_unique<Wt::WAnchor>(link,
//                                                    "e-Belediye");

//            auto text = layout->addWidget(std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
//        }

        {
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(8,157,236,0.95)+Style::Border::border("1px solid white"));
            btn->setHeight(mHeight);
            btn->setWidth(mWidth);
            btn->setMargin(15,AllSides);
            //            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            btn->addStyleClass(CSSStyle::Shadows::shadow8px);


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());

            Wt::WLink link = Wt::WLink("https://bulutkbs.gov.tr/Rehber/#/app?86685935");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                    "KENT BİLGİ SİSTEMİ");

            auto text = layout->addWidget(std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }

        {
            auto btn = createButton("GİRİŞ");
            btn->clicked().connect([=](){
                _Giris.emit(NoClass());
            });
        }



        {   // Kurumsal
            auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Hidden::hidden_lg+Bootstrap::Grid::Hidden::hidden_md);
            container->addStyleClass(bootsrapString);
            container->setContentAlignment(AlignmentFlag::Center);
            auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,75),
                                                                               this->getRandom(25,75),
                                                                               this->getRandom(25,75),0.85));
            btn->setHeight(mHeight);
            btn->setWidth(mWidth);
            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _Yonetim.emit(NoClass());
            });


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("YÖNETİM"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
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
            btn->setHeight(mHeight);
            btn->setWidth(mWidth);
            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

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
            btn->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(25,75),
                                                                               this->getRandom(25,75),
                                                                               this->getRandom(25,75),0.85));
            btn->setHeight(mHeight);
            btn->setWidth(mWidth);
            btn->setMargin(15,AllSides);
//            btn->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->clicked().connect([=](){
              _hakkinda.emit(NoClass());
            });


            auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("SERİK"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);
        }







    }



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

Signal<NoClass> &MainPageController::ClickYonetim()
{
    return _Yonetim;

}

WContainerWidget *MainPageController::createButton(const std::string &btnName)
{
    auto container = bContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(bootsrapString);
    container->setContentAlignment(AlignmentFlag::Center);

    auto btn = container->addWidget(cpp14::make_unique<WContainerWidget>());

    btn->setHeight(mHeight);
    btn->setWidth(mWidth);
    btn->setMargin(15,AllSides);
    auto color = this->getRandom(20,100);
    btn->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(0,color,color+25,.75));

    btn->decorationStyle().setCursor(Cursor::PointingHand);

//    btn->clicked().connect([=](){
//        _Haber.emit(NoClass());
//    });

    auto layout = btn->setLayout(cpp14::make_unique<WVBoxLayout>());
    auto text = layout->addWidget(cpp14::make_unique<WText>(btnName),0,AlignmentFlag::Center|AlignmentFlag::Middle);
    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::weight::bold);

    return btn;

}

