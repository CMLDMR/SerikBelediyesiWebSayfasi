#include "calismamanagercontainer.h"



v2::CalismaManagerContainer::CalismaManagerContainer(SerikBLDCore::User *_user)
    :SerikBLDCore::CalismaManager ( _user->getDB () ),mUser(_user)
{

    mCalismaKategoriManager = new SerikBLDCore::CalismaKategoriManager(_user->getDB ());
    mCalismaKategoriManager->setLimit (1000);

    Header ()->setMargin (15,Side::Bottom);
    initHeader ();


    this->initFooter ();
}

void v2::CalismaManagerContainer::onList(const QVector<SerikBLDCore::Calisma::Calisma> *mlist)
{

    Content ()->clear ();


    for( auto item : *mlist ){
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addStyleClass (CSSStyle::Shadows::shadow8px);
        container->setMargin (10,Side::Top);
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
        container->decorationStyle ().setCursor (Cursor::PointingHand);

        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);

        auto baslikText = rContainer->addWidget (cpp14::make_unique<WText>(item.CalismaAdi ()));
        baslikText->addStyleClass (Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto mahalleText = rContainer->addWidget (cpp14::make_unique<WText>(item.Mahalle ()));
        mahalleText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto isTipiText = rContainer->addWidget (cpp14::make_unique<WText>(item.IsTipii ()));
        isTipiText->addStyleClass (Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto isBirimi = rContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.Miktar ())+" "+item.IsBirim ()));
        isBirimi->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto tarih = rContainer->addWidget (cpp14::make_unique<WText>(item.tarihText ()));
        tarih->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);

        container->clicked ().connect ([=](){
            this->loadCalisma (container->attributeValue (Style::dataoid).toUTF8 ());
        });
    }

}

void v2::CalismaManagerContainer::initHeader()
{

    Header ()->clear ();

    {
        auto container = Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->addWidget (cpp14::make_unique<WText>("Çalışmalar"));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (CSSStyle::Button::blueButton);
        container->clicked ().connect (this,[=](){
            mCurrentPage = CurrentPage::Calismalar;
            this->initFooter ();
            this->UpdateList (SerikBLDCore::Calisma::Calisma().setBirim (mUser->Birimi ()));
        });
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->addWidget (cpp14::make_unique<WText>("İstatistik"));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (CSSStyle::Button::blueButton);
        container->clicked ().connect (this,&v2::CalismaManagerContainer::initIstatistik );
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->addWidget (cpp14::make_unique<WText>("Kategoriler"));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (CSSStyle::Button::blueButton);
        container->clicked ().connect (this,&v2::CalismaManagerContainer::initKategoriler );
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->addWidget (cpp14::make_unique<WText>("Yeni Ekle"));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (CSSStyle::Button::redButton);
        container->clicked ().connect (this,&v2::CalismaManagerContainer::initYeniCalismaEkle);
    }


    {
        auto container = Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto filterComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
        filterComboBox->addItem ("Ekleme Tarihine Göre");
        filterComboBox->addItem ("Çalışma Tarihine Göre");
        filterComboBox->addItem ("Mahalleye Göre");
        filterComboBox->addItem ("Çalışma Tipine Göre");

        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->addStyleClass (CSSStyle::Button::blueButton);

        filterComboBox->sactivated ().connect ([=](const WString &filterStr ){

            mCurrentSortFilterStr = filterStr.toUTF8 ();

            SerikBLDCore::FindOptions findOptions;

            SerikBLDCore::Item sortDoc("sort");
            if( filterStr.toUTF8 () == "Ekleme Tarihine Göre" ){
                sortDoc.append("_id",-1);
            }

            if( filterStr.toUTF8 () == "Çalışma Tarihine Göre" ){
                sortDoc.append(SerikBLDCore::Calisma::Key::julianDate,-1);
            }

            if( filterStr.toUTF8 () == "Mahalleye Göre" ){
                sortDoc.append(SerikBLDCore::Calisma::Key::mahalle,1);
            }

            if( filterStr.toUTF8 () == "Çalışma Tipine Göre" ){
                sortDoc.append(SerikBLDCore::Calisma::Key::isTipi,1);
            }

            findOptions.setSort (sortDoc);

            if( mCurrentMahalleFilterStr != "NULL" ){
                this->UpdateList (SerikBLDCore::Calisma::Calisma().setBirim (mUser->Birimi ()).setMahalle (mCurrentMahalleFilterStr),findOptions);
            }else{
                this->UpdateList (SerikBLDCore::Calisma::Calisma().setBirim (mUser->Birimi ()),findOptions);
            }


        });

    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto filterComboBox = container->addWidget (cpp14::make_unique<WComboBox>());

        for( const auto &mahalle : this->getMahalleler () ){
            filterComboBox->addItem (mahalle.toStdString ());
        }

        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->addStyleClass (CSSStyle::Button::blueButton);

        filterComboBox->sactivated ().connect ([=](const WString &filterStr ){

            mCurrentMahalleFilterStr = filterStr.toUTF8 ();


            SerikBLDCore::FindOptions findOptions;

            SerikBLDCore::Item sortDoc("sort");
            if( mCurrentSortFilterStr == "Ekleme Tarihine Göre" ){
                sortDoc.append("_id",-1);
            }

            if( mCurrentSortFilterStr == "Çalışma Tarihine Göre" ){
                sortDoc.append(SerikBLDCore::Calisma::Key::julianDate,-1);
            }

            if( mCurrentSortFilterStr == "Mahalleye Göre" ){
                sortDoc.append(SerikBLDCore::Calisma::Key::mahalle,1);
            }

            if( mCurrentSortFilterStr == "Çalışma Tipine Göre" ){
                sortDoc.append(SerikBLDCore::Calisma::Key::isTipi,1);
            }

            findOptions.setSort (sortDoc);



            if( mCurrentMahalleFilterStr != "NULL" ){
                this->UpdateList (SerikBLDCore::Calisma::Calisma().setBirim (mUser->Birimi ()).setMahalle (filterStr.toUTF8 ()),findOptions);
            }else{
                this->UpdateList (SerikBLDCore::Calisma::Calisma().setBirim (mUser->Birimi ()),findOptions);

            }
        });

    }

}

