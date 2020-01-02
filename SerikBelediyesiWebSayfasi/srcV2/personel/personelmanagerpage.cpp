#include "personelmanagerpage.h"
#include <QDate>
#include <QTime>


IKManagerPage::IKManagerPage(SerikBLDCore::DB *_db)
    :ContainerWidget ("İnsan Kaynakları Yönetimi"),mDB(_db)
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  "boxShadow");
        container->setHeight (100);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::DeepPink));
        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Personel Yönetimi"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::weight::bold);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&IKManagerPage::initPersonelManager );
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());

        container->addStyleClass (Bootstrap::Grid::Offset::Large::col_lg_4+
                                  Bootstrap::Grid::Offset::Medium::col_md_4+
                                  Bootstrap::Grid::Offset::Small::col_sm_2+
                                  Bootstrap::Grid::Offset::ExtraSmall::col_xs_2+
                                  "boxShadow");

        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  "boxShadow");
        container->setHeight (100);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::MediumVioletRed));
        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Birim Yönetimi"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::weight::bold);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ( this , &IKManagerPage::initBirimManager );

    }

    Content ()->setMargin (15,Side::Top);
}

void IKManagerPage::initPersonelManager()
{
    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<PersonelManagerPage>(mDB));
}

void IKManagerPage::initBirimManager()
{
    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<BirimManagerPage>(mDB));
}


PersonelManagerPage::PersonelManagerPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::PersonelManager (_db),
      ContainerWidget ("Personel Yönetimi"),
      mBirimManager( new SerikBLDCore::BirimManager(_db))
{
    initBirimList ();
    Content ()->setMargin (15,Side::Top|Side::Bottom);
    Content ()->addStyleClass ("boxShadow");
    this->setLimit (1000);
}

void PersonelManagerPage::onList(const QVector<SerikBLDCore::IK::Personel> *mlist)
{

    Content ()->clear ();

    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (15,Side::Bottom);
        container->addWidget (cpp14::make_unique<WText>(WString("{1} Adet Personel").arg (mlist->count ())));
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_warning);
    }


    if( mlist->count () == 0 )
    {
        this->showPopUpMessage ("Bu Birim de Hiç Personel Yok.","msg");
        return;
    }

    int mudurExist = 0;

    for( auto item : *mlist )
    {
        auto container = Content ()->addWidget (cpp14::make_unique<PersonelThumpPage>(item,this->getDB ()));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin (15,Side::Bottom);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setContentAlignment (AlignmentFlag::Center);
        container->clicked ().connect ([=](){
            Content ()->clear ();
            Content ()->addWidget (cpp14::make_unique<PersonelPage>(item,this->getDB ()));
        });

        if( item.statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur && item.Birim () != "Başkanlık" )
        {
            mudurExist++;
        }
    }

    if( !mListFromSearchText ){
        if( mudurExist == 0 )
        {
            this->showPopUpMessage ("!Dikkat! Bu Birime <u>Müdür</u> Atanmamış","hata");
        }

        if( mudurExist > 1 )
        {
            this->showPopUpMessage ("!Dikkat! Bu Birim de Birden Fazla <u>Müdür</u> Atanmış","hata");
        }
    }





}

void PersonelManagerPage::initBirimList()
{
    Header ()->clear ();
    Header ()->setMargin (25,Side::Top|Side::Bottom);
    Header ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    Header ()->addStyleClass ("boxShadow");

    mListFromSearchText = false;

    {
        auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);

        auto newPersonelBtn = container->addWidget (cpp14::make_unique<WPushButton>("Yeni Personel Tanımla"));

        newPersonelBtn->addStyleClass (Bootstrap::Button::Primary);
        newPersonelBtn->clicked ().connect ( this , &PersonelManagerPage::initNewPersonelWidget );


    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);

        mBirimManager->setLimit (500);
        mBirimManager->UpdateList();
        birimComboBoxFilter = container->addWidget (cpp14::make_unique<WComboBox>());
        for( const auto& item : mBirimManager->List () )
        {
            birimComboBoxFilter->addItem (item.birimAdi ().toStdString ());
        }

        birimComboBoxFilter->changed ().connect ([=](){
            mListFromSearchText = false;
            this->UpdateList (SerikBLDCore::IK::Personel().setBirim (birimComboBoxFilter->currentText ().toUTF8 ().c_str ()));
        });
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);

        auto statuComboBox = container->addWidget (cpp14::make_unique<WComboBox>());

