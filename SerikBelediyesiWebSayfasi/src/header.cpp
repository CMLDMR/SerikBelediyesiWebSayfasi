#include "header.h"

Header::Header::Header()
    :WContainerWidget()
{
    addStyleClass(Bootstrap::Grid::container_fluid+"headerBar");
    setContentAlignment(AlignmentFlag::Center);
    setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::Black));
    setPadding(0,AllSides);
    setMargin(0,AllSides);

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());


    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mMainContainer->setMargin(0,AllSides);
    mMainContainer->setPadding(0,AllSides);
    mMainContainer->setContentAlignment(AlignmentFlag::Center);
//    mMainContainer->setAttributeValue(Style::style,Style::Border::border("1px solid red"));
    mMainContainer->setMaximumSize(1600,WLength::Auto);






    auto _MMainContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    _MMainContainer->addStyleClass(Bootstrap::Grid::row);
    _MMainContainer->setPadding(0,AllSides);
    _MMainContainer->setMargin(0,AllSides);
    _MMainContainer->setId("__MMAinContainer");
    _MMainContainer->setMaximumSize(1450,WLength::Auto);


    auto layout = _MMainContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    layout->setContentsMargins(0,0,0,0);



    {
        auto container = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPadding(0,AllSides);
        container->setMargin(0,AllSides);
        container->addStyleClass("headerlogoContainer");


        auto imgContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->setPadding(0,AllSides);
        imgContainer->setMargin(0,AllSides);
        imgContainer->setAttributeValue(Style::style,Style::background::url("img/logo.jpg")+
                                     Style::background::size::cover+
                                     Style::background::origin::border_box+
                                     Style::background::repeat::norepeat+
                                     Style::background::position::center_center);


        imgContainer->addStyleClass("headerlogo");
        container->clicked().connect([=](){
            mClickAnaSayfa_.emit(NoClass());
        });
    }




    auto _Container = layout->addWidget(cpp14::make_unique<WContainerWidget>());
    _Container->setHeight(140);
    _Container->setPadding(0,AllSides);
    _Container->setMargin(0,AllSides);

    auto _Layout = _Container->setLayout(cpp14::make_unique<WVBoxLayout>());
    _Layout->setContentsMargins(0,0,0,0);








    // Dil Alanı
    {

        auto container = _Layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
        container->addStyleClass("LangHeaderBar");

        container->setHeight(30);
        {
            auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::row);
            _container->addStyleClass("LanguAgeRow");
            _container->setId("RowIDLang");

            //1
            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2
                                           +Bootstrap::Grid::Small::col_sm_3
                                           +Bootstrap::Grid::ExtraSmall::col_xs_4);
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>("Türkçe"),0,AlignmentFlag::Middle);
                trText->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
                trText->decorationStyle().setCursor(Cursor::PointingHand);
            }

            //2
            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2
                                           +Bootstrap::Grid::Small::col_sm_3
                                           +Bootstrap::Grid::ExtraSmall::col_xs_4);
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>("English"),0,AlignmentFlag::Middle);
                trText->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
                trText->decorationStyle().setCursor(Cursor::PointingHand);

                trContainer->clicked().connect([=](){
                    this->showMessage("info","This Page is in Under Construction!. Please Revisit Later");
                });
            }

//            //2
//            {
//                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
//                trContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2
//                                           +Bootstrap::Grid::Small::col_sm_3
//                                           +Bootstrap::Grid::ExtraSmall::col_xs_4);
//                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//                auto trText = layout->addWidget(cpp14::make_unique<WText>("e-Belediye"),0,AlignmentFlag::Middle);
//                trText->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
//                trText->decorationStyle().setCursor(Cursor::PointingHand);

