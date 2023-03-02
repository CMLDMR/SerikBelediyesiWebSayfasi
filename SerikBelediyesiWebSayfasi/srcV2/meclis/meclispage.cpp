#include "meclispage.h"


#include "meclisitempage.h"
#include "meclisuyesipage.h"

#include <QTime>



v2::MeclisPageManager::MeclisPageManager(SerikBLDCore::DB *db)
    :ContainerWidget ("Meclis Yönetimi") , SerikBLDCore::Meclis::MeclisManager (db)
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
            item.append (SerikBLDCore::Meclis::Key::julianDate,-1);
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
            this->Content ()->clear ();
            auto newItem = this->Content ()->addWidget (cpp14::make_unique<v2::MeclisYonetimPage>(this->getDB ()));
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
            this->Content ()->clear ();

            this->Content()->addNew<MeclisCanliYayin>(this->getDB());
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


    std::int64_t year = mlist->first().yil();
    std::string _color;

    auto contentContainer = this->Content()->addNew<WContainerWidget>();
    contentContainer->addStyleClass (Bootstrap::Grid::col_full_12);

    for( auto item : *mlist )
    {


        if( year != item.yil () )
        {
//            _color = Style::background::color::rgb (this->getRandom (240,255),this->getRandom (240,255),this->getRandom (240,255));
//            container->setMargin (25,Side::Top);
            year = item.yil ();

            contentContainer = this->Content()->addNew<WContainerWidget>();
            contentContainer->addStyleClass (Bootstrap::Grid::col_full_12);
//            contentContainer->setAttributeValue (Style::style,_color);
            contentContainer->setMargin (25,Side::Top);
            contentContainer->setPadding(5,Side::Top|Side::Bottom);

            _color = Style::background::color::rgb (this->getRandom (200,225),this->getRandom (200,225),this->getRandom (200,225));

            auto containerYear = contentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            containerYear->addStyleClass (Bootstrap::Grid::col_full_12);
            containerYear->addNew<WText>(WString("<b>{1}</b>").arg(item.yil()));
            containerYear->setContentAlignment(AlignmentFlag::Center);

        }

        auto container = contentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        if( WDate::currentDate().year() == year ){
            container->addStyleClass (Bootstrap::Grid::col_full_12);

        }else{
            container->addStyleClass (Bootstrap::Grid::col_full_6);

        }
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
        container->setAttributeValue (Style::style,_color);

        container->clicked ().connect ([=](){
            this->initMeclisPage (item);
        });

//        auto _Container = container->addWidget (cpp14::make_unique<WContainerWidget>());
//        _Container->addStyleClass (Bootstrap::Grid::row);
//        _Container->setMargin (5,Side::Top|Side::Bottom);


//        {
//            auto __container = _Container->addWidget (cpp14::make_unique<WContainerWidget>());
//            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
//                                        Bootstrap::Grid::Medium::col_md_2+
//                                        Bootstrap::Grid::Small::col_sm_2+
//                                        Bootstrap::Grid::ExtraSmall::col_xs_6);
//            __container->addWidget (cpp14::make_unique<WText>("<b>"+std::to_string (item.yil ())+"</b>"));
//        }

        {
            auto __container = container->addWidget (cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_4+
                                        Bootstrap::Grid::ExtraSmall::col_xs_8);
            auto meclisAdi = item.ay().toStdString();
            if( meclisAdi.size() >18 ){
                meclisAdi = meclisAdi.substr(0,17);
                meclisAdi += "...";
            }
            __container->addWidget (cpp14::make_unique<WText>(meclisAdi));
        }



        {
            auto __container = container->addWidget (cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                        Bootstrap::Grid::Medium::col_md_3+
                                        Bootstrap::Grid::Small::col_sm_3+
                                        Bootstrap::Grid::ExtraSmall::col_xs_5);
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


        {
            auto __container = container->addWidget (cpp14::make_unique<WContainerWidget>());
            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                        Bootstrap::Grid::Medium::col_md_5+
                                        Bootstrap::Grid::Small::col_sm_5+
                                        Bootstrap::Grid::ExtraSmall::col_xs_11);
            __container->addWidget (cpp14::make_unique<WText>(QDate::fromJulianDay (item.julianDay ()).toString ("dd/MM/yyyy ddd").toStdString () + " " + QTime::fromMSecsSinceStartOfDay (item.saat ()).toString ("hh:mm").toStdString ()));
        }


//        {
//            auto __container = _Container->addWidget (cpp14::make_unique<WContainerWidget>());
//            __container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
//                                        Bootstrap::Grid::Medium::col_md_1+
//                                        Bootstrap::Grid::Small::col_sm_1+
//                                        Bootstrap::Grid::ExtraSmall::col_xs_2);
//            __container->addWidget (cpp14::make_unique<WText>(QTime::fromMSecsSinceStartOfDay (item.saat ()).toString ("hh:mm").toStdString ()));
//        }

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
        item.setJulianDay(newItem->julianDay());
        auto count = this->countItem (item);
        if( count )
        {
            this->showMessage ("Uyarı","Bu Meclis Kayıtlı\nBaşka Bir Meclis Seçiniz");
        }else{
            item.setYayinda (newItem->yayinda ());
            item.setGundem (newItem->gundem());
            item.setJulianDay (newItem->julianDay ());
            item.setSaat (newItem->saat ());
            if( this->InsertItem (item).size () ){
                SerikBLDCore::FindOptions options;
                options.setLimit (1000);
                options.setSkip (0);
                SerikBLDCore::Item item("none");
                item.append(SerikBLDCore::Meclis::Key::yil,-1);
                options.setSort (item);
                this->UpdateList (SerikBLDCore::Meclis::MeclisItem(),options);
                this->showMessage ("Bilgi","Meclis Başarılı Bir Şekilde Eklendi.");
            }
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
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
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
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("Tarih"),1,AlignmentFlag::Center);
        mMeclisTarihi = vLayout->addWidget (cpp14::make_unique<WDateEdit>(),1,AlignmentFlag::Center);

    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("Saat"),1,AlignmentFlag::Center);
        mMeclisSaat = vLayout->addWidget (cpp14::make_unique<WTimeEdit>(),1,AlignmentFlag::Center);
        mMeclisSaat->setFormat ("hh:mm");
        mMeclisSaat->setTime (WTime::currentServerTime ());

    }

