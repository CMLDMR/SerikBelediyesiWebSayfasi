#include "meclisuyesipage.h"



v2::MeclisYonetimPage::MeclisYonetimPage(SerikBLDCore::DB *_db)
    :ContainerWidget (""),
      mDB(_db)
{
    //    addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    //    setWidth (WLength("100%"));
    this->initController ();
}

void v2::MeclisYonetimPage::initController()
{

    // Meclis Üyesi Ekle
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        container->addWidget (cpp14::make_unique<WText>("<b>Üyeler</b>"))->setPadding (5);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&MeclisYonetimPage::meclisUyeleri );
    }

    // Meclis Dönemi
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        container->addWidget (cpp14::make_unique<WText>("<b>Meclis Dönemi</b>"));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&MeclisYonetimPage::meclisDonemi);

    }

    // Parti Ekle/Çıkar
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        container->addWidget (cpp14::make_unique<WText>("<b>Partiler</b>"));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&MeclisYonetimPage::partiler);
    }

    // Komisyon Yönetimi
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        container->addWidget (cpp14::make_unique<WText>("<b>Komisyonlar</b>"));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&MeclisYonetimPage::komisyonlar);
    }


}

void v2::MeclisYonetimPage::meclisUyeleri()
{
    Content ()->setMargin (25,Side::Top|Side::Bottom);
    this->Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<MeclisUyeleriPage>(mDB));
}

void v2::MeclisYonetimPage::meclisDonemi()
{
    Content ()->setMargin (25,Side::Top|Side::Bottom);
    this->Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<MeclisDonemPage>(mDB));

}

void v2::MeclisYonetimPage::partiler()
{
    Content ()->setMargin (25,Side::Top|Side::Bottom);
    this->Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<PartiManagerPage>(mDB));

}

void v2::MeclisYonetimPage::komisyonlar()
{
    Content ()->setMargin (25,Side::Top|Side::Bottom);
    this->Content ()->clear ();
}

v2::MeclisUyeleriPage::MeclisUyeleriPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::Meclis::UyeManager (_db),tcManager(new SerikBLDCore::TCManager(_db))
{
    setMargin (25,Side::Top|Side::Bottom);
    auto ekleBtn = Header ()->addWidget (cpp14::make_unique<WPushButton>("Yeni Meclis Üyesi Ekle"));
    ekleBtn->addStyleClass (Bootstrap::Button::Primary);
    ekleBtn->clicked ().connect (this,&MeclisUyeleriPage::yeniKayit );



}

void v2::MeclisUyeleriPage::onList(const QVector<SerikBLDCore::Meclis::MeclisUyesi> *mlist)
{

    Content()->clear ();

}

void v2::MeclisUyeleriPage::yeniKayit()
{
    Content()->clear ();


    auto containerTC = Content ()->addWidget (cpp14::make_unique<VatandasYeniKayitWidget>(this->getDB ()));
    containerTC->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                Bootstrap::Grid::Medium::col_md_6+
                                Bootstrap::Grid::Small::col_sm_6+
                                Bootstrap::Grid::ExtraSmall::col_xs_12);
    containerTC->setMargin (15,Side::Top);

    auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                              Bootstrap::Grid::Medium::col_md_6+
                              Bootstrap::Grid::Small::col_sm_6+
                              Bootstrap::Grid::ExtraSmall::col_xs_12);
    container->setMargin (15,Side::Top);

    auto gLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());


    //TCOID
    {
        auto tcLineEdit = gLayout->addWidget (cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
        tcLineEdit->setPlaceholderText ("tcOid");
        tcLineEdit->setEnabled (false);
    }


    //Meclis Dönemi
    {
        gLayout->addWidget (cpp14::make_unique<WText>("Meclis Dönemini Seçiniz"));
        auto mMeclisDonemi = gLayout->addWidget (cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
    }


    //Parti Uyeligi
    {
        gLayout->addWidget (cpp14::make_unique<WText>("Parti Üyeligini Seçiniz"));
        auto mPartiGrubu = gLayout->addWidget (cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
    }

    //Komisyon Uyelikleri
    {
        gLayout->addWidget (cpp14::make_unique<WText>("Üye Olduğu Komisyonlar"));
        auto mKomisyonlar = gLayout->addWidget (cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
    }

}







v2::MeclisDonemPage::MeclisDonemPage(DB *_db)
    :SerikBLDCore::Meclis::DonemManager (_db)
{
    auto btn = Footer ()->addWidget (cpp14::make_unique<WPushButton>("Yeni Dönem Ekle"));
    btn->addStyleClass (Bootstrap::Button::Primary);
    btn->clicked ().connect ([=](){
        auto mDialog = createDialog ("Yeni Meclis Dönemi Ekle");
        auto hLayout = mDialog->contents ()->setLayout (cpp14::make_unique<WHBoxLayout>());
        hLayout->addWidget (cpp14::make_unique<WText>("Başlangıç Yılı"));
        auto baslangicYili = hLayout->addWidget (cpp14::make_unique<WSpinBox>());
        baslangicYili->setRange (1990,2999);
        baslangicYili->setValue (2019);
        hLayout->addWidget (cpp14::make_unique<WText>("Bitiş Yılı"));
        auto bitisYili = hLayout->addWidget (cpp14::make_unique<WSpinBox>());
        bitisYili->setRange (1990,2999);
        bitisYili->setValue (2024);
        auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        svBtn->addStyleClass (Bootstrap::Button::Primary);
        svBtn->clicked ().connect ([=](){
            if( this->InsertItem (SerikBLDCore::Meclis::MeclisDonemi()
                                  .setDonem ( std::to_string (baslangicYili->value ())+
                                              "-"+
                                              std::to_string (bitisYili->value ()))).size () )
            {
                SerikBLDCore::Meclis::DonemManager::UpdateList (SerikBLDCore::Meclis::MeclisDonemi());
                remogeDialog (mDialog);
            }
        });
    });


    auto guncelle = Header ()->addWidget (cpp14::make_unique<WPushButton>("Guncelle"));
    btn->addStyleClass (Bootstrap::Button::Primary);
    guncelle->clicked ().connect ([=](){
        SerikBLDCore::Meclis::DonemManager::UpdateList (SerikBLDCore::Meclis::MeclisDonemi());
    });


    SerikBLDCore::Meclis::DonemManager::UpdateList (SerikBLDCore::Meclis::MeclisDonemi());
}

void v2::MeclisDonemPage::onList(const QVector<SerikBLDCore::Meclis::MeclisDonemi> *mlist)
{
    Content ()->clear ();
    Content ()->setMargin (15,Side::Top|Side::Bottom);
    for( auto item : *mlist )
    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addStyleClass ("boxShadow");
        container->addStyleClass (Bootstrap::Grid::row + Bootstrap::ContextualBackGround::bg_success);
        container->setMargin (7,Side::Top);
        auto text = container->addWidget (cpp14::make_unique<WText>(item.donem ().toStdString ()));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                             Bootstrap::Grid::Medium::col_md_11+
                             Bootstrap::Grid::Small::col_sm_11+
                             Bootstrap::Grid::ExtraSmall::col_xs_10);

        auto delText = container->addWidget (cpp14::make_unique<WText>("<b>X</v>",TextFormat::UnsafeXHTML));
        delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                Bootstrap::Grid::Medium::col_md_1+
                                Bootstrap::Grid::Small::col_sm_1+
                                Bootstrap::Grid::ExtraSmall::col_xs_2);

        delText->decorationStyle ().setCursor (Cursor::PointingHand);
        delText->clicked ().connect ([=](){
            std::cout << text->text ().toUTF8 () << std::endl;

            SerikBLDCore::Meclis::MeclisDonemi item;
            item.setDonem (text->text ().toUTF8 ());
            SerikBLDCore::Meclis::DonemManager::deleteItem (item);
            SerikBLDCore::Meclis::DonemManager::UpdateList (SerikBLDCore::Meclis::MeclisDonemi());
        });

    }
}

