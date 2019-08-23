#include "sikayetyonetimwidget.h"
#include "sikayetlistitemwidget.h"
#include "sikayetitemwidget.h"
#include "SerikBelediyesiWebSayfasi/baseWidget/tcitemwidget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/dialog.h"


SikayetYonetimWidget::SikayetYonetimWidget(mongocxx::database *_db, bsoncxx::document::value &userValue)
    :UserClass (userValue),DBClass(_db)
{

    Header()->setMargin(10,Side::Bottom);

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<ContainerWiget>());
        container->setContainerStyle(ContainerStyleType::ROW);
        container->setContentAlignment(AlignmentFlag::Center);
        container->setWidth(WLength("100%"));
        auto text = container->addWidget(cpp14::make_unique<WText>("<h4>Şikayet Talep Yönetim Paneli</h4>"));
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<ContainerWiget>());
        container->setContainerStyle(ContainerStyleType::ROW);
        container->setContentAlignment(AlignmentFlag::Center);
        container->setMargin(15,Side::Top|Side::Bottom);
        container->setWidth(WLength("100%"));


        {
            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

            sContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                      Bootstrap::Grid::Medium::col_md_6+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6+
                                      Bootstrap::ImageShape::img_thumbnail);
            sContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
            sContainer->setContentAlignment(AlignmentFlag::Center);
            sContainer->decorationStyle().setCursor(Cursor::PointingHand);
            auto text = sContainer->addWidget(cpp14::make_unique<WText>("Sorgula"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));

            sContainer->clicked().connect(this,&SikayetYonetimWidget::Sorgula);
        }

        {
            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

            sContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                      Bootstrap::Grid::Medium::col_md_6+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6+
                                      Bootstrap::ImageShape::img_thumbnail);
            sContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
            sContainer->setContentAlignment(AlignmentFlag::Center);
            sContainer->decorationStyle().setCursor(Cursor::PointingHand);
            auto text = sContainer->addWidget(cpp14::make_unique<WText>("Yeni oluştur"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
            sContainer->clicked().connect(this,&SikayetYonetimWidget::createNewSikayet);
        }

        {
            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

            sContainer->addStyleClass(Bootstrap::Grid::col_full_12+
                                      Bootstrap::ImageShape::img_thumbnail);
            sContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
            sContainer->setContentAlignment(AlignmentFlag::Center);
            sContainer->decorationStyle().setCursor(Cursor::PointingHand);
            auto text = sContainer->addWidget(cpp14::make_unique<WText>("TC Kaydet"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
        }



    }


    // Kontrol Paneli
    {
        auto container = this->Header()->addWidget(cpp14::make_unique<ContainerWiget>());
        container->setContainerStyle(ContainerStyleType::ROW);

        mongocxx::pipeline pipline;

        pipline.group(make_document(kvp("_id","$Durum"),kvp("count",make_document(kvp("$sum",1)))));

        auto cursor = this->db()->collection(Sikayet::KEY::collection).aggregate(pipline);

        int Tumu = 0;
        for ( auto&& doc : cursor ) {

            auto _container = container->addWidget(cpp14::make_unique<ContainerWiget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            _container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            _container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,200),this->getRandom(170,220),this->getRandom(180,230)));
            _container->decorationStyle().setCursor(Cursor::PointingHand);
            //            _container->setHeight(60);

            auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());


            auto text1 = vLayout->addWidget(cpp14::make_unique<WText>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text1->setText(WString("{1}").arg(doc["_id"].get_utf8().value.to_string()));
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black)+
                                     Style::font::size::s14px+
                                     Style::font::family::tahoma+
                                     Style::font::weight::bold);

            {
                auto text = vLayout->addWidget(cpp14::make_unique<WText>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setText(WString("<h4>{1}</h4>").arg(Bootstrap::Badges::badget(std::to_string(doc["count"].get_int32().value))));
                Tumu += doc["count"].get_int32().value;
            }

            filterListCount[doc["_id"].get_utf8().value.to_string()] = doc["count"].get_int32().value;

            _container->clicked().connect([=](){
                skip = 0;
                this->initSikayetler(text1->text().toUTF8());
            });
        }
    }


}

