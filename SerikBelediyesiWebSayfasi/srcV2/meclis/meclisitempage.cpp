#include "meclisitempage.h"



v2::MeclisItemPage::MeclisItemPage(DB *_db, const MeclisItem &item)
    :ContainerWidget (std::to_string (item.yil ()) + " " + item.ay ().toStdString () + " Meclisi"),
      SerikBLDCore::Meclis::MeclisItem(item),
      SerikBLDCore::Meclis::KararManager (_db),
      SerikBLDCore::Meclis::YouTubeManager( _db )
{

    this->initMeclisBilgileri ();

    this->Content ()->setMargin (15,Side::Top);

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

    for( auto item : *mlist )
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
                                "Gündem Linki");
        container->addWidget (std::move(anchor));

        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        container->setMargin (25,Side::Bottom);
    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                 Bootstrap::Grid::Medium::col_md_4+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        mYil = container->addWidget (cpp14::make_unique<WSpinBox>());
        mYil->setRange (2014,2024);
        mYil->setValue (this->yil ());
    }


    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                 Bootstrap::Grid::Medium::col_md_4+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        mAyLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mAyLineEdit->setText (this->ay ().toStdString ());
        mAyLineEdit->setPlaceholderText ("Meclis Ayını: ( Ocak , Mart Olağanüstü )");
    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                 Bootstrap::Grid::Medium::col_md_4+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_12);
        mYayinda = container->addWidget (cpp14::make_unique<WCheckBox>("Yayında"));
        mYayinda->setChecked (this->yayinda ());
    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (20,Side::Top|Side::Bottom);
        container->addStyleClass ("boxShadow");

        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

        vLayout->addWidget (cpp14::make_unique<WText>("Gündem"),0,AlignmentFlag::Center);

        mGundemEdit = vLayout->addWidget (cpp14::make_unique<WTextEdit>(),0,AlignmentFlag::Justify);
        mGundemEdit->setText (this->gundem ().toStdString ());
        mGundemEdit->setHeight (350);
    }

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);
        auto guncelleBtn = container->addWidget (cpp14::make_unique<WPushButton>("Meclis Bilgilerini Güncelle"));
        guncelleBtn->addStyleClass (Bootstrap::Button::Primary);
    }
}



void v2::MeclisItemPage::initKararContoller()
{
    this->Footer ()->clear ();
    this->Footer ()->setMargin (15,Side::Top);
    {
        auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);
        auto kararBtn = container->addWidget (cpp14::make_unique<WPushButton>("Karar Ekle"));
        kararBtn->addStyleClass (Bootstrap::Button::Primary);
        kararBtn->clicked ().connect ([&](){
            this->addKarar ();
        });
    }

    {
        auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
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


    auto __KararOidWidget = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>(SerikBLDCore::Meclis::KararManager::db ()));
    __KararOidWidget->setType (FileUploaderWidget::Pdf);
    __KararOidWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    __KararOidWidget->Uploaded ().connect ([=](){
        __kararOidText->setText (__KararOidWidget->fileLocation ().toStdString ());
    });

    auto mKaydetBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    mKaydetBtn->addStyleClass (Bootstrap::Button::Primary);

    mKaydetBtn->clicked ().connect ([=](){
        SerikBLDCore::Meclis::KararItem item;
        auto kararOid__ = SerikBLDCore::Meclis::KararManager::uploadfile (__kararOidText->text ().toUTF8 ().c_str ());
        item.setKararOid (kararOid__.get_oid ().value.to_string ());
        item.setMeclisOid (this->oid ().value ().to_string ());
        item.setSayi (__SayiWidget->value ());
        SerikBLDCore::Meclis::KararManager::InsertItem (item);
        SerikBLDCore::Meclis::KararManager::UpdateList (SerikBLDCore::Meclis::KararItem().setMeclisOid (this->oid ().value ().to_string ()));
        this->remogeDialog (mDialog);
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
        SerikBLDCore::Meclis::YouTubeLink item;
        item.setVideoID (__link->text ().toUTF8 ());
        item.setMeclisOid (this->oid ().value ().to_string ());
        SerikBLDCore::Meclis::YouTubeManager::InsertItem (item);
        SerikBLDCore::Meclis::YouTubeManager::UpdateList (SerikBLDCore::Meclis::YouTubeLink().setMeclisOid (this->oid ().value ().to_string ()));
        this->remogeDialog (mDialog);
    });


}
