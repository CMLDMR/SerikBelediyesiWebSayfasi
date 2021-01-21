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
    Content ()->addWidget (cpp14::make_unique<KomisyonManagerPage>(mDB));
}

v2::MeclisUyeleriPage::MeclisUyeleriPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::Meclis::UyeManager (_db),tcManager(new SerikBLDCore::TCManager(_db))
{
    setMargin (25,Side::Top|Side::Bottom);

    {
        auto container = Header()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto ekleBtn = container->addWidget (cpp14::make_unique<WPushButton>("Yeni Meclis Üyesi Ekle"));
        ekleBtn->addStyleClass (Bootstrap::Button::Primary);
        ekleBtn->clicked ().connect (this,&MeclisUyeleriPage::yeniKayit );
    }


    {

        auto container = Header()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto donemFilter = container->addWidget (cpp14::make_unique<WComboBox>());
        for( auto item : this->getDonemList () )
        {
            donemFilter->addItem (item.donem ().toStdString ());
        }
        donemFilter->sactivated ().connect ([=](const WString& str){
            this->UpdateList (SerikBLDCore::Meclis::MeclisUyesi().setDonemAdi (str.toUTF8 ()));
        });
    }



    this->UpdateList (SerikBLDCore::Meclis::MeclisUyesi());



}

