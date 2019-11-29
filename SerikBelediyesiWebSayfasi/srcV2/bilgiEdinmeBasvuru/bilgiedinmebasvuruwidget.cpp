#include "bilgiedinmebasvuruwidget.h"

#include "bilgiedinmeitem.h"


BilgiEdinmeBasvuruWidget::BilgiEdinmeBasvuruWidget(SerikBLDCore::DB *_db)
    :ContainerWidget ("Yeni Bilgi Edinme Başvurusu"),SerikBLDCore::BilgiEdinmeManager(_db)
{
    this->initHeader ();

    this->initPublic ();
}

BilgiEdinmeBasvuruWidget::BilgiEdinmeBasvuruWidget(SerikBLDCore::DB *_db, const SerikBLDCore::TC *mTCUser)
    :ContainerWidget ("Yeni Başvuru"),SerikBLDCore::BilgiEdinmeManager(_db),mTC(mTCUser)
{
    this->initHeader ();

    this->initWithTC ();
}

void BilgiEdinmeBasvuruWidget::initHeader()
{
    this->Header ()->clear ();
    this->Header ()->setContentAlignment (AlignmentFlag::Center);
    this->Header ()->addWidget (cpp14::make_unique<WText>("Bilgi Edinme kanunun tanıdığı hakkı belediyemiz ile ilgili konularda kullanmak için İnternet üzerinden Bilgi Edinme Formlarını doldurmanız yeterli olacaktır."));
    {
        auto link1 = WLink(LinkType::Url,"file/4982-sayili-Bilgi-Edinme-Hakki-Kanunu.pdf");
        auto anchor = cpp14::make_unique<WAnchor>(link1,"<h4>4982 Sayılı Bilgi Edinme Hakkı Kanunu</h4>");
        this->Header ()->addWidget (std::move(anchor));
    }

    {
        auto link1 = WLink(LinkType::Url,"file/Bilgi-Edinme-Uygulamasina-Iliskin-Yonetmelik.pdf");
        auto anchor = cpp14::make_unique<WAnchor>(link1,"<h4>Bilgi Edinme Uygulamasına İlişkin Yönetmenlik</h4>");
        this->Header ()->addWidget (std::move(anchor));
    }
    this->Header ()->addWidget (cpp14::make_unique<WText>("Aşağıdaki Bilgileri Doldurarak Kaydet Tuşuna Basınız"));


}

void BilgiEdinmeBasvuruWidget::initWithTC()
{

    this->Content ()->clear ();

    auto vLayout = this->Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());

    auto adres = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    adres->setPlaceholderText ("Lütfen Adresinizi Giriniz");
    adres->setText (mTC->TamAdres ().toStdString ());
    vLayout->addWidget (cpp14::make_unique<WText>("<b>!Dikkat, Adresiniz Doğru Değilse Bilgilerim Bölümünden Adresinizi Güncelleyiniz</b>"),0,AlignmentFlag::Justify)
            ->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed));


    auto konu = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    konu->setPlaceholderText ("Konuyu Giriniz");


    auto mesajTextEdit = vLayout->addWidget (cpp14::make_unique<WTextEdit>(),0,AlignmentFlag::Justify);
    mesajTextEdit->setHeight (250);

    auto mesajTextCount = vLayout->addWidget (cpp14::make_unique<WText>("<b>Geçersiz Mesaj: 0 Karakter. Min 50 Karakter Olmalı</b>"),0,AlignmentFlag::Justify);
    mesajTextEdit->changed ().connect ([=](){
        if( mesajTextEdit->text ().toUTF8 ().size () < 50 )
        {
            mesajTextCount->setText (WString("<b>Geçersiz Mesaj: {1} Karakter. Min 50 Karakter Olmalı</b>").arg (mesajTextEdit->text ().toUTF8 ().size ()));
            mesajTextCount->setAttributeValue (Style::style,Style::color::color (Style::color::Red::FireBrick));
        }else{
            mesajTextCount->setText (WString("<b>Geçerli Mesaj: {1} Karakter</b>").arg (mesajTextEdit->text ().toUTF8 ().size ()));
            mesajTextCount->setAttributeValue (Style::style,Style::color::color (Style::color::Green::DarkGreen));
        }
    });

    this->Footer ()->addStyleClass (Bootstrap::ContextualBackGround::bg_warning);
    auto svBtn = this->Footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    svBtn->addStyleClass (Bootstrap::Button::Primary);

    this->Footer ()->addWidget (cpp14::make_unique<WText>("<br>Şikayet ve Taleplerinizi Lütfen İletişim Bölümünden Yapınız"
                                                          "<br><b>Bu Alandan Sadece Bilgi Edinme Amaçlı Başvurular Yapılabilir</b>"
                                                          "<br>Başvurularınız Cevabını Size Gösterilecek olan Linkten Yada Telefon Numarası ve Şifreniz İle Giriş Yaparak Takip Edebilirsiniz",TextFormat::UnsafeXHTML));





    svBtn->clicked ().connect ([=](){

        if( konu->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","Konu Kısmı Hatalı. Lütfen Kontrol Ediniz");
            return;
        }

        if( mesajTextEdit->text ().toUTF8 ().size () < 50 )
        {
            this->showMessage ("Uyarı","Mesajını 50 Karakterden Az Olamaz");
            return;
        }

        if( adres->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","Adres Kısmı Hatalı. Lütfen Kontrol Ediniz");
            return;
        }


        SerikBLDCore::BilgiEdinmeItem item;

        item.setTCoid (mTC->oid ().value ());
        item.setKonu (konu->text ().toUTF8 ().c_str ());
        item.setBirim (this->getMahalleler ().first ());
        item.setMesaj (mesajTextEdit->text ().toUTF8 ().c_str ());
        auto insertedItem = this->InsertItem (item);

        if( insertedItem.size ())
        {
            auto dialog = WApplication::instance ()->addChild (cpp14::make_unique<WDialog>());

            dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("Başvurunuz Başarılı"));
            dialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);

            auto container = dialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::row);

            {
                auto text = container->addWidget (cpp14::make_unique<WText>("Başvurunuzu"));
                text->addStyleClass (Bootstrap::Grid::col_full_12);
            }
            auto link = WLink(LinkType::Url,"?type=basvuru&_id="+insertedItem);
            link.setTarget (LinkTarget::NewWindow);
            auto anchor = cpp14::make_unique<WAnchor>(link,"http://www.serik.bel.tr/"+link.url ());
            anchor->addStyleClass (Bootstrap::Grid::col_full_12);
            container->addWidget (std::move(anchor));

            {
                auto text = container->addWidget (cpp14::make_unique<WText>("Adresinden Takip Edebilirsiniz"));
                text->addStyleClass (Bootstrap::Grid::col_full_12);
            }

            dialog->contents ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);


            auto clsBtn = dialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Tamam"));
            clsBtn->addStyleClass (Bootstrap::Button::Primary);

            dialog->show ();

            clsBtn->clicked ().connect ([=](){
                WApplication::instance ()->removeChild (dialog);
            });

        }else{
            this->showMessage ("Hata","Başvurunuz Kayıt Edilemedi. Lütfen Daha Sonra Tekrar Deneyiniz");
        }
    });

}

