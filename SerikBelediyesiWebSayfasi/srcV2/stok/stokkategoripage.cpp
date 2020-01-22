#include "stokkategoripage.h"

#include "SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.h"


v2::StokKategoriPage::StokKategoriPage(DB *_db, SerikBLDCore::User *_mUser)
    :ContainerWidget ("Stok Kategori"),
      SerikBLDCore::Stok::StokKategoriManager (_db),
      mUser(_mUser)
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
                                  Bootstrap::Grid::Medium::col_md_10+
                                  Bootstrap::Grid::Small::col_sm_10+
                                  Bootstrap::Grid::ExtraSmall::col_xs_10);
        mKategoriAdiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mKategoriAdiLineEdit->setPlaceholderText ("Yeni Kategori Adını Giriniz");
        mKategoriAdiLineEdit->setWidth (WLength("100%"));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        container->setContentAlignment (AlignmentFlag::Center);
        auto svBtn = container->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        svBtn->addStyleClass (Bootstrap::Button::Primary);

        svBtn->clicked ().connect ([=](){

            if( !mKategoriAdiLineEdit->text ().toUTF8 ().size () )
            {
                this->errorOccured ("Kategori Adını Girmediniz");
                return;
            }


            SerikBLDCore::Stok::StokKategori kategori(mUser->Birimi ());
            kategori.setKategoriName (mKategoriAdiLineEdit->text ().toUTF8 ());

            auto countItem = this->countItem (kategori);

            if( countItem != 0 )
            {
                this->errorOccured ("Bu Kategoriden Mevcut");
            }else{
                if( this->InsertItem (kategori).size () )
                {
                    this->UpdateList (SerikBLDCore::Stok::StokKategori(mUser->Birimi ()));
                    this->showPopUpMessage ("Kategori Eklendi");
                }
            }


        });

    }

    this->UpdateList (SerikBLDCore::Stok::StokKategori(mUser->Birimi ()));

}

void v2::StokKategoriPage::onList( const QVector<SerikBLDCore::Stok::StokKategori> *mlist )
{
    Content ()->clear ();
    for( auto item : *mlist )
    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                  Bootstrap::Grid::Medium::col_md_11+
                                  Bootstrap::Grid::Small::col_sm_11+
                                  Bootstrap::Grid::ExtraSmall::col_xs_11);
        container->addWidget (cpp14::make_unique<WText>(item.kategoriName ()));
        container->addStyleClass ("boxRadius");
        container->setMargin (3,Side::Top|Side::Bottom);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+Style::color::color (Style::color::Grey::Black));
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ([=](){
            auto selectOid = item.oid ();
            if( selectOid )
            {
                _SelectedKategoriOid.emit (selectOid.value ().to_string (),item.kategoriName ());
            }
        });


        auto delContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        delContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                     Bootstrap::Grid::Medium::col_md_1+
                                     Bootstrap::Grid::Small::col_sm_1+
                                     Bootstrap::Grid::ExtraSmall::col_xs_1);
        delContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Crimson)+Style::color::color (Style::color::White::Snow));
        delContainer->addStyleClass ("boxRadius");
        delContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        delContainer->setMargin (3,Side::Top|Side::Bottom);
        delContainer->clicked ().connect ([=](){
            auto delBtn = this->askConfirm ("Silmek İstediğinize Emin misiniz?");
            delBtn->clicked ().connect ([=](){
                SerikBLDCore::Stok::StokKategori filter(mUser->Birimi ());
                filter.setOid (item.oid ().value ().to_string ());
                if( !this->DeleteItem (filter) )
                {

                }else{
                    this->UpdateList (SerikBLDCore::Stok::StokKategori(mUser->Birimi ()));
                }
            });
        });
    }
}

void v2::StokKategoriPage::errorOccured( const std::string &errorText )
{
    this->showPopUpMessage (errorText,"hata");
}

Signal<std::string, std::string> &v2::StokKategoriPage::SelectedKategoriOid()
{
    return _SelectedKategoriOid;
}




