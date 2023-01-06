#include "meclisitempage.h"

#include <QTime>


v2::MeclisItemPage::MeclisItemPage(DB *_db, const MeclisItem &item )
    :ContainerWidget (std::to_string (item.yil ()) + " " + item.ay ().toStdString () + " Meclisi"),
      SerikBLDCore::Meclis::MeclisItem(item),
      SerikBLDCore::Meclis::KararManager (_db),
      SerikBLDCore::Meclis::YouTubeManager( _db ),
      SerikBLDCore::Meclis::RaporManager ( _db ),
      SerikBLDCore::Meclis::TeklifManager ( _db ),
      SerikBLDCore::Meclis::GundemManager ( _db )
{

    this->initMeclisBilgileri ();

    this->Content ()->setMargin (15,Side::Top);


    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mGundemContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mGundemContainer->addStyleClass (Bootstrap::Grid::row);


        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12);
            container->setContentAlignment (AlignmentFlag::Center);
            auto guncelleBtn = container->addWidget (cpp14::make_unique<WPushButton>("Gündem Ekle"));
            guncelleBtn->addStyleClass (Bootstrap::Button::Primary);
            guncelleBtn->clicked ().connect([=](){
                addGundem ();
            });
        }
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mRaporContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mRaporContainer->addStyleClass (Bootstrap::Grid::row);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mTeklifContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mTeklifContainer->addStyleClass (Bootstrap::Grid::row);
    }



    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mKararContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mKararContainer->addStyleClass (Bootstrap::Grid::row);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mLinkContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mLinkContainer->addStyleClass (Bootstrap::Grid::row);
    }



    this->initKararContoller ();





    SerikBLDCore::Meclis::KararManager::UpdateList (SerikBLDCore::Meclis::KararItem().setMeclisOid (this->oid ().value ().to_string ()));
    SerikBLDCore::Meclis::YouTubeManager::UpdateList (SerikBLDCore::Meclis::YouTubeLink().setMeclisOid (this->oid ().value ().to_string ()));
    SerikBLDCore::Meclis::RaporManager::UpdateList (SerikBLDCore::Meclis::RaporItem().setMeclisOid (this->oid ().value ().to_string ()));
    SerikBLDCore::Meclis::TeklifManager::UpdateList (SerikBLDCore::Meclis::TeklifItem().setMeclisOid (this->oid ().value ().to_string ()));

    {
        SerikBLDCore::FindOptions findOptions;
        findOptions.setLimit (1000);
        findOptions.setSort (SerikBLDCore::Item("none").append("_id",1));
        SerikBLDCore::Meclis::GundemManager::UpdateList (SerikBLDCore::Meclis::GundemItem().setMeclisOid (this->oid ().value ().to_string ()),findOptions);
    }



}

void v2::MeclisItemPage::onList(const QVector<SerikBLDCore::Meclis::KararItem> *mlist)
{
    if( !mKararContainer )
    {
        std::cout << "null mKararContainer\n";
        return;
    }

    mKararContainer->clear ();

    mKararContainer->setMargin (25,Side::Top);
    mKararContainer->addWidget (cpp14::make_unique<WText>("<b>Kararlar</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( const auto &item : *mlist )
    {
        {
            auto container = mKararContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_3+
                                      Bootstrap::Grid::ExtraSmall::col_xs_5);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            {
                auto fileLink = SerikBLDCore::Meclis::KararManager::downloadFileWeb (item.kararOid ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            std::to_string (item.sayi ()) + " Sayılı Karar");

                container->addWidget (std::move(anchor));
            }
        }

        {
            auto container = mKararContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                      Bootstrap::Grid::Medium::col_md_1+
                                      Bootstrap::Grid::Small::col_sm_1+
                                      Bootstrap::Grid::ExtraSmall::col_xs_1);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            container->setAttributeValue (Style::style,Style::background::color::rgba (255,0,0,1.0));
            auto SilText = container->addWidget (cpp14::make_unique<WText>("Sil"));
            SilText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            container->clicked ().connect ([=](){

                auto askConfirm = this->askConfirm("Silmek İstediğinize Eminmisiniz?");

                askConfirm->clicked().connect([=](){
                    SerikBLDCore::Meclis::KararItem _item;
                    _item.setOid (item.oid ().value ().to_string ());
                    if( !SerikBLDCore::Meclis::KararManager::deleteGridFS (item.kararOid ().c_str ()) ){
                        this->showMessage ("Uyarı","Karar Veri Tabanından Silinemedi");
                    }else{
                        SerikBLDCore::Meclis::KararManager::DeleteItem ( _item );
                        SerikBLDCore::Meclis::KararManager::UpdateList (SerikBLDCore::Meclis::KararItem().setMeclisOid (this->oid ().value ().to_string ()));
                        this->showMessage ("Bilgi","Karar Başarılı Bir Şekilde Silindi.");
                    }
                });



            });
        }
    }
}

