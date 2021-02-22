#include "talepwidget.h"
#include <QDate>
#include <QDateTime>

using namespace SerikBLDCore;

TalepWidget::TalepWidget::TalepWidget(DB *db)
    :ContainerWidget() , TalepManager(db)
{
    this->init ();
}

TalepWidget::TalepWidget::TalepWidget(mongocxx::database *_db)
    :ContainerWidget() , TalepManager(_db)
{
    this->init ();
}


void TalepWidget::TalepWidget::init()
{
    this->Header ()->addWidget (cpp14::make_unique<WText>("<h4>Talepte/Şikayette Bulun</h4>"));
    this->Header ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);


    this->Content ()->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Snow));

    tcWidget = this->Content ()->addWidget (cpp14::make_unique<TCWidget>(this->db ()));

    tcWidget->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                             Bootstrap::Grid::Medium::col_md_6+
                             Bootstrap::Grid::Small::col_sm_12+
                             Bootstrap::Grid::ExtraSmall::col_xs_12);


    talepWidget = this->Content ()->addWidget (cpp14::make_unique<TalepItemWidget>(DB(this->db ())));
    talepWidget->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                Bootstrap::Grid::Medium::col_md_6+
                                Bootstrap::Grid::Small::col_sm_12+
                                Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto warningTextTitle = this->Content ()->addWidget(cpp14::make_unique<WText>("<h4>Talebinizin Tam Olarak Yerine Getirilebilmesi için "
                                                                                  "Lütfen Bilgilerinizi Eksiksiz Doldurunuz</h4>"));

    warningTextTitle->addStyleClass (Bootstrap::Grid::col_full_12);

    auto mSaveBtn = this->Content ()->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
    mSaveBtn->addStyleClass(Bootstrap::Button::Primary);
    warningTextTitle->addStyleClass (Bootstrap::Grid::col_full_12);

    mSaveBtn->clicked ().connect ([=](){

        auto tcVal = tcWidget->TCItem ();

        if( !tcVal )
        {
            this->showPopUpMessage (tcWidget->lastError () + ".  Kayıt Ediliyor...");

            if( tcWidget->lasterrorCode () == TCWidget::tcoidHatali )
            {
                tcVal = tcWidget->saveTCItem ();
                if( tcVal )
                {
                    this->showPopUpMessage ("TC Bilgileriniz Başarılı Bir Şekilde Kayıt Edildi","msg");
                }else{
                    this->showPopUpMessage ("TC Bilgileriniz Kayıt Edilemedi");
                    return;
                }
            }else{
                this->showPopUpMessage ("Lütfen Bilgilerinizi Eksiksiz Giriniz","hata");
                return;
            }
        }

        if( talepWidget->adresString ().size () < 10 )
        {
            this->showMessage ("Uyarı","Talep/Şikayet Adresi Yetersiz");
            return;
        }
        if( talepWidget->mahalleString ().size () == 0 )
        {
            this->showMessage ("Uyarı","Talep/Şikayet Mahallesi Seçmediniz");
            return;
        }
        if( talepWidget->talepString ().size () < 50 )
        {
            this->showMessage ("Uyarı","Talep/Şikayet Konusu Yetersiz");
            return;
        }

        auto fotoFileOid = talepWidget->fotoOid ();



        Talep talepItem;

        talepItem.setTCOID (tcVal->oid ().get ().to_string ().c_str ());
        talepItem.setAy (QDate::currentDate ().toString ("MMMM"));
        talepItem.setYil (QDate::currentDate ().year ());
        talepItem.setSec (QDateTime::currentDateTime ().time ().msecsSinceStartOfDay ());
        talepItem.setJulianDay (QDate::currentDate ().toJulianDay ());
        talepItem.setDurum (TalepKey::DurumKey::DevamEdiyor.c_str ());
        talepItem.setKaynak (TalepKey::KaynakKey::Web.c_str ());
        talepItem.setAddress (talepWidget->adresString ().c_str ());
        talepItem.setMahalle (talepWidget->mahalleString ().c_str ());
        talepItem.setKonu (talepWidget->talepString ().c_str ());
        talepItem.setBirim ("Başkanlık");
        talepItem.setKisiGizle (talepWidget->bilgilerimGizli ());

        if( !fotoFileOid.empty () )
        {
            talepItem.setFoto (fotoFileOid.c_str ());
        }


        auto insertedID = this->insertTalep (talepItem);
        if( insertedID.size () == 24 )
        {


            auto dialog = WApplication::instance ()->root ()->addChild (cpp14::make_unique<WDialog>());
            dialog->setHeight (250);

            dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("Talebiniz Başarılı Bir Şekilde Alındı"));
            dialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_success);

            auto container = dialog->contents ()->addWidget (cpp14::make_unique<ContainerWidget>());


            container->addWidget (cpp14::make_unique<WText>(WString("<p>Talebiniz Başarılı Bir Şekilde Alındı.</p>"
                                                                    "<p><a href=\"http://www.serik.bel.tr/?type=talep&_id={1}\" target=\"_blank\"><b>http://www.serik.bel.tr/?type=talep&_id={2}</b></a></p>"
                                                                    "<p>Bu Linkten yada</p>"
                                                                    "<p>Telefon Numarası ve Şifreniz İle Giriş Yaparak Takip Edebilirsiniz</p>").arg (insertedID).arg (insertedID).toUTF8 ().c_str (),TextFormat::UnsafeXHTML));

            auto closeBtn = dialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kapat"));
            closeBtn->clicked ().connect ([=](){
                tcWidget->setDefault ();
                talepWidget->setDefault ();
                WApplication::instance ()->root ()->removeChild (dialog);
            });
            dialog->show ();
        }


    });

    this->Footer ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    {
        auto container = this->Footer ()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        container->setAttributeValue(Style::style,Style::background::color::rgba(125,175,225));
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("<h5><p><b>Sinek ve İlaçlama İçin Lütfen</b></p>"
                                                                "<p>Antalya Büyükşehir Belediyesi</p>"
                                                                "<p>Çevre Koruma ve Kontrol Daire Başkanlığı Çevre Sağlığı Şube Müdürlüğünü (Vektörel Mücadele) Arayınız</p>"
                                                                "<p>İletişim: 0 242 249 52 00 - 0 242 249 52 01</p>"
                                                                "<p>Arama Saatleri 09:00 - 16:00</p></h5>"));
    }


}