v2::StokKodPage::StokKodPage(SerikBLDCore::DB *_db)
    :ContainerWidget("Stok Birim "),
      SerikBLDCore::Stok::StokKodManager (_db)
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mCurrentKategoriOidText = container->addWidget (cpp14::make_unique<WText>(""));
        mCurrentKategoriOidText->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        mCurrentKategoriOidText->setMargin (5,Side::Top|Side::Bottom);
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mKodAdiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mKodAdiLineEdit->setPlaceholderText ("Yeni Birim Adını Giriniz");
        mKodAdiLineEdit->setWidth (WLength("100%"));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mBirimTipiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mBirimTipiLineEdit->setPlaceholderText ("Yeni Birim Tipini Giriniz (KG, TON, Adet...)");
        mBirimTipiLineEdit->setWidth (WLength("100%"));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_12+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setContentAlignment (AlignmentFlag::Center);
        auto svBtn = container->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        svBtn->addStyleClass (Bootstrap::Button::Primary);

        svBtn->clicked ().connect ([=](){

            if( !mCurrentKategoriOid.size () )
            {
                this->errorOccured ("Kategori Seçmediniz");
                return;
            }

            if( !mBirimTipiLineEdit->text ().toUTF8 ().size () )
            {
                this->errorOccured ("Birim Tipini Girmediniz");
                return;
            }

            if( !mKodAdiLineEdit->text ().toUTF8 ().size () )
            {
                this->errorOccured ("Birim Adını Girmediniz");
                return;
            }


            SerikBLDCore::Stok::StokKodItem birim;
            birim.setAdi (mKodAdiLineEdit->text ().toUTF8 ());
            birim.setBirim (mBirimTipiLineEdit->text ().toUTF8 ());
            birim.setKategoriOid (bsoncxx::oid{mCurrentKategoriOid});

            auto countItem = this->countItem (birim);

            if( countItem != 0 )
            {
                this->errorOccured ("Bu Stok Biriminden Mevcut");
            }else{
                if( this->InsertItem (birim).size () )
                {
                    this->UpdateList (SerikBLDCore::Stok::StokKodItem());
                    this->showPopUpMessage ("Birim Eklendi");
                }
            }


        });

    }

    this->UpdateList (SerikBLDCore::Stok::StokKodItem());
}

void v2::StokKodPage::onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist)
{
    Content ()->clear ();
    for( auto item : *mlist )
    {
        auto birimcontainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        birimcontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                       Bootstrap::Grid::Medium::col_md_6+
                                       Bootstrap::Grid::Small::col_sm_6+
                                       Bootstrap::Grid::ExtraSmall::col_xs_6);
        birimcontainer->addWidget (cpp14::make_unique<WText>(item.birim ()));
        birimcontainer->addStyleClass ("boxRadius");
        birimcontainer->setMargin (3,Side::Top|Side::Bottom);
        birimcontainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+Style::color::color (Style::color::Grey::Black));



        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5);
        container->addWidget (cpp14::make_unique<WText>(item.adi ()));
        container->addStyleClass ("boxRadius");
        container->setMargin (3,Side::Top|Side::Bottom);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+Style::color::color (Style::color::Grey::Black));



        auto delContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        delContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                     Bootstrap::Grid::Medium::col_md_1+
                                     Bootstrap::Grid::Small::col_sm_1+
                                     Bootstrap::Grid::ExtraSmall::col_xs_1);
        delContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Crimson)+Style::color::color (Style::color::White::Snow));
        delContainer->addStyleClass ("boxRadius");
        delContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        delContainer->setMargin (3,Side::Top|Side::Bottom);
        delContainer->clicked ().connect ([=](){
            auto delBtn = this->askConfirm ("Silmek İstediğinize Emin misiniz?");
            delBtn->clicked ().connect ([=](){
                SerikBLDCore::Stok::StokKodItem filter;
                filter.setOid (item.oid ().value ().to_string ());
                if( !this->DeleteItem (filter) )
                {

                }else{
                    this->UpdateList (SerikBLDCore::Stok::StokKodItem());
                }
            });
        });
    }
}

void v2::StokKodPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (errorText,"hata");
}

void v2::StokKodPage::selectedKategoriOid(const std::string &selectedKategoriOid, const std::string &kategoriAdi)
{
    mCurrentKategoriOid = selectedKategoriOid;
    mCurrentKategoriOidText->setText (kategoriAdi + " - " +mCurrentKategoriOid);
    this->UpdateList (SerikBLDCore::Stok::StokKodItem().setKategoriOid (bsoncxx::oid{mCurrentKategoriOid}));
}