void v2::MeclisItemPage::onList(const QVector<SerikBLDCore::Meclis::YouTubeLink> *mlist)
{
    if( !mLinkContainer )
    {
        std::cout << "mLink Container null \n";
        return;
    }
    mLinkContainer->clear ();

    mLinkContainer->setMargin (25,Side::Top);
    mLinkContainer->addWidget (cpp14::make_unique<WText>("<b>Videolar</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( const auto &item : *mlist )
    {
        auto imgContainer = mLinkContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        imgContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        imgContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_4+
                                     Bootstrap::Grid::ExtraSmall::col_xs_4);
        imgContainer->setMargin (5,Side::Top);
        imgContainer->setHeight (140);
        imgContainer->setAttributeValue (Style::style,Style::background::url (item.videothump0 ())+
                                         Style::background::size::cover+
                                         Style::background::position::center_center+
                                         Style::background::repeat::norepeat);

        auto vLayout = imgContainer->setLayout (cpp14::make_unique<WVBoxLayout>());

        {
            auto container = vLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Top|AlignmentFlag::Right);
            container->setWidth (WLength("100%"));
            container->setAttributeValue (Style::style,Style::background::color::rgba (255,0,0,1.0));
            container->setContentAlignment (AlignmentFlag::Right);
            auto silText = container->addWidget (cpp14::make_unique<WText>("<b>Sil</b>"));
            silText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            silText->decorationStyle ().setCursor (Cursor::PointingHand);
            silText->clicked ().connect ([=](){
                SerikBLDCore::Meclis::YouTubeLink _item;
                _item.setOid (item.oid ().value ().to_string ());
                SerikBLDCore::Meclis::YouTubeManager::DeleteItem ( _item );
                SerikBLDCore::Meclis::YouTubeManager::UpdateList (SerikBLDCore::Meclis::YouTubeLink().setMeclisOid (this->oid ().value ().to_string ()));
            });
        }

        vLayout->addStretch (1);





        {
            auto container = vLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Bottom|AlignmentFlag::Justify);
            container->setWidth (WLength("100%"));
            container->setAttributeValue (Style::style,Style::background::color::rgba (205,225,255,1.0));
            container->setContentAlignment (AlignmentFlag::Center);
            Wt::WLink link = Wt::WLink(item.videoUrl ());
            link.setTarget(Wt::LinkTarget::NewWindow);
            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,"Video");
            container->addWidget (std::move(anchor));
        }
    }
}

