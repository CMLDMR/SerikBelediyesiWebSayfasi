#include "gelenmesajlarwidget.h"



PersonelV2::GelenMesajlarWidget::GelenMesajlarWidget(mongocxx::database *_database, bsoncxx::document::value _user)
    :ContainerWidget(_database , _user , "Gelen Mesajlar")
{

    this->initHeader();

    this->getReinit().connect(this,&GelenMesajlarWidget::initHeader);

}

void PersonelV2::GelenMesajlarWidget::initHeader()
{

    this->getHeaderRowContainer()->clear();
    // Gelen Mesajlar
    {

        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(this->getBackGroundImg())+Style::background::size::cover);

        auto colorContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        colorContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7)+Style::Border::border("1px solid white"));
        colorContainer->setHeight(100);

        auto vLayout = colorContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Gelen Mesajlar"));
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::GhostWhite));
        vLayout->addStretch(1);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){

            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::birim,this->UserBirim()));
            } catch (bsoncxx::exception &e) {
                std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";
            }


            this->listMesajlar(filter.view());
        });
    }


    // Cevaplanmayan Mesajlar
    {

        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(this->getBackGroundImg())+Style::background::size::cover);

        auto colorContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        colorContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7)+Style::Border::border("1px solid white"));
        colorContainer->setHeight(100);

        auto vLayout = colorContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Cevaplanmayan"));
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::GhostWhite));
        vLayout->addStretch(1);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::birim,this->UserBirim()));
            } catch (bsoncxx::exception &e) {
                std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

            }

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,make_document(kvp("$exists",false))));
            } catch (bsoncxx::exception &e) {
                std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

            }


            this->listMesajlar(filter.view());
        });
    }

    // Cevaplanan Mesajlar
    {

        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::background::url(this->getBackGroundImg())+Style::background::size::cover);

        auto colorContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        colorContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7)+Style::Border::border("1px solid white"));
        colorContainer->setHeight(100);

        auto vLayout = colorContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Cevaplanan"));
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::GhostWhite));
        vLayout->addStretch(1);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::birim,this->UserBirim()));
            } catch (bsoncxx::exception &e) {
                std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

            }

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,make_document(kvp("$exists",true))));
            } catch (bsoncxx::exception &e) {
                std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

            }


            this->listMesajlar(filter.view());
        });
    }

}

void PersonelV2::GelenMesajlarWidget::listMesajlar(bsoncxx::document::view filterView)
{

    this->getContentRowContainer()->clear();

    auto container = getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::col_full_12);

    auto fCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fCOntainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());


    skip = 0;

    this->addList(rContainer,filterView);


}

void PersonelV2::GelenMesajlarWidget::addList(WContainerWidget *rContainer, bsoncxx::document::view filterView)
{


    mongocxx::options::find findOptions;

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::julianDate,-1));
    } catch (bsoncxx::exception &e) {
        std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

    }
    try {
        sortDoc.append(kvp(SBLDKeys::oid,-1));
    } catch (bsoncxx::exception &e) {
        std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

    }

    findOptions.sort(sortDoc.view());

    findOptions.limit(this->limit);
    findOptions.skip(this->skip);


    // Menu -Tarih-Saat-AdSoyad-Telefon-Mesaj-
    {
        auto cContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        cContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto fContainer = cContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto row = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);

        // Tarih
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Tarih"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
        }
        // Saat
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Saat"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
        }
        // AdSoyad
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("AdSoyad"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
        }
        // Telefon
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Telefon"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
        }
        // Mesaj
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            auto text = container->addWidget(cpp14::make_unique<WText>("Mesaj"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
        }
    }

    try {
        auto cursor = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).find(filterView,findOptions);

        for( auto doc : cursor )
        {
            auto cContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            cContainer->addStyleClass(Bootstrap::Grid::col_full_12);

            auto fContainer = cContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            fContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
            fContainer->setPadding(10,Side::Bottom|Side::Top);

            fContainer->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string());
            fContainer->clicked().connect([=](){
                this->setDetail(fContainer->attributeValue(Style::dataoid).toUTF8());
            });
            fContainer->decorationStyle().setCursor(Cursor::PointingHand);

            auto row = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);


            // Tarih
            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                try {
                    auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(doc[SBLDKeys::Yonetim::Baskan::MESAJ::julianDate].get_int64().value).toString("dd/MM/yyyy")));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {
                    auto text = container->addWidget(cpp14::make_unique<WText>("Tarih Yok"));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";
                }

            }
            // Saat
            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                try {
                    auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::saat].get_string().value.data()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {
                    auto text = container->addWidget(cpp14::make_unique<WText>("Saat Yok"));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

                }

            }
            // AdSoyad
            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                try {
                    auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::ad].get_string().value.data()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {
                    auto text = container->addWidget(cpp14::make_unique<WText>("AdSoyad Yok"));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

                }
            }
            // Telefon
            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                try {
                    auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::tel].get_string().value.data()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {
                    auto text = container->addWidget(cpp14::make_unique<WText>("Telefon Yok"));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

                }
            }
            // Mesaj
            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                try {
                    auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::mesaj].get_string().value.data()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {
                    auto text = container->addWidget(cpp14::make_unique<WText>("Mesaj Yok"));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

                }
            }


        }

    } catch (mongocxx::exception &e) {
        this->showMessage("Hata","DB Error: " , e );
    }

}