void v2::MeclisUyeleriPage::onList(const QVector<SerikBLDCore::Meclis::MeclisUyesi> *mlist)
{

    Content()->clear ();
    Content ()->setMargin (25,Side::Top);

    {
        auto container = Content ()->addWidget (cpp14::make_unique<ContainerWidget>("Meclis Üyeleri"));
        container->addStyleClass (Bootstrap::Grid::col_full_12);
    }


    for( auto item : *mlist )
    {

        auto tcItem = tcManager->Load_byOID (item.tcOid ().toStdString ());

        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->clicked ().connect ([=](){
            Content ()->clear ();
            Content ()->addWidget (cpp14::make_unique<MeclisUyesiProfilPage>(item,this->getDB ()));
        });

        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);


        {
            auto mPhotoText = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            auto filePath = this->downloadFileWeb (tcItem.value ()->FotoOid ());
            mPhotoText->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                       Bootstrap::Grid::Medium::col_md_4+
                                       Bootstrap::Grid::Small::col_sm_4+
                                       Bootstrap::Grid::ExtraSmall::col_xs_12);
            mPhotoText->setAttributeValue (Style::style,Style::background::url (filePath)+
                                           Style::background::size::cover+
                                           Style::background::position::center_center+
                                           Style::background::repeat::norepeat);
            mPhotoText->setHeight (140);

            mPhotoText->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        }

        auto infoContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        infoContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                      Bootstrap::Grid::Medium::col_md_8+
                                      Bootstrap::Grid::Small::col_sm_8+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto vLayout = infoContainer->setLayout (cpp14::make_unique<WVBoxLayout>());

        {
            auto mAdSoyadText = vLayout->addWidget (cpp14::make_unique<WText>(tcItem.value ()->AdSoyad ().toStdString ()),1,AlignmentFlag::Center);
            mAdSoyadText->setAttributeValue (Style::style,Style::font::size::s11px);
            mAdSoyadText->setMargin (0);

        }

        {
            auto mPartiText = vLayout->addWidget (cpp14::make_unique<WText>("<b>"+item.partiAdi ().toStdString ()+"</b>"),1,AlignmentFlag::Center);
            mPartiText->setAttributeValue (Style::style,Style::font::size::s10px);
        }

        {
            auto mDonemText = vLayout->addWidget (cpp14::make_unique<WText>("<i><b>"+item.donemAdi ().toStdString ()+"</b></i>"),1,AlignmentFlag::Center);
            mDonemText->setAttributeValue (Style::style,Style::font::size::s10px);
        }

        for( auto __item : item.komisyonUyelikleri () )
        {
            auto mKomisyonText = vLayout->addWidget (cpp14::make_unique<WText>("● <i>"+__item.toStdString ()+"</i>"),1,AlignmentFlag::Center);
            mKomisyonText->setAttributeValue (Style::style,Style::font::size::s10px);
            mKomisyonText->setMargin (0);
        }
        container->setMargin (5,Side::Top);
    }

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





    auto container = Content ()->addWidget (cpp14::make_unique<ContainerWidget>("Yeni Üye Belirle"));
    container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                              Bootstrap::Grid::Medium::col_md_6+
                              Bootstrap::Grid::Small::col_sm_6+
                              Bootstrap::Grid::ExtraSmall::col_xs_12);
    container->setMargin (15,Side::Top);



    auto gLayout = container->Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());


    auto gecerliDurumText = gLayout->addWidget (cpp14::make_unique<WText>("Geçersiz TC Kaydı"));
    gecerliDurumText->addStyleClass (Bootstrap::Label::Danger);


    //TCOID
    auto tcLineEdit = gLayout->addWidget (cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
    tcLineEdit->setPlaceholderText ("tcOid Yan Taraftan TC yazınca Çıkacak");
    tcLineEdit->setEnabled (false);

    tcLineEdit->textInput ().connect ([=](){
        if( tcLineEdit->text ().toUTF8 ().size () != 24 )
        {
            gecerliDurumText->removeStyleClass (Bootstrap::Label::Success,true);
            gecerliDurumText->addStyleClass (Bootstrap::Label::Danger,true);
        }else{
            gecerliDurumText->removeStyleClass (Bootstrap::Label::Danger,true);
            gecerliDurumText->addStyleClass (Bootstrap::Label::Success,true);
        }
    });


    containerTC->onExisted ().connect ([=](const std::string& tcoid){
        gecerliDurumText->setText ("Geçerli TC Kaydı");
        gecerliDurumText->removeStyleClass (Bootstrap::Label::Danger,true);
        gecerliDurumText->addStyleClass (Bootstrap::Label::Success,true);
        tcLineEdit->setText (tcoid);
    });




    //Meclis Dönemi
    gLayout->addWidget (cpp14::make_unique<WText>("Meclis Dönemini Seçiniz"));
    auto mMeclisDonemi = gLayout->addWidget (cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
    auto donemList = this->getDonemList ();
    for( auto item : donemList )
    {
        mMeclisDonemi->addItem (item.donem ().toStdString ());
    }




    //Parti Uyeligi
    gLayout->addWidget (cpp14::make_unique<WText>("Parti Üyeligini Seçiniz"));
    auto mPartiGrubu = gLayout->addWidget (cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
    auto partiList = this->getPartiList ();
    for( auto item : partiList )
    {
        mPartiGrubu->addItem (item.parti ().toStdString ());
    }


    //Komisyon Uyelikleri
    gLayout->addWidget (cpp14::make_unique<WText>("Üye Olduğu Komisyonlar"));
    auto komisyonContainer = gLayout->addWidget (cpp14::make_unique<WContainerWidget>());
    komisyonContainer->addStyleClass (Bootstrap::Grid::row);

    auto mKomisyonlar = komisyonContainer->addWidget (cpp14::make_unique<WComboBox>());
    mKomisyonlar->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                 Bootstrap::Grid::Medium::col_md_8+
                                 Bootstrap::Grid::Small::col_sm_7+
                                 Bootstrap::Grid::ExtraSmall::col_xs_7);
    auto komisyonList = this->getKomisyonList ();
    for( auto item : komisyonList )
    {
        mKomisyonlar->addItem (item.komisyonAdi ().toStdString ());
    }
    auto selectText = komisyonContainer->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
    selectText->addStyleClass (Bootstrap::Button::Primary);
    selectText->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                               Bootstrap::Grid::Medium::col_md_4+
                               Bootstrap::Grid::Small::col_sm_5+
                               Bootstrap::Grid::ExtraSmall::col_xs_5);

    auto selectedContainer = gLayout->addWidget (cpp14::make_unique<WContainerWidget>());
    selectedContainer->addStyleClass (Bootstrap::Grid::row);
    selectText->clicked ().connect ([=](){
        auto text = selectedContainer->addWidget (cpp14::make_unique<WText>(mKomisyonlar->currentText ().toUTF8 ()));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                             Bootstrap::Grid::Medium::col_md_11+
                             Bootstrap::Grid::Small::col_sm_11+
                             Bootstrap::Grid::ExtraSmall::col_xs_11);
        text->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        text->setMargin (5,Side::Bottom);
        mSelectedKomisyon.push_back (mKomisyonlar->currentText ().toUTF8 ().c_str ());

        auto delText = selectedContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                Bootstrap::Grid::Medium::col_md_1+
                                Bootstrap::Grid::Small::col_sm_1+
                                Bootstrap::Grid::ExtraSmall::col_xs_1);
        delText->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
        delText->setMargin (5,Side::Bottom);
        delText->decorationStyle ().setCursor (Cursor::PointingHand);

        delText->clicked ().connect ([=](){
            selectedContainer->removeWidget(text);
            selectedContainer->removeWidget(delText);
            mSelectedKomisyon.removeOne (text->text ().toUTF8 ().c_str ());
        });
    });




    //Kayıt Yapma
    auto svBtn = gLayout->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    svBtn->clicked ().connect ([=](){

        SerikBLDCore::Meclis::MeclisUyesi uyeItem;
        uyeItem.setTCOid (tcLineEdit->text ().toUTF8 ());
        uyeItem.setPartiAdi (mPartiGrubu->currentText ().toUTF8 ());
        uyeItem.setDonemAdi (mMeclisDonemi->currentText ().toUTF8 ());
        for( auto item : mSelectedKomisyon )
        {
            uyeItem.addKomisyonAdi (item.toStdString ());
        }
        if( this->InsertItem (uyeItem).size () ){
            this->UpdateList (SerikBLDCore::Meclis::MeclisUyesi());
        }
    });


}


