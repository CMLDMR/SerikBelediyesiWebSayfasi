#include "calismamanagercontainer.h"



v2::CalismaManagerContainer::CalismaManagerContainer(SerikBLDCore::User *_user)
    :SerikBLDCore::CalismaManager ( _user->getDB () ),mUser(_user)
{

    Header ()->setMargin (15,Side::Bottom);
    initHeader ();
}

void v2::CalismaManagerContainer::initHeader()
{

    Header ()->clear ();


    {
        auto container = Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  Bootstrap::Grid::Offset::Large::col_lg_9+
                                  Bootstrap::Grid::Offset::Medium::col_md_9+
                                  Bootstrap::Grid::Offset::Small::col_sm_8+
                                  Bootstrap::Grid::Offset::ExtraSmall::col_xs_7);
        container->addWidget (cpp14::make_unique<WText>("Yeni Ekle"));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (CSSStyle::Button::blueButton);
        container->clicked ().connect (this,&v2::CalismaManagerContainer::initYeniCalismaEkle);
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
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mMiktar = container->addWidget (cpp14::make_unique<WDoubleSpinBox>());
        mMiktar->setMaximum (999999);
    }

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
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

            for( auto imgOid : mUploadedPictureList ){
                auto fileOid = this->uploadfile (imgOid.c_str ());
                calismaItem.addPhoto (fileOid.get_oid ().value);
            }

            if( this->InsertItem (calismaItem).size () ){
                this->showPopUpMessage ("Çalışma Kayıt Edildi");
            }else{
                this->showPopUpMessage ("Eksik Bilgi. Çalışma Kayıt Edilemedi!","warn");
            }


        });
    }
}