v2::StokManagerPage::StokManagerPage(const std::string &stokTitle, SerikBLDCore::User *_mUser)
    :ContainerWidget (stokTitle),
      SerikBLDCore::Stok::StokManager (_mUser),
      mTCManager( new SerikBLDCore::TCManagerV2(_mUser->getDB ())),
      mCurrentTC( new SerikBLDCore::TC())
{




    {
        auto container = Header() ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_12+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto mTcLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mTcLineEdit->setPlaceholderText ("TCNO ile Ara ( 11 Hane ) ");

        mTcLineEdit->textInput ().connect ([=](){

            mTCViewWidget->Header ()->clear ();
            mTCViewWidget->Content ()->clear ();
            mTCViewWidget->Footer ()->clear ();

            mCurrentTC->clear ();


            if( mTcLineEdit->text ().toUTF8 ().size () < 11 )
            {
                return;
            }

            if( mTcLineEdit->text ().toUTF8 ().size () > 11 )
            {
                this->showPopUpMessage ("Telefon Numarası Hatalı","hata");
                return;
            }

            auto mtc = mTCManager->Load_byTCNO (mTcLineEdit->text ().toUTF8 ());

            if( mtc )
            {
                mCurrentTC = mtc.value ();
                mTCViewWidget->initTCView (mCurrentTC);
            }else{
                this->showPopUpMessage ("TC Bilgileri Bulunamadı","hata");
            }


        });
    }



    {
        auto container = Header() ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_12+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto mTelefonLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mTelefonLineEdit->setPlaceholderText ("Cep Telefonu Numarası İle Ara ( 11 Hane ) ");

        mTelefonLineEdit->textInput ().connect ([=](){

            mTCViewWidget->Header ()->clear ();
            mTCViewWidget->Content ()->clear ();
            mTCViewWidget->Footer ()->clear ();

            mCurrentTC->clear ();


            if( mTelefonLineEdit->text ().toUTF8 ().size () < 11 )
            {
                return;
            }

            if( mTelefonLineEdit->text ().toUTF8 ().size () > 11 )
            {
                this->showPopUpMessage ("Telefon Numarası Hatalı","hata");
                return;
            }

            auto mtc = mTCManager->Load_byTEL (mTelefonLineEdit->text ().toUTF8 ());

            if( mtc )
            {
                mCurrentTC = mtc.value ();
                mTCViewWidget->initTCView (mCurrentTC);
            }else{
                this->showPopUpMessage ("TC Bilgileri Bulunamadı","hata");
            }


        });
    }



    {
        mTCViewWidget = Header() ->addWidget (cpp14::make_unique<VatandasWidget>(this->user ()->getDB (),mCurrentTC));
        mTCViewWidget->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                      Bootstrap::Grid::Medium::col_md_4+
                                      Bootstrap::Grid::Small::col_sm_12+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);

        mTCViewWidget->Header ()->clear ();
        mTCViewWidget->Content ()->clear ();
        mTCViewWidget->Footer ()->clear ();

        mTCViewWidget->setMargin ( 15 , Side::Top|Side::Bottom );
    }


    {
        auto _SearchContentContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        _SearchContentContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                               Bootstrap::Grid::Medium::col_md_8+
                                               Bootstrap::Grid::Small::col_sm_12+
                                               Bootstrap::Grid::ExtraSmall::col_xs_12);


        mSearchContentContainer = _SearchContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mSearchContentContainer->addStyleClass (Bootstrap::Grid::row);

        mSearchContentContainer->setMargin ( 15 , Side::Top|Side::Bottom );



    }



    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mKategoriComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
        mKategoriComboBox->changed ().connect ([=](){
            this->updateStokKodList (SerikBLDCore::Stok::StokKodItem().setKategoriOid (mKategoriList[mKategoriComboBox->currentText ().toUTF8 ()]));
        });
    }



    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mStokItemComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
    }



    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_8+
                                  Bootstrap::Grid::ExtraSmall::col_xs_8);
        mMiktarSpinBox = container->addWidget (cpp14::make_unique<WSpinBox>());
        mMiktarSpinBox->setRange (0,999999);
    }



    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                  Bootstrap::Grid::Medium::col_md_1+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto ekleBtn = container->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        ekleBtn->addStyleClass (Bootstrap::Button::Primary);

        ekleBtn->clicked ().connect ([=](){

            if( mCurrentTC->view ().empty () )
            {
                this->showPopUpMessage ("TC Bilgileri Hatalı","hata");
                return;
            }

            if( mMiktarSpinBox->value () <= 0 )
            {
                this->showPopUpMessage ("Miktar Hatalı","hata");
                return;
            }


            SerikBLDCore::Stok::Stok stokItem(SerikBLDCore::Stok::Stok::Giris::giris,this->user ()->Birimi ());
            stokItem.setAdi (mStokItemComboBox->currentText ().toUTF8 ());
            stokItem.setKodu (mStokKodList[mStokItemComboBox->currentText ().toUTF8 ()]);
            stokItem.setKategori (mKategoriList[mKategoriComboBox->currentText ().toUTF8 ()]);
            stokItem.setMiktar (mMiktarSpinBox->value ());
            stokItem.setTcOid (mCurrentTC->oid ().value ());
            if( this->addStok (stokItem) ){
                this->showPopUpMessage (stokItem.adi () + " Eklendi","msg");

                stokItem.clear ();

                stokItem.setTcOid (mCurrentTC->oid ().value ());
                stokItem.setMudurluk (this->user ()->Birimi ());
                this->updateStokList (stokItem);
            }
        });
    }


    mCurrentTC->clear ();

    this->updateKategoriList(SerikBLDCore::Stok::StokKategori(this->user ()->Birimi ()));

}