void BilgiEdinmeBasvuruWidget::initPublic()
{

    mTCManager = new SerikBLDCore::TCManager(new SerikBLDCore::DB(this->getDB ()));

    this->Content ()->clear ();


    auto vLayout = this->Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());


    auto tcno = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    tcno->setPlaceholderText ("TCNO Giriniz ( Zorunlu )");


    auto adsoyad = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    adsoyad->setPlaceholderText ("AdSoyad Giriniz ( Zorunlu )");


    auto telefonNumarasi = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    telefonNumarasi->setPlaceholderText ("Telefon Numarası Giriniz ( Zorunlu )");


    auto adres = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    adres->setPlaceholderText ("Lütfen Adresinizi Giriniz");



    auto konu = vLayout->addWidget (cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    konu->setPlaceholderText ("Konuyu Giriniz");


    auto mesajTextEdit = vLayout->addWidget (cpp14::make_unique<WTextEdit>(),0,AlignmentFlag::Justify);
    mesajTextEdit->setHeight (150);

    auto mesajTextCount = vLayout->addWidget (cpp14::make_unique<WText>("<b>Geçersiz Mesaj: 0 Karakter. Min 50 Karakter Olmalı</b>"),0,AlignmentFlag::Justify);
    mesajTextEdit->changed ().connect ([=](){
        if( mesajTextEdit->text ().toUTF8 ().size () < 50 )
        {
            mesajTextCount->setText (WString("<b>Geçersiz Mesaj: {1} Karakter. Min 50 Karakter Olmalı</b>").arg (mesajTextEdit->text ().toUTF8 ().size ()));
            mesajTextCount->setAttributeValue (Style::style,Style::color::color (Style::color::Red::FireBrick));
        }else{
            mesajTextCount->setText (WString("<b>Geçerli Mesaj: {1} Karakter</b>").arg (mesajTextEdit->text ().toUTF8 ().size ()));
            mesajTextCount->setAttributeValue (Style::style,Style::color::color (Style::color::Green::DarkGreen));
        }
    });

    this->Footer ()->addStyleClass (Bootstrap::ContextualBackGround::bg_warning);
    auto svBtn = this->Footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    svBtn->addStyleClass (Bootstrap::Button::Primary);

    this->Footer ()->addWidget (cpp14::make_unique<WText>("<br>Şikayet ve Taleplerinizi Lütfen İletişim Bölümünden Yapınız"
                                                          "<br><b>Bu Alandan Sadece Bilgi Edinme Amaçlı Başvurular Yapılabilir</b>"
                                                          "<br>Başvurularınız Cevabını Size Gösterilecek olan Linkten Yada Telefon Numarası ve Şifreniz İle Giriş Yaparak Takip Edebilirsiniz",TextFormat::UnsafeXHTML));



    svBtn->clicked ().connect ([=](){

        if( tcno->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","TCNO Kısmı Hatalı. Lütfen Kontrol Ediniz");
            return;
        }

        if( adsoyad->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","AdSoyad Kısmı Hatalı. Lütfen Kontrol Ediniz");
            return;
        }

        if( konu->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","Konu Kısmı Hatalı. Lütfen Kontrol Ediniz");
            return;
        }

        if( mesajTextEdit->text ().toUTF8 ().size () < 50 )
        {
            this->showMessage ("Uyarı","Mesajını 50 Karakterden Az Olamaz");
            return;
        }

        if( adres->text ().toUTF8 ().size () < 5 )
        {
            this->showMessage ("Uyarı","Adres Kısmı Hatalı. Lütfen Kontrol Ediniz");
            return;
        }


        SerikBLDCore::BilgiEdinmeItem item;

        auto val = mTCManager->Load_byTCNO (tcno->text ().toUTF8 ());

        if( val )
        {
            item.setTCoid (val.value ()->oid ().value ());
        }else{
            SerikBLDCore::TC tcItem;
            tcItem.setTCNO (tcno->text ().toUTF8 ().c_str ());
            tcItem.setAdSoyad (adsoyad->text ().toUTF8 ().c_str ());
            tcItem.setPassword ();
            tcItem.setCalismaSMS (false);
            tcItem.setCepTelefonu (telefonNumarasi->text ().toUTF8 ().c_str ());
            tcItem.setTamAdress (adres->text ().toUTF8 ().c_str ());
            if( mTCManager->insertTC (&tcItem) ){
                auto val_ = mTCManager->Load_byTEL (tcItem.CepTelefonu ().toStdString ());
                if( val_ )
                {
                    item.setTCoid (val_.value ()->oid ().value ());
                }
            }
        }




        item.setKonu (konu->text ().toUTF8 ().c_str ());
        item.setBirim (this->getMahalleler ().first ());
        item.setMesaj (mesajTextEdit->text ().toUTF8 ().c_str ());
        auto insertedItem = this->InsertItem (item);

        if( insertedItem.size ())
        {
            auto dialog = WApplication::instance ()->addChild (cpp14::make_unique<WDialog>());

            dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("Başvurunuz Başarılı"));
            dialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);

            auto container = dialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::row);

            {
                auto text = container->addWidget (cpp14::make_unique<WText>("Başvurunuzu"));
                text->addStyleClass (Bootstrap::Grid::col_full_12);
            }
            auto link = WLink(LinkType::Url,"?type=basvuru&_id="+insertedItem);
            link.setTarget (LinkTarget::NewWindow);
            auto anchor = cpp14::make_unique<WAnchor>(link,"http://www.serik.bel.tr/"+link.url ());
            anchor->addStyleClass (Bootstrap::Grid::col_full_12);
            container->addWidget (std::move(anchor));

            {
                auto text = container->addWidget (cpp14::make_unique<WText>("Adresinden Takip Edebilirsiniz"));
                text->addStyleClass (Bootstrap::Grid::col_full_12);
            }

            dialog->contents ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);


            auto clsBtn = dialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Tamam"));
            clsBtn->addStyleClass (Bootstrap::Button::Primary);

            dialog->show ();

            clsBtn->clicked ().connect ([=](){
                adsoyad->setText ("");
                telefonNumarasi->setText ("");
                adres->setText ("");
                mesajTextEdit->setText ("");
                konu->setText ("");
                tcno->setText ("");
                mesajTextCount->setText (WString("<b>Geçersiz Mesaj: {1} Karakter. Min 50 Karakter Olmalı</b>").arg (mesajTextEdit->text ().toUTF8 ().size ()));

                WApplication::instance ()->removeChild (dialog);
            });

        }else{
            this->showMessage ("Hata","Başvurunuz Kayıt Edilemedi. Lütfen Daha Sonra Tekrar Deneyiniz");
        }
    });


    tcno->textInput ().connect ([=](){

        if( tcno->text ().toUTF8 ().size () == 11 )
        {
            auto val = mTCManager->Load_byTCNO (tcno->text ().toUTF8 ());
            if( val )
            {

                adsoyad->setText (val.value ()->AdSoyad ().toStdString ());
                telefonNumarasi->setText (val.value ()->CepTelefonu ().toStdString ());
                adres->setText (val.value ()->TamAdres ().toStdString ());

                if( val.value ()->TamAdres ().size () == 0 )
                {
                    adres->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed));
                    tcno->setText ("");
                    this->showMessage ("Hata","Adres Kısmı Hatalı. Lütfen Giriş Yaparak Bilgilerim Bölümünden Adresinizi Güncelleyiniz");
                    return;
                }
                adsoyad->setEnabled (false);
                telefonNumarasi->setEnabled (false);
                adres->setEnabled (false);
            }else{
                adsoyad->setEnabled (true);
                telefonNumarasi->setEnabled (true);
                adres->setEnabled (true);

                adsoyad->setText ("");
                telefonNumarasi->setText ("");
                adres->setText ("");
            }
        }else{
            adres->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Snow));

            adsoyad->setEnabled (true);
            telefonNumarasi->setEnabled (true);
            adres->setEnabled (true);

            adsoyad->setText ("");
            telefonNumarasi->setText ("");
            adres->setText ("");
        }


    });



}