void PersonelV2::GelenMesajlarWidget::setDetail(std::string oid)
{


    this->getContentRowContainer()->clear();

    auto fContainer = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fContainer->setMargin(15,Side::Top|Side::Bottom);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

    }



    try {
        auto val = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).find_one(filter.view());

        if( val )
        {
            if( !val.value().view().empty() )
            {
                auto view = val.value().view();

                // Tarih
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(view[SBLDKeys::Yonetim::Baskan::MESAJ::julianDate].get_int64().value).toString("dd/MM/yyyy").toUTF8()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // Saat
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::saat].get_string().value.data()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // AD
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::ad].get_string().value.data()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // TEL
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::tel].get_string().value.data()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }


                // Mesaj
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->setPadding(25,Side::Top|Side::Bottom);
                    container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                    auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

                    vLayout->addStretch(1);
                    {
                        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Mesaj"));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }


                    try {
                        auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::mesaj].get_string().value.data()));
                        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s12px+Style::font::weight::bold);
                    } catch (bsoncxx::exception &e) {
                        auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }

                    vLayout->addStretch(1);
                }

                // Birim
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->setPadding(15,Side::Top|Side::Bottom);
                    container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(std::string("Görevli Birim: ")+view[SBLDKeys::Yonetim::Baskan::MESAJ::birim].get_string().value.data()));
                        text->addStyleClass(Bootstrap::Label::Warning);
                        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s12px+Style::font::weight::bold);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>("Birime Yönlendirilmemiş"));
                        text->addStyleClass(Bootstrap::Label::Danger);
                        std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

                    }
                }

                // Cevap Var mı
                bool cevapExist = false;
                bsoncxx::document::value cevapValue(document{}.view());
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->setPadding(25,Side::Top|Side::Bottom);
                    container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                    try {
                        auto cevapDoc = view[SBLDKeys::Yonetim::Baskan::MESAJ::cevap].get_document().view();
                        cevapValue = bsoncxx::document::value(cevapDoc);
                        cevapExist = true;


                        auto _fContainer_ = container->addWidget(cpp14::make_unique<WContainerWidget>());
                        _fContainer_->addStyleClass(Bootstrap::Grid::container_fluid);

                        auto _rContainer_ = _fContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                        _rContainer_->addStyleClass(Bootstrap::Grid::row);

                        {
                            auto TarihText = _rContainer_->addWidget(cpp14::make_unique<WText>("Cevap"));
                            TarihText->addStyleClass(Bootstrap::Label::Danger);
                            TarihText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s10px+Style::font::weight::bold);
                            TarihText->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                        }

                        {
                            auto TarihText = _rContainer_->addWidget(cpp14::make_unique<WText>("Tarih: "+WDate::fromJulianDay(cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::julianDate].get_int64().value).toString("dd/MM/yyyy").toUTF8()));
                            TarihText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s10px+Style::font::weight::bold);
                            TarihText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                        }
                        {
                            auto SaatText = _rContainer_->addWidget(cpp14::make_unique<WText>(std::string("Saat: ")+cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::saat].get_string().value.data()));
                            SaatText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s10px+Style::font::weight::bold);
                            SaatText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                        }
                        {
                            auto YazarText = _rContainer_->addWidget(cpp14::make_unique<WText>(std::string("Yazar: ")+cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::personelAdSoyad].get_string().value.data()));
                            YazarText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s10px+Style::font::weight::bold);
                            YazarText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                        }
                        {
                            auto CevapText = _rContainer_->addWidget(cpp14::make_unique<WText>(cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::cevap].get_string().value.data()));
                            CevapText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s12px+Style::font::weight::bold);
                            CevapText->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                            CevapText->setMargin(10,Side::Top|Side::Bottom);
                        }
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>("Cevaplanmamış Mesaj"));
                        text->addStyleClass(Bootstrap::Label::Danger);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                        std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

                    }
                }

                if( cevapExist )
                {


                }else{

                    auto cevapStack = rContainer->addWidget(cpp14::make_unique<WStackedWidget>());
                    cevapStack->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::CubicBezier,250));




                    // Cevap Ekleme Container
                    {
                        auto container = cpp14::make_unique<WContainerWidget>();
                        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                        vLayout->addStretch(1);
                        auto text = vLayout->addWidget(cpp14::make_unique<CevapYazWidget>(this->db(),this->User(),view[SBLDKeys::oid].get_oid().value.to_string()));
                        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        text->SuccessCevap().connect([=](){
                            cevapStack->setCurrentIndex(1);
                        });
                        vLayout->addStretch(1);
                        cevapStack->insertWidget(0,std::move(container));
                    }


                    // Seçenekler Container
                    {
                        auto container= cpp14::make_unique<WContainerWidget>();

                        auto fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                        fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

                        auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        rowContainer->addStyleClass(Bootstrap::Grid::row);


                        {
                            auto birimContanier = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                            birimContanier->addStyleClass(Bootstrap::Grid::col_full_12);
                            birimContanier->setHeight(40);
                            birimContanier->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                            auto vLayout = birimContanier->setLayout(cpp14::make_unique<WVBoxLayout>());
                            vLayout->addStretch(1);
                            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Cevap Yaz"));
                            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            vLayout->addStretch(1);
                            birimContanier->clicked().connect([=](){
                                cevapStack->setCurrentIndex(0);
                            });
                            birimContanier->decorationStyle().setCursor(Cursor::PointingHand);
                        }
                        cevapStack->insertWidget(1,std::move(container));
                    }
                    cevapStack->setCurrentIndex(1);
                    cevapStack->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                }

            }
        }


    } catch (mongocxx::exception &e) {
        std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

    }



}