void SikayetYonetimWidget::initSikayetler(const std::string &durumFilter)
{



    auto filter = document{};

    if( durumFilter != "Tümü" )
    {
        try {
            filter.append(kvp(Sikayet::KEY::durum,durumFilter));
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
            std::cout << str << std::endl;
        }
    }

    this->Content()->clear();
    this->Content()->setMargin(25,Side::Top|Side::Bottom);

    mongocxx::options::find findOptions;

    findOptions.limit(limit);
    findOptions.skip(skip);

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp("_id",-1));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    findOptions.sort(sortDoc.view());

    auto list = Sikayet::SikayetItem::GetList(this->db(),std::move(filter),findOptions);

    auto rContainer = this->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    int count = skip+1;
    for( auto item : list )
    {
        auto _Vdurum = item->Element(Sikayet::KEY::durum);
        auto _Vtarih = item->Element(Sikayet::KEY::tarih);
        auto _Vmahalle = item->Element(Sikayet::KEY::mahalle);
        auto _Vbirim = item->Element(Sikayet::KEY::birim);
        auto _Vkategori = item->Element(Sikayet::KEY::kategori);
        auto _VadSoyad = item->Element(Sikayet::KEY::adSoyad);

        std::string durum,tarih,mahalle,birim,kategori,adSoyad;
        if( _Vdurum ){ durum = std::to_string(count++)+ " " + _Vdurum.value().get_utf8().value.to_string();}
        if( _Vtarih ){ tarih = _Vtarih.value().get_utf8().value.to_string();}
        if( _Vmahalle ){ mahalle = _Vmahalle.value().get_utf8().value.to_string();}
        if( _Vbirim ){ birim = _Vbirim.value().get_utf8().value.to_string();}
        if( _Vkategori ){ kategori = _Vkategori.value().get_utf8().value.to_string();}
        if( _VadSoyad ){ adSoyad = _VadSoyad.value().get_utf8().value.to_string();}

        auto container = rContainer->addWidget(cpp14::make_unique<SikayetListItemWidget>(item->oid(),durum,
                                                                                         tarih,
                                                                                         mahalle,
                                                                                         birim,
                                                                                         kategori,
                                                                                         adSoyad));
        container->ClickItem().connect(this,&SikayetYonetimWidget::initSikayet);
        container->setMargin(5,Side::Top|Side::Bottom);

    }


    this->Footer()->clear();

    {
        auto container = this->Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto backBtn = container->addWidget(cpp14::make_unique<WPushButton>("Geri"));
        backBtn->addStyleClass(Bootstrap::Button::Primary);
        backBtn->clicked().connect([=](){
            if( skip >= limit )
            {
                skip -= limit;
                this->initSikayetler(durumFilter);
            }
        });

    }

    {
        auto container = this->Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto backBtn = container->addWidget(cpp14::make_unique<WText>("---"));
        backBtn->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
    }

    {
        auto container = this->Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto backBtn = container->addWidget(cpp14::make_unique<WPushButton>("İleri"));
        backBtn->addStyleClass(Bootstrap::Button::Primary);
        backBtn->clicked().connect([=](){
            if( skip < filterListCount[durumFilter] - limit )
            {
                skip += limit;
                this->initSikayetler(durumFilter);
            }
        });
    }


}

void SikayetYonetimWidget::initSikayetler( bsoncxx::builder::basic::document filter)
{



}