TalepWidget::TCWidget::TCWidget(mongocxx::database *db)
    :ContainerWidget (),TCManager (db),mTCLoadFromExternal(false)
{
    this->Header ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    this->Header ()->addWidget (cpp14::make_unique<WText>("<h5>Kişisel Bilgileriniz</h5>"));

    this->Content ()->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Azure));


    auto layout = this->Content ()->setLayout(cpp14::make_unique<WGridLayout>());

    {
        auto text = layout->addWidget(cpp14::make_unique<WText>("TC NO"),0,0,AlignmentFlag::Middle);
        mTcNO = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,1);
        mTcNO->setPlaceholderText("TCNO Kayıtlı ise Bilgileriniz Otomatik Dolacaktır");
        mTcNO->textInput().connect(this,&TCWidget::TCChanged);
        mTcNO->setInputMask("99999999999");
    }

    {
        auto text = layout->addWidget(cpp14::make_unique<WText>("Ad Soyad"),1,0);
        mAdSoyad = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,1);
        mAdSoyad->setPlaceholderText("Adınızı ve Soyadınızı Giriniz");
    }

    {
        auto text = layout->addWidget(cpp14::make_unique<WText>("Mahalle"),2,0,AlignmentFlag::Middle);
        mMahalle = layout->addWidget(cpp14::make_unique<WComboBox>(),2,1);
        auto cursor = this->getMahalleler ();
        for( auto item : cursor )
        {
            mMahalle->addItem (item.toStdString ());
        }
    }
    {
        auto text = layout->addWidget(cpp14::make_unique<WText>("Telefon"),3,0,AlignmentFlag::Middle);
        mTelefon = layout->addWidget(cpp14::make_unique<WLineEdit>(),3,1);
        mTelefon->setPlaceholderText("Cep Telefonu Numaranızı Giriniz(05321234567)");
        //                mTelefon->setInputMask("99999999999");
    }
}