void v2::StokManagerPage::onList(const QVector<SerikBLDCore::Stok::Stok> *mlist)
{

    Content ()->setMargin (25,Side::Top|Side::Bottom);
    Content ()->clear ();

    auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::col_full_12);
    container->setMargin (5,Side::Top|Side::Bottom);

    container->addWidget (cpp14::make_unique<WText>("Teslim Alınan Yardımlar"));
    container->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);

    for( auto item : *mlist )
    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6+
                                  Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (5,Side::Top|Side::Bottom);
        auto hLayout = container->setLayout (cpp14::make_unique<WHBoxLayout>());
        auto nameText = hLayout->addWidget (cpp14::make_unique<WText>(item.adi ()),0,AlignmentFlag::Left);
        auto miktarText = hLayout->addWidget (cpp14::make_unique<WText>(std::to_string (item.miktar ())),0,AlignmentFlag::Right);
    }
}

void v2::StokManagerPage::onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist)
{
    mStokItemComboBox->clear ();
    mStokKodList.clear ();
    for( auto item : *mlist )
    {
        mStokKodList[item.adi ()] = item.oid ().value ();
        mStokItemComboBox->addItem (item.adi ());
    }
}

void v2::StokManagerPage::onList(const QVector<SerikBLDCore::Stok::StokKategori> *mlist)
{
    mKategoriComboBox->clear ();
    mKategoriList.clear ();
    for( auto item : *mlist )
    {
        mKategoriComboBox->addItem (item.kategoriName ());
        mKategoriList[item.kategoriName ()] = item.oid ().value ();
    }
}

void v2::StokManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (errorText,"hata");
}





