v2::MeclisDonemPage::MeclisDonemPage(DB *_db)
    :SerikBLDCore::Meclis::DonemManager (_db),
      mUyeManager( new SerikBLDCore::Meclis::UyeManager(_db))
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
                                  .setDonem ( baslangicYili->value () , bitisYili->value ())).size () )
            {
                SerikBLDCore::Meclis::DonemManager::UpdateList (SerikBLDCore::Meclis::MeclisDonemi());
                removeDialog (mDialog);
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

        text->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());

        auto delText = container->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                Bootstrap::Grid::Medium::col_md_1+
                                Bootstrap::Grid::Small::col_sm_1+
                                Bootstrap::Grid::ExtraSmall::col_xs_2);

        delText->decorationStyle ().setCursor (Cursor::PointingHand);
        delText->clicked ().connect ([=](){

            SerikBLDCore::Item Checkitem(SerikBLDCore::Meclis::UyeKey::Collection);
            Checkitem.append(SerikBLDCore::Meclis::UyeKey::donemAdi,text->text ().toUTF8 ());
            auto count = this->mUyeManager->countItem (Checkitem);

            if( count < 0 )
            {
                this->showPopUpMessage ("Veritabanı Hatası: " + this->mUyeManager->getLastError ().toStdString (),"hata");
                return;
            }

            if( count > 0 )
            {
                this->showPopUpMessage (WString("Bu Dönemden Seçilmiş {1} Adet Meclis Üyesi Var! İlk Önce Onlar Kaldırınız").arg (count).toUTF8 (),"msg");
                return;
            }

            auto yesBtn = this->askConfirm ("Silmek İstediniz den Emin misiniz? " + std::to_string (count));

            yesBtn->clicked ().connect ([=](){
                SerikBLDCore::Meclis::MeclisDonemi item;
                item.setOid (text->attributeValue (Style::dataoid).toUTF8 ());
                auto delResult = SerikBLDCore::Meclis::DonemManager::deleteItem (item);
                if( delResult )
                {
                    if( delResult.value ().deleted_count () )
                    {
                        this->showPopUpMessage ("Silindi","msg");
                        SerikBLDCore::Meclis::DonemManager::UpdateList (SerikBLDCore::Meclis::MeclisDonemi());
                    }else{
                        this->showPopUpMessage ("Silme İşlemi Başarısız Oldu. " + SerikBLDCore::Meclis::DonemManager::getLastError ().toStdString (),"hata");
                    }
                }else{
                    this->showPopUpMessage ("Silme İşlemi Başarısız Oldu. " + SerikBLDCore::Meclis::DonemManager::getLastError ().toStdString (),"hata");
                }

            });


        });

    }
}