PersonelV2::GelenMesajlarWidget::CevapYazWidget::CevapYazWidget(mongocxx::database *_database, bsoncxx::document::value _user, std::string mesajoid)
    :ContainerWidget::ContainerWidget(_database,_user),mesajOid(mesajoid)
{
    this->getContentRowContainer()->addWidget(cpp14::make_unique<WText>("Cevap Yaz"))->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    auto textEdit = this->getContentRowContainer()->addWidget(cpp14::make_unique<WTextEdit>());
    textEdit->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    textEdit->setText("Cevabı, Bu Yazıyı Silip Buraya Yazınız");

    auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Cevabı Ekle"));
    btn->addStyleClass(Bootstrap::Button::Primary);

    btn->setMargin(WLength::Auto,Side::Right|Side::Left);

    btn->clicked().connect([=](){

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{mesajOid}));
        } catch (bsoncxx::exception &e) {
            std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

        }

        auto doc = document{};
        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::cevap,textEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {
            std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

        }

        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::julianDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
        } catch (bsoncxx::exception &e) {
            std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

        }



        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::saat,WTime::currentTime().toString("hh:mm").toUTF8()));
        } catch (bsoncxx::exception &e) {
            std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

        }

        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::personelAdSoyad,this->User().view()[SBLDKeys::Personel::ad].get_string().value.data()));
        } catch (bsoncxx::exception &e) {
            std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

        }

        auto setDoc = document{};

        try {
            setDoc.append(kvp("$set",make_document(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,doc))));
        } catch (bsoncxx::exception &e) {
            std::cout << __LINE__ << " " << __FILE__ << " "<< e.what() << "\n";

        }

        try {
            auto upt = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).update_one(filter.view(),setDoc.view());
            if( upt )
            {
                if( upt.value().modified_count() )
                {
                    this->showMessage("Bilgi","Cevap Eklendi");
                    this->_SuccessCevap.emit(NoClass());
                }else{
                    this->showMessage("Hata","Dosya Set Edilemedi");
                }
            }else{
                this->showMessage("Hata","Server Cevap Vermedi");
            }
        } catch (mongocxx::exception &e) {
            this->showMessage("Hata","Error: " , e);
        }


    });

}