//    //WTimerWidget

//    {
//        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass (Bootstrap::Grid::col_full_12);

//        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

//        vLayout->addWidget (cpp14::make_unique<WText>("Meclis Tarihi"),1,AlignmentFlag::Center);
//        mGundemEdit = vLayout->addWidget (cpp14::make_unique<WTextEdit>(),1,AlignmentFlag::Justify);
//        mGundemEdit->setHeight (250);

//    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        auto btn = vLayout->addWidget (cpp14::make_unique<WPushButton>("Kaydet"),1,AlignmentFlag::Center);
        btn->clicked ().connect ([&](){
            this->setAy (ayTextEdit->text ().toUTF8 ().c_str ());
            this->setYil (yilSpinBox->value ());
            this->setYayinda (onlineCheckBox->isChecked ());
//            this->setGundem (mGundemEdit->text ().toUTF8 ().c_str ());
            if( mMeclisTarihi->date ().isValid ( ) )
            {
                this->setJulianDay (mMeclisTarihi->date ().toJulianDay ());

                if( mMeclisSaat->time ().isValid () )
                {
                    this->setSaat (QTime( mMeclisSaat->time ().hour (),
                                          mMeclisSaat->time ().minute () ).msecsSinceStartOfDay ());
                    _ClickSaved.emit (NoClass());
                }else{
                    this->showMessage ("Hata","Saat Hatalı");
                }
            }else{
                this->showMessage ("Hata","Tarih Hatalı");
            }
        });
        btn->addStyleClass (Bootstrap::Button::Primary);
    }

}

Signal<NoClass> &v2::MeclisNewItemPage::ClickSaved()
{
    return _ClickSaved;
}

v2::MeclisCanliYayin::MeclisCanliYayin(SerikBLDCore::DB *_db)
    :ContainerWidget("Canlı Yayın Bilgileri"),mDb(_db)
{

    this->addStyleClass(Bootstrap::Grid::col_full_12);

    this->loadInterface();

}

void v2::MeclisCanliYayin::loadInterface()
{

    this->Content()->clear();
    this->Footer()->clear();

    SerikBLDCore::Item filter("mecliscanli");

    auto canliitem = this->mDb->findOneItem(filter);

    if( canliitem ){


        try {
            currentOid = canliitem.value().view()["_id"].get_oid().value.to_string();
        } catch (bsoncxx::exception &e) {
            this->Footer()->addNew<WText>(WString("Oid Error: {1}").arg(e.what()));
            this->Footer()->addNew<WBreak>();
            currentOid = "";
        }

        mCanliYayinLink = this->Content()->addNew<WLineEdit>();
        mCanliYayinLink->setPlaceholderText("Sadece Video ID");
        mCanliYayinLink->addStyleClass(Bootstrap::Grid::col_full_12);



        mCanliYayinAktif = this->Content()->addNew<WCheckBox>();
        mCanliYayinAktif->setText("Yayın Aktif");
        mCanliYayinAktif->addStyleClass(Bootstrap::Grid::col_full_12);

        try {
            auto aktif = canliitem.value().view()["online"].get_bool().value;
            mCanliYayinAktif->setChecked(aktif);
            mCanliYayinAktif->setText(aktif ? "Yayın Aktif" : "Yayın Kapalı");

        } catch (bsoncxx::exception &e) {
            this->Footer()->addNew<WText>(WString("Yayın Aktif Error: {1}").arg(e.what()));
            this->Footer()->addNew<WBreak>();
        }

        mCanliYayinAktif->changed().connect([=](){
            this->updateInterface();
        });



        mCanliYayinBaslik = this->Content()->addNew<WLineEdit>();
        mCanliYayinBaslik->setPlaceholderText("Yayın Başlığı");
        mCanliYayinBaslik->addStyleClass(Bootstrap::Grid::col_full_12);

        auto guncelleBtn = this->Content()->addNew<WPushButton>("Güncelle");
        guncelleBtn->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::Button::Primary);



    }

}

void v2::MeclisCanliYayin::updateInterface()
{

    if( currentOid.empty() ) return;

    SerikBLDCore::Item filter("mecliscanli");

    filter.setOid(currentOid);

    filter.append("online",mCanliYayinAktif->isChecked());
    filter.append("link","https://www.youtube.com/embed/"+mCanliYayinLink->text().toUTF8());
    filter.append("title",mCanliYayinBaslik->text().toUTF8());

    std::cout << bsoncxx::to_json(filter.view()) << "\n";


    auto upt = this->mDb->updateItem(filter);

    if( upt ){
        if( upt.value().modified_count() ){
            if( mCanliYayinAktif->isChecked() ){
                this->showPopUpMessage("Canlı Yayın Başlatıldı");
            }else{
                this->showPopUpMessage("Canlı Yayın Kapatıldı");
            }
            this->loadInterface();
        }
    }else{
        this->showPopUpMessage("Update Yapılamadı");
    }





}