//        statuComboBox->addItem (SerikBLDCore::IK::Statu::Personel);
//        statuComboBox->addItem (SerikBLDCore::IK::Statu::Sef);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Mudur);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::BaskanYardimcisi);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Baskan);

        statuComboBox->changed ().connect ([=](){
                mListFromSearchText = true;
                this->UpdateList (SerikBLDCore::IK::Personel().setStatu (statuComboBox->currentText ().toUTF8 ().c_str ()));
        });
    }





    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setContentAlignment (AlignmentFlag::Center);
        auto searchLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        searchLineEdit->setPlaceholderText ("En Az 3 Harf Giriniz + Enter");
        searchLineEdit->changed ().connect ([=](){
            if( searchLineEdit->text ().toUTF8 ().size () >= 3 )
            {
                mListFromSearchText = true;
                SerikBLDCore::IK::Personel item;
                item.append("$regex",searchLineEdit->text ().toUTF8 ()).append("$options","i");
                SerikBLDCore::IK::Personel filter;
                filter.append(SerikBLDCore::IK::Personel::KeyAdSoyad,item.view ());
                this->UpdateList (filter);
            }
        });
    }




}

void PersonelManagerPage::initNewPersonelWidget()
{
    Content ()->clear ();
    auto yeniPersonel = Content ()->addWidget (cpp14::make_unique<YeniPersonelWidget>());
    yeniPersonel->addStyleClass (Bootstrap::Grid::col_full_12);
    yeniPersonel->PersonelSaved ().connect ([=]( ){
        auto val = this->uploadfile (yeniPersonel->photoFilePath ());



        yeniPersonel->setFotoOid (val.get_oid ().value.to_string ().c_str ());

        auto insertedID = this->InsertItem (*yeniPersonel);
        if( insertedID.size () )
        {
            this->showPopUpMessage ("Personel Kaydı Başarılı","msg");
            SerikBLDCore::IK::Personel filter;
            filter.setOid (insertedID);
            this->UpdateList (filter);
        }
    });
}

void PersonelManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (errorText,"hata");
}

BirimManagerPage::BirimManagerPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::BirimManager (_db),
      ContainerWidget ("Birim Yönetimi"),
      mPersonelManager( new SerikBLDCore::PersonelManager(_db))
{

    Succes.connect ([=](const std::string &msg){
        this->showPopUpMessage (msg);
    });

    Error.connect ([=](const std::string &msg){
        this->showPopUpMessage (msg,"hata");
    });


    this->setLimit (100);
    this->UpdateList ();


    Header ()->setMargin (25,Side::Top|Side::Bottom);
    Header ()->addStyleClass ("boxShadow");
    Header ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    auto newBirimContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    newBirimContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                      Bootstrap::Grid::Medium::col_md_7+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto newBirimLineEdit = newBirimContainer->addWidget (cpp14::make_unique<WLineEdit>());
    newBirimLineEdit->setPlaceholderText ("Yeni Birim Adını Giriniz");

    auto newBirimSaveContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    newBirimSaveContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                          Bootstrap::Grid::Medium::col_md_5+
                                          Bootstrap::Grid::Small::col_sm_6+
                                          Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto newBirimKaydet = newBirimSaveContainer->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    newBirimKaydet->addStyleClass (Bootstrap::Button::Primary);
    newBirimKaydet->clicked ().connect ([=](){

        auto count = this->countItem (SerikBLDCore::IK::BirimItem().setBirimAdi (newBirimLineEdit->text ().toUTF8 ().c_str ()));

        if( count )
        {
            this->showPopUpMessage ("Bu Birim Zaten Var","hata");
            return;
        }

        if( this->InsertItem (SerikBLDCore::IK::BirimItem().setBirimAdi (newBirimLineEdit->text ().toUTF8 ().c_str ())).size () ){
            this->showPopUpMessage ("Yeni Birim Başarılı Bir Şekilde Kayıt Edildi");
            this->UpdateList ();
        }
    });

}