void v2::MeclisItemPage::onList(const QVector<SerikBLDCore::Meclis::RaporItem> *mlist)
{
    if( !mRaporContainer )
    {
        return;
    }

    mRaporContainer->clear ();

    mRaporContainer->setMargin (25,Side::Top);
    mRaporContainer->addWidget (cpp14::make_unique<WText>("<b>Raporlar</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( const auto &item : *mlist )
    {
        {
            auto container = mRaporContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                      Bootstrap::Grid::Medium::col_md_11+
                                      Bootstrap::Grid::Small::col_sm_11+
                                      Bootstrap::Grid::ExtraSmall::col_xs_10);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setContentAlignment (AlignmentFlag::Left);
            container->setMargin (5,Side::Top);
            {
                auto fileLink = SerikBLDCore::Meclis::RaporManager::downloadFileWeb (item.raporOid ().toStdString ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            item.raporAdi ().toStdString ());
                container->addWidget (std::move(anchor));
            }
        }

        {
            auto container = mRaporContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                      Bootstrap::Grid::Medium::col_md_1+
                                      Bootstrap::Grid::Small::col_sm_1+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            container->setAttributeValue (Style::style,Style::background::color::rgba (175,75,0,1.0));
            auto SilText = container->addWidget (cpp14::make_unique<WText>("Sil"));
            SilText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            container->clicked ().connect ([=](){

                auto askConfirm = this->askConfirm("Silmek İstediğinize Emin misiniz?");

                askConfirm->clicked().connect([=](){

                    SerikBLDCore::Meclis::RaporItem _item;
                    _item.setOid (item.oid ().value ().to_string ());
                    if( !SerikBLDCore::Meclis::RaporManager::deleteGridFS (item.raporOid ()) ){
                        this->showMessage ("Uyarı","Rapor Veri Tabanından Silinemedi");
                    }else{
                        SerikBLDCore::Meclis::RaporManager::DeleteItem ( _item );
                        SerikBLDCore::Meclis::RaporManager::UpdateList (SerikBLDCore::Meclis::RaporItem().setMeclisOid (this->oid ().value ().to_string ()));
                        this->showMessage ("Bilgi","Rapor Başarılı Bir Şekilde Silindi.");
                    }

                });




            });
        }
    }
}

void v2::MeclisItemPage::onList(const QVector<SerikBLDCore::Meclis::TeklifItem> *mlist)
{
    if( !mTeklifContainer )
    {
        return;
    }

    mTeklifContainer->clear ();

    mTeklifContainer->setMargin (25,Side::Top);
    mTeklifContainer->addWidget (cpp14::make_unique<WText>("<b>Teklifler</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( auto item : *mlist )
    {
        {
            auto container = mTeklifContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                      Bootstrap::Grid::Medium::col_md_11+
                                      Bootstrap::Grid::Small::col_sm_11+
                                      Bootstrap::Grid::ExtraSmall::col_xs_10);            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            container->setContentAlignment (AlignmentFlag::Left);
            {
                auto fileLink = SerikBLDCore::Meclis::TeklifManager::downloadFileWeb (item.teklifOid ().toStdString ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            item.teklifAdi ().toStdString ());
                container->addWidget (std::move(anchor));
            }
        }

        {
            auto container = mTeklifContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                      Bootstrap::Grid::Medium::col_md_1+
                                      Bootstrap::Grid::Small::col_sm_1+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            container->setAttributeValue (Style::style,Style::background::color::rgba (125,105,175,1.0));
            auto SilText = container->addWidget (cpp14::make_unique<WText>("Sil"));
            SilText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            container->clicked ().connect ([=](){

                auto askConfirm = this->askConfirm("Silmek İstediğinize Emin misiniz?");

                askConfirm->clicked().connect([=](){

                    SerikBLDCore::Meclis::TeklifItem _item;
                    _item.setOid (item.oid ().value ().to_string ());
                    if( !SerikBLDCore::Meclis::TeklifManager::deleteGridFS (item.teklifOid ()) ){
                        this->showMessage ("Uyarı","Teklif Veri Tabanından Silinemedi");
                    }else{
                        SerikBLDCore::Meclis::TeklifManager::DeleteItem ( _item );
                        SerikBLDCore::Meclis::TeklifManager::UpdateList (SerikBLDCore::Meclis::TeklifItem().setMeclisOid (this->oid ().value ().to_string ()));
                        this->showMessage ("Bilgi","Teklif Başarılı Bir Şekilde Silindi.");
                    }

                });

            });
        }
    }
}

void v2::MeclisItemPage::onList(const QVector<SerikBLDCore::Meclis::GundemItem> *mlist)
{
    if( !mGundemContainer )
    {
        return;
    }

    mGundemContainer->clear ();

    mGundemContainer->setMargin (25,Side::Top);
    mGundemContainer->addWidget (cpp14::make_unique<WText>("<b>Gündem Maddeleri</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( auto item : *mlist )
    {
        {
            auto container = mGundemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                      Bootstrap::Grid::Medium::col_md_11+
                                      Bootstrap::Grid::Small::col_sm_11+
                                      Bootstrap::Grid::ExtraSmall::col_xs_10);

            container->setMargin (5,Side::Top);
            container->setContentAlignment (AlignmentFlag::Left);
            if( item.gundemDosyasiVar () ){
                auto fileLink = SerikBLDCore::Meclis::GundemManager::downloadFileWeb (item.gundemDosyasi ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            item.gundemAdi ());
                container->addWidget (std::move(anchor));
                container->decorationStyle ().setCursor (Cursor::PointingHand);

            }else{
                container->addWidget (cpp14::make_unique<WText>(item.gundemAdi ()));
            }
        }

        {
            auto container = mGundemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                      Bootstrap::Grid::Medium::col_md_1+
                                      Bootstrap::Grid::Small::col_sm_1+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            container->setAttributeValue (Style::style,Style::background::color::rgba (105,145,175,1.0));
            auto SilText = container->addWidget (cpp14::make_unique<WText>("Sil"));
            SilText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));



            container->clicked ().connect ([=](){

                auto askConfirm = this->askConfirm("Silmek İstediğinizden Emin misiniz?");

                askConfirm->clicked().connect([=](){

                    SerikBLDCore::Meclis::GundemItem _item;
                    _item.setOid (item.oid ().value ().to_string ());
                    if( item.gundemDosyasiVar () )
                    {
                        if( !SerikBLDCore::Meclis::GundemManager::deleteGridFS (item.gundemDosyasi ().c_str ()) ){
                            this->showMessage ("Uyarı","Gündem Veri Tabanından Silinemedi");
                        }else{
                            SerikBLDCore::Meclis::GundemManager::DeleteItem ( _item );
                            SerikBLDCore::Meclis::GundemManager::UpdateList (SerikBLDCore::Meclis::GundemItem().setMeclisOid (this->oid ().value ().to_string ()));
                            this->showMessage ("Bilgi","Gündem Başarılı Bir Şekilde Silindi.");
                        }
                    }else{
                        SerikBLDCore::Meclis::GundemManager::DeleteItem ( _item );
                        SerikBLDCore::Meclis::GundemManager::UpdateList (SerikBLDCore::Meclis::GundemItem().setMeclisOid (this->oid ().value ().to_string ()));
                        this->showMessage ("Bilgi","Gündem Başarılı Bir Şekilde Silindi.");
                    }

                });

            });
        }
    }
}

void v2::MeclisItemPage::initMeclisBilgileri()
{
    this->Header ()->setMargin (25,Side::Top);


    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);
        WLink link = WLink(LinkType::Url,"?type=gundem&_id="+this->oid ().value ().to_string ());
        link.setTarget(Wt::LinkTarget::NewWindow);

        std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                    "<b>Meclis Linki</b>");
        container->addWidget (std::move(anchor));

        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        container->setMargin (25,Side::Bottom);
    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mYil = container->addWidget (cpp14::make_unique<WSpinBox>());
        mYil->setRange (2014,2024);
        mYil->setValue (this->yil ());
    }


    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mAyLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mAyLineEdit->setText (this->ay ().toStdString ());
        mAyLineEdit->setPlaceholderText ("Meclis Ayını: ( Ocak , Mart Olağanüstü )");
    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);
        mYayinda = container->addWidget (cpp14::make_unique<WCheckBox>("Yayında"));
        mYayinda->setChecked (this->yayinda ());
    }


    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin (5,Side::Top);
        mMeclisTarih = container->addWidget (cpp14::make_unique<WDateEdit>());
        mMeclisTarih->setDate (WDate::fromJulianDay (this->julianDay ()));
        mMeclisTarih->setFormat ("dd/MM/yyyy");
    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin (5,Side::Top);
        mMeclisSaat = container->addWidget (cpp14::make_unique<WTimeEdit>());
        mMeclisSaat->setTime (WTime(QTime::fromMSecsSinceStartOfDay (this->saat ()).hour (),
                                    QTime::fromMSecsSinceStartOfDay (this->saat ()).minute ()));
        mMeclisSaat->setFormat ("hh:mm");
    }


    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);
        auto guncelleBtn = container->addWidget (cpp14::make_unique<WPushButton>("Meclis Bilgilerini Güncelle"));
        guncelleBtn->addStyleClass (Bootstrap::Button::Primary);
        guncelleBtn->clicked ().connect([=](){
            this->setYayinda (mYayinda->isChecked ());
            this->setYil (mYil->value ());
            this->setAy (mAyLineEdit->text ().toUTF8 ().c_str ());

            if( mMeclisTarih->date ().isValid ( ) )
            {
                this->setJulianDay (mMeclisTarih->date ().toJulianDay ());
                if( mMeclisSaat->time ().isValid () )
                {
                    this->setSaat (QTime( mMeclisSaat->time ().hour (),
                                          mMeclisSaat->time ().minute () ).msecsSinceStartOfDay ());
                    _updateRequest.emit (*this);
                }else{
                    this->showMessage ("Hata","Saat Hatalı");
                }
            }else{
                this->showMessage ("Hata","Tarih Hatalı");
            }
        });
    }
}



