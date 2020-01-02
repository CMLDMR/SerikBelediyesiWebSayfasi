#include "yenipersonelwidget.h"

YeniPersonelWidget::YeniPersonelWidget()
    :ContainerWidget ("Yeni Personel Ekle"),
      mPhotoFilePath("docroot/img/person.jpg")
{


    auto fContainer = Header()->addWidget (cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    auto fotoContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    fotoContainer->setMinimumSize (120,160);
    fotoContainer->setWidth (120);
    fotoContainer->setHeight (160);
    fotoContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    fotoContainer->setAttributeValue (Style::style,Style::background::url ("img/person.jpg")+
                                      Style::background::size::cover+
                                      Style::background::position::center_center+
                                      Style::background::repeat::norepeat);
    auto fileUploaderContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    fileUploaderContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    fileUploaderContainer->setContentAlignment (AlignmentFlag::Center);
    auto fileUploader = fileUploaderContainer->addWidget (cpp14::make_unique<FileUploaderWidget>("Resim Yükle"));
    fileUploader->setType (FileUploaderWidget::Image);

    fileUploader->Uploaded ().connect ([=](){

        mPhotoFilePath  = fileUploader->fileLocation ();

        fotoContainer->setAttributeValue (Style::style,Style::background::url (fileUploader->doocRootLocation ().toStdString ())+
                                          Style::background::size::cover+
                                          Style::background::position::center_center+
                                          Style::background::repeat::norepeat);

        fotoContainer->setMinimumSize (120,160);
        fotoContainer->setWidth (120);
        fotoContainer->setHeight (160);
    });

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        adsoyadLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        adsoyadLineEdit->setPlaceholderText ("Ad Soyad Giriniz");
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto birimLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        birimLineEdit->setPlaceholderText ("Geçerli Birim Başkanlık ( Kaydettikten Sonra Değiştiriniz )");
        birimLineEdit->setEnabled (false);
    }


    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        ofisPersonel = container->addWidget (cpp14::make_unique<WCheckBox>("Büro Personeli"));
        ofisPersonel->setChecked (true);
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        telefonLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        telefonLineEdit->setPlaceholderText ("Telefon Numrası Giriniz");
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto sifreLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        sifreLineEdit->setPlaceholderText ("Şifre: Personeli Kaydettikten Sonra Oluştur&Gönder Seçeneğini Kullanınız");
        sifreLineEdit->setEnabled (false);
    }


    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto statuLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        statuLineEdit->setPlaceholderText ("Geçerli Statu: Personel. Kaydettikten Sonra Değiştiriniz");
        statuLineEdit->setEnabled (false);
    }


    {
        auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto saveBtn = container->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Default);

        saveBtn->clicked ().connect (this,&YeniPersonelWidget::savePersonel );

    }



}

Signal<NoClass> &YeniPersonelWidget::PersonelSaved()
{

    return _personelSaved;

}

void YeniPersonelWidget::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (errorText,"hata");
}

QString YeniPersonelWidget::photoFilePath() const
{
    return mPhotoFilePath;
}

void YeniPersonelWidget::savePersonel()
{

    if( mPhotoFilePath.size () == 0 )
    {
        this->showPopUpMessage ("Resim Seçmediniz","hata");
        return;
    }
    SerikBLDCore::IK::Personel personel;
    this->setAdSoyad (adsoyadLineEdit->text ().toUTF8 ().c_str ());
    this->setTelefon (telefonLineEdit->text ().toUTF8 ().c_str ());
    this->setBuroPersoneli (ofisPersonel->isChecked ());
    this->setStatu (SerikBLDCore::IK::Statu::Personel.c_str ());
    this->setBirim ("Başkanlık");

    if( !SerikBLDCore::IK::Personel::checkTelefonFormat (telefonLineEdit->text ().toUTF8 ().c_str ()) )
    {
        this->showPopUpMessage ("Telefon Numarasını Düzeltiniz");
        return;
    }


    _personelSaved.emit (NoClass());

}