void BirimManagerPage::onList(const QVector<SerikBLDCore::IK::BirimItem> *mlist)
{

    Content ()->clear ();
    for ( auto item : *mlist ) {



        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail+
                                  Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6+
                                  Bootstrap::ContextualBackGround::bg_warning);
        container->setContentAlignment (AlignmentFlag::Center);
        container->setMargin (5,Side::Top|Side::Bottom);
        auto text = container->addWidget (cpp14::make_unique<WText>(item.birimAdi ().toStdString ()));
        text->setMargin (5,Side::Top|Side::Bottom);
        container->setPositionScheme (PositionScheme::Relative);
        container->setHeight (65);

        auto xContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        xContainer->setPositionScheme (PositionScheme::Absolute);
        xContainer->addStyleClass ("boxShadow");
        xContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::IndianRed));
        auto deleteText = xContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
        deleteText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                       Style::font::weight::bold);
        deleteText->setPadding (5,Side::Left|Side::Right);
        xContainer->setOffsets (0,Side::Top|Side::Right);
        xContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        xContainer->clicked ().connect ([=](){

            SerikBLDCore::IK::Personel personelItem;

            personelItem.setBirim ( text->text ().toUTF8 ().c_str () );

            auto __countItem = mPersonelManager->countItem (personelItem);
            if( __countItem ){
                this->showPopUpMessage (WString("Bu Birime Ait {1} Adet Personel Var.Önce Bu Personelleri Taşıyın").arg (__countItem).toUTF8 (),"hata");
            }else{
                auto btn = askConfirm ("Silmek İstediğinize Eminmisiniz?");
                btn->clicked ().connect ([=](){
                    SerikBLDCore::IK::BirimItem item;
                    item.setBirimAdi (text->text ().toUTF8 ().c_str ());

                    if( this->DeleteItem (item) ){
                        this->showPopUpMessage ("Birimi Silindi");
                        this->UpdateList ();
                    }else{
                        this->showPopUpMessage ("Bir Hata Oluştu: " + this->getLastError ().toStdString (),"hata");
                    }
                });
            }



        });
    }

}



PersonelThumpPage::PersonelThumpPage(SerikBLDCore::IK::Personel &personel, SerikBLDCore::DB *_db)
    :SerikBLDCore::IK::Personel (personel),mDB(_db)
{


    auto fotoContinaer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    fotoContinaer->setWidth (90);
    fotoContinaer->setHeight (120);
    fotoContinaer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto filePath = mDB->downloadFileWeb (this->FotoOid ());
    fotoContinaer->setAttributeValue (Style::style,Style::background::url (filePath)+
                                      Style::background::size::cover+
                                      Style::background::repeat::norepeat+
                                      Style::background::position::center_center);

    Content ()->addWidget (cpp14::make_unique<WBreak>());
    Content ()->addWidget (cpp14::make_unique<WText>("<b>"+this->AdSoyad ().toStdString ()+"</b>"));
    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    }

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
    }

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
    }
    Content ()->addWidget (cpp14::make_unique<WBreak>());
    Content ()->addWidget (cpp14::make_unique<WText>("<i>"+this->statu ().toStdString ()+"</i>"));
}

void PersonelThumpPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (this->AdSoyad ().toStdString () +": " +errorText,"hata");
}

