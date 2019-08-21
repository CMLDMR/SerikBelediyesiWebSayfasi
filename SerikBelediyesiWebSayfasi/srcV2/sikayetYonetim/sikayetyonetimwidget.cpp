#include "sikayetyonetimwidget.h"
#include "sikayetlistitemwidget.h"
#include "sikayetitemwidget.h"

SikayetYonetimWidget::SikayetYonetimWidget(mongocxx::database *_db, bsoncxx::document::value &userValue)
    :UserClass (userValue),DBClass(_db)
{


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

    this->Content()->clear();
    this->Content()->setMargin(25,Side::Top|Side::Bottom);

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

void SikayetYonetimWidget::initSikayet(const bsoncxx::oid &oid)
{

    this->Content()->clear();
    this->Footer()->clear();

    this->Content()->addWidget(cpp14::make_unique<SikayetItemWidget>(this->db(),this->User(),oid));

    this->Footer()->setContentAlignment(AlignmentFlag::Center);
    auto BackBtn = this->Footer()->addWidget(cpp14::make_unique<WText>("Sayfa Sonu"));

}