void v2::MeclisDonemPage::yeniKayit()
{

}

v2::PartiManagerPage::PartiManagerPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::Meclis::PartiManager (_db),
      mUyeManager( new SerikBLDCore::Meclis::UyeManager(_db))
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
                removeDialog (mDialog);
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

        auto delText = container->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                Bootstrap::Grid::Medium::col_md_1+
                                Bootstrap::Grid::Small::col_sm_1+
                                Bootstrap::Grid::ExtraSmall::col_xs_2);

        delText->decorationStyle ().setCursor (Cursor::PointingHand);
        delText->clicked ().connect ([=](){

            SerikBLDCore::Item Checkitem(SerikBLDCore::Meclis::UyeKey::Collection);
            Checkitem.append(SerikBLDCore::Meclis::UyeKey::partiAdi,text->text ().toUTF8 ());
            auto count = this->mUyeManager->countItem (Checkitem);

            if( count < 0 )
            {
                this->showPopUpMessage ("Veritabanı Hatası: " + this->mUyeManager->getLastError ().toStdString (),"hata");
                return;
            }

            if( count > 0 )
            {
                this->showPopUpMessage (WString("Bu Partiden Seçilmiş {1} Adet Meclis Üyesi Var! İlk Önce Onlar Kaldırınız").arg (count).toUTF8 (),"msg");
                return;
            }

            auto yesBtn = this->askConfirm ("Silmek İstediniz den Emin misiniz? " + std::to_string (count));

            yesBtn->clicked ().connect ([=](){
                SerikBLDCore::Meclis::PartiItem item;
                item.setParti (text->text ().toUTF8 ());
                auto delResult = SerikBLDCore::Meclis::PartiManager::deleteItem (item);
                if( delResult )
                {
                    if( delResult.value ().deleted_count () )
                    {
                        this->showPopUpMessage ("Silindi","msg");
                        SerikBLDCore::Meclis::PartiManager::UpdateList (SerikBLDCore::Meclis::PartiItem());
                    }else{
                        this->showPopUpMessage ("Silme İşlemi Başarısız Oldu. " + SerikBLDCore::Meclis::PartiManager::getLastError ().toStdString (),"hata");
                    }
                }else{
                    this->showPopUpMessage ("Silme İşlemi Başarısız Oldu. " + SerikBLDCore::Meclis::PartiManager::getLastError ().toStdString (),"hata");
                }

            });



        });

    }
}

v2::KomisyonManagerPage::KomisyonManagerPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::Meclis::KomisyonManager (_db),
      mUyeManager( new SerikBLDCore::Meclis::UyeManager(_db))
{
    auto btn = Footer ()->addWidget (cpp14::make_unique<WPushButton>("Yeni Komisyon Ekle"));
    btn->addStyleClass (Bootstrap::Button::Primary);
    btn->clicked ().connect ([=](){
        auto mDialog = createDialog ("Yeni Parti Ekle");
        auto lineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Parti Adını Giriniz");
        auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        svBtn->addStyleClass (Bootstrap::Button::Primary);
        svBtn->clicked ().connect ([=](){
            if( this->InsertItem (SerikBLDCore::Meclis::KomisyonItem()
                                  .setKomisyonAdi ( lineEdit->text ().toUTF8 ())).size () )
            {
                SerikBLDCore::Meclis::KomisyonManager::UpdateList (SerikBLDCore::Meclis::KomisyonItem());
                removeDialog (mDialog);
            }
        });
    });


    auto guncelle = Header ()->addWidget (cpp14::make_unique<WPushButton>("Guncelle"));
    btn->addStyleClass (Bootstrap::Button::Primary);
    guncelle->clicked ().connect ([=](){
        SerikBLDCore::Meclis::KomisyonManager::UpdateList (SerikBLDCore::Meclis::KomisyonItem());
    });


    SerikBLDCore::Meclis::KomisyonManager::UpdateList (SerikBLDCore::Meclis::KomisyonItem());
}