PersonelPage::PersonelPage(const Personel &personel, SerikBLDCore::DB *_db)
    :SerikBLDCore::IK::Personel (personel),SerikBLDCore::PersonelManager (_db)
{

    Footer ()->setMargin (25,Side::Top);
    mSMSManager = Footer ()->addWidget (cpp14::make_unique<SMSManager>(this->getDB ()));
    mSMSManager->addStyleClass (Bootstrap::Grid::col_full_12);

    mSMSManager->smsSended ().connect ([=](const std::string &sms){
        this->showPopUpMessage ("SMS Gönderildi: " + sms);
        this->initSMSLog ();
    });

    mSMSManager->ErrorOccured ().connect ([=](const std::string &errMsg){
        this->showPopUpMessage (errMsg,"hata");
    });



    initHeader ();

    initContent ();

    this->initSMSLog ();

    mSMSManager->messageOccured ().connect ([=](const std::string &errMsg){
        this->showPopUpMessage (errMsg,"msg");
        this->initSMSLog ();
    });

}

void PersonelPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (this->AdSoyad ().toStdString () +": " +errorText,"hata");
}

void PersonelPage::initHeader()
{
    Header ()->clear ();
    auto container = Header() -> addWidget (cpp14::make_unique<WContainerWidget>());
    auto filePath = this->downloadFileWeb (this->FotoOid ());
    container->setAttributeValue (Style::style,Style::background::url (filePath)+Style::background::size::cover+
                                  Style::background::repeat::norepeat+
                                  Style::background::position::center_center);
    container->setWidth (120);
    container->setHeight (160);
    container->setMinimumSize (120,160);
    container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    Header ()->addWidget (cpp14::make_unique<WBreak>());
    auto fileManagerContainer = Header ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Fotoğraf Yükle") );
    fileManagerContainer->setType (FileUploaderWidget::Image);

    fileManagerContainer->Uploaded ().connect ([=](){

        if( fileManagerContainer->isUploaded () )
        {
            auto val = this->uploadfile (fileManagerContainer->fileLocation ());
            this->setFotoOid (val.get_oid ().value.to_string ().c_str ());
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyFotoOid,val) ){
                auto filePath = this->downloadFileWeb (this->FotoOid ());
                container->setAttributeValue (Style::style,Style::background::url (filePath)+Style::background::size::contain+Style::background::repeat::norepeat+Style::background::position::center_center);
                container->setWidth (120);
                container->setHeight (160);
                container->setMinimumSize (120,160);

                this->showPopUpMessage ("Fotoğraf Değiştirildi");
            }
        }else{
            this->showPopUpMessage ("Fotoğraf Yüklenmedi","hata");
        }

    });
}