void v2::MeclisItemPage::initKararContoller()
{
    this->Footer ()->clear ();
    this->Footer ()->setMargin (15,Side::Top);
    {
        auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setMargin (10,Side::Top);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->setContentAlignment (AlignmentFlag::Center);
        auto kararBtn = container->addWidget (cpp14::make_unique<WPushButton>("Karar Ekle"));
        kararBtn->addStyleClass (Bootstrap::Button::Primary);
        kararBtn->clicked ().connect ([&](){
            this->addKarar ();
        });
    }


    {
        auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setMargin (10,Side::Top);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->setContentAlignment (AlignmentFlag::Center);
        auto kararBtn = container->addWidget (cpp14::make_unique<WPushButton>("Rapor Ekle"));
        kararBtn->addStyleClass (Bootstrap::Button::Primary);
        kararBtn->clicked ().connect ([&](){
            this->addRapor ();
        });
    }


    {
        auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setMargin (10,Side::Top);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->setContentAlignment (AlignmentFlag::Center);
        auto kararBtn = container->addWidget (cpp14::make_unique<WPushButton>("Teklif Yazısı Ekle"));
        kararBtn->addStyleClass (Bootstrap::Button::Primary);
        kararBtn->clicked ().connect ([&](){
            this->addTeklif ();
        });
    }

    {
        auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setMargin (10,Side::Top);
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget (cpp14::make_unique<WText>("https://www.youtube.com/watch?v=<b>9uIk_91GQYI</b>",TextFormat::UnsafeXHTML));
    }


    {

        auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);
        auto videoLinkBtn = container->addWidget (cpp14::make_unique<WPushButton>("Youtube Linki Ekle"));
        videoLinkBtn->addStyleClass (Bootstrap::Button::Primary);
        videoLinkBtn->clicked ().connect ([&](){
            this->addYoutubeLink ();
        });
    }
}

