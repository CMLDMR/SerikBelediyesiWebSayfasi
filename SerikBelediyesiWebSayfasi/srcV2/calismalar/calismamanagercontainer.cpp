#include "calismamanagercontainer.h"



v2::CalismaManagerContainer::CalismaManagerContainer(SerikBLDCore::User *_user)
    :SerikBLDCore::CalismaManager ( _user->getDB () ),mUser(_user)
{

    Header ()->setMargin (15,Side::Bottom);
    initHeader ();


    this->initFooter ();
}

void v2::CalismaManagerContainer::onList(const QVector<SerikBLDCore::Calisma> *mlist)
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
            this->UpdateList (SerikBLDCore::Calisma().setBirim (mUser->Birimi ()));
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
                                  Bootstrap::Grid::ExtraSmall::col_xs_4+
                                  Bootstrap::Grid::Offset::Large::col_lg_3+
                                  Bootstrap::Grid::Offset::Medium::col_md_3);
        container->addWidget (cpp14::make_unique<WText>("Yeni Ekle"));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (CSSStyle::Button::blueButton);
        container->clicked ().connect (this,&v2::CalismaManagerContainer::initYeniCalismaEkle);
    }

}

void v2::CalismaManagerContainer::initFooter()
{
    this->Footer ()->setMargin (25,Side::Top);

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

            auto filter = SerikBLDCore::Calisma();
            filter.setBirim (mUser->Birimi ());

            this->back (filter);

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

            auto filter = SerikBLDCore::Calisma();
            filter.setBirim (mUser->Birimi ());

            this->next (filter);

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

        if( mUser->Birimi () == "Fen İşleri Müdürlüğü" ){
            TipfilterComboBox->addItem ("NULL");
            TipfilterComboBox->addItem ("Sathi Asfalt Kaplama");
            TipfilterComboBox->addItem ("Suçla Asfalt");
            TipfilterComboBox->addItem ("Kaldırım");
            TipfilterComboBox->addItem ("Yol Reglaj");
            TipfilterComboBox->addItem ("Adet");
            TipfilterComboBox->addItem ("İmar Yolu Açımı");
            TipfilterComboBox->addItem ("Büz");
            TipfilterComboBox->addItem ("Yama");
            TipfilterComboBox->addItem ("Kilit Taşı Tamiratı");
            TipfilterComboBox->addItem ("Sıcak Asfalt");
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
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto ayrintiliBtn = container->addWidget (cpp14::make_unique<WPushButton>("Ayrıntılı Listele"));
        ayrintiliBtn->addStyleClass (Bootstrap::Button::Primary);
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto sayisalBtn = container->addWidget (cpp14::make_unique<WPushButton>("Sayısal Listele"));
        sayisalBtn->addStyleClass (Bootstrap::Button::Primary);
    }
}

void v2::CalismaManagerContainer::initYeniCalismaEkle()
{


    Content ()->clear ();

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mCalismaAdiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mCalismaAdiLineEdit->setPlaceholderText ("Çalışma Adını Giriniz");
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mMahalleComboBox = container->addWidget (cpp14::make_unique<WComboBox>());

        auto mahList = this->getMahalleler ();

        for( auto mah : mahList ){
            mMahalleComboBox->addItem (mah.toStdString ());
        }
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

        if( mUser->Birimi () == "Fen İşleri Müdürlüğü" ){
            mTipComboBox->addItem ("Sathi Asfalt Kaplama");
            mTipComboBox->addItem ("Suçla Asfalt");
            mTipComboBox->addItem ("Kaldırım");
            mTipComboBox->addItem ("Yol Reglaj");
            mTipComboBox->addItem ("Adet");
            mTipComboBox->addItem ("İmar Yolu Açımı");
            mTipComboBox->addItem ("Büz");
            mTipComboBox->addItem ("Yama");
            mTipComboBox->addItem ("Kilit Taşı Tamiratı");
            mTipComboBox->addItem ("Sıcak Asfalt");
        }




    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        mBirimComboBox = container->addWidget (cpp14::make_unique<WComboBox>());

        mBirimComboBox->addItem ("Metre");
        mBirimComboBox->addItem ("KM");
        mBirimComboBox->addItem ("TON");
        mBirimComboBox->addItem ("KG");
        mBirimComboBox->addItem ("Adet");
        mBirimComboBox->addItem ("M2");
        mBirimComboBox->addItem ("M3");
        mBirimComboBox->addItem ("Dekar");
        mBirimComboBox->addItem ("Tül");

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

            if( mBirimComboBox->currentText () == "NULL" ){
                this->showPopUpMessage ("Mahalle Seçmediniz!","msg");
                return;
            }

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


            SerikBLDCore::Calisma calismaItem;

            calismaItem.setAciklama (mAciklamaTextEdit->text ().toUTF8 ());
            calismaItem.setJulianDay (mDateEdit->date ().toJulianDay ());
            calismaItem.setMahalle (mMahalleComboBox->currentText ().toUTF8 ());
            calismaItem.setBirim (mUser->Birimi ());
            calismaItem.setCalimaAdi (mCalismaAdiLineEdit->text ().toUTF8 ());
            calismaItem.setisBirimi (mBirimComboBox->currentText ().toUTF8 ());
            calismaItem.setMiktar( mMiktar->value () );
            calismaItem.setIsTipi (mTipComboBox->currentText ().toUTF8 ());

            for( auto imgOid : mUploadedPictureList ){
                auto fileOid = this->uploadfile (imgOid.c_str ());
                calismaItem.addPhoto (fileOid.get_oid ().value);
            }

            if( this->InsertItem (calismaItem).size () ){
                this->showPopUpMessage ("Çalışma Kayıt Edildi");

                SerikBLDCore::Calisma calismaFilter;
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
      SerikBLDCore::Calisma filter;
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
}
