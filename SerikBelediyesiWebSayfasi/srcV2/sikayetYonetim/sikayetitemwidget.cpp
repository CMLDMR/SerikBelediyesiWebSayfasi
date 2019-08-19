#include "sikayetitemwidget.h"

SikayetItemWidget::SikayetItemWidget(mongocxx::database *_db, UserClass &userValue, const bsoncxx::oid &_oid)
    :DBClass (_db),UserClass (userValue),mCurrentSikayet(Sikayet::SikayetItem::Load_Sikayet(_db,_oid).value())
{
    this->initHeader();
    this->initContent();
    this->initController();
}

void SikayetItemWidget::initHeader()
{
//    Header()->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Right);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::adSoyad)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Left);

        auto text = container->addWidget(cpp14::make_unique<WText>("05335018051 TCNOITEM class Yazılacak"));
        text->addStyleClass(Bootstrap::Label::Success);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
    }



    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Right);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::tarih)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }
    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Left);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::saat)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }




    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::mahalle)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::tamAdress)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        container->setContentAlignment(AlignmentFlag::Justify);
        container->addWidget(cpp14::make_unique<WText>("<h4><b>Konu:</b></h4>"));
        container->addWidget(cpp14::make_unique<WBreak>());

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::konu)->get_utf8().value.to_string()));
    }
}

void SikayetItemWidget::initContent()
{

    this->Content()->clear();


    if( mCurrentSikayet->keyList().contains(Sikayet::KEY::asama.c_str()) )
    {
        try {
            auto array = mCurrentSikayet->Element(Sikayet::KEY::asama).value().get_array().value;
            for( auto _item : array )
            {
                this->Content()->addWidget(cpp14::make_unique<AsamaItemWidget>(_item.get_document().value));
            }
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
            std::cout << str << std::endl;
        }
    }






}