void v2::KomisyonManagerPage::onList(const QVector<SerikBLDCore::Meclis::KomisyonItem> *mlist)
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
        auto text = container->addWidget (cpp14::make_unique<WText>(item.komisyonAdi ().toStdString ()));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                             Bootstrap::Grid::Medium::col_md_11+
                             Bootstrap::Grid::Small::col_sm_11+
                             Bootstrap::Grid::ExtraSmall::col_xs_10);

        auto delText = container->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                Bootstrap::Grid::Medium::col_md_1+
                                Bootstrap::Grid::Small::col_sm_1+
                                Bootstrap::Grid::ExtraSmall::col_xs_2);

        delText->decorationStyle ().setCursor (Cursor::PointingHand);
        delText->clicked ().connect ([=](){
            SerikBLDCore::Item Checkitem(SerikBLDCore::Meclis::UyeKey::Collection);
            Checkitem.append(SerikBLDCore::Meclis::UyeKey::komisyonAdi,text->text ().toUTF8 ());
            auto count = this->mUyeManager->countItem (Checkitem);

            if( count < 0 )
            {
                this->showPopUpMessage ("Veritabanı Hatası: " + this->mUyeManager->getLastError ().toStdString (),"hata");
                return;
            }

            if( count > 0 )
            {
                this->showPopUpMessage (WString("Bu Komisyona Seçilmiş {1} Adet Meclis Üyesi Var! İlk Önce Onlar Kaldırınız").arg (count).toUTF8 (),"msg");
                return;
            }

            auto yesBtn = this->askConfirm ("Silmek İstediniz den Emin misiniz? " + std::to_string (count));

            yesBtn->clicked ().connect ([=](){
                SerikBLDCore::Meclis::KomisyonItem item;
                item.setKomisyonAdi (text->text ().toUTF8 ());
                auto delResult = SerikBLDCore::Meclis::KomisyonManager::deleteItem (item);
                if( delResult )
                {
                    if( delResult.value ().deleted_count () )
                    {
                        this->showPopUpMessage ("Silindi","msg");
                        SerikBLDCore::Meclis::KomisyonManager::UpdateList (SerikBLDCore::Meclis::KomisyonItem());
                    }else{
                        this->showPopUpMessage ("Silme İşlemi Başarısız Oldu. " + SerikBLDCore::Meclis::KomisyonManager::getLastError ().toStdString (),"hata");
                    }
                }else{
                    this->showPopUpMessage ("Silme İşlemi Başarısız Oldu. " + SerikBLDCore::Meclis::KomisyonManager::getLastError ().toStdString (),"hata");
                }

            });

        });

    }
}

void v2::KomisyonManagerPage::yeniKayit()
{

}

v2::MeclisUyesiProfilPage::MeclisUyesiProfilPage(const MeclisUyesi &uye, SerikBLDCore::DB* _db  )
    :SerikBLDCore::Meclis::MeclisUyesi (uye),
      mDB(_db),
      mTcManager(new SerikBLDCore::TCManager (_db) )
{

    mUyeManager = new SerikBLDCore::Meclis::UyeManager(mDB);
    auto val = mTcManager->Load_byOID (uye.tcOid ().toStdString ());
    if( !val )
    {
        this->showMessage ("Uyarı","TC Bilgileri Yüklenemedi");
        return;
    }
    mTC = val.value ();
    this->Header ()->addWidget (cpp14::make_unique<VatandasWidget>(mDB,mTC));

    Content ()->setMargin (25,Side::Top);
    Content ()->addStyleClass ("boxShadow");
    meclisUyesiBilgileri ();

}