void PersonelPage::initContent()
{
    auto vLayout = Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());

    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Ad Soyad</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Ad Soyad Giriniz");
        lineEdit->setText (this->AdSoyad ().toStdString ());

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);

        kaydetText->clicked ().connect ([=](){
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyAdSoyad,lineEdit->text ().toUTF8 ().c_str ()) ){
                this->showPopUpMessage ("İsim Değiştirildi","msg");
            }
        });
    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Telefon Numarası</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Telefon Numarası Giriniz");
        lineEdit->setText (this->telefon ().toStdString ());

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);
        kaydetText->clicked ().connect ([=](){

            if( SerikBLDCore::IK::Personel::checkTelefonFormat (lineEdit->text ().toUTF8 ().c_str ()) )
            {
                if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyTelefon,lineEdit->text ().toUTF8 ().c_str ()) ){
                    this->showPopUpMessage ("Telefon Değiştirildi","msg");
                }
            }else{
                this->showPopUpMessage ("Telefon Numarası Hatalı","hata");
            }
        });
    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Birimi</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto birimComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        auto counter = 0;
        for( auto item : this->birimList () )
        {
            birimComboBox->addItem (item.toStdString ());
            if( this->Birim () == item )
            {
                birimComboBox->setCurrentIndex (counter);
            }
            counter++;
        }

        birimComboBox->sactivated ().connect ([=](const WString& str){
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyBirimi,str.toUTF8 ().c_str ()) ){
                this->showPopUpMessage ("Birim Değiştirildi","msg");
            }
        });
    }

    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Statu</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);

        auto statuComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Personel);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Sef);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Mudur);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::BaskanYardimcisi);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Baskan);

        if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Personel )
        {
            statuComboBox->setCurrentIndex (0);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Sef  ){
            statuComboBox->setCurrentIndex (1);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur  ){
            statuComboBox->setCurrentIndex (2);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi  ){
            statuComboBox->setCurrentIndex (3);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan  ){
            statuComboBox->setCurrentIndex (4);
        }

        statuComboBox->sactivated ().connect ([=](const WString& str){

            if( str.toUTF8 () == SerikBLDCore::IK::Statu::Baskan )
            {
                if( this->Birim () != "Başkanlık" )
                {
                    this->showPopUpMessage ("Statunun <u><i>Başkan</i></u> Yapılabilmesi için Önce Birimi <u><i>Başkanlık</i></u> Olarak Değiştiriniz!","hata");


                    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Personel )
                    {
                        statuComboBox->setCurrentIndex (0);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Sef  ){
                        statuComboBox->setCurrentIndex (1);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur  ){
                        statuComboBox->setCurrentIndex (2);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi  ){
                        statuComboBox->setCurrentIndex (3);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan  ){
                        statuComboBox->setCurrentIndex (4);
                    }


                    return;
                }
            }

            if( this->statu ().toStdString () == str.toUTF8 () )
            {
                return;
            }


            if( str.toUTF8 () == SerikBLDCore::IK::Statu::Sef || str.toUTF8 () == SerikBLDCore::IK::Statu::Personel || str.toUTF8 () == SerikBLDCore::IK::Statu::BaskanYardimcisi )
            {
                if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyStatu,str.toUTF8 ().c_str ()) ){
                    this->setStatu (str.toUTF8 ().c_str ());
                    this->showPopUpMessage ("Statu: <i>" + str.toUTF8 () +"</i> Değiştirildi: ","msg");
                }
            }else{
                auto count = this->countItem (SerikBLDCore::IK::Personel().setStatu (str.toUTF8 ().c_str ()).setBirim (this->Birim ()));

                if( count )
                {
                    this->showPopUpMessage ("Bu Birime Ait Bir <u><i>"+ str.toUTF8 () + "</i></u> Bulunmakta. Mevcut <u><i>"+ str.toUTF8 () +"</i></u> Değiştiriniz","hata");

                    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Personel )
                    {
                        statuComboBox->setCurrentIndex (0);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Sef  ){
                        statuComboBox->setCurrentIndex (1);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur  ){
                        statuComboBox->setCurrentIndex (2);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi  ){
                        statuComboBox->setCurrentIndex (3);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan  ){
                        statuComboBox->setCurrentIndex (4);
                    }


                    return;
                }

                if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyStatu,str.toUTF8 ().c_str ()) ){
                    this->setStatu (str.toUTF8 ().c_str ());
                    this->showPopUpMessage ("Statu: <i>" + str.toUTF8 () +"</i> Değiştirildi: ","msg");
                }
            }

        });

    }

    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Büro Personeli</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WCheckBox>());
        lineEdit->setChecked (this->BuroPersoneli ());

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);
        kaydetText->clicked ().connect ([=](){
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyBuroPersonel,lineEdit->isChecked ()) ){
                this->showPopUpMessage ("Büro Personeli Durumu Değiştirildi","msg");
            }

        });
    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Şifresi</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setText (this->sifre ().toStdString ());
        lineEdit->setEchoMode (EchoMode::Password);
        lineEdit->setEnabled (false);

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Oluştur & Gönder</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);

        kaydetText->clicked ().connect ([=](){
            this->showPopUpMessage ("SMS ID Alana Kadar Bekleyin","msg");
            auto newSifre = QRandomGenerator::global ()->generate64 ()%90000 + 10000;
            SerikBLDCore::SMS::SMSItem item;
            item.setSMS (QString("Şifreniz: %1").arg (newSifre));
            item.setNumara (this->telefon ());
            if( mSMSManager->insertAndSendSMS (item) ){
                this->showPopUpMessage ("SMS Gönderildi");
                if( this->setField(Personel().setOid (this->oid ().value ().to_string ()),Personel::KeyPassword,std::to_string (newSifre)) ){
                    this->setSifre (std::to_string (newSifre).c_str ());
                    this->initSMSLog ();
                }
            }
        });
    }
}