void v2::MeclisDonemPage::yeniKayit()
{

}

v2::PartiManagerPage::PartiManagerPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::Meclis::PartiManager (_db)
{
    auto btn = Footer ()->addWidget (cpp14::make_unique<WPushButton>("Yeni Parti Ekle"));
    btn->addStyleClass (Bootstrap::Button::Primary);
    btn->clicked ().connect ([=](){
        auto mDialog = createDialog ("Yeni Parti Ekle");
        auto lineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Parti Adını Giriniz");
        auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        svBtn->addStyleClass (Bootstrap::Button::Primary);
        svBtn->clicked ().connect ([=](){
            if( this->InsertItem (SerikBLDCore::Meclis::PartiItem()
                                  .setParti ( lineEdit->text ().toUTF8 ())).size () )
            {
                SerikBLDCore::Meclis::PartiManager::UpdateList (SerikBLDCore::Meclis::PartiItem());
                remogeDialog (mDialog);
            }
        });
    });


    auto guncelle = Header ()->addWidget (cpp14::make_unique<WPushButton>("Guncelle"));
    btn->addStyleClass (Bootstrap::Button::Primary);
    guncelle->clicked ().connect ([=](){
        SerikBLDCore::Meclis::PartiManager::UpdateList (SerikBLDCore::Meclis::PartiItem());
    });


    SerikBLDCore::Meclis::PartiManager::UpdateList (SerikBLDCore::Meclis::PartiItem());
}

void v2::PartiManagerPage::onList(const QVector<SerikBLDCore::Meclis::PartiItem> *mlist)
{
    Content ()->clear ();
    Content ()->setMargin (15,Side::Top|Side::Bottom);
    for( auto item : *mlist )
    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addStyleClass ("boxShadow");
        container->addStyleClass (Bootstrap::Grid::row + Bootstrap::ContextualBackGround::bg_success);
        container->setMargin (7,Side::Top);
        auto text = container->addWidget (cpp14::make_unique<WText>(item.parti ().toStdString ()));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                             Bootstrap::Grid::Medium::col_md_11+
                             Bootstrap::Grid::Small::col_sm_11+
                             Bootstrap::Grid::ExtraSmall::col_xs_10);

        auto delText = container->addWidget (cpp14::make_unique<WText>("<b>X</v>",TextFormat::UnsafeXHTML));
        delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                Bootstrap::Grid::Medium::col_md_1+
                                Bootstrap::Grid::Small::col_sm_1+
                                Bootstrap::Grid::ExtraSmall::col_xs_2);

        delText->decorationStyle ().setCursor (Cursor::PointingHand);
        delText->clicked ().connect ([=](){
            std::cout << text->text ().toUTF8 () << std::endl;

            SerikBLDCore::Meclis::PartiItem item;
            item.setParti (text->text ().toUTF8 ());
            SerikBLDCore::Meclis::PartiManager::deleteItem (item);
            SerikBLDCore::Meclis::PartiManager::UpdateList (SerikBLDCore::Meclis::PartiItem());
        });

    }
}