void v2::MeclisUyesiProfilPage::meclisUyesiBilgileri()
{

    Content()->clear ();

    auto container = Content ()->addWidget (cpp14::make_unique<ContainerWidget>("Meclis Üyeliği Bilgileri"));
    container->addStyleClass (Bootstrap::Grid::col_full_12);
    container->setMargin (15,Side::Top);

    auto gLayout = container->Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());




    //TCOID
    auto tcLineEdit = gLayout->addWidget (cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
    tcLineEdit->setPlaceholderText (mTC->oid ().value ().to_string ());
    tcLineEdit->setEnabled (false);

    //    tcLineEdit->textInput ().connect ([=](){
    //        if( tcLineEdit->text ().toUTF8 ().size () != 24 )
    //        {
    //            gecerliDurumText->removeStyleClass (Bootstrap::Label::Success,true);
    //            gecerliDurumText->addStyleClass (Bootstrap::Label::Danger,true);
    //        }else{
    //            gecerliDurumText->removeStyleClass (Bootstrap::Label::Danger,true);
    //            gecerliDurumText->addStyleClass (Bootstrap::Label::Success,true);
    //        }
    //    });


    //    containerTC->onExisted ().connect ([=](const std::string& tcoid){
    //        gecerliDurumText->setText ("Geçerli TC Kaydı");
    //        gecerliDurumText->removeStyleClass (Bootstrap::Label::Danger,true);
    //        gecerliDurumText->addStyleClass (Bootstrap::Label::Success,true);
    //        tcLineEdit->setText (tcoid);
    //    });




    //Meclis Dönemi
    gLayout->addWidget (cpp14::make_unique<WText>("Meclis Dönemi"));
    auto mMeclisDonemi = gLayout->addWidget (cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
    auto donemList = mUyeManager->getDonemList ();
    int partiCount = 0;
    for( auto item : donemList )
    {
        mMeclisDonemi->addItem (item.donem ().toStdString ());
        if( this->donemAdi () == item.donem () ){
            mMeclisDonemi->setCurrentIndex (partiCount);
        }
        partiCount++;
    }
    mMeclisDonemi->changed ().connect ([=](){
        SerikBLDCore::Meclis::MeclisUyesi uyeItem;
        uyeItem.setOid (this->oid ().value ().to_string ());
        auto val = mUyeManager->setField(uyeItem,SerikBLDCore::Meclis::UyeKey::donemAdi,mMeclisDonemi->currentText ().toUTF8 ());
        if( val )
        {
            this->setDonemAdi (mMeclisDonemi->currentText ().toUTF8 ().c_str ());
            this->showPopUpMessage ("Meclis Dönemi Değiştirildi");
        }else{
            this->showPopUpMessage (mUyeManager->getLastError ().toStdString (),"hata");
        }
    });




    //Parti Uyeligi
    gLayout->addWidget (cpp14::make_unique<WText>("Parti Üyeligi"));
    auto mPartiGrubu = gLayout->addWidget (cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
    auto partiList = mUyeManager->getPartiList ();
    partiCount = 0;
    for( auto item : partiList )
    {
        mPartiGrubu->addItem (item.parti ().toStdString ());
        if( this->partiAdi () == item.parti () ){
            mPartiGrubu->setCurrentIndex (partiCount);
        }
        partiCount++;
    }

    mPartiGrubu->changed ().connect ([=](){
        SerikBLDCore::Meclis::MeclisUyesi uyeItem;
        uyeItem.setOid (this->oid ().value ().to_string ());
        auto val = mUyeManager->setField(uyeItem,SerikBLDCore::Meclis::UyeKey::partiAdi,mPartiGrubu->currentText ().toUTF8 ());
        if( val )
        {
            this->setPartiAdi (mPartiGrubu->currentText ().toUTF8 ().c_str ());
            this->showPopUpMessage ("Parti Grubu Değiştirildi");
        }else{
            this->showPopUpMessage (mUyeManager->getLastError ().toStdString (),"hata");
        }
    });



    //Komisyon Uyelikleri
    gLayout->addWidget (cpp14::make_unique<WText>("Üye Olduğu Komisyonlar"));
    auto komisyonContainer = gLayout->addWidget (cpp14::make_unique<WContainerWidget>());
    komisyonContainer->addStyleClass (Bootstrap::Grid::row);

    auto mKomisyonlar = komisyonContainer->addWidget (cpp14::make_unique<WComboBox>());
    mKomisyonlar->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                 Bootstrap::Grid::Medium::col_md_8+
                                 Bootstrap::Grid::Small::col_sm_7+
                                 Bootstrap::Grid::ExtraSmall::col_xs_7);
    auto komisyonList = mUyeManager->getKomisyonList ();
    if( komisyonList.size() == 0 ) this->showPopUpMessage ("Komisyon Listesi Çekilemedi","Hata");
    for( auto item : komisyonList )
    {
        mKomisyonlar->addItem (item.komisyonAdi ().toStdString ());
    }


    auto selectBtn = komisyonContainer->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
    selectBtn->addStyleClass (Bootstrap::Button::Primary);
    selectBtn->addStyleClass (Bootstrap::Grid::col_full_12);

    auto selectedContainer = gLayout->addWidget (cpp14::make_unique<WContainerWidget>());
    selectedContainer->addStyleClass (Bootstrap::Grid::row);


    for( auto item : this->komisyonUyelikleri ())
    {

        auto text = selectedContainer->addWidget (cpp14::make_unique<WText>(item.toStdString ()));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                             Bootstrap::Grid::Medium::col_md_11+
                             Bootstrap::Grid::Small::col_sm_11+
                             Bootstrap::Grid::ExtraSmall::col_xs_11);
        text->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        text->setMargin (5,Side::Bottom);

        auto delText = selectedContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                Bootstrap::Grid::Medium::col_md_1+
                                Bootstrap::Grid::Small::col_sm_1+
                                Bootstrap::Grid::ExtraSmall::col_xs_1);
        delText->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
        delText->setMargin (5,Side::Bottom);
        delText->decorationStyle ().setCursor (Cursor::PointingHand);

        delText->clicked ().connect ([=](){
            SerikBLDCore::Meclis::MeclisUyesi uyeItem;
            uyeItem.setOid (this->oid ().value ().to_string ());
            auto val = mUyeManager->pullValue(uyeItem,SerikBLDCore::Meclis::UyeKey::komisyonAdi,text->text ().toUTF8 ());
            if( val )
            {
                this->delKomisyonAdi (text->text ().toUTF8 ());
                selectedContainer->removeWidget(text);
                selectedContainer->removeWidget(delText);
                this->showPopUpMessage ("Komisyon Silindi.");
            }else{
                this->showPopUpMessage (mUyeManager->getLastError ().toStdString (),"hata");
            }
        });

    }


    selectBtn->clicked ().connect ([=](){


        if( this->komisyonUyelikleri ().contains (QString::fromStdString (mKomisyonlar->currentText ().toUTF8 ())) )
        {
            this->showPopUpMessage ("Hata: Bu Komisyona Zaten Üye","hata");
            return;
        }


        SerikBLDCore::Meclis::MeclisUyesi uyeItem;
        uyeItem.setOid (this->oid ().value ().to_string ());

        auto val = mUyeManager->pushValue(uyeItem,SerikBLDCore::Meclis::UyeKey::komisyonAdi,mKomisyonlar->currentText ().toUTF8 ());
        if( val )
        {
            this->addKomisyonAdi (mKomisyonlar->currentText ().toUTF8 ());
            auto text = selectedContainer->addWidget (cpp14::make_unique<WText>(mKomisyonlar->currentText ().toUTF8 ()));
            text->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                 Bootstrap::Grid::Medium::col_md_11+
                                 Bootstrap::Grid::Small::col_sm_11+
                                 Bootstrap::Grid::ExtraSmall::col_xs_11);
            text->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
            text->setMargin (5,Side::Bottom);

            auto delText = selectedContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
            delText->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                    Bootstrap::Grid::Medium::col_md_1+
                                    Bootstrap::Grid::Small::col_sm_1+
                                    Bootstrap::Grid::ExtraSmall::col_xs_1);
            delText->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
            delText->setMargin (5,Side::Bottom);
            delText->decorationStyle ().setCursor (Cursor::PointingHand);

            delText->clicked ().connect ([=](){
                auto val = mUyeManager->pullValue(uyeItem,SerikBLDCore::Meclis::UyeKey::komisyonAdi,text->text ().toUTF8 ());
                if( val )
                {
                    this->delKomisyonAdi (text->text ().toUTF8 ());
                    this->showPopUpMessage ("Komisyon Silindi.");
                    selectedContainer->removeWidget(text);
                    selectedContainer->removeWidget(delText);
                }else{
                    this->showPopUpMessage (mUyeManager->getLastError ().toStdString (),"hata");
                }
            });
            this->showPopUpMessage ("Komisyon Eklendi.");
        }else{
            this->showPopUpMessage (mUyeManager->getLastError ().toStdString (),"hata");
        }
    });

}