void PersonelPage::initSMSLog()
{


    mSMSManager->Content ()->clear ();

    {
        auto container = mSMSManager->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info+"boxShadow");
        container->setWidth (WLength("100%"));
        container->addWidget (cpp14::make_unique<WText>("Gönderilen SMSler"));
        container->setMargin (5,Side::Top|Side::Bottom);
        container->setPositionScheme (PositionScheme::Relative);
    }
    for( auto item : mSMSManager->listSMS (this->telefon ()) )
    {
        auto container = mSMSManager->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setWidth (WLength("100%"));
        container->setMargin (25,Side::Top|Side::Bottom);
        container->setPositionScheme (PositionScheme::Relative);
        container->setHeight (78);
        container->addStyleClass ("boxShadow");


        {
            auto smsText = container->addWidget (cpp14::make_unique<WText>("<h6>"+item.smsText ().toStdString ()+"</h6>",TextFormat::UnsafeXHTML));
            smsText->setPositionScheme (PositionScheme::Absolute);
            smsText->setOffsets (0,Side::Left);
            smsText->addStyleClass (Bootstrap::Label::Default+"boxShadow");
            smsText->setOffsets (0,Side::Bottom);
        }


        {
            auto iletiRaporID = item.raporID ();
            std::string rapor_text = "";
            if( iletiRaporID != 6 )
            {
                rapor_text = item.rapor ().toStdString ();
            }else{
                rapor_text = "İleti Raporunu Kontrol Et";
            }
            auto smsText = container->addWidget (cpp14::make_unique<WText>("<h6>"+rapor_text+"</h6>"));

            smsText->setPositionScheme (PositionScheme::Absolute);
            smsText->setOffsets (0,Side::Top|Side::Right);

            if( iletiRaporID != 6 )
            {
                rapor_text = item.rapor ().toStdString ();
            }else{
                rapor_text = "İleti Raporunu Kontrol Et";
                smsText->decorationStyle ().setCursor (Cursor::PointingHand);
            }

            smsText->addStyleClass (Bootstrap::Label::Danger);
            smsText->setAttributeValue (Style::userdata1,item.idText ().toStdString ());
            smsText->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
            smsText->clicked ().connect ([=](){


                    if( item.raporID () != 6 )
                    {
                        this->showPopUpMessage ("Bilgi: "+ item.rapor ().toStdString (),"msg");
                        return;
                    }

                    this->showPopUpMessage ("Bekleyin","msg");
                    SerikBLDCore::SMS::SMSItem smsItem;
                    smsItem.setID (smsText->attributeValue (Style::userdata1).toUTF8 ().c_str ());
                    smsItem.setOid (smsText->attributeValue (Style::dataoid).toUTF8 ().c_str ());

                    if( this->mSMSManager->checkRapor (smsItem) )
                    {
    //                    this->showPopUpMessage ("SMS ")
                    }
            });
        }




        {
            auto tarihText = container->addWidget (cpp14::make_unique<WText>("<h6>"+QDate::fromJulianDay (item.julianDay ()).toString ("dd/MM/yyyy dddd").toStdString ()+
                                                                             " - "+QTime::fromMSecsSinceStartOfDay (item.secStartOfDay ()).toString ("hh:mm").toStdString ()+"</h6>",TextFormat::UnsafeXHTML));
            tarihText->setPositionScheme (PositionScheme::Absolute);
            tarihText->setOffsets (0,Side::Top|Side::Left);
            tarihText->addStyleClass (Bootstrap::Label::Primary);
        }

    }


}
