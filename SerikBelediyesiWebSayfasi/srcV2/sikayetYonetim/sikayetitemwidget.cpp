#include "sikayetitemwidget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dialog.h"
#include "SerikBelediyesiWebSayfasi/baseWidget/tcitemwidget.h"

SikayetItemWidget::SikayetItemWidget(mongocxx::database *_db, const UserClass &userValue, const bsoncxx::oid &_oid)
    :SerikBLDCore::DB (_db),UserClass (userValue),mCurrentSikayet(Sikayet::SikayetItem::Load_Sikayet(_db,_oid).value())
{
    this->initHeader();
    this->initContent();
    this->initController();
}


void SikayetItemWidget::initHeader()
{
    Header()->clear();

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Right);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::adSoyad)->get_string().value.data()));
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


        auto tcitem = TC::TCItem::LoadByTC(this->db(),this->mCurrentSikayet->Element(Sikayet::KEY::talepSahibi)->get_string().value.data());

        if( tcitem )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(tcitem.value()->Element(TC::KEY::cepTelefonu)->get_string().value.data()));
            text->addStyleClass(Bootstrap::Label::Success);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }else{
            auto text = container->addWidget(cpp14::make_unique<WText>("!Telefon Bulunamadı!"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }

    }



    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Right);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::tarih)->get_string().value.data()));
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

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::saat)->get_string().value.data()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }



    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto _kategori = this->mCurrentSikayet->Element(Sikayet::KEY::kategori);
        if( _kategori )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(_kategori->get_string().value.data()));
            text->addStyleClass(Bootstrap::Label::info);
            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);


        }else{
            auto text = container->addWidget(cpp14::make_unique<WText>("......!Kategori Seçilmemiş!......"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
        }

    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto _birim = this->mCurrentSikayet->Element(Sikayet::KEY::birim);
        if( _birim )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(_birim->get_string().value.data()));
            text->addStyleClass(Bootstrap::Label::info);
            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);


        }else{
            auto text = container->addWidget(cpp14::make_unique<WText>("......!Birim Atanmamış!......"));
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
        }

    }



    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::mahalle)->get_string().value.data()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::tamAdress)->get_string().value.data()));
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

        container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::konu)->get_string().value.data()));
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



    /// BİRİM DEĞİŞTİR
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
                        comboBox->addItem(doc["Birim"].get_string().value.data());
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                }

            } catch (mongocxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }

            auto __container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                       Bootstrap::Grid::Medium::col_md_2+
                                       Bootstrap::Grid::Small::col_sm_2+
                                       Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto setBirimBtn = __container->addWidget(cpp14::make_unique<WPushButton>("Değiştir"));
            setBirimBtn->addStyleClass(Bootstrap::Button::Primary);

            setBirimBtn->clicked().connect([=](){

                std::cout << "setBirimBtn Clicked" << std::endl;

                if( mCurrentSikayet->addElement(Sikayet::KEY::birim,comboBox->currentText().toUTF8()) ){

                    this->initHeader();

                    auto pushDoc = document{};

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::tip_utf8,Sikayet::KEY::ASAMAKEY::TIP::degisiklik));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::degisim_utf8,comboBox->currentText().toUTF8()+" Müdürlüğüne Yönlendirildi."));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::saat_utf8,WTime::currentServerTime().toString("hh:mm").toUTF8()));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::tarih_utf8,WDate::currentServerDate().toString("dddd dd/MM/yyyy").toUTF8()));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    auto userBirim = this->User().birim();
                    if( userBirim )
                    {
                        try {
                            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::birim_utf8,userBirim.value()));
                        } catch (bsoncxx::exception &e) {
                            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                            std::cout << str << std::endl;
                        }
                    }

                    auto userAdSoyad = this->User().adSoyad();
                    if( userAdSoyad )
                    {
                        try {
                            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::personel_doc,make_document(kvp(Sikayet::KEY::ASAMAKEY::personel_doc_adsoyad,userAdSoyad.value()))));
                        } catch (bsoncxx::exception &e) {
                            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                            std::cout << str << std::endl;
                        }
                    }


                    if( mCurrentSikayet->pushElement(Sikayet::KEY::asama,pushDoc) )
                    {
                        if( mCurrentSikayet->reLoad() )
                        {
                            this->initHeader();
                            this->initContent();
                        }
                    }else{
                        std::cout << "Can not PushElement" << std::endl;
                    }

                }else{
                    std::cout << "Can not addElement" << std::endl;
                }




            });
        }

        Footer()->addWidget(cpp14::make_unique<WBreak>());
    }






    /// KATEGORI DEGĞİŞTİR
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
                        comboBox->addItem(doc["baslik"].get_string().value.data());
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                }

            } catch (mongocxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }

            auto __container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                       Bootstrap::Grid::Medium::col_md_2+
                                       Bootstrap::Grid::Small::col_sm_2+
                                       Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto setKategoriBtn = __container->addWidget(cpp14::make_unique<WPushButton>("Değiştir"));
            setKategoriBtn->addStyleClass(Bootstrap::Button::Primary);

            setKategoriBtn->clicked().connect([=](){
                if( mCurrentSikayet->addElement(Sikayet::KEY::kategori,comboBox->currentText().toUTF8()) ){
                    if( mCurrentSikayet->reLoad() )
                    {
                        this->initHeader();
                        this->initContent();
                    }
                }

            });
        }



        Footer()->addWidget(cpp14::make_unique<WBreak>());
    }



    /// DURUM DEĞİŞTİR
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

            auto __container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                       Bootstrap::Grid::Medium::col_md_2+
                                       Bootstrap::Grid::Small::col_sm_2+
                                       Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto setDurumBtn = __container->addWidget(cpp14::make_unique<WPushButton>("Değiştir"));
            setDurumBtn->addStyleClass(Bootstrap::Button::Primary);

            setDurumBtn->clicked().connect([=](){

                if( mCurrentSikayet->addElement(Sikayet::KEY::durum,comboBox->currentText().toUTF8()) ){

                    this->initContent();

                    auto pushDoc = document{};

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::tip_utf8,Sikayet::KEY::ASAMAKEY::TIP::degisiklik));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::degisim_utf8,"Yeni Durum: "+comboBox->currentText().toUTF8()));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::saat_utf8,WTime::currentServerTime().toString("hh:mm").toUTF8()));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    try {
                        pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::tarih_utf8,WDate::currentServerDate().toString("dd/MM/yyyy").toUTF8()));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    auto userBirim = this->User().birim();
                    if( userBirim )
                    {
                        try {
                            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::birim_utf8,userBirim.value()));
                        } catch (bsoncxx::exception &e) {
                            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                            std::cout << str << std::endl;
                        }
                    }

                    auto userAdSoyad = this->User().adSoyad();
                    if( userAdSoyad )
                    {
                        try {
                            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::personel_doc,make_document(kvp(Sikayet::KEY::ASAMAKEY::personel_doc_adsoyad,userAdSoyad.value()))));
                        } catch (bsoncxx::exception &e) {
                            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                            std::cout << str << std::endl;
                        }
                    }


                    if( mCurrentSikayet->pushElement(Sikayet::KEY::asama,pushDoc) )
                    {
                        if( mCurrentSikayet->reLoad() )
                        {
                            this->initHeader();
                            this->initContent();
                        }
                    }else{
                        std::cout << "Can not PushElement" << std::endl;
                    }


                }else{
                    std::cout << "Can not addElement" << std::endl;
                }

            });


        }



        Footer()->addWidget(cpp14::make_unique<WBreak>());
    }



    {
        auto text = Footer()->addWidget(cpp14::make_unique<WText>("Açıklama Ekle"));
        text->addStyleClass(Bootstrap::Label::info);
        text->setAttributeValue(Style::style,Style::font::size::s12px);
    }




    Footer()->addWidget(cpp14::make_unique<WBreak>());
    auto textEdit = this->Footer()->addWidget(cpp14::make_unique<WTextEdit>());
    textEdit->setWidth(WLength("100%"));
    textEdit->setHeight(250);


    Footer()->addWidget(cpp14::make_unique<WBreak>());
    auto PushBtn = Footer()->addWidget(cpp14::make_unique<WPushButton>("Ekle"));
    PushBtn->addStyleClass(Bootstrap::Button::Primary);

    PushBtn->clicked().connect([=](){

        this->initContent();

        auto pushDoc = document{};

        try {
            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::tip_utf8,Sikayet::KEY::ASAMAKEY::TIP::aciklama));
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
            std::cout << str << std::endl;
        }

        try {
            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::aciklama_utf8,textEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
            std::cout << str << std::endl;
        }

        try {
            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::saat_utf8,WTime::currentServerTime().toString("hh:mm").toUTF8()));
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
            std::cout << str << std::endl;
        }

        try {
            pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::tarih_utf8,WDate::currentServerDate().toString("dd/MM/yyyy").toUTF8()));
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
            std::cout << str << std::endl;
        }

        auto userBirim = this->User().birim();
        if( userBirim )
        {
            try {
                pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::birim_utf8,userBirim.value()));
            } catch (bsoncxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }
        }

        auto userAdSoyad = this->User().adSoyad();
        if( userAdSoyad )
        {
            try {
                pushDoc.append(kvp(Sikayet::KEY::ASAMAKEY::personel_doc,make_document(kvp(Sikayet::KEY::ASAMAKEY::personel_doc_adsoyad,userAdSoyad.value()))));
            } catch (bsoncxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }
        }


        if( mCurrentSikayet->pushElement(Sikayet::KEY::asama,pushDoc) )
        {
            if( mCurrentSikayet->reLoad() )
            {
                this->initHeader();
                this->initContent();
            }
        }else{
            std::cout << "Can not PushElement" << std::endl;
        }




    });
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

