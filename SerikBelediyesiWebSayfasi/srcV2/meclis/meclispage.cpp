#include "meclispage.h"


#include "meclisitempage.h"




v2::MeclisPageManager::MeclisPageManager(SerikBLDCore::DB *db)
    :ContainerWidget ("Meclis Kararlar - Gündem") , SerikBLDCore::Meclis::MeclisManager (db)
{
    this->initController ();

    this->Content ()->addStyleClass ("boxShadow");
    this->Content ()->setMargin (25,Side::Top|Side::Bottom);

    this->Footer ()->addStyleClass ("boxShadow");
}

void v2::MeclisPageManager::initController()
{

    // YENI MECLIS
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                      Style::color::color (Style::color::Grey::Black)+
                                      Style::font::size::s12px);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget (cpp14::make_unique<WText>("<b>Yeni Meclis</b>"));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&v2::MeclisPageManager::yeniEkle);
    }


    // Listele
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                      Style::color::color (Style::color::Grey::Black)+
                                      Style::font::size::s12px);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget (cpp14::make_unique<WText>("<b>Listele</b>"));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ([&](){
            SerikBLDCore::FindOptions options;
            options.setLimit (1000);
            options.setSkip (0);
            SerikBLDCore::Item item("none");
            item.append(SerikBLDCore::Meclis::Key::yil,-1);
            options.setSort (item);
            this->UpdateList (SerikBLDCore::Meclis::MeclisItem(),options);
        });
    }


    // Meclis Üyeleri
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                      Style::color::color (Style::color::Grey::Black)+
                                      Style::font::size::s12px);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget (cpp14::make_unique<WText>("<b>Meclis Üyeleri</b>"));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ([&](){

        });
    }


    // Canlı Yayın
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                      Style::color::color (Style::color::Grey::Black)+
                                      Style::font::size::s12px);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget (cpp14::make_unique<WText>("<b>Canlı Yayın</b>"));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ([&](){

        });
    }
}

void v2::MeclisPageManager::onList(const QVector<SerikBLDCore::Meclis::MeclisItem> *mlist)
{



    this->Content ()->clear ();
    this->Content ()->removeStyleClass ("boxShadow");

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("<b>Meclisler</b>"));
        container->addStyleClass (Bootstrap::Grid::col_full_12);
    }


    std::int64_t year = 0;
    std::string _color;
    for( auto item : *mlist )
    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());

        container->clicked ().connect ([=](){
            this->initMeclisPage (item);
        });

        auto _Container = container->addWidget (cpp14::make_unique<WContainerWidget>());
        _Container->addStyleClass (Bootstrap::Grid::row);
        _Container->setMargin (5,Side::Top|Side::Bottom);

        if( year != item.yil () )
        {
            _color = Style::background::color::rgb (this->getRandom (200,225),this->getRandom (200,225),this->getRandom (200,225));
            container->setMargin (25,Side::Top);
            year = item.yil ();
        }
        container->setAttributeValue (Style::style,_color);


        {
            auto __container = _Container->addWidget (cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_4+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6);
            __container->addWidget (cpp14::make_unique<WText>("<b>"+std::to_string (item.yil ())+"</b>"));
        }

        {
            auto __container = _Container->addWidget (cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_4+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6);
            __container->addWidget (cpp14::make_unique<WText>(item.ay ().toStdString ()));
        }



        {
            auto __container = _Container->addWidget (cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_4+
                                        Bootstrap::Grid::ExtraSmall::col_xs_12);
            auto text = __container->addWidget (cpp14::make_unique<WText>((item.yayinda () ? "Yayında" : "Yayında Değil")));
            if( !item.yayinda () )
            {
                text->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed)
                                         +Style::font::weight::bold);
            }else{
                text->setAttributeValue (Style::style,Style::color::color (Style::color::Green::DarkGreen)
                                         +Style::font::weight::bold);
            }
        }
    }

}

void v2::MeclisPageManager::yeniEkle()
{
    this->Content ()->clear ();

    auto newItem = this->Content ()->addWidget (cpp14::make_unique<MeclisNewItemPage>());

    newItem->ClickSaved ().connect ([=](){

        SerikBLDCore::Meclis::MeclisItem item;
        item.setAy (newItem->ay());
        item.setYil (newItem->yil ());
        auto count = this->countItem (item);
        if( count )
        {
            this->showMessage ("Uyarı","Bu Meclis Kayıtlı\nBaşka Bir Meclis Seçiniz");
        }else{
            item.setYayinda (newItem->yayinda ());
            item.setGundem (newItem->gundem());
            item.setJulianDay (WDate::currentDate ().toJulianDay ());
            this->InsertItem (item);
        }





    });



}

void v2::MeclisPageManager::initMeclisPage( const SerikBLDCore::Meclis::MeclisItem &meclisItem )
{
    this->Content ()->clear ();
    auto item = this->Content ()->addWidget (cpp14::make_unique<v2::MeclisItemPage>(this->getDB (),meclisItem));
    item->updateRequest ().connect (this,&v2::MeclisPageManager::updateMeclis );
}

void v2::MeclisPageManager::updateMeclis(const SerikBLDCore::Meclis::MeclisItem &item)
{
    SerikBLDCore::Meclis::MeclisItem __item;
    __item.setDocumentView (item.view ());
    if( this->UpdateItem (__item) ){
        this->initMeclisPage (__item);
    }else{
        this->showMessage ("Hata","Bilgiler Güncellenemedi");
    }
}

v2::MeclisNewItemPage::MeclisNewItemPage()
{

    setMargin (25,Side::Top|Side::Bottom);

    this->Header ()->addWidget (cpp14::make_unique<WText>("<b>Yeni Meclis Ekle</b>"));

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("Yıl Seç"),1,AlignmentFlag::Center);
        yilSpinBox = vLayout->addWidget (cpp14::make_unique<WSpinBox>(),1,AlignmentFlag::Center);
        yilSpinBox->setRange (2014,2024);
        yilSpinBox->setValue (WDate::currentDate ().year ());
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("Ay Seç"),1,AlignmentFlag::Center);
        ayTextEdit = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Center);
        ayTextEdit->setPlaceholderText ("Örnek: Ocak , Ağustos Olağanüstü");

    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("Yayında"),1,AlignmentFlag::Center);
        onlineCheckBox = vLayout->addWidget (cpp14::make_unique<WCheckBox>(),1,AlignmentFlag::Center);

    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("Gündem"),1,AlignmentFlag::Center);
        mGundemEdit = vLayout->addWidget (cpp14::make_unique<WTextEdit>(),1,AlignmentFlag::Justify);
        mGundemEdit->setHeight (250);

    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        auto btn = vLayout->addWidget (cpp14::make_unique<WPushButton>("Kaydet"),1,AlignmentFlag::Center);
        btn->clicked ().connect ([&](){
            this->setAy (ayTextEdit->text ().toUTF8 ().c_str ());
            this->setYil (yilSpinBox->value ());
            this->setYayinda (onlineCheckBox->isChecked ());
            this->setGundem (mGundemEdit->text ().toUTF8 ().c_str ());
            _ClickSaved.emit (NoClass());
        });
        btn->addStyleClass (Bootstrap::Button::Primary);
    }

}

Signal<NoClass> &v2::MeclisNewItemPage::ClickSaved()
{

    return _ClickSaved;

}