//                trContainer->clicked().connect([=](){
//                    this->showMessage("Uyarı","e-Belediye Ödeme Sistemi 11 Mart 2019 Tarihinden İtibaren Devreye Alınacaktır.");
//                });
//            }

            //3
            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AliceBlue));
                trContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2
                                           +Bootstrap::Grid::Small::col_sm_3
                                           +Bootstrap::Grid::ExtraSmall::col_xs_4);
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//                auto trText = layout->addWidget(cpp14::make_unique<WText>("e-Belediye"),0,AlignmentFlag::Middle);
//                trText->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
//                trText->decorationStyle().setCursor(Cursor::PointingHand);



                Wt::WLink link = Wt::WLink("https://webportal.serik.bel.tr/web/guest/2");
                link.setTarget(Wt::LinkTarget::NewWindow);



                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                        "e-Belediye");


                layout->addWidget(std::move(anchor),1,AlignmentFlag::Middle);
            }


        }
    }












    // Menülerin alanı
    {
        auto container = _Layout->addWidget(cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
        container->addStyleClass("headerMenuContainer");
        container->setAttributeValue(Style::style,Style::background::color::rgb("55,55,55"));
//        container->setAttributeValue(Style::style,Style::Border::border("1px dotted green"));
        container->setPadding(0,AllSides);
        container->setMargin(0,AllSides);
        container->setHeight(80);


        {
            auto _container = container->setLayout(cpp14::make_unique<WHBoxLayout>());
//            {
//                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
//                trContainer->addStyleClass("HeaderMenuItem");
//                trContainer->clicked().connect([=](){
//                    mClickAnaSayfa_.emit(NoClass());
//                });
//                trContainer->setMargin(5,Side::Left);
//                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//                auto trText = layout->addWidget(cpp14::make_unique<WText>("Ana Sayfa"),0,AlignmentFlag::Middle);
//            }

            // Yayın Test Değilse Meclis Butonunu Yayınla
            //TODO: test Yayınından Sonra Kaldırılacak
            if( !TEST ){
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass("HeaderMenuItem");
                trContainer->clicked().connect([=](){
                    mClickMeclis_.emit(NoClass());
                });
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>("Meclis"),0,AlignmentFlag::Middle);
            }


            // Yayın Test ise Kurumsal Menüyü Yayınla
            if( TEST ){

                auto _trCOntainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                _trCOntainer->setPadding(0,AllSides);
                _trCOntainer->setMargin(0,AllSides);
                auto vLayout = _trCOntainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                vLayout->setContentsMargins(0,0,0,0);

                auto trContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Middle);
                trContainer->addStyleClass("HeaderMenuItem");
                auto btn = trContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                auto text = btn->addWidget(cpp14::make_unique<WText>("Kurumsal"));

                btn->addStyleClass("dropbtn");

                auto dContainer = trContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                dContainer->setId("myDropdownKurumsal");
                dContainer->addStyleClass("dropdown-content");
                btn->clicked().connect([=](){
                    btn->doJavaScript("document.getElementById(\"myDropdownKurumsal\").classList.toggle(\"show\");");
                });

                {
                    auto menu1 = dContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    menu1->setHeight(50);
                    menu1->addStyleClass("dropbtnMenuItem");
                    menu1->setAttributeValue(Style::style,Style::background::color::rgb(255,255,255));
                    auto vLayout = menu1->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Yönetim"),0,AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
                    menu1->clicked().connect([=](){
//                        mClickProje_.emit(NoClass());
                    });
                }

                {
                    auto menu1 = dContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    menu1->setHeight(50);
                    menu1->addStyleClass("dropbtnMenuItem");
                    menu1->setAttributeValue(Style::style,Style::background::color::rgb(255,255,255));
                    auto vLayout = menu1->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Birimler"),0,AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
                    menu1->clicked().connect([=](){
//                        mClickCalisma_.emit(NoClass());
                    });
                }

                {
                    auto menu1 = dContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    menu1->setHeight(50);
                    menu1->addStyleClass("dropbtnMenuItem");
                    menu1->setAttributeValue(Style::style,Style::background::color::rgb(255,255,255));
                    auto vLayout = menu1->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Meclis Kararları"),0,AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
                    menu1->clicked().connect([=](){
                        mClickMeclis_.emit(NoClass());
                    });
                }

                {
                    auto menu1 = dContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    menu1->setHeight(50);
                    menu1->addStyleClass("dropbtnMenuItem");
                    menu1->setAttributeValue(Style::style,Style::background::color::rgb(255,255,255));
                    auto vLayout = menu1->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Meclis Üyeleri"),0,AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
                    menu1->clicked().connect([=](){
//                        mClickCalisma_.emit(NoClass());
                    });
                }
            }



            {
                auto _trCOntainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                _trCOntainer->setPadding(0,AllSides);
                _trCOntainer->setMargin(0,AllSides);
                auto vLayout = _trCOntainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                vLayout->setContentsMargins(0,0,0,0);

                auto trContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Middle);
                trContainer->addStyleClass("HeaderMenuItem");
                auto btn = trContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                auto text = btn->addWidget(cpp14::make_unique<WText>("Proje Çalışma"));
                btn->clicked().connect([=](){
                    btn->doJavaScript("document.getElementById(\"myDropdown\").classList.toggle(\"show\");");
                });
                btn->addStyleClass("dropbtn");

                auto dContainer = trContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                dContainer->setId("myDropdown");
                dContainer->addStyleClass("dropdown-content");

                {
                    auto menu1 = dContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    menu1->setHeight(50);
                    menu1->addStyleClass("dropbtn");
                    menu1->setAttributeValue(Style::style,Style::background::color::rgb(125,150,200));
                    auto vLayout = menu1->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Projeler"),0,AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
                    menu1->clicked().connect([=](){
                        mClickProje_.emit(NoClass());
                    });
                }

                {
                    auto menu1 = dContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    menu1->setHeight(50);
                    menu1->addStyleClass("dropbtn");
                    menu1->setAttributeValue(Style::style,Style::background::color::rgb(125,150,200));
                    auto vLayout = menu1->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Çalışmalar"),0,AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
                    menu1->clicked().connect([=](){
                        mClickCalisma_.emit(NoClass());
                    });
                }
            }




            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass("HeaderMenuItem");
                trContainer->clicked().connect([=](){
                    mClickHaber_.emit(NoClass());
                });
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>("Haberler"),0,AlignmentFlag::Middle);
            }

            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass("HeaderMenuItem");
                trContainer->clicked().connect([=](){
                    mClickEtkinlik_.emit(NoClass());
                });
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>("Etkinlikler"),0,AlignmentFlag::Middle);
            }


            {

                auto _trCOntainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                _trCOntainer->setPadding(0,AllSides);
                _trCOntainer->setMargin(0,AllSides);
                auto vLayout = _trCOntainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                vLayout->setContentsMargins(0,0,0,0);

                auto trContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Middle);
                trContainer->addStyleClass("HeaderMenuItem");
                auto btn = trContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                auto text = btn->addWidget(cpp14::make_unique<WText>("Galeri"));
                text->clicked().connect([=](){
                    text->doJavaScript("DropDownGaleri()");
                });
                btn->setAttributeValue("onclick","DropDownGaleri()");
                btn->addStyleClass("dropbtn");

                auto dContainer = trContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                dContainer->setId("myDropdownGaleri");
                dContainer->addStyleClass("dropdown-content");

                {
                    auto menu1 = dContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    menu1->setHeight(50);
                    menu1->addStyleClass("dropbtn");
                    menu1->setAttributeValue(Style::style,Style::background::color::rgb(125,120,200));
                    auto vLayout = menu1->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Videolar"),0,AlignmentFlag::Middle);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
                    menu1->clicked().connect([=](){
                        mClickVideo_.emit(NoClass());
                    });
                }
            }


            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass("HeaderMenuItem");
                trContainer->clicked().connect([=](){
                    mClickBilgiEdin_.emit(NoClass());
                });

                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>("Bilgi Edinme"),0,AlignmentFlag::Middle);
            }
            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass("HeaderMenuItem");
                trContainer->clicked().connect([=](){
                    mClickIletisim_.emit(NoClass());
                });
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("İletişim")),0,AlignmentFlag::Middle);
            }
            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass("HeaderMenuItem");
                trContainer->clicked().connect([=](){
                    mClickHakkinda_.emit(NoClass());
                });
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("Serik")),0,AlignmentFlag::Middle);
                trText->addStyleClass(Bootstrap::Label::Warning);
                trText->setAttributeValue(Style::style,Style::font::size::s16px);
            }
            {
                auto trContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                trContainer->addStyleClass("HeaderMenuItem");
                trContainer->clicked().connect([=](){
                    mClickGiris_.emit(NoClass());
                });
                auto layout = trContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto trText = layout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("Giriş")),0,AlignmentFlag::Middle);
            }

        }


    }




    // Navigation Bar
    {
        auto container = _Layout->addWidget(cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
        container->addStyleClass("NavigationBarSet");
//        container->setAttributeValue(Style::style,Style::Border::border("3px solid white"));
        Wt::WNavigationBar *navigation = container->addWidget(Wt::cpp14::make_unique<Wt::WNavigationBar>());
//        navigation->setTitle("Serik Belediyesi","http://www.serik.bel.tr");
        navigation->setResponsive(true);
//        navigation->setAttributeValue(Style::style,Style::Border::border("2px solid red"));
        navigation->setHeight(WLength("100%"));


        // Setup a Left-aligned menu.
        auto leftMenu = Wt::cpp14::make_unique<Wt::WMenu>();
        auto leftMenu_ = navigation->addMenu(std::move(leftMenu),AlignmentFlag::Left);

//        {
//            auto item = leftMenu_->addItem("Ana Sayfa");
//                item->addStyleClass("navbarItem");
//            item->clicked().connect([=](){
//                mClickAnaSayfa_.emit(NoClass());
//            });
//        }
        {
            auto item = leftMenu_->addItem("Meclis");
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickMeclis_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem("Projeler");
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickProje_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem("Haberler");
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickHaber_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem("Etkinlikler");
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickEtkinlik_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem("Videolar");
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickVideo_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem("Bilgi Edinme");
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickBilgiEdin_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem(WString::fromUTF8("İletişim"));
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickIletisim_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem(WString::fromUTF8("Serik"));
            item->addStyleClass(Bootstrap::Label::Warning+"navbarItem");
            item->clicked().connect([=](){
                mClickHakkinda_.emit(NoClass());
            });
        }

        {
            auto item = leftMenu_->addItem(WString::fromUTF8("Giriş"));
            item->addStyleClass("navbarItem");
            item->clicked().connect([=](){
                mClickGiris_.emit(NoClass());
            });
        }

    }
}