void v2::CalismaManagerContainer::initFooter()
{
    this->Footer ()->clear ();
    this->Footer ()->setMargin (25,Side::Top|Side::Bottom);

    if( mCurrentPage == CurrentPage::YeniEkle || mCurrentPage == CurrentPage::Kategoriler ){
        return;
    }

    {
        auto backBtn = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        backBtn->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                Bootstrap::Grid::Medium::col_md_2+
                                Bootstrap::Grid::Small::col_sm_3+
                                Bootstrap::Grid::ExtraSmall::col_xs_5);
        auto text = backBtn->addWidget (cpp14::make_unique<WText>("Geri"));
        backBtn->addStyleClass (CSSStyle::Button::blueButton);
        backBtn->decorationStyle ().setCursor (Cursor::PointingHand);
        backBtn->clicked ().connect ([=](){



            switch (mCurrentPage) {
            case CurrentPage::Calismalar:
            {
                auto filter = SerikBLDCore::Calisma::Calisma();
                filter.setBirim (mUser->Birimi ());
                this->back (filter);

            }

            case CurrentPage::Istatistik:
            {
//                auto filter = SerikBLDCore::Calisma::Calisma();
//                filter.setBirim (mUser->Birimi ());
//                this->back (filter);
            }
                break;
            default:
                break;
            }

        });
    }

    {
        auto backBtn = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        backBtn->addStyleClass (Bootstrap::Grid::Offset::Large::col_lg_8+
                                Bootstrap::Grid::Offset::Medium::col_md_8+
                                Bootstrap::Grid::Small::col_sm_6+
                                Bootstrap::Grid::ExtraSmall::col_xs_2+
                                Bootstrap::Grid::Large::col_lg_2+
                                Bootstrap::Grid::Medium::col_md_2+
                                Bootstrap::Grid::Small::col_sm_3+
                                Bootstrap::Grid::ExtraSmall::col_xs_5);
        auto text = backBtn->addWidget (cpp14::make_unique<WText>("İleri"));
        backBtn->addStyleClass (CSSStyle::Button::blueButton);
        backBtn->decorationStyle ().setCursor (Cursor::PointingHand);
        backBtn->clicked ().connect ([=](){


            switch (mCurrentPage) {
            case CurrentPage::Calismalar:
            {
                auto filter = SerikBLDCore::Calisma::Calisma();
                filter.setBirim (mUser->Birimi ());
                this->next (filter);

            }

            case CurrentPage::Istatistik:
            {
//                auto filter = SerikBLDCore::Calisma::Calisma();
//                filter.setBirim (mUser->Birimi ());
//                this->next (filter);
            }
                break;
            default:
                break;
            }

        });
    }
}

