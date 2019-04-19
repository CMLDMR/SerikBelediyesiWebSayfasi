#include "headerpage.h"

HeaderPage::HeaderPage(mongocxx::database *_db)
    :DataBaseWidget(_db)
{
    setWidth(WLength("100%"));
    setPositionScheme(PositionScheme::Absolute);
    setOffsets(0,Side::Top|Side::Left|Side::Right);

    {

        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->setWidth(WLength::Auto);
        container->setAttributeValue(Style::style,Style::background::color::rgba(25,25,25,0.65));
        container->setHeight(25);

        container->setContentAlignment(AlignmentFlag::Center);

        auto tContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        tContainer->setWidth(WLength("100%"));
        tContainer->setMaximumSize(1270,WLength::Auto);
        tContainer->setHeight(25);
        tContainer->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid white"));

        auto rContainer = tContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        rContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_10+Bootstrap::Grid::Offset::Medium::col_md_10+Bootstrap::Grid::Offset::Small::col_sm_8+Bootstrap::Grid::Offset::ExtraSmall::col_xs_6);
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto title = _container->addWidget(cpp14::make_unique<WText>("Türkçe"));
            title->setAttributeValue(Style::style,Style::color::rgb("255,255,255")+Style::font::size::s11px);
            _container->setContentAlignment(AlignmentFlag::Center);
        }

        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto title = _container->addWidget(cpp14::make_unique<WText>("English"));
            title->setAttributeValue(Style::style,Style::color::rgb("255,255,255")+Style::font::size::s11px);
            _container->setContentAlignment(AlignmentFlag::Center);
            _container->clicked().connect([=](){
               this->showMessage("Notice","This Page is Underconstruction!");
            });
        }

    }


    {

        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->setWidth(WLength::Auto);
        container->setAttributeValue(Style::style,Style::background::color::rgba(25,25,25,0.45));
        container->setHeight(65);
        container->setContentAlignment(AlignmentFlag::Center);


        auto tContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        tContainer->setWidth(WLength("100%"));
        tContainer->setMaximumSize(1270,WLength::Auto);
        tContainer->setHeight(65);


        {
            auto lContainer = tContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            lContainer->setHeight(65);
            lContainer->setWidth(240);
            lContainer->setPositionScheme(PositionScheme::Absolute);
            lContainer->setAttributeValue(Style::style,Style::background::url("v2/img/logo.png")+Style::background::size::cover+Style::background::repeat::norepeat);
            lContainer->clicked().connect([=](){
               _Anasayfa.emit(NoClass());
            });

            lContainer->decorationStyle().setCursor(Cursor::PointingHand);
        }

        {
            auto rContainer = tContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            rContainer->addStyleClass(Bootstrap::Grid::row);

            if( 0 ){
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_4+Bootstrap::Grid::Offset::Medium::col_md_4);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("Meclis"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _Meclis.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            if( 0 ){
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("Projeler"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _Projeler.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            if( 0 ){
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("Haberler"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _Haber.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            if( 0 ){
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("Etkinlikler"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _Etkinlikler.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            if( 0 ){
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("Bilgi Edinme"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _BilgiEdinme.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            {
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_8+Bootstrap::Grid::Offset::Medium::col_md_8+Bootstrap::Grid::Offset::Small::col_sm_10+Bootstrap::Grid::Offset::ExtraSmall::col_xs_9);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("Başkan"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _baskan.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            {
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
//                _container->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_9+Bootstrap::Grid::Offset::Medium::col_md_9);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("İletişim"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _Iletisim.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            {
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>("Hakkında"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
                _container->clicked().connect([=](){
                   _Hakkinda.emit(NoClass());
                });
                text->decorationStyle().setCursor(Cursor::PointingHand);
            }

            {
                auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->setHeight(65);
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Hidden::hidden_sm+Bootstrap::Grid::Hidden::hidden_xs);
                auto layout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());


                Wt::WLink link = Wt::WLink("https://webportal.serik.bel.tr/web/guest/2");
                link.setTarget(Wt::LinkTarget::NewWindow);



                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                        "e-Belediye");
                auto text = layout->addWidget(std::move(anchor),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s14px);
            }
        }
    }
}

Signal<NoClass> &HeaderPage::ClickAnaSayfa()
{
    return _Anasayfa;
}

Signal<NoClass> &HeaderPage::ClickHaber()
{
    return _Haber;
}

Signal<NoClass> &HeaderPage::ClickCalisma()
{
    return _Calisma;
}

Signal<NoClass> &HeaderPage::ClickProjeler()
{
    return _Projeler;
}

Signal<NoClass> &HeaderPage::ClickEtkinlikler()
{
    return _Etkinlikler;
}

Signal<NoClass> &HeaderPage::ClickBilgiEdinme()
{
    return _BilgiEdinme;
}

Signal<NoClass> &HeaderPage::ClickGiris()
{
    return _Giris;
}

Signal<NoClass> &HeaderPage::ClickMeclis()
{
    return _Meclis;
}

Signal<NoClass> &HeaderPage::ClickHakkinda()
{
    return _Hakkinda;
}

Signal<NoClass> &HeaderPage::ClickIletisim()
{
    return _Iletisim;
}

Signal<NoClass> &HeaderPage::ClickBaskan()
{
    return _baskan;
}
