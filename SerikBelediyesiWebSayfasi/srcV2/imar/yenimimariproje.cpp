#include "yenimimariproje.h"
//#include "SerikBelediyesiWebSayfasi/BaseClass/

v2::YeniMimariProje::YeniMimariProje(SerikBLDCore::DB* _db , const SerikBLDCore::TC *mTCUser, const SerikBLDCore::Firma::FirmaItem *mFirma)
    :ContainerWidget("Yeni Mimari Proje Başvurusu"),
      SerikBLDCore::Imar::MimariProje::MimariProje (mFirma->oid ().value (),WDate::currentDate ().toJulianDay ()),
      SerikBLDCore::DB(_db)
{




    auto projeAdiContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    projeAdiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                      Bootstrap::Grid::Medium::col_md_6+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);
    projeAdiContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto projeadiLineEdit = projeAdiContainer->addWidget (cpp14::make_unique<WLineEdit>());
    projeadiLineEdit->setPlaceholderText ("Proje Adını Giriniz");


    auto adaContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
    adaContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto adaLayout = adaContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
    adaLayout->addWidget (cpp14::make_unique<WText>("Ada NO"));
    auto adaSpinBox = adaLayout->addWidget (cpp14::make_unique<WSpinBox>());
    adaSpinBox->setRange (1,999999999);


    auto parselContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
    parselContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto parselLayout = parselContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
    parselLayout->addWidget (cpp14::make_unique<WText>("Parsel NO"));
    auto parselSpinBox = parselLayout->addWidget (cpp14::make_unique<WSpinBox>());
    parselSpinBox->setRange (1,999999999);



    auto projeMahalleContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    projeMahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                      Bootstrap::Grid::Medium::col_md_6+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);
    projeMahalleContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto projeMahalleComboBox = projeMahalleContainer->addWidget (cpp14::make_unique<WComboBox>());
    auto mahList = this->getDB ()->getMahalleler ();
    for( auto item : mahList ){
        projeMahalleComboBox->addItem (item.toStdString ());
    }


    auto projeMimariDosyaContainer = Header ()->addWidget (cpp14::make_unique<FileUploaderWidget>("DWG Yükle"));
    projeMimariDosyaContainer->setType (FileUploaderWidget::FilterType::DWG);
    projeMimariDosyaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                      Bootstrap::Grid::Medium::col_md_6+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);
    projeMimariDosyaContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);


    auto projeKaydetContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    projeKaydetContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    projeKaydetContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto projeKayitbtn= projeKaydetContainer->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    projeKayitbtn->addStyleClass (Bootstrap::Button::Primary);
    projeKayitbtn->clicked ().connect ([=](){

        this->setAdi (projeadiLineEdit->text ().toUTF8 ());
        this->setAda (adaSpinBox->value ());
        this->setParsel (parselSpinBox->value ());
        this->setMahalle (projeMahalleComboBox->currentText ().toUTF8 ());
        auto val = this->getDB ()->uploadfile (projeMimariDosyaContainer->fileLocation ());
        this->setDWGFileOid (val.get_oid ().value);
        this->setProjectAktif (false);

        auto ins = this->getDB ()->insertItem (*this);

        if( ins )
        {
            this->showPopUpMessage ("Proje Kayıt Edildi","msg");
            _backList.emit (NoClass());
        }else{
            this->showPopUpMessage ("Bir Hata Oluştu Proje Kayıt Edilemedi","err");
        }

    });


}

Signal<NoClass> &v2::YeniMimariProje::BackList()
{
    return _backList;
}