void v2::MeclisItemPage::addGundem()
{

    auto mDialog = this->createDialog ("Gündem Ekle");

    mDialog->contents ()->addStyleClass (Bootstrap::Grid::row);
    mDialog->contents ()->setContentAlignment (AlignmentFlag::Center);

    auto __GundemAdi = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    __GundemAdi->setPlaceholderText ("Gündem Adını Giriniz");
    __GundemAdi->addStyleClass (Bootstrap::Grid::col_full_12);



    auto __contentWidget = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    __contentWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    auto __gundemOidText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>());
    __gundemOidText->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __KararOidWidget = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>());
    __KararOidWidget->setType (FileUploaderWidget::Pdf);
    __KararOidWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    __KararOidWidget->Uploaded ().connect ([=](){
        __gundemOidText->setText (__KararOidWidget->fileLocation ().toStdString ());
    });

    auto mKaydetBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    mKaydetBtn->addStyleClass (Bootstrap::Button::Primary);

    mKaydetBtn->clicked ().connect ([=](){

        SerikBLDCore::Meclis::GundemItem item;
        if( __gundemOidText->text ().toUTF8 ().size () != 0 )
        {
            auto kararOid__ = SerikBLDCore::Meclis::GundemManager::uploadfile (__gundemOidText->text ().toUTF8 ().c_str ());
            item.setGundemDosyasi (kararOid__.view().get_oid ().value);
        }
        item.setMeclisOid (this->oid ().value ().to_string ());
        item.setGundemAdi (__GundemAdi->text ().toUTF8 ());
        SerikBLDCore::Meclis::GundemManager::InsertItem (item);
        {
            SerikBLDCore::FindOptions findOptions;
            findOptions.setLimit (1000);
            findOptions.setSort (SerikBLDCore::Item("none").append("_id",1));
            SerikBLDCore::Meclis::GundemManager::UpdateList (SerikBLDCore::Meclis::GundemItem().setMeclisOid (this->oid ().value ().to_string ()),findOptions);
        }
        this->removeDialog (mDialog);
    });


}

void v2::MeclisItemPage::addKarar()
{
    auto mDialog = this->createDialog ("Karar Ekle");

    mDialog->contents ()->addStyleClass (Bootstrap::Grid::row);
    mDialog->contents ()->setContentAlignment (AlignmentFlag::Center);

    auto __SayiWidget = mDialog->contents ()->addWidget (cpp14::make_unique<WSpinBox>());
    __SayiWidget->setRange (0,9999);
    __SayiWidget->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __contentWidget = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    __contentWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    auto __kararOidText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>());
    __kararOidText->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __KararOidWidget = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>());
    __KararOidWidget->setType (FileUploaderWidget::Pdf);
    __KararOidWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    __KararOidWidget->Uploaded ().connect ([=](){
        __kararOidText->setText (__KararOidWidget->fileLocation ().toStdString ());
    });


    auto __infoText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>(""));
    __infoText->addStyleClass (Bootstrap::Grid::col_full_12);

    __SayiWidget->valueChanged ().connect ([=]( const int& sayi ){
        SerikBLDCore::Meclis::KararItem item;
        item.setSayi (sayi);
        item.setMeclisOid (this->oid ().value ().to_string ());


        if( SerikBLDCore::Meclis::KararManager::countItem (item) )
        {
            __infoText->setText ("<b>Hata: Bu Sayı Daha Önce Eklenmiş !</b>");
            __infoText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed));
        }else{
            __infoText->setText ("<b>Geçerli Sayı</b>");
            __infoText->setAttributeValue (Style::style,Style::color::color (Style::color::Green::SeaGreen));
        }
    });

    auto mKaydetBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    mKaydetBtn->addStyleClass (Bootstrap::Button::Primary);

    mKaydetBtn->clicked ().connect ([=](){

        if( __kararOidText->text ().toUTF8 ().size () == 0 )
        {
            __infoText->setText ("<b>Hata: Karar Dosyası Eklemediniz !</b>");
            __infoText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed));
            return;
        }


        SerikBLDCore::Meclis::KararItem item;
        item.setSayi (__SayiWidget->value ());
        item.setMeclisOid (this->oid ().value ().to_string ());


        if( SerikBLDCore::Meclis::KararManager::countItem (item) )
        {
            __infoText->setText ("Hata: Bu Sayı Daha Önce Eklenmiş !");
            __infoText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed));
        }else{
            auto kararOid__ = SerikBLDCore::Meclis::KararManager::uploadfile (__kararOidText->text ().toUTF8 ().c_str ());
            item.setKararOid (kararOid__.view().get_oid ().value.to_string ());
            item.setMeclisOid (this->oid ().value ().to_string ());
            SerikBLDCore::Meclis::KararManager::InsertItem (item);
            SerikBLDCore::Meclis::KararManager::UpdateList (SerikBLDCore::Meclis::KararItem().setMeclisOid (this->oid ().value ().to_string ()));
            this->removeDialog (mDialog);
        }





    });


}