void SikayetYonetimWidget::initSikayetlerBySahibi(const std::string &sahibi)
{
    this->Content()->clear();
    this->Content()->setMargin(25,Side::Top|Side::Bottom);


    auto filter = document{};

    try {
        filter.append(kvp(Sikayet::KEY::talepSahibi,sahibi));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }


    mongocxx::options::find findOptions;

//    findOptions.limit(limit);
    skip = 0;
//    findOptions.skip(skip);

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp("_id",-1));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    findOptions.sort(sortDoc.view());


    auto list = Sikayet::SikayetItem::GetList(this->db(),std::move(filter),findOptions);

    auto rContainer = this->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    int count = skip+1;
    for( auto item : list )
    {
        auto _Vdurum = item->Element(Sikayet::KEY::durum);
        auto _Vtarih = item->Element(Sikayet::KEY::tarih);
        auto _Vmahalle = item->Element(Sikayet::KEY::mahalle);
        auto _Vbirim = item->Element(Sikayet::KEY::birim);
        auto _Vkategori = item->Element(Sikayet::KEY::kategori);
        auto _VadSoyad = item->Element(Sikayet::KEY::adSoyad);

        std::string durum,tarih,mahalle,birim,kategori,adSoyad;
        if( _Vdurum ){ durum = std::to_string(count++)+ " " + _Vdurum.value().get_utf8().value.to_string();}
        if( _Vtarih ){ tarih = _Vtarih.value().get_utf8().value.to_string();}
        if( _Vmahalle ){ mahalle = _Vmahalle.value().get_utf8().value.to_string();}
        if( _Vbirim ){ birim = _Vbirim.value().get_utf8().value.to_string();}
        if( _Vkategori ){ kategori = _Vkategori.value().get_utf8().value.to_string();}
        if( _VadSoyad ){ adSoyad = _VadSoyad.value().get_utf8().value.to_string();}

        auto container = rContainer->addWidget(cpp14::make_unique<SikayetListItemWidget>(item->oid(),durum,
                                                                                         tarih,
                                                                                         mahalle,
                                                                                         birim,
                                                                                         kategori,
                                                                                         adSoyad));
        container->ClickItem().connect(this,&SikayetYonetimWidget::initSikayet);
        container->setMargin(5,Side::Top|Side::Bottom);

    }



}

void SikayetYonetimWidget::initSikayet(const bsoncxx::oid &oid)
{

    this->Content()->clear();
    this->Footer()->clear();

    this->Content()->addWidget(cpp14::make_unique<SikayetItemWidget>(this->db(),this->User(),oid));



}

void SikayetYonetimWidget::createNewSikayet()
{
    this->Content()->clear();
    this->Footer()->clear();

    this->Content()->addWidget(cpp14::make_unique<newSikayetItemWidget>(this->db(),this->User()));

    this->Footer()->setContentAlignment(AlignmentFlag::Center);
    auto BackBtn = this->Footer()->addWidget(cpp14::make_unique<WText>("Sayfa Sonu"));
}