void TalepWidget::TCWidget::TCChanged()
{

    if( mTcNO->text ().toUTF8 ().size () == 11 )
    {

        auto tcItem = this->Load_byTCNO (mTcNO->text ().toUTF8 ());

        if( tcItem )
        {
            mTCOid = tcItem.value ()->oid ().get ().to_string ();
            std::string isim = tcItem.value ()->AdSoyad ().toStdString ();
            for( decltype (isim.size()) i = 1 ; i < isim.size()-1 ; i++ )
            {
                if( isim.at(i) == ' ' )
                {
                    i += 2;
                }
                if( i < isim.size() )
                {
                    isim.replace(i,1,"*");
                }
            }
            this->mAdSoyad->setText (isim);



            std::string tel = tcItem.value ()->CepTelefonu ().toStdString ();

            for( decltype (tel.size()) i = 3 ; i < tel.size()-3 ; i++ )
            {
                if( i == tel.size()/2-1 )
                {
                    i += 1;
                }
                if( i < tel.size() )
                {
                    tel.replace(i,1,"*");
                }
            }

            mTelefon->setText(tel.c_str());

            for( int i = 0 ; i < mMahalle->count() ; i++ ){
                if( mMahalle->itemText(i).toUTF8() == tcItem.value ()->Mahalle ().toStdString () )
                {
                    mMahalle->setCurrentIndex(i);
                    break;
                }
            }

            this->mMahalle->setEnabled (false);
            this->mTelefon->setEnabled (false);
            this->mAdSoyad->setEnabled (false);
        }

    }else{
        this->mAdSoyad->setText ("");
        this->mTelefon->setText ("");
        this->mAdSoyad->setEnabled (true);
        this->mTelefon->setEnabled (true);
        mMahalle->setEnabled (true);
        mMahalle->setCurrentIndex (0);
    }

}

void TalepWidget::TCWidget::setDefault()
{
    if( !mTCLoadFromExternal )
    {
        this->mTcNO->setText ("");
        this->mAdSoyad->setText ("");
        this->mTelefon->setText ("");
        this->mAdSoyad->setEnabled (true);
        this->mTelefon->setEnabled (true);
        mMahalle->setEnabled (true);
        mMahalle->setCurrentIndex (0);
    }



}

boost::optional<TC> TalepWidget::TCWidget::TCItem()
{

    if( mTcNO->text ().toUTF8 ().size () != 11 )
    {
        setError ("TCNO Hatalaı");
        setErrorCode (errorCode::tcnoHatali);
        return boost::none;
    }

    if( mAdSoyad->text ().toUTF8 ().size () <= 5 )
    {
        setError ("Ad Soyad Çok Kısa");
        setErrorCode (errorCode::adsoyadHatali);
        return boost::none;
    }

    if( mMahalle->currentIndex () == 0 )
    {
        setError ("Mahalle Seçilmedi");
        setErrorCode (errorCode::mahalleHatali);
        return boost::none;
    }

    if( mTelefon->text ().toUTF8 ().size () != 11 )
    {
        setError ("Telefon Numarası Uygun Formatta Değil ( 05321234567) ");
        setErrorCode (errorCode::telefonHatali);
        return boost::none;
    }

    if( mTCOid.size () != 24 )
    {
        setError ("TC Kayıtlı Değil");
        setErrorCode (errorCode::tcoidHatali);
        return boost::none;
    }

    TC item;
    item.setTCNO (mTcNO->text ().toUTF8 ().c_str ())
            .setAdSoyad (mAdSoyad->text ().toUTF8 ().c_str ())
            .setMahalle (mMahalle->currentText ().toUTF8 ().c_str ())
            .setCepTelefonu (mTelefon->text ().toUTF8 ().c_str ());
    item.setOid (mTCOid);

    return std::move(item);
}

boost::optional<TC> TalepWidget::TCWidget::saveTCItem()
{

    TC item;
    item.setTCNO (mTcNO->text ().toUTF8 ().c_str ())
            .setAdSoyad (mAdSoyad->text ().toUTF8 ().c_str ())
            .setMahalle (mMahalle->currentText ().toUTF8 ().c_str ())
            .setCepTelefonu (mTelefon->text ().toUTF8 ().c_str ());

    auto val = this->insertItem (item);
    if( val )
    {
        auto _oid = val.value ().inserted_id ();
        item.setOid (_oid.get_oid ().value.to_string ());
        return std::move(item);
    }else{
        return boost::none;
    }


}

std::string TalepWidget::TCWidget::lastError() const
{
    return mErrorString;
}

void TalepWidget::TCWidget::setError(const std::string &errorStr)
{
    this->mErrorString = errorStr;
}

void TalepWidget::TCWidget::setTCItem(TC &tcItem)
{
    mTCOid = tcItem.oid ().value ().to_string ();
    mTcNO->setText (tcItem.TCNO ().toStdString ());
    mAdSoyad->setText (tcItem.AdSoyad ().toStdString ());
    mTelefon->setText (tcItem.CepTelefonu ().toStdString ());

    auto cursor = this->getMahalleler ();
    int currentIndex = -1;
    mMahalle->clear ();
    for( auto item : cursor )
    {
        currentIndex++;
        mMahalle->addItem (item.toStdString ());
        if( item == tcItem.Mahalle () )
        {
            mMahalle->setCurrentIndex (currentIndex);
        }
    }
    mTcNO->setEnabled (false);
    mAdSoyad->setEnabled (false);
    mTelefon->setEnabled (false);
    mMahalle->setEnabled (false);

    mTCLoadFromExternal = true;

}