v2::StokManagerCikisPage::StokManagerCikisPage(const std::string &stokTitle, SerikBLDCore::User *_mUser)
    :ContainerWidget (stokTitle),
      SerikBLDCore::Stok::StokManager (_mUser),
      mTCManager( new SerikBLDCore::TCManagerV2(_mUser->getDB ())),
      mCurrentTC( new SerikBLDCore::TC())
{


    {
        auto container = Header() ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_12+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto mTcLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mTcLineEdit->setPlaceholderText ("TCNO ile Ara ( 11 Hane ) ");

        mTcLineEdit->textInput ().connect ([=](){

            mTCViewWidget->Header ()->clear ();
            mTCViewWidget->Content ()->clear ();
            mTCViewWidget->Footer ()->clear ();

            mCurrentTC->clear ();


            if( mTcLineEdit->text ().toUTF8 ().size () < 11 )
            {
                return;
            }

            if( mTcLineEdit->text ().toUTF8 ().size () > 11 )
            {
                this->showPopUpMessage ("Telefon Numarası Hatalı","hata");
                return;
            }

            auto mtc = mTCManager->Load_byTCNO (mTcLineEdit->text ().toUTF8 ());

            if( mtc )
            {
                mCurrentTC = mtc.value ();
                mTCViewWidget->initTCView (mCurrentTC);
            }else{
                this->showPopUpMessage ("TC Bilgileri Bulunamadı","hata");
            }


        });
    }



    {
        auto container = Header() ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_12+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto mTelefonLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mTelefonLineEdit->setPlaceholderText ("Cep Telefonu Numarası İle Ara ( 11 Hane ) ");

        mTelefonLineEdit->textInput ().connect ([=](){

            mTCViewWidget->Header ()->clear ();
            mTCViewWidget->Content ()->clear ();
            mTCViewWidget->Footer ()->clear ();

            mCurrentTC->clear ();


            if( mTelefonLineEdit->text ().toUTF8 ().size () < 11 )
            {
                return;
            }

            if( mTelefonLineEdit->text ().toUTF8 ().size () > 11 )
            {
                this->showPopUpMessage ("Telefon Numarası Hatalı","hata");
                return;
            }

            auto mtc = mTCManager->Load_byTEL (mTelefonLineEdit->text ().toUTF8 ());

            if( mtc )
            {
                mCurrentTC = mtc.value ();
                mTCViewWidget->initTCView (mCurrentTC);
                this->teslimler ();
            }else{
                this->showPopUpMessage ("TC Bilgileri Bulunamadı","hata");
            }


        });
    }



    {
        mTCViewWidget = Header() ->addWidget (cpp14::make_unique<VatandasWidget>(this->user ()->getDB (),mCurrentTC));
        mTCViewWidget->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_12+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);

        mTCViewWidget->Header ()->clear ();
        mTCViewWidget->Content ()->clear ();
        mTCViewWidget->Footer ()->clear ();

        mTCViewWidget->setMargin ( 15 , Side::Top|Side::Bottom );
    }


    {
        auto _SearchContentContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        _SearchContentContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_9+
                                               Bootstrap::Grid::Medium::col_md_9+
                                               Bootstrap::Grid::Small::col_sm_12+
                                               Bootstrap::Grid::ExtraSmall::col_xs_12);


        mSearchContentContainer = _SearchContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mSearchContentContainer->addStyleClass (Bootstrap::Grid::row);

        mSearchContentContainer->setMargin ( 15 , Side::Top|Side::Bottom );



    }



    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mKategoriComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
        mKategoriComboBox->changed ().connect ([=](){
            this->updateStokKodList (SerikBLDCore::Stok::StokKodItem().setKategoriOid (mKategoriList[mKategoriComboBox->currentText ().toUTF8 ()]));
        });
    }



    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mStokItemComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
    }



    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_12+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto listeleBtn = container->addWidget (cpp14::make_unique<WPushButton>("Listele"));
        listeleBtn->addStyleClass (Bootstrap::Button::Primary);

        listeleBtn->clicked ().connect ([=](){
            SerikBLDCore::Stok::Stok stokItem(SerikBLDCore::Stok::Stok::Giris::giris,this->user ()->Birimi ());
            stokItem.setKodu (mStokKodList[mStokItemComboBox->currentText ().toUTF8 ()]);
            this->updateStokList (stokItem);
        });
    }


    {
        auto container = mSearchContentContainer ->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mTeslimAlinanContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        mTeslimAlinanContainer->addStyleClass (Bootstrap::Grid::row);
        mTeslimAlinanContainer->addStyleClass ("boxShadow boxRadius");
        mTeslimAlinanContainer->setMargin (25,Side::Bottom|Side::Top);
    }


    mCurrentTC->clear ();

    this->updateKategoriList(SerikBLDCore::Stok::StokKategori(this->user ()->Birimi ()));

}