void v2::CalismaManagerContainer::initIstatistik()
{
    Content ()->clear ();

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto mahComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
        for( auto mahItem : this->getMahalleler () ){
            mahComboBox->addItem (mahItem.toStdString ());
        }
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto TipfilterComboBox = container->addWidget (cpp14::make_unique<WComboBox>());


        auto list = mCalismaKategoriManager->UpdateList (SerikBLDCore::Calisma::Kategori().setKategoriMudurluk (mUser->Birimi ()));

        for( const auto &item : list ){
            TipfilterComboBox->addItem (item.getKategoriAdi ()); //KM
        }

    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto DateFilterStartDate = container->addWidget (cpp14::make_unique<WDateEdit>());
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto DateFilterEndDate = container->addWidget (cpp14::make_unique<WDateEdit>());
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto ListelemefilterComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
        ListelemefilterComboBox->addItem ("Ayrıntılı");
        ListelemefilterComboBox->addItem ("Sayısal");
    }
}

void v2::CalismaManagerContainer::initKategoriler()
{
    mCurrentPage = CurrentPage::Kategoriler;
    this->initFooter ();
    Content ()->clear ();


        auto kateogoriAdicontainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        kateogoriAdicontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5);
        auto kategoriAdiLineEdit = kateogoriAdicontainer->addWidget (cpp14::make_unique<WLineEdit>());
        kategoriAdiLineEdit->setPlaceholderText ("Kategori Adını Giriniz");


        auto kateogoriBirimcontainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        kateogoriBirimcontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5);
        auto kategoriBirimComboBox = kateogoriBirimcontainer->addWidget (cpp14::make_unique<WComboBox>());
        kategoriBirimComboBox->addItem ("Metre");  //0
        kategoriBirimComboBox->addItem ("KM");     //1
        kategoriBirimComboBox->addItem ("TON");    //2
        kategoriBirimComboBox->addItem ("KG");     //3
        kategoriBirimComboBox->addItem ("Adet");   //4
        kategoriBirimComboBox->addItem ("M2");     //5
        kategoriBirimComboBox->addItem ("M3");     //6
        kategoriBirimComboBox->addItem ("Dekar");  //7
        kategoriBirimComboBox->addItem ("Tül");    //8

        auto kateogoriEkleContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        kateogoriEkleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        auto kateogoriEkleBtn = kateogoriEkleContainer->addWidget (cpp14::make_unique<WPushButton>("Ekle"));


        auto contentContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        contentContainer->addStyleClass (Bootstrap::Grid::col_full_12);


        kateogoriEkleBtn->clicked ().connect ([=](){

            SerikBLDCore::Calisma::Kategori mKategori;
            mKategori.setKategoriAdi (kategoriAdiLineEdit->text ().toUTF8 ());
            mKategori.setKategoriBirim (kategoriBirimComboBox->currentText ().toUTF8 ());
            mKategori.setKategoriMudurluk (mUser->Birimi ());

            if( mCalismaKategoriManager->InsertItem (mKategori).empty () ){
                this->showPopUpMessage ("Kategori Eklenemedi","err");
                contentContainer->clear ();
            }else{

                auto list = mCalismaKategoriManager->UpdateList (SerikBLDCore::Calisma::Kategori().setKategoriMudurluk (mUser->Birimi ()));

                contentContainer->clear ();

                for( const auto &item : list ){

                    auto rContentContainer = contentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    rContentContainer->addStyleClass (Bootstrap::Grid::row);
                    rContentContainer->addStyleClass (CSSStyle::Shadows::shadow8px);
                    rContentContainer->setMargin (15,Side::Top|Side::Bottom);
                    rContentContainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
                    {
                        auto container = rContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                                  Bootstrap::Grid::Medium::col_md_6+
                                                  Bootstrap::Grid::Small::col_sm_6+
                                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
                        auto text = container->addWidget (cpp14::make_unique<WText>(item.getKategoriAdi ()));
                    }

                    {
                        auto container = rContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                                  Bootstrap::Grid::Medium::col_md_4+
                                                  Bootstrap::Grid::Small::col_sm_4+
                                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
                        auto text = container->addWidget (cpp14::make_unique<WText>(item.getKategoriBirimi ()));
                    }

                    {
                        auto container = rContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                                  Bootstrap::Grid::Medium::col_md_2+
                                                  Bootstrap::Grid::Small::col_sm_2+
                                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
                        auto delBtn = container->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
                        container->addStyleClass (CSSStyle::Button::redButton);
                        container->clicked ().connect ([=](){
                           auto btn = this->askConfirm ("Silmek İstediğinize Eminmisiniz?");
                           btn->clicked ().connect ([=](){

                               auto filter = SerikBLDCore::Calisma::Kategori();
                               filter.setOid (rContentContainer->attributeValue (Style::dataoid).toUTF8 ());
                               if( this->mCalismaKategoriManager->DeleteItem (filter) ){
                                   this->showPopUpMessage ("Silindi");
                                   contentContainer->removeWidget(rContentContainer);
                               }else{
                                   this->showPopUpMessage ("Bir Hata Oluştu. Kategori Silinemedi","err");
                               }

                           });
                        });
                    }


                }

            }


        });

        {
            auto list = mCalismaKategoriManager->UpdateList (SerikBLDCore::Calisma::Kategori().setKategoriMudurluk (mUser->Birimi ()));

            contentContainer->clear ();

            for( const auto &item : list ){

                auto rContentContainer = contentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                rContentContainer->addStyleClass (Bootstrap::Grid::row);
                rContentContainer->addStyleClass (CSSStyle::Shadows::shadow8px);
                rContentContainer->setMargin (15,Side::Top|Side::Bottom);
                rContentContainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
                {
                    auto container = rContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                              Bootstrap::Grid::Medium::col_md_6+
                                              Bootstrap::Grid::Small::col_sm_6+
                                              Bootstrap::Grid::ExtraSmall::col_xs_6);
                    auto text = container->addWidget (cpp14::make_unique<WText>(item.getKategoriAdi ()));
                }

                {
                    auto container = rContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                              Bootstrap::Grid::Medium::col_md_4+
                                              Bootstrap::Grid::Small::col_sm_4+
                                              Bootstrap::Grid::ExtraSmall::col_xs_4);
                    auto text = container->addWidget (cpp14::make_unique<WText>(item.getKategoriBirimi ()));
                }

                {
                    auto container = rContentContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                              Bootstrap::Grid::Medium::col_md_2+
                                              Bootstrap::Grid::Small::col_sm_2+
                                              Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto delBtn = container->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
                    container->addStyleClass (CSSStyle::Button::redButton);
                    container->clicked ().connect ([=](){
                       auto btn = this->askConfirm ("Silmek İstediğinize Eminmisiniz?");
                       btn->clicked ().connect ([=](){

                           auto filter = SerikBLDCore::Calisma::Kategori();
                           filter.setOid (rContentContainer->attributeValue (Style::dataoid).toUTF8 ());
                           if( this->mCalismaKategoriManager->DeleteItem (filter) ){
                               this->showPopUpMessage ("Silindi");
                               contentContainer->removeWidget(rContentContainer);
                           }else{
                               this->showPopUpMessage ("Bir Hata Oluştu. Kategori Silinemedi","err");
                           }

                       });
                    });
                }


            }
        }



}

