#include "vatandaswidget.h"



VatandasWidget::VatandasWidget(SerikBLDCore::DB *db, SerikBLDCore::TC *tc)
    :ContainerWidget ("Bilgilerim"),
      SerikBLDCore::TCManager (db),
      mTC(tc)
{

    this->initTC ();



}

void VatandasWidget::initChangeTC()
{
    this->Content ()->clear ();
    this->Content ()->setContentAlignment (AlignmentFlag::Center);
    this->Content ()->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    this->Content ()->setWidth (WLength("100%"));
    auto photoUploadWidget = this->Content ()->addWidget (cpp14::make_unique<FileUploaderWidget>(this->db (),"Resim Yükle"));
    photoUploadWidget->setType (FileUploaderWidget::Image);

    auto tcLineEdit = Content ()->addWidget (cpp14::make_unique<WLineEdit>());
    tcLineEdit->setWidth (WLength("100%"));
    tcLineEdit->setPlaceholderText ("TCNO Giriniz");
    tcLineEdit->setText (mTC->CepTelefonu ().toStdString ());

    auto adsoyadLineEdit = Content ()->addWidget (cpp14::make_unique<WLineEdit>());
    adsoyadLineEdit->setWidth (WLength("100%"));
    adsoyadLineEdit->setPlaceholderText ("Adınızı Soyadınızı Giriniz");
    adsoyadLineEdit->setText (mTC->AdSoyad ().toStdString ());

    auto telefonLineEdit = Content ()->addWidget (cpp14::make_unique<WLineEdit>());
    telefonLineEdit->setWidth (WLength("100%"));
    telefonLineEdit->setPlaceholderText ("Cep Telefonunuzu Giriniz");
    telefonLineEdit->setText (mTC->CepTelefonu ().toStdString ());

    auto mahalleComboBox = Content ()->addWidget (cpp14::make_unique<WComboBox>());
    mahalleComboBox->setWidth (WLength("100%"));
    auto mahList = this->getMahalleler ();
    int __currentMahalle = 0;
    for( auto item : mahList )
    {
        mahalleComboBox->addItem (item.toStdString ());
    }

    for( auto item : mahList )
    {
        if( mTC->Mahalle () == item )
        {
            mahalleComboBox->setCurrentIndex (__currentMahalle);
            break;
        }
        __currentMahalle++;
    }

    auto adreslineEdit = Content ()->addWidget (cpp14::make_unique<WLineEdit>());
    adreslineEdit->setPlaceholderText ("Adresinizi Giriniz");
    adreslineEdit->setText (mTC->TamAdres ().toStdString ());
    adreslineEdit->setWidth (WLength("100%"));

    Footer ()->clear ();

    auto updateBtn = Footer ()->addWidget (cpp14::make_unique<WPushButton>("Güncelle"));
    updateBtn->addStyleClass (Bootstrap::Button::Primary);
    updateBtn->clicked ().connect ([=](){


        if( photoUploadWidget->isUploaded () )
        {
            auto fileOid = this->uploadfile (photoUploadWidget->fileLocation ());
            mTC->setFotoOid (fileOid.get_oid ().value.to_string ().c_str ());
        }


        if( mahalleComboBox->currentIndex () == 0 )
        {
            this->showMessage ("Uyarı","Mahalle Seçmediniz");
            return;
        }

        if( tcLineEdit->text ().toUTF8 ().size () != 11 )
        {
            this->showMessage ("Uyarı","TCNO Hatalı Lütfen Kontrol Ediniz");
            return;
        }

        if( adsoyadLineEdit->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","Ad Soyad Hatalı Lütfen Kontrol Ediniz");
            return;
        }

        if( adreslineEdit->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","Adres Hatalı Lütfen Kontrol Ediniz");
            return;
        }

        if( telefonLineEdit->text ().toUTF8 ().size () != 11 )
        {
            this->showMessage ("Uyarı","Cep Telefonu Hatalı Lütfen Kontrol Ediniz");
            return;
        }

        mTC->setTCNO (tcLineEdit->text ().toUTF8 ().c_str ());
        mTC->setAdSoyad (adsoyadLineEdit->text ().toUTF8 ().c_str ());
        mTC->setMahalle (mahalleComboBox->currentText ().toUTF8 ().c_str ());
        mTC->setCepTelefonu (telefonLineEdit->text ().toUTF8 ().c_str ());
        mTC->setTamAdress (adreslineEdit->text ().toUTF8 ().c_str ());
        if( this->updateTC (mTC) ){
            this->Content ()->clear ();
            this->Footer ()->clear ();

            this->initTC ();
        }else{
            this->showMessage ("Uyarı","Güncellenemedi");
        }
    });




}

void VatandasWidget::initTC()
{
    this->Header ()->clear ();

    {
        auto photoContainer = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
        photoContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        auto layout = photoContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        layout->addStretch(1);

        auto fotoUrl = this->downloadFileWeb (mTC->FotoOid ());

        if( fotoUrl == "NULL" )
        {
            auto img = layout->addWidget(cpp14::make_unique<WImage>(WLink("img/person.jpg")));
            img->setWidth (160);
            img->addStyleClass (Bootstrap::ImageShape::img_circle);
        }else{
            auto img = layout->addWidget(cpp14::make_unique<WImage>(WLink(fotoUrl)));
            img->setWidth (160);
            img->addStyleClass (Bootstrap::ImageShape::img_circle);
        }
        layout->addStretch(1);
    }


    // Diğer Bilgiler
    {
        auto container = this->Header ()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto isim = layout->addWidget(cpp14::make_unique<WText>(mTC->AdSoyad ().toStdString ()),0,AlignmentFlag::Center);
        isim->setAttributeValue(Style::style,Style::font::size::s18px);
        layout->addWidget(cpp14::make_unique<WText>(mTC->CepTelefonu ().toStdString ()),0,AlignmentFlag::Center);

        layout->addWidget(cpp14::make_unique<WText>(mTC->Mahalle ().toStdString ()),0,AlignmentFlag::Center);

        if( mTC->TamAdres ().isEmpty () )
        {
            auto text = layout->addWidget(cpp14::make_unique<WText>("<h6><b>Adres Eksik</b></h6>",TextFormat::UnsafeXHTML),0,AlignmentFlag::Center);
            text->addStyleClass (Bootstrap::Label::Danger);
        }else{
            layout->addWidget(cpp14::make_unique<WText>(mTC->TamAdres ().toStdString ()),0,AlignmentFlag::Center);
        }
    }

    {
        auto mSvBtn = this->Footer ()->addWidget (cpp14::make_unique<WPushButton>("Bilgilerimi Değiştir"));
        mSvBtn->addStyleClass (Bootstrap::Button::Primary);
        mSvBtn->clicked ().connect ( this , &VatandasWidget::initChangeTC );
    }
}