void v2::StokManagerCikisPage::onList(const QVector<SerikBLDCore::Stok::Stok> *mlist)
{
    Content ()->setMargin (25,Side::Top|Side::Bottom);
    Content ()->clear ();

    auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::col_full_12);
    container->setMargin (5,Side::Top|Side::Bottom);

    container->addWidget (cpp14::make_unique<WText>("<h4>Mevcut Stok</h4>",TextFormat::UnsafeXHTML));
    container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    for( auto item : *mlist )
    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6+
                                  Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (5,Side::Top|Side::Bottom);

        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);

        auto adText = rContainer->addWidget (cpp14::make_unique<WText>(item.adi ()));
        adText->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                               Bootstrap::Grid::Medium::col_md_6+
                               Bootstrap::Grid::Small::col_sm_12+
                               Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto miktarText = rContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.miktar ())));
        miktarText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                               Bootstrap::Grid::Medium::col_md_2+
                               Bootstrap::Grid::Small::col_sm_3+
                               Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto kalanmiktarText = rContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.kalanMiktar ())));
        kalanmiktarText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                               Bootstrap::Grid::Medium::col_md_2+
                               Bootstrap::Grid::Small::col_sm_3+
                               Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto teslimEtBtn = rContainer->addWidget (cpp14::make_unique<WPushButton>("Teslim Et"));
        teslimEtBtn->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                               Bootstrap::Grid::Medium::col_md_2+
                               Bootstrap::Grid::Small::col_sm_6+
                               Bootstrap::Grid::ExtraSmall::col_xs_6);
        teslimEtBtn->addStyleClass (Bootstrap::Button::Default);

        teslimEtBtn->clicked ().connect ([=](){
                if( mCurrentTC->view ().empty () ){
                    this->showPopUpMessage ("TC Bilgilerini Girmediniz","hata");
                    return;
                }

                if( item.kalanMiktar () <= 0 )
                {
                    this->showPopUpMessage ("Bu Malzemeden Kalmadı","hata");
                    return;
                }

                auto mDialog = createDialog (mCurrentTC->AdSoyad ().toStdString () + " Teslimat Bilgileri");



                auto addBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Teslim Et"));
                addBtn->addStyleClass (Bootstrap::Button::Primary);


                addBtn->clicked ().connect ([=](){

                    SerikBLDCore::Stok::Stok::TeslimAlan teslim;

                    teslim.miktar = 1;
                    teslim.julianDay = QDate::currentDate ().toJulianDay ();
                    teslim.mSecSinceStartOfDay = QTime::currentTime ().msecsSinceStartOfDay ();
                    teslim.teslimAlan = mCurrentTC->oid ().value ().to_string ();

                    auto _item = static_cast<SerikBLDCore::Stok::Stok>(item);

                    _item.addTeslimAlan (teslim);

                    if( SerikBLDCore::ListItem<SerikBLDCore::Stok::Stok>::UpdateItem ( _item ) )
                    {
                        this->showPopUpMessage ("Yardım Teslim Edildi","msg");
                        this->remogeDialog (mDialog);
                        teslimler ();
                        SerikBLDCore::Stok::Stok stokItem(SerikBLDCore::Stok::Stok::Giris::giris,this->user ()->Birimi ());
                        stokItem.setKodu (mStokKodList[mStokItemComboBox->currentText ().toUTF8 ()]);
                        this->updateStokList (stokItem);
                    }else{
                        this->showPopUpMessage ("Bir Hata Oluştu","hata");
                    }



                });


                mDialog->show ();


        });


    }
}

void v2::StokManagerCikisPage::onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist)
{
    mStokItemComboBox->clear ();
    mStokKodList.clear ();
    for( auto item : *mlist )
    {
        mStokKodList[item.adi ()] = item.oid ().value ();
        mStokItemComboBox->addItem (item.adi ());
    }
}

void v2::StokManagerCikisPage::onList(const QVector<SerikBLDCore::Stok::StokKategori> *mlist)
{
    mKategoriComboBox->clear ();
    mKategoriList.clear ();
    for( auto item : *mlist )
    {
        mKategoriComboBox->addItem (item.kategoriName ());
        mKategoriList[item.kategoriName ()] = item.oid ().value ();
    }
}

void v2::StokManagerCikisPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (errorText,"hata");
}

void v2::StokManagerCikisPage::teslimler()
{


    mTeslimAlinanContainer->clear ();

    auto filter = document{};

    try {
        filter.append (kvp(SerikBLDCore::Stok::StokKey::teslimAlan,make_document(kvp("$elemMatch",make_document(kvp(SerikBLDCore::Stok::StokKey::teslimAlan,mCurrentTC->oid ().value ()))))));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        this->showPopUpMessage (str,"hata");
        return;
    }


    try {
        auto cursor = this->user ()->db ()->collection (SerikBLDCore::Stok::StokKey::Collection).find (filter.view ());

        for( auto item : cursor )
        {



            SerikBLDCore::Stok::Stok stokItem;
            stokItem.setDocumentView (item);

            for( auto _teslim : stokItem.Teslimler () )
            {
                auto container = mTeslimAlinanContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_4+
                                          Bootstrap::Grid::ExtraSmall::col_xs_6+
                                          Bootstrap::ImageShape::img_thumbnail);

                container->addWidget (cpp14::make_unique<WText>(stokItem.adi () + " : <b>" + std::to_string (_teslim.miktar)+"</b>",TextFormat::UnsafeXHTML));
                container->addWidget (cpp14::make_unique<WBreak>());

                container->addWidget (cpp14::make_unique<WText>(QDate::fromJulianDay (_teslim.julianDay).toString ("dd/MM/yyyy").toStdString ()));
                container->addWidget (cpp14::make_unique<WBreak>());
                container->setAttributeValue (Style::style,Style::font::size::s10px);
            }


        }


    } catch (mongocxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        this->showPopUpMessage (str,"hata");
        return;
    }


}