void v2::MeclisItemPage::addYoutubeLink()
{


    auto mDialog = this->createDialog ("YouTube Videosu Ekle");

    mDialog->contents ()->addStyleClass (Bootstrap::Grid::row);
    mDialog->contents ()->setContentAlignment (AlignmentFlag::Center);




    auto __linkText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Link"));
    __linkText->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __link = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    __link->addStyleClass (Bootstrap::Grid::col_full_12);
    __link->setPlaceholderText ("Video ID Yazınız: ( acQS2Fef8tU )");




    auto imgContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    imgContainer->addStyleClass (Bootstrap::Grid::col_full_12);

    imgContainer->setHeight (150);

    __link->textInput ().connect ([=](){

        __linkText->setText ("Link: https://www.youtube.com/watch?v="+__link->text ().toUTF8 ());

        if( __link->text ().toUTF8 ().size () == 11 )
        {
            imgContainer->setAttributeValue (Style::style,Style::background::url ("https://img.youtube.com/vi/"+__link->text ().toUTF8 ()+"/0.jpg")+
                                             Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            imgContainer->setHeight (150);

        }else{
            imgContainer->setAttributeValue (Style::style,Style::background::url (""));
            imgContainer->setHeight (0);
        }

    });

    auto mKaydetBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    mKaydetBtn->addStyleClass (Bootstrap::Button::Primary);

    mKaydetBtn->clicked ().connect ([=](){
        if( __link->text ().toUTF8 ().size () == 0 )
        {
            this->showPopUpMessage ("Link Eklemediniz","hata");
            return;
        }
        SerikBLDCore::Meclis::YouTubeLink item;
        item.setVideoID (__link->text ().toUTF8 ());
        item.setMeclisOid (this->oid ().value ().to_string ());
        SerikBLDCore::Meclis::YouTubeManager::InsertItem (item);
        SerikBLDCore::Meclis::YouTubeManager::UpdateList (SerikBLDCore::Meclis::YouTubeLink().setMeclisOid (this->oid ().value ().to_string ()));
        this->removeDialog (mDialog);
    });


}

void v2::MeclisItemPage::addRapor()
{
    auto mDialog = this->createDialog ("Rapor Ekle");

    mDialog->contents ()->addStyleClass (Bootstrap::Grid::row);
    mDialog->contents ()->setContentAlignment (AlignmentFlag::Center);

    auto __RaporAdi = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    __RaporAdi->setPlaceholderText ("Rapor Adını Giriniz");
    __RaporAdi->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __RaporKimden = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    __RaporKimden->setPlaceholderText ("Kimden Geldiğini Giriniz");
    __RaporKimden->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __contentWidget = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    __contentWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    auto __kararOidText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>());
    __kararOidText->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __KararOidWidget = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>());
    __KararOidWidget->setType (FileUploaderWidget::Pdf);
    __KararOidWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    __KararOidWidget->Uploaded ().connect ([=](){
        __kararOidText->setText (__KararOidWidget->fileLocation ().toStdString ());
    });

    auto mKaydetBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    mKaydetBtn->addStyleClass (Bootstrap::Button::Primary);

    mKaydetBtn->clicked ().connect ([=](){

        if( __kararOidText->text ().toUTF8 ().size () == 0 )
        {
            this->showPopUpMessage ("Rapor Eklemediniz","hata");
            return;
        }



        SerikBLDCore::Meclis::RaporItem item;
        auto kararOid__ = SerikBLDCore::Meclis::RaporManager::uploadfile (__kararOidText->text ().toUTF8 ().c_str ());
        item.setRaporOid (kararOid__.view().get_oid ().value.to_string ());
        item.setMeclisOid (this->oid ().value ().to_string ());
        item.setRaporAdi (__RaporAdi->text ().toUTF8 ());
        item.setKimden (__RaporKimden->text ().toUTF8 ());
        SerikBLDCore::Meclis::RaporManager::InsertItem (item);
        SerikBLDCore::Meclis::RaporManager::UpdateList (SerikBLDCore::Meclis::RaporItem().setMeclisOid (this->oid ().value ().to_string ()));
        this->removeDialog (mDialog);
    });
}

void v2::MeclisItemPage::addTeklif()
{
    auto mDialog = this->createDialog ("Teklif Yazısı Ekle");

    mDialog->contents ()->addStyleClass (Bootstrap::Grid::row);
    mDialog->contents ()->setContentAlignment (AlignmentFlag::Center);

    auto __RaporAdi = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    __RaporAdi->setPlaceholderText ("Teklif Adını Giriniz");
    __RaporAdi->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __RaporKimden = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    __RaporKimden->setPlaceholderText ("Kimden Geldiğini Giriniz");
    __RaporKimden->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __contentWidget = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    __contentWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    auto __kararOidText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>());
    __kararOidText->addStyleClass (Bootstrap::Grid::col_full_12);


    auto __KararOidWidget = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>());
    __KararOidWidget->setType (FileUploaderWidget::Pdf);
    __KararOidWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    __KararOidWidget->Uploaded ().connect ([=](){
        __kararOidText->setText (__KararOidWidget->fileLocation ().toStdString ());
    });

    auto mKaydetBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    mKaydetBtn->addStyleClass (Bootstrap::Button::Primary);

    mKaydetBtn->clicked ().connect ([=](){
        if( __kararOidText->text ().toUTF8 ().size () == 0 )
        {
            this->showPopUpMessage ("Teklif Eklemediniz","hata");
            return;
        }
        SerikBLDCore::Meclis::TeklifItem item;
        auto kararOid__ = SerikBLDCore::Meclis::TeklifManager::uploadfile (__kararOidText->text ().toUTF8 ().c_str ());
        item.setTeklifOid (kararOid__.view().get_oid ().value.to_string ());
        item.setMeclisOid (this->oid ().value ().to_string ());
        item.setTeklifAdi (__RaporAdi->text ().toUTF8 ());
        item.setKimden (__RaporKimden->text ().toUTF8 ());
        SerikBLDCore::Meclis::TeklifManager::InsertItem (item);
        SerikBLDCore::Meclis::TeklifManager::UpdateList (SerikBLDCore::Meclis::TeklifItem().setMeclisOid (this->oid ().value ().to_string ()));
        this->removeDialog (mDialog);
    });
}