void v2::CalismaManagerContainer::initYeniCalismaEkle()
{
    mCurrentPage = CurrentPage::YeniEkle;
    this->initFooter ();

    Content ()->clear ();

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5);
        mCalismaAdiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mCalismaAdiLineEdit->setPlaceholderText ("Çalışma Adını Giriniz");
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5);
        mMahalleComboBox = container->addWidget (cpp14::make_unique<WComboBox>());

        auto mahList = this->getMahalleler ();

        for( auto mah : mahList ){
            mMahalleComboBox->addItem (mah.toStdString ());
        }
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        mIsKategoriComboBox = container->addWidget (cpp14::make_unique<WComboBox>());
        mIsKategoriComboBox->addItem ("Genel");
        mIsKategoriComboBox->addItem ("Kurum");
        mIsKategoriComboBox->addItem ("Okul");
        mIsKategoriComboBox->addItem ("Cami");
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        mMiktar = container->addWidget (cpp14::make_unique<WDoubleSpinBox>());
        mMiktar->setMaximum (999999);
    }


    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        mTipComboBox = container->addWidget (cpp14::make_unique<WComboBox>());

        auto birimcontainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        birimcontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        mBirimComboBox = birimcontainer->addWidget (cpp14::make_unique<WText>());


        auto list = mCalismaKategoriManager->UpdateList (SerikBLDCore::Calisma::Kategori().setKategoriMudurluk (mUser->Birimi ()));

        for( const auto &item : list ){
            mTipComboBox->addItem (item.getKategoriAdi ()); //KM
            mBirimComboBox->setText (item.getKategoriBirimi ());
        }

        mTipComboBox->sactivated ().connect ([=]( const WString& tipStr){
            auto item = mCalismaKategoriManager->FindOneItem (SerikBLDCore::Calisma::Kategori().setKategoriMudurluk (mUser->Birimi ()).setKategoriAdi (tipStr.toUTF8 ()));
            mBirimComboBox->setText (item.getKategoriBirimi ());

        });


    }



    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mDateEdit = container->addWidget (cpp14::make_unique<WDateEdit>());
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        mAciklamaTextEdit = container->addWidget (cpp14::make_unique<WTextEdit>());
        mAciklamaTextEdit->setHeight (250);
        auto aciklamaCharacterCountText = container->addWidget (cpp14::make_unique<WText>());
        mAciklamaTextEdit->changed ().connect ([=](){
            aciklamaCharacterCountText->setText (WString("{1} Karakter").arg (mAciklamaTextEdit->text ().toUTF8 ().size ()));
        });
    }


    {

        mResimAlaniContainerWidget = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        mResimAlaniContainerWidget->addStyleClass (Bootstrap::Grid::row);
        mResimAlaniContainerWidget->setMargin (15,Side::Top);


        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto fileUploader = container->addWidget (cpp14::make_unique<FileUploaderWidget>("Resim Yükle"));
        fileUploader->setType (FileUploaderWidget::FilterType::Image);
        fileUploader->Uploaded ().connect ([=](){

            auto filepath = fileUploader->doocRootLocation ();
            mUploadedPictureList.push_back (fileUploader->fileLocation ().toStdString ());

            auto pictureContainer = mResimAlaniContainerWidget->addWidget (cpp14::make_unique<WContainerWidget>());
            pictureContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                             Bootstrap::Grid::Medium::col_md_3+
                                             Bootstrap::Grid::Small::col_sm_4+
                                             Bootstrap::Grid::ExtraSmall::col_xs_6);
            pictureContainer->setMargin (5,Side::Top|Side::Bottom);
            pictureContainer->setHeight (150);
            pictureContainer->setAttributeValue (Style::style,Style::background::url (filepath.toStdString ())+
                                                 Style::background::size::contain+
                                                 Style::background::repeat::norepeat+
                                                 Style::background::position::center_center);
            pictureContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            pictureContainer->setPositionScheme (PositionScheme::Relative);
            auto closeContainer = pictureContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            closeContainer->setPositionScheme (PositionScheme::Absolute);
            closeContainer->setOffsets (0,Side::Top|Side::Right);
            closeContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)+
                                               Style::color::color (Style::color::White::AliceBlue));
            closeContainer->decorationStyle ().setCursor (Cursor::PointingHand);
            closeContainer->setAttributeValue (Style::customData,fileUploader->fileLocation ().toStdString ());
            auto closeText = closeContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
            closeText->setPadding (2,Side::Left|Side::Right);

            closeContainer->clicked ().connect ([=](){

                std::vector<std::string> tempList;

                for( auto item : mUploadedPictureList ){
                    if( item != closeContainer->attributeValue (Style::customData).toUTF8 () ){
                        tempList.push_back (item);
                    }
                }

                std::swap (mUploadedPictureList,tempList);

                mResimAlaniContainerWidget->removeWidget(pictureContainer);

            });


        });
    }



    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto mKaydetPushButton = container->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        mKaydetPushButton->addStyleClass (Bootstrap::Button::Primary);

        mKaydetPushButton->clicked ().connect ([=](){

            if( mMahalleComboBox->currentText ().toUTF8 () == "NULL" ){
                this->showPopUpMessage ("Mahalle Girmediniz","msg");
                return ;
            }


            if( mDateEdit->date ().toJulianDay () < 2456658 ){
                this->showPopUpMessage ("Tarih Hatalı!","msg");
                return;
            }

//            if( mBirimComboBox->currentText () == "NULL" ){
//                this->showPopUpMessage ("Mahalle Seçmediniz!","msg");
//                return;
//            }

            if( mCalismaAdiLineEdit->text ().toUTF8 ().size () < 6 ){
                this->showPopUpMessage ("Çalışma Adı Min 7 Karakter Olmalı","msg");
                return;
            }

            if( mAciklamaTextEdit->text ().toUTF8 ().size () < 49 ){
                this->showPopUpMessage ("Açıklama Min 50 Karakter Olmalı","msg");
                return;
            }

            if( mTipComboBox->currentText ().toUTF8 ().size () == 0 ){
                this->showPopUpMessage ("Çalışma Miktar Tipini Girmediniz!","msg");
                return;
            }


            if( mMiktar->value () <= 0.0 ){
                this->showPopUpMessage ("Miktar Girmedniz!","msg");
                return;
            }


            SerikBLDCore::Calisma::Calisma calismaItem;

            calismaItem.setAciklama (mAciklamaTextEdit->text ().toUTF8 ());
            calismaItem.setJulianDay (mDateEdit->date ().toJulianDay ());
            calismaItem.setMahalle (mMahalleComboBox->currentText ().toUTF8 ());
            calismaItem.setBirim (mUser->Birimi ());
            calismaItem.setCalimaAdi (mCalismaAdiLineEdit->text ().toUTF8 ());
            calismaItem.setisBirimi (mBirimComboBox->text ().toUTF8 ());
            calismaItem.setMiktar( mMiktar->value () );
            calismaItem.setIsTipi (mTipComboBox->currentText ().toUTF8 ());
            calismaItem.setIsKategorisi (mIsKategoriComboBox->currentText ().toUTF8 ());

            for( auto imgOid : mUploadedPictureList ){
                auto fileOid = this->uploadfile (imgOid.c_str ());
                calismaItem.addPhoto (fileOid.get_oid ().value);
            }

            if( this->InsertItem (calismaItem).size () ){
                this->showPopUpMessage ("Çalışma Kayıt Edildi");

                SerikBLDCore::Calisma::Calisma calismaFilter;
                calismaFilter.setBirim (mUser->Birimi ());

                this->UpdateList (calismaFilter);
            }else{
                this->showPopUpMessage ("Eksik Bilgi. Çalışma Kayıt Edilemedi!","warn");
            }


        });
    }
}