void TalepWidget::TCWidget::setErrorCode(const errorCode &errorCode)
{
    mErrorCode = errorCode;
}

TalepWidget::TCWidget::errorCode TalepWidget::TCWidget::lasterrorCode() const
{

    return mErrorCode;

}


















TalepWidget::TalepItemWidget::TalepItemWidget(const DB &db )
    :ContainerWidget(),mDB(db)
{
    this->Header ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    this->Header ()->addWidget (cpp14::make_unique<WText>("<h5>Talep/Şikayet Bilgileri</h5>"));

    this->Content ()->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::GhostWhite));

    auto vLayout = this->Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());

    mBilgilerimiGizle = vLayout->addWidget (cpp14::make_unique<WCheckBox>("Kişisel Bilgilerimi Gizle"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
    mBilgilerimiGizle->setChecked (false);
    mBilgilerimiGizle->checked ().connect ([=](){
        if( mBilgilerimiGizle->isChecked () ){
            this->informDialog ("Adınız Soyadınız, Telefon Numaranız ve Diğer Bilgilerinizi Personel Dahil Kimse Göremez.\nSadece Talebiniz ve Talep/Şikayet Adresi Görüntülenir");
        }
    });

    auto mahalleTitleText = vLayout->addWidget (cpp14::make_unique<WText>("Mahalle"),0,AlignmentFlag::Center|AlignmentFlag::Middle);

    mTalepMahalle = vLayout->addWidget (cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Justify|AlignmentFlag::Middle);

    for( auto mahItem : mDB.getMahalleler () )
    {
        mTalepMahalle->addItem (mahItem.toStdString());
    }

    auto talepTitleText = vLayout->addWidget (cpp14::make_unique<WText>("Talebiniz/Şikayetiniz"),0,AlignmentFlag::Center|AlignmentFlag::Middle);

    mEdit = vLayout->addWidget (cpp14::make_unique<WTextEdit>(),0,AlignmentFlag::Justify);
    mEdit->setHeight (250);
    auto characterCountText = vLayout->addWidget (cpp14::make_unique<WText>("Geçersiz Talep 0|Min(50) Karakter"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
    characterCountText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed)+
                                           Style::font::weight::bold);
    mEdit->changed ().connect ([=](){

        if( mEdit->text ().toUTF8 ().size () < 50 ){

            characterCountText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed)+
                                                   Style::font::weight::bold);
            characterCountText->setText (WString("Geçersiz Talep {1}|Min(50) Karakter").arg (mEdit->text ().toUTF8 ().size ()));


        }else{
            characterCountText->setAttributeValue (Style::style,Style::color::color (Style::color::Green::DarkGreen)+
                                                   Style::font::weight::bold);
            characterCountText->setText (WString("Geçerli"));
        }
    });


    mAdres = vLayout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
    mAdres->setPlaceholderText("Şikayet/Talep Adresi Giriniz");

    mFileUploadManager = vLayout->addWidget (cpp14::make_unique<FileUploaderWidget>("Fotoğraf Yükle"),0,AlignmentFlag::Justify);
    mFileUploadManager->setType (FileUploaderWidget::Image);

}


std::string TalepWidget::TalepItemWidget::mahalleString() const
{
    return mTalepMahalle->currentText ().toUTF8 ();
}

std::string TalepWidget::TalepItemWidget::talepString() const
{
    return mEdit->text ().toUTF8 ();
}

std::string TalepWidget::TalepItemWidget::adresString() const
{
    return mAdres->text ().toUTF8 ();
}

std::string TalepWidget::TalepItemWidget::fotoOid() const
{
    if( mFileUploadManager->isUploaded () )
    {
        auto fileValue = this->mDB.uploadfile (mFileUploadManager->fileLocation ());
        return fileValue.get_oid ().value.to_string ();
    }else{
        return "";
    }
}

bool TalepWidget::TalepItemWidget::bilgilerimGizli() const
{
    return mBilgilerimiGizle->isChecked ();
}

void TalepWidget::TalepItemWidget::setDefault()
{
    mEdit->setText ("");
    mTalepMahalle->setCurrentIndex (0);
    mAdres->setText ("");
    mAdres->setPlaceholderText("Şikayet/Talep Adresi Giriniz");

}