Signal<NoClass> &Header::Header::mClickAnaSayfa()
{
    return this->mClickAnaSayfa_;
}

Signal<NoClass> &Header::Header::mClickYonetim()
{
    return this->mClickYonetim_;
}

Signal<NoClass> &Header::Header::mClickMeclis()
{
    return this->mClickMeclis_;
}

Signal<NoClass> &Header::Header::mClickProje()
{
    return this->mClickProje_;
}

Signal<NoClass> &Header::Header::mClickHaber()
{
    return this->mClickHaber_;
}

Signal<NoClass> &Header::Header::mClickEtkinlik()
{
    return mClickEtkinlik_;
}

Signal<NoClass> &Header::Header::mClickVideo()
{
    return mClickVideo_;
}

Signal<NoClass> &Header::Header::mClickBilgiEdin()
{
    return mClickBilgiEdin_;
}

Signal<NoClass> &Header::Header::mClickIletisim()
{
    return mClickIletisim_;
}

Signal<NoClass> &Header::Header::mClickGiris()
{
    return mClickGiris_;
}

Signal<NoClass> &Header::Header::mClickHakkinda()
{
    return mClickHakkinda_;
}

Signal<NoClass> &Header::Header::mClickCalisma()
{
    return mClickCalisma_;
}

Signal<NoClass> &Header::Header::mClickMeclisUyeleri()
{
    return mClickMeclisUyeleri_;
}

void Header::Header::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}