newSikayetItemWidget::newSikayetItemWidget(mongocxx::database *_db, const UserClass &userValue)
    :SerikBLDCore::DB (_db),UserClass (userValue)
{
    setWidth(WLength("100%"));
    addStyleClass(Bootstrap::Grid::row+Bootstrap::ImageShape::img_thumbnail);
    Header()->setMargin(15,Side::Top|Side::Bottom);
    setMargin(25,Side::Top|Side::Bottom);

    Header()->addWidget(cpp14::make_unique<WText>("<h4>Yeni Şikayet Oluştur</h4>"));
    Header()->addStyleClass(Bootstrap::ContextualBackGround::bg_success);

    {

        auto sContainer = Content()->addWidget(cpp14::make_unique<WContainerWidget>());
        sContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);

        auto mSikayetContainer = Content()->addWidget(cpp14::make_unique<WContainerWidget>());
        mSikayetContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                        Bootstrap::Grid::Medium::col_md_6+
                                        Bootstrap::Grid::Small::col_sm_6+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6+
                                         Bootstrap::ImageShape::img_thumbnail);

        auto rSorguContainer = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rSorguContainer->addStyleClass(Bootstrap::Grid::row);

        // Sorgu Container
        auto sorguContainer = rSorguContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        sorguContainer->addStyleClass(Bootstrap::Grid::col_full_12);


        // tcContainer
        auto tcContainer = rSorguContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        tcContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto hLayout = sorguContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto mSorguTipi = hLayout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Left);
        mSorguTipi->addItem("TCNO ile");
        mSorguTipi->addItem("Telefon ile");
        mSorguTipi->addItem("Ad Soyad ile");

        auto mSorguAlani = hLayout->addWidget(cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
        mSorguAlani->setPlaceholderText("TCNO Giriniz");
        mSorguTipi->activated().connect([=](int index){
            switch (index) {
            case 0:
                mSorguAlani->setPlaceholderText("TCNO Giriniz");
                break;
            case 1:
                mSorguAlani->setPlaceholderText("Telefon Numarası Giriniz");
                break;
            case 2:
                mSorguAlani->setPlaceholderText("Ad Soyad Giriniz ( en az 3 harf )");
                break;
            default:
                break;
            }
        });

        auto mSorguBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("Sorgula"),0,AlignmentFlag::Right);
        mSorguBtn->addStyleClass(Bootstrap::Button::Primary);

        mSorguBtn->clicked().connect([=](){

            tcContainer->clear();

            if( mSorguTipi->currentIndex() == 0 ){
                auto tcitem = TC::TCItem::LoadByTC(this->db(),mSorguAlani->text().toUTF8());


                if( tcitem )
                {
                    tcContainer->addWidget(cpp14::make_unique<TCItemWidget>(this->db(),this->UserValue(),tcitem.value()));
                }
            }else if (mSorguTipi->currentIndex() == 1 ) {
                auto tcitem = TC::TCItem::LoadByTel(this->db(),mSorguAlani->text().toUTF8());

                if( tcitem )
                {
                    tcContainer->addWidget(cpp14::make_unique<TCItemWidget>(this->db(),this->UserValue(),tcitem.value()));
                }
            }else{
                if( mSorguAlani->text().toUTF8().size() > 2 )
                {

                    auto mDialog = wApp->instance()->root()->addChild(cpp14::make_unique<Dialog>(this->db(),this->UserValue(),"Vatandaş Sorgula"));

                    mDialog->contents()->setContentAlignment(AlignmentFlag::Center);
                    auto rContainer = mDialog->contents()->addWidget(cpp14::make_unique<WContainerWidget>());
                    rContainer->addStyleClass(Bootstrap::Grid::row);
                    rContainer->setWidth(WLength("100%"));


                    auto filter = document{};

                    try {
                        filter.append(kvp(TC::KEY::adsoyad,make_document(kvp("$regex",mSorguAlani->text().toUTF8()),kvp("$options","i"))));
                    } catch (bsoncxx::exception &e) {
                        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                        std::cout << str << std::endl;
                    }

                    auto list = TC::TCItem::GetList<TC::TCItem>(this->db(),TC::collection,std::move(filter));

                    rContainer->setHeight(350);
                    rContainer->setOverflow(Overflow::Auto);

                    for( auto _item : list )
                    {
                        auto __item = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        __item->addStyleClass(Bootstrap::Grid::col_full_12);

                        auto _rContainer = __item->addWidget(cpp14::make_unique<WContainerWidget>());
                        _rContainer->addStyleClass(Bootstrap::Grid::row);
                        _rContainer->setWidth(WLength("100%"));

                        {
                            QString str = QString::fromStdString(_item->Element(TC::KEY::adsoyad)->get_string().value.data());
                            str.replace(QString::fromStdString(mSorguAlani->text().toUTF8()),"<mark><b>"+QString::fromStdString(mSorguAlani->text().toUTF8())+"</b></mark>",Qt::CaseInsensitive);
                            auto _adItem = _rContainer->addWidget(cpp14::make_unique<WText>(str.toStdString()));
                            _adItem->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                                   Bootstrap::Grid::Medium::col_md_6+
                                                   Bootstrap::Grid::Small::col_sm_6+
                                                   Bootstrap::Grid::ExtraSmall::col_xs_12);

                        }

                        {
                            auto _adItem = _rContainer->addWidget(cpp14::make_unique<WText>(_item->Element(TC::KEY::cepTelefonu)->get_string().value.data()));
                            _adItem->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                                   Bootstrap::Grid::Medium::col_md_6+
                                                   Bootstrap::Grid::Small::col_sm_6+
                                                   Bootstrap::Grid::ExtraSmall::col_xs_12);
                        }

                        _rContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
                        _rContainer->setMargin(5,Side::Top);
                        _rContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
                        _rContainer->decorationStyle().setCursor(Cursor::PointingHand);
                        _rContainer->setAttributeValue(Style::dataoid,_item->Element(TC::KEY::tcno)->get_string().value.data());

                        _rContainer->clicked().connect([=](){

                            auto tcitem = TC::TCItem::LoadByTC(this->db(),_rContainer->attributeValue(Style::dataoid).toUTF8());


                            if( tcitem )
                            {
                                tcContainer->addWidget(cpp14::make_unique<TCItemWidget>(this->db(),this->UserValue(),tcitem.value()));
                            }


                            wApp->instance()->root()->removeChild(mDialog);
                        });
                    }
                    mDialog->show();
                }
            }
        });
    }
}