Signal<const SerikBLDCore::Meclis::MeclisItem &> &v2::MeclisItemPage::updateRequest()
{
    return _updateRequest;
}





v2::MeclisItemPublicPage::MeclisItemPublicPage(SerikBLDCore::DB *_db, const SerikBLDCore::Meclis::MeclisItem &item)
    :ContainerWidget ("Serik Belediyesi "+std::to_string (item.yil ()) + " " + item.ay ().toStdString () + " Meclisi"),
      SerikBLDCore::Meclis::MeclisItem(item),
      SerikBLDCore::Meclis::KararManager (_db),
      SerikBLDCore::Meclis::YouTubeManager( _db ),
      SerikBLDCore::Meclis::RaporManager ( _db ),
      SerikBLDCore::Meclis::TeklifManager ( _db ),
      SerikBLDCore::Meclis::GundemManager ( _db )
{

    this->initMeclisBilgileri ();

    this->Content ()->setMargin (15,Side::Top);

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mGundemContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mGundemContainer->addStyleClass (Bootstrap::Grid::row);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mRaporContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mRaporContainer->addStyleClass (Bootstrap::Grid::row);
        mRaporContainer->addStyleClass ("boxShadow");
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mTeklifContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mTeklifContainer->addStyleClass (Bootstrap::Grid::row);
        mTeklifContainer->addStyleClass ("boxShadow");
    }


    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mKararContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mKararContainer->addStyleClass (Bootstrap::Grid::row);
        mKararContainer->addStyleClass ("boxShadow");
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mLinkContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mLinkContainer->addStyleClass ("boxShadow");
        mLinkContainer->addStyleClass (Bootstrap::Grid::row);


    }

    {
        SerikBLDCore::FindOptions findOptions;
        findOptions.setLimit (1000);
        findOptions.setSort (SerikBLDCore::Item("none").append(SerikBLDCore::Meclis::KararKey::Sayi,1));
        SerikBLDCore::Meclis::KararManager::UpdateList (SerikBLDCore::Meclis::KararItem().setMeclisOid (this->oid ().value ().to_string ()),findOptions);
    }

    SerikBLDCore::Meclis::YouTubeManager::UpdateList (SerikBLDCore::Meclis::YouTubeLink().setMeclisOid (this->oid ().value ().to_string ()));
    SerikBLDCore::Meclis::RaporManager::UpdateList (SerikBLDCore::Meclis::RaporItem().setMeclisOid (this->oid ().value ().to_string ()));
    SerikBLDCore::Meclis::TeklifManager::UpdateList (SerikBLDCore::Meclis::TeklifItem().setMeclisOid (this->oid ().value ().to_string ()));

    {
        SerikBLDCore::FindOptions findOptions;
        findOptions.setLimit (1000);
        findOptions.setSort (SerikBLDCore::Item("none").append("_id",1));
        SerikBLDCore::Meclis::GundemManager::UpdateList (SerikBLDCore::Meclis::GundemItem().setMeclisOid (this->oid ().value ().to_string ()),findOptions);
    }



}