void SikayetItemWidget::initController()
{
    this->Footer()->clear();
    this->Footer()->setMargin(25,Side::Top);

    this->Footer()->setPadding(10,AllSides);

    Footer()->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    Footer()->setWidth(WLength("100%"));
    Footer()->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),
                                                                            this->getRandom(170,190),
                                                                            this->getRandom(190,210)));



    {
        auto container = Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12+
                                 Bootstrap::ImageShape::img_thumbnail+
                                 Bootstrap::ContextualBackGround::bg_info);
        container->setMargin(15,Side::Top|Side::Bottom);

        auto rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto text = rContainer->addWidget(cpp14::make_unique<WText>("Birim Değiştir"));
            text->addStyleClass(Bootstrap::Grid::col_full_12);
            text->setAttributeValue(Style::style,Style::font::size::s12px);
        }

        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_10+
                                      Bootstrap::Grid::Medium::col_md_10+
                                      Bootstrap::Grid::Small::col_sm_10+
                                      Bootstrap::Grid::ExtraSmall::col_xs_9);
            auto comboBox = _container->addWidget(cpp14::make_unique<WComboBox>());

            auto filter = document{};

            try {
                filter.append(kvp("Haberleşme Kodu",make_document(kvp("$ne","0"))));
            } catch (bsoncxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }

            try {
                auto cursor = this->db()->collection("Müdürlükler").find(filter.view());

                for( auto doc : cursor )
                {

                    try {
                        comboBox->addItem(doc["Birim"].get_utf8().value.to_string());
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                }

            } catch (mongocxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }


        }

        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto setBirimBtn = _container->addWidget(cpp14::make_unique<WPushButton>("Değiştir"));
            setBirimBtn->addStyleClass(Bootstrap::Button::Primary);
        }





        Footer()->addWidget(cpp14::make_unique<WBreak>());
    }


    {
        auto container = Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12+
                                 Bootstrap::ImageShape::img_thumbnail+
                                 Bootstrap::ContextualBackGround::bg_info);
        container->setMargin(15,Side::Top|Side::Bottom);

        auto rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto text = rContainer->addWidget(cpp14::make_unique<WText>("Kategori Değiştir"));
            text->addStyleClass(Bootstrap::Grid::col_full_12);
            text->setAttributeValue(Style::style,Style::font::size::s12px);
        }

        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_10+
                                      Bootstrap::Grid::Medium::col_md_10+
                                      Bootstrap::Grid::Small::col_sm_10+
                                      Bootstrap::Grid::ExtraSmall::col_xs_9);
            auto comboBox = _container->addWidget(cpp14::make_unique<WComboBox>());

            auto filter = document{};

            try {
                auto cursor = this->db()->collection("SikayetKategorileri").find(filter.view());

                for( auto doc : cursor )
                {

                    try {
                        comboBox->addItem(doc["baslik"].get_utf8().value.to_string());
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                }

            } catch (mongocxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }
        }
        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto setKategoriBtn = _container->addWidget(cpp14::make_unique<WPushButton>("Değiştir"));
            setKategoriBtn->addStyleClass(Bootstrap::Button::Primary);
        }


        Footer()->addWidget(cpp14::make_unique<WBreak>());
    }



    {
        auto container = Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12+
                                 Bootstrap::ImageShape::img_thumbnail+
                                 Bootstrap::ContextualBackGround::bg_info);
        container->setMargin(15,Side::Top|Side::Bottom);

        auto rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto text = rContainer->addWidget(cpp14::make_unique<WText>("Durum Değiştir"));
            text->addStyleClass(Bootstrap::Grid::col_full_12);
            text->setAttributeValue(Style::style,Style::font::size::s12px);
        }

        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_10+
                                      Bootstrap::Grid::Medium::col_md_10+
                                      Bootstrap::Grid::Small::col_sm_10+
                                      Bootstrap::Grid::ExtraSmall::col_xs_9);

            auto comboBox = _container->addWidget(cpp14::make_unique<WComboBox>());
            comboBox->addItem(Sikayet::KEY::DURUM::beklemede);
            comboBox->addItem(Sikayet::KEY::DURUM::tekrarAcildi);
            comboBox->addItem(Sikayet::KEY::DURUM::iptalEdildi);
            comboBox->addItem(Sikayet::KEY::DURUM::teyitEdilmemis);
            comboBox->addItem(Sikayet::KEY::DURUM::tamamlandi);
            comboBox->addItem(Sikayet::KEY::DURUM::devamEdiyor);
        }

        {
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto setBirimBtn = _container->addWidget(cpp14::make_unique<WPushButton>("Değiştir"));
            setBirimBtn->addStyleClass(Bootstrap::Button::Primary);
        }


        Footer()->addWidget(cpp14::make_unique<WBreak>());
    }



    {
        auto text = Footer()->addWidget(cpp14::make_unique<WText>("Açıklama Ekle"));
        text->addStyleClass(Bootstrap::Label::info);
        text->setAttributeValue(Style::style,Style::font::size::s12px);
    }




    Footer()->addWidget(cpp14::make_unique<WBreak>());
    auto text = this->Footer()->addWidget(cpp14::make_unique<WTextEdit>());
    text->setWidth(WLength("100%"));
    text->setHeight(250);


    Footer()->addWidget(cpp14::make_unique<WBreak>());
    auto PushBtn = Footer()->addWidget(cpp14::make_unique<WPushButton>("Ekle"));
    PushBtn->addStyleClass(Bootstrap::Button::Primary);
}

AsamaItemWidget::AsamaItemWidget(bsoncxx::document::view &&view)
    :Sikayet::ASAMA (view)
{

    addStyleClass(Bootstrap::Grid::col_full_12);
    addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    setMargin(10,Side::Top);
    setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),this->getRandom(170,190),this->getRandom(190,210)));

    addWidget(cpp14::make_unique<WText>(tarih()));
    addWidget(cpp14::make_unique<WBreak>());
    addWidget(cpp14::make_unique<WText>(saat()));

    if( tip() == Sikayet::KEY::ASAMAKEY::TIP::aciklama )
    {
        addWidget(cpp14::make_unique<WBreak>());
        addWidget(cpp14::make_unique<WText>(aciklama()));
    }else{
        addWidget(cpp14::make_unique<WBreak>());
        addWidget(cpp14::make_unique<WText>(degisiklik()));
    }

    {
        addWidget(cpp14::make_unique<WBreak>());
        auto birimText = addWidget(cpp14::make_unique<WText>(birim()));
        birimText->addStyleClass(Bootstrap::Label::Primary);
        birimText->setAttributeValue(Style::style,Style::font::size::s12px);
    }

    {
        addWidget(cpp14::make_unique<WBreak>());
        auto birimText = addWidget(cpp14::make_unique<WText>(personelAdi()));
        birimText->addStyleClass(Bootstrap::Label::Success);
        birimText->setAttributeValue(Style::style,Style::font::size::s10px);
    }

}