void v2::CalismaManagerContainer::loadCalisma(const std::string &calismaOid)
{
    SerikBLDCore::Calisma::Calisma filter;
    filter.setOid (calismaOid);

    auto calisma = this->FindOneItem (filter);

    std::vector<std::string> imgPath;

    auto imgOidlist = calisma.imgOidList ();

    for( auto item : imgOidlist ){
        auto path = this->downloadFileWeb (item.to_string ().c_str ());
        imgPath.push_back (path);
    }

    Content ()->clear ();

    auto calismaContainer = Content ()->addWidget (cpp14::make_unique<v2::CalismaContainerWidget>(imgPath));
    calismaContainer->setDocumentView (calisma.view ());
    calismaContainer->initCalismaWidget ();

    calismaContainer->deleteClicked ().connect ([=](const std::string& calismaOid , const std::vector<std::string> imgListOid){

        for( const auto &fileOid : imgListOid ){

            for( const auto &item : imgOidlist ){
                if( QString::fromStdString (fileOid).contains (QString::fromStdString (item.to_string ()))){
                    this->deleteGridFS (item.to_string ().c_str ());
                }
            }
        }

        SerikBLDCore::Calisma::Calisma filter;
        filter.setOid (calismaOid);
        if( this->DeleteItem (filter) ){
            this->UpdateList (SerikBLDCore::Calisma::Calisma().setBirim (mUser->Birimi ()));
        }
    });
}

v2::CalismaKategoriManager::CalismaKategoriManager(SerikBLDCore::User *_mUser)
:SerikBLDCore::CalismaKategoriManager ( _mUser->getDB () ),mUser(_mUser)
{

}

void v2::CalismaKategoriManager::onList(const QVector<SerikBLDCore::Calisma::Kategori> *mlist)
{

}