void v2::MeclisItemPublicPage::onList(const QVector<SerikBLDCore::Meclis::KararItem> *mlist)
{
    if( !mKararContainer )
    {
        return;
    }

    mKararContainer->clear ();

    mKararContainer->setMargin (25,Side::Top);
    mKararContainer->addWidget (cpp14::make_unique<WText>("<b>Kararlar</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);


    if( mlist->empty () )
    {
        auto container = mKararContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setMargin (5,Side::Top);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget(cpp14::make_unique<WText>("<b>Henüz Karar Eklenmemiş</b>"));
    }

    for( auto item : *mlist )
    {
        {
            auto container = mKararContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            {
                auto fileLink = SerikBLDCore::Meclis::KararManager::downloadFileWeb (item.kararOid ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            std::to_string (item.sayi ()) + " Sayılı Karar");
                container->addWidget (std::move(anchor));
            }
        }

    }
}

void v2::MeclisItemPublicPage::onList(const QVector<SerikBLDCore::Meclis::YouTubeLink> *mlist)
{
    if( !mLinkContainer )
    {
        return;
    }
    mLinkContainer->clear ();

    mLinkContainer->setMargin (25,Side::Top);
    mLinkContainer->setMargin (50,Side::Bottom);

    mLinkContainer->addWidget (cpp14::make_unique<WText>("<b>Videolar</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( auto item : *mlist )
    {
        auto imgContainer = mLinkContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        imgContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        imgContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_4+
                                     Bootstrap::Grid::ExtraSmall::col_xs_4);
        imgContainer->setMargin (5,Side::Top);
        imgContainer->setHeight (140);
        imgContainer->setAttributeValue (Style::style,Style::background::url (item.videothump0 ())+
                                         Style::background::size::cover+
                                         Style::background::position::center_center+
                                         Style::background::repeat::norepeat);

        auto vLayout = imgContainer->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch (1);

        {
            auto container = vLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Bottom|AlignmentFlag::Justify);
            container->setWidth (WLength("100%"));
            container->setAttributeValue (Style::style,Style::background::color::rgba (205,225,255,1.0));
            container->setContentAlignment (AlignmentFlag::Center);
            Wt::WLink link = Wt::WLink(item.videoUrl ());
            link.setTarget(Wt::LinkTarget::NewWindow);
            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,"Video");
            container->addWidget (std::move(anchor));
        }
    }
}

void v2::MeclisItemPublicPage::onList(const QVector<SerikBLDCore::Meclis::RaporItem> *mlist)
{
    if( !mRaporContainer )
    {
        return;
    }

    mRaporContainer->clear ();

    mRaporContainer->setMargin (25,Side::Top);
    mRaporContainer->addWidget (cpp14::make_unique<WText>("<b>Raporlar</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( const auto &item : *mlist )
    {
        {
            auto container = mRaporContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            //            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::col_full_12);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setMargin (5,Side::Top);
            container->setContentAlignment (AlignmentFlag::Left);
            {
                auto fileLink = SerikBLDCore::Meclis::RaporManager::downloadFileWeb (item.raporOid ().toStdString ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            item.raporAdi ().toStdString ());
                container->addWidget (std::move(anchor));
            }
        }

    }
}

void v2::MeclisItemPublicPage::onList(const QVector<SerikBLDCore::Meclis::TeklifItem> *mlist)
{
    if( !mTeklifContainer )
    {
        return;
    }

    mTeklifContainer->clear ();

    mTeklifContainer->setMargin (25,Side::Top);
    mTeklifContainer->addWidget (cpp14::make_unique<WText>("<b>Teklifler</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( auto item : *mlist )
    {
        {
            auto container = mTeklifContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            //            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::col_full_12);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setContentAlignment (AlignmentFlag::Left);
            container->setMargin (5,Side::Top);
            {
                auto fileLink = SerikBLDCore::Meclis::TeklifManager::downloadFileWeb (item.teklifOid ().toStdString ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            item.teklifAdi ().toStdString ());
                container->addWidget (std::move(anchor));
            }
        }

    }
}

void v2::MeclisItemPublicPage::onList(const QVector<SerikBLDCore::Meclis::GundemItem> *mlist)
{
    if( !mGundemContainer )
    {
        return;
    }

    mGundemContainer->clear ();

    mGundemContainer->setMargin (25,Side::Top);
    mGundemContainer->addWidget (cpp14::make_unique<WText>("<b>Gündem Maddeleri</b>",TextFormat::UnsafeXHTML))->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_info);

    for( auto item : *mlist )
    {
        {
            auto container = mGundemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_12+
                                      Bootstrap::Grid::Medium::col_md_12+
                                      Bootstrap::Grid::Small::col_sm_12+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);

            container->setMargin (5,Side::Top);
            container->setContentAlignment (AlignmentFlag::Left);
            if( item.gundemDosyasiVar () ){
                auto fileLink = SerikBLDCore::Meclis::GundemManager::downloadFileWeb (item.gundemDosyasi ().c_str ());
                Wt::WLink link = Wt::WLink(fileLink);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            item.gundemAdi ());
                container->addWidget (std::move(anchor));
                container->decorationStyle ().setCursor (Cursor::PointingHand);

            }else{
                container->addWidget (cpp14::make_unique<WText>(item.gundemAdi ()));
            }
        }
    }
}

void v2::MeclisItemPublicPage::initMeclisBilgileri()
{
    this->Header ()->setMargin (25,Side::Top);



    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addWidget (cpp14::make_unique<WText>("Yıl: "+std::to_string (this->yil ())));
    }


    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addWidget (cpp14::make_unique<WText>(this->ay ().toStdString ()));
    }



    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin (5,Side::Top);
        container->addWidget (cpp14::make_unique<WText>("Tarih: "+QDate::fromJulianDay (this->julianDay ()).toString ("dd/MM/yyyy").toStdString ()));

    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin (5,Side::Top);
        container->addWidget (cpp14::make_unique<WText>("Saat: "+WTime(QTime::fromMSecsSinceStartOfDay (this->saat ()).hour (),
                                                                       QTime::fromMSecsSinceStartOfDay (this->saat ()).minute ()).toString ("hh:mm")));

    }

    if( this->gundem ().toStdString ().size () ){
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (20,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail + "boxShadow");
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Ivory));

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("<b>Gündem</b>"),0,AlignmentFlag::Center);

        vLayout->addWidget (cpp14::make_unique<WText>(this->gundem ().toStdString (),TextFormat::UnsafeXHTML),0,AlignmentFlag::Justify);
    }


}