void SikayetYonetimWidget::Sorgula()
{

    auto mDialog = wApp->instance()->root()->addChild(cpp14::make_unique<Dialog>(this->db(),this->UserValue(),"Şikayet Sorgula"));
    //    mDialog->contents()->addWidget(cpp14::make_unique<TCItemWidget>(this->db(),this->UserValue()));

    mDialog->contents()->setContentAlignment(AlignmentFlag::Center);
    auto rContainer = mDialog->contents()->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setWidth(WLength("100%"));


    auto tContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                              Bootstrap::Grid::Medium::col_md_3+
                              Bootstrap::Grid::Small::col_sm_4+
                              Bootstrap::Grid::ExtraSmall::col_xs_4);

    auto tComboBox = tContainer->addWidget(cpp14::make_unique<WComboBox>());
    tComboBox->addItem("Telefon ile");
    tComboBox->addItem("TC ile");
    tComboBox->addItem("Ad Soyad ile");



    auto tContainer_ = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    tContainer_->addStyleClass(Bootstrap::Grid::Large::col_lg_7+
                              Bootstrap::Grid::Medium::col_md_7+
                              Bootstrap::Grid::Small::col_sm_7+
                              Bootstrap::Grid::ExtraSmall::col_xs_5);

    auto tLineEdit = tContainer_->addWidget(cpp14::make_unique<WLineEdit>());
    tLineEdit->setPlaceholderText("Cep Telefonu Numarasını Yazınız");

    tComboBox->activated().connect([=](int index){
        switch (index) {
        case 0:
            tLineEdit->setPlaceholderText("Cep Telefonu Numarasını Yazınız");
            break;
        case 1:
            tLineEdit->setPlaceholderText("TC Numarasını Yazınız");
            break;
        case 2:
            tLineEdit->setPlaceholderText("İsim Soyisim Yazınız ( en az 3 harf giriniz )");
            break;
        default:
            break;
        }

    });


    auto tContainer__ = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    tContainer__->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                              Bootstrap::Grid::Medium::col_md_3+
                              Bootstrap::Grid::Small::col_sm_3+
                              Bootstrap::Grid::ExtraSmall::col_xs_3);

    auto tSorgulaBtn = tContainer__->addWidget(cpp14::make_unique<WPushButton>("Sorgula"));

    tSorgulaBtn->addStyleClass(Bootstrap::Button::Primary);

    tSorgulaBtn->clicked().connect([=](){

        auto filter = document{};

        try {
            filter.append(kvp(TC::KEY::cepTelefonu,tLineEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
            std::cout << str << std::endl;
        }

        if( tComboBox->currentIndex() == 0 ){
            auto tcitem = TC::TCItem::LoadByTel(this->db(),tLineEdit->text().toUTF8());

            if( tcitem )
            {
                this->initSikayetlerBySahibi(tcitem.value()->Element(TC::KEY::tcno).value().get_utf8().value.to_string());
                wApp->instance()->root()->removeChild(mDialog);
            }
        }else if (tComboBox->currentIndex() == 1 ) {
            auto tcitem = TC::TCItem::LoadByTC(this->db(),tLineEdit->text().toUTF8());

            if( tcitem )
            {
                this->initSikayetlerBySahibi(tcitem.value()->Element(TC::KEY::tcno).value().get_utf8().value.to_string());
                wApp->instance()->root()->removeChild(mDialog);
            }
        }else{
            if( tLineEdit->text().toUTF8().size() > 2 )
            {
                auto filter = document{};

                try {
                    filter.append(kvp(TC::KEY::adsoyad,make_document(kvp("$regex",tLineEdit->text().toUTF8()),kvp("$options","i"))));
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
                        QString str = QString::fromStdString(_item->Element(TC::KEY::adsoyad)->get_utf8().value.to_string());
                        str.replace(QString::fromStdString(tLineEdit->text().toUTF8()),"<mark><b>"+QString::fromStdString(tLineEdit->text().toUTF8())+"</b></mark>",Qt::CaseInsensitive);
                        auto _adItem = _rContainer->addWidget(cpp14::make_unique<WText>(str.toStdString()));
                        _adItem->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                               Bootstrap::Grid::Medium::col_md_6+
                                               Bootstrap::Grid::Small::col_sm_6+
                                               Bootstrap::Grid::ExtraSmall::col_xs_12);

                    }

                    {
                        auto _adItem = _rContainer->addWidget(cpp14::make_unique<WText>(_item->Element(TC::KEY::cepTelefonu)->get_utf8().value.to_string()));
                        _adItem->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                               Bootstrap::Grid::Medium::col_md_6+
                                               Bootstrap::Grid::Small::col_sm_6+
                                               Bootstrap::Grid::ExtraSmall::col_xs_12);
                    }

                    _rContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
                    _rContainer->setMargin(5,Side::Top);
                    _rContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
                    _rContainer->decorationStyle().setCursor(Cursor::PointingHand);
                    _rContainer->setAttributeValue(Style::dataoid,_item->Element(TC::KEY::tcno)->get_utf8().value.to_string());

                    _rContainer->clicked().connect([=](){

                        this->initSikayetlerBySahibi(_rContainer->attributeValue(Style::dataoid).toUTF8());
                        wApp->instance()->root()->removeChild(mDialog);
                    });
                }
            }
        }


    });


    mDialog->show();

}
