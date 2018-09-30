#include "giriswidget.h"
#include "yenilikler.h"
#include <QLatin1String>

#include "anonswidget.h"

#include "SerikBelediyesiWebSayfasi/srcV2/kadinailestock.h"
#include "SerikBelediyesiWebSayfasi/srcV2/ik.h"
#include "SerikBelediyesiWebSayfasi/srcV2/girisCikisWidget/giriscikiswidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetimyonetim.h"
#include "SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeclient.h"


Giris::GirisWidget::GirisWidget(mongocxx::database *_db)
    :WContainerWidget(),
      db(_db),
      User(document{}.view())
{

    setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));
    setPadding(0,AllSides);
    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    //    mMainContainer->setMaximumSize(1250,WLength::Auto);
    mMainContainer->setPadding(0,AllSides);
    mMainContainer->setId("GirisWidgetID");
    mMainContainer->setMargin(90,Side::Top);
    mMainContainer->setContentAlignment(AlignmentFlag::Center);


    mHeaderContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mHeaderContainer->setContentAlignment(AlignmentFlag::Center);
    //    mHeaderContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    mHeaderContainer->setPadding(0,AllSides);
    mHeaderContainer->setMaximumSize(1250,WLength::Auto);



    mContentContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->setContentAlignment(AlignmentFlag::Center);
    //    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    mContentContainer->setPadding(0,AllSides);
    mContentContainer->setMaximumSize(1250,WLength::Auto);

    this->initHeader();

    this->initLoginScreen();



}

void Giris::GirisWidget::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::GirisWidget::initHeader()
{
    std::string path = "img/baskanBackGround.JPG";

    auto img = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
    img->setHeight(100);
    img->setPadding(0,AllSides);

    auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
    gradientContainer->setHeight(100);
    gradientContainer->addStyleClass("SliderDetailTextBackground");
    auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
    layout->addStretch(1);
    mPageTitle = layout->addWidget(cpp14::make_unique<WText>("Giriş"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
    mPageTitle->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
}

void Giris::GirisWidget::initLoginScreen()
{

    mContentContainer->clear();
    auto loginWidget = mContentContainer->addWidget(cpp14::make_unique<LoginWidget>(db));
    loginWidget->getClickLoginSucces().connect(this,&Giris::GirisWidget::initMenu);
    loginWidget->getClickLogicPersonelSucces().connect(this,&Giris::GirisWidget::initPersonelMenu);
}

void Giris::GirisWidget::initMenu(bsoncxx::document::value vatandas)
{

    mContentContainer->clear();
    this->User = vatandas;
    mPageTitle->setText("www.serik.bel.tr");



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Personel::telefon,this->User.view()[SBLDKeys::TC::cepTel].get_utf8().value));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("Filter Error {1}").arg(e.what()).toUTF8());
        return;
    }


    try {
        auto val = db->collection(SBLDKeys::Personel::collection).find_one(filter.view());
        if( val )
        {
            if( val.value().view().empty() )
            {
                std::cout << "init Sivil " << std::endl;
                this->initSivil();
            }else{
                std::cout << "init Option " << std::endl;
                this->initOption();
            }
        }else{
            std::cout << "init Sivil " << std::endl;
            this->initSivil();
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error",WString("DB Error").arg(e.what()).toUTF8());
    }

}

void Giris::GirisWidget::initPersonelMenu(bsoncxx::document::value vatandas)
{
    std::cout << "127: mContentContainer->clear();" << std::endl;
    mContentContainer->clear();
    //    this->User = std::move(vatandas);
    this->User = vatandas;

    std::cout << "132:mPageTitle->setText(\"INTERAKTIF ALAN\");" << std::endl;

    mPageTitle->setText("INTERAKTIF ALAN");



    std::string str;
    try {
        str = User.view()[SBLDKeys::Personel::telefon].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::cout << bsoncxx::to_json(User.view()) << std::endl;
        this->showMessage("Error",WString("Error initPersonel {1}").arg(e.what()).toUTF8());
        return;
    }

//    std::cout << "147: add Personel Widget" << std::endl;

    mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,str));

}

void Giris::GirisWidget::initOption()
{

    mContentContainer->clear();

    auto mContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    mContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto mRow = mContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    mRow->addStyleClass(Bootstrap::Grid::row);

    {
        auto text = mRow->addWidget(cpp14::make_unique<WText>("Belediye Personeli Oluduğunuz Tespit Edildi. Ne Olarak Devam Etmek istersiniz?"));
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        text->setMargin(50,Side::Top);
        text->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    }

    {
        auto SivilContainer = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
        SivilContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        SivilContainer->setPadding(20,AllSides);
        SivilContainer->setContentAlignment(AlignmentFlag::Center);
        auto container = SivilContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->clicked().connect(this,&Giris::GirisWidget::initSivil);

        container->setHeight(200);
        container->setWidth(200);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::Fuchsia));
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        layout->addStretch(1);
        auto text = layout->addWidget(cpp14::make_unique<WText>("Sivil Olarak Devam Et!"));
        text->setAttributeValue(Style::style,Style::font::size::s20px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        layout->addStretch(1);

    }

    {
        auto SivilContainer = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
        SivilContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        SivilContainer->setPadding(20,AllSides);
        SivilContainer->setContentAlignment(AlignmentFlag::Center);
        auto container = SivilContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->clicked().connect(this,&Giris::GirisWidget::initPersonel);

        container->setHeight(200);
        container->setWidth(200);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::Indigo));
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        layout->addStretch(1);
        auto text = layout->addWidget(cpp14::make_unique<WText>("Personel Olarak Devam Et!"));
        text->setAttributeValue(Style::style,Style::font::size::s20px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        layout->addStretch(1);
    }

}

void Giris::GirisWidget::initSivil()
{

    std::cout << "initSivil " << std::endl;
    mContentContainer->clear();
    auto sivil = mContentContainer->addWidget(cpp14::make_unique<SivilWidget>(db,&User));
}

void Giris::GirisWidget::initPersonel()
{

    {
        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Personel::telefon,User.view()[SBLDKeys::TC::cepTel].get_utf8().value));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Error initPersonel {1}").arg(e.what()).toUTF8());
            return;
        }

        try {
            auto var = db->collection(SBLDKeys::Personel::collection).find_one(filter.view());
            if( var )
            {
                if( !var.value().view().empty() )
                {
                    User = var.value();
                }else{
                    this->showMessage("Error",WString("Error initPersonel Document Empty").toUTF8());
                    return;
                }
            }else{
                this->showMessage("Error",WString("Error initPersonel No Document").toUTF8());
                return;
            }
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Error initPersonel {1}").arg(e.what()).toUTF8());
            return;
        }

    }


    //    std::cout << "INit Personel" << std::endl;
    std::string str;
    try {
        str = User.view()[SBLDKeys::Personel::telefon].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::cout << bsoncxx::to_json(User.view()) << std::endl;
        this->showMessage("Error",WString("Error initPersonel {1}").arg(e.what()).toUTF8());
        return;
    }
    mContentContainer->clear();
    auto sivil = mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,str));

}

Giris::LoginWidget::LoginWidget(mongocxx::database *_db)
    :WContainerWidget(),
      db(_db)
{

    WApplication::instance()->setInternalPath("/Login",true);
    setUserisVatandas(false);
    setUserisPersonel(false);

    addStyleClass(Bootstrap::Grid::container_fluid);

    this->initLoginScreen();

}

void Giris::LoginWidget::sendtempPasswordSMS()
{

    if( mTelefonNumarasi->text().toUTF8().size() != 11 )
    {
        this->showMessage("Uyarı","Telefon Numarası Uygun Formatta Değil. Lütfen Telefon Numaranızı başında 0 Kullanarak Giriniz");
        return;
    }

    if( mTelefonNumarasi->text().toUTF8().at(0) != '0' || mTelefonNumarasi->text().toUTF8().at(1) != '5' )
    {
        this->showMessage("Uyarı","Telefon Numarası Uygun Formatta Değil. Lütfen Telefon Numaranızı başında 0 Kullanarak Giriniz");
        return;
    }


    if( this->checkTCCollection(mTelefonNumarasi->text().toUTF8() ) )
    {
        setUserisPersonel(false);
        setUserisVatandas(true);

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::TC::cepTel,mTelefonNumarasi->text().toUTF8()));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("Bir Hata Oluştu: {1}").arg(e.what()).toUTF8());
            return;
        }

        try {
            auto val = this->db->collection(SBLDKeys::TC::collection).find_one(filter.view());
            if( val )
            {
                if( !val.value().view().empty() )
                {
                    try {
                        verificationCODE = val.value().view()[SBLDKeys::TC::password].get_utf8().value.to_string();
                    } catch (bsoncxx::exception &e) {
                        this->showMessage("UYARI",WString("Şifreniz Yok: {1}").arg(e.what()).toUTF8());
                        return;
                    }
                }
            }
        } catch (mongocxx::exception &e) {
            this->showMessage("UYARI",WString("Şifreniz Yok: {1}").arg(e.what()).toUTF8());
            return;
        }

    }else {
        setUserisVatandas(false);
        if( this->checkPersonelCollection(mTelefonNumarasi->text().toUTF8() ) )
        {
            setUserisPersonel(true);
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Personel::telefon,mTelefonNumarasi->text().toUTF8()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("UYARI",WString("Bir Hata Oluştu: {1}").arg(e.what()).toUTF8());
                return;
            }

            try {
                auto val = this->db->collection(SBLDKeys::Personel::collection).find_one(filter.view());
                if( val )
                {
                    if( !val.value().view().empty() )
                    {
                        try {
                            verificationCODE = val.value().view()[SBLDKeys::Personel::password].get_utf8().value.to_string();
                        } catch (bsoncxx::exception &e) {
                            this->showMessage("UYARI",WString("Şifreniz Yok: {1}").arg(e.what()).toUTF8());
                            return;
                        }
                    }
                }
            } catch (mongocxx::exception &e) {
                this->showMessage("UYARI",WString("Şifreniz Yok: {1}").arg(e.what()).toUTF8());
                return;
            }
        }else {
            setUserisPersonel(false);
            this->showMessage("UYARI","Kaydınız Bulunmamaktadır\nLütfen Kayıt Olunuz");
            return;
        }
    }


    sms = QString("Şifreniz: ") + verificationCODE.c_str() + QString(" .Bu İsteği Siz Bulunmadıysanız Dikkate Almayınız");

    QByteArray ar;

    ar.clear();

    QXmlStreamWriter stream(&ar);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("MainmsgBody");

    stream.writeTextElement("Command", "0");
    stream.writeTextElement("PlatformID", "1");
    stream.writeTextElement("ChannelCode", "474");
    stream.writeTextElement("UserName", "serikbel");
    stream.writeTextElement("PassWord", "haziran2018");
    stream.writeTextElement("Mesgbody", sms );
    stream.writeTextElement("Numbers", mTelefonNumarasi->text().toUTF8().c_str());
    stream.writeTextElement("Type", "1");
    stream.writeTextElement("Concat", "1");
    stream.writeTextElement("Originator", "SERiK BLD");

    stream.writeEndElement();

    stream.writeEndDocument();

    auto httpclient = this->addChild(std::make_unique<Http::Client>());

    Http::Message msg;
    msg.setHeader("Content-Type","application/json");

    msg.addBodyText(ar.toStdString());

    httpclient->setSslCertificateVerificationEnabled(false);

    mtelefonNumarasi = mTelefonNumarasi->text().toUTF8();

    httpclient->done().connect([=](AsioWrapper::error_code code, Http::Message message){

        auto filter = document{};

        if( getUserisVatandas() )
        {
            try {
                filter.append(kvp(SBLDKeys::TC::cepTel,mtelefonNumarasi.c_str()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("UYARI","Bir Hata Oluştu",e);
            }

            auto smsDoc = document{};

            try {
                smsDoc.append(kvp(SBLDKeys::TC::SmsLog::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }

            try {
                smsDoc.append(kvp(SBLDKeys::TC::SmsLog::sms,sms.toStdString().c_str()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }

            try {
                smsDoc.append(kvp(SBLDKeys::TC::SmsLog::smsid,message.body().c_str()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }


            try {
                smsDoc.append(kvp(SBLDKeys::TC::SmsLog::tarih,QDate::currentDate().toJulianDay()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }

            auto pushDoc = document{};

            try {
                pushDoc.append(kvp("$push",make_document(kvp(SBLDKeys::TC::smsLog,smsDoc))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }



            try {
                auto val = db->collection(SBLDKeys::TC::collection).update_one(filter.view(),pushDoc.view());
                if( val )
                {
                    if( !val.value().modified_count() )
                    {
                        this->showMessage("Error","Hata Oluştu");
                    }
                }else{
                    this->showMessage("Error","Hata Oluştu");
                }
            } catch (mongocxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }

        }else if ( getUserisPersonel() ) {
            try {
                filter.append(kvp(SBLDKeys::Personel::telefon,mtelefonNumarasi.c_str()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("UYARI","Bir Hata Oluştu",e);
            }


            auto smsDoc = document{};

            try {
                smsDoc.append(kvp(SBLDKeys::Personel::SmsLog::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
                smsDoc.append(kvp(SBLDKeys::Personel::SmsLog::sms,sms.toStdString().c_str()));
                smsDoc.append(kvp(SBLDKeys::Personel::SmsLog::smsid,message.body().c_str()));
                smsDoc.append(kvp(SBLDKeys::Personel::SmsLog::tarih,QDate::currentDate().toJulianDay()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }

            auto pushDoc = document{};

            try {
                pushDoc.append(kvp("$push",make_document(kvp(SBLDKeys::Personel::smsLog,smsDoc))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }


            try {
                auto val = db->collection(SBLDKeys::Personel::collection).update_one(filter.view(),pushDoc.view());
                if( val )
                {
                    if( !val.value().modified_count() )
                    {
                        this->showMessage("Error","Hata Oluştu");
                    }
                }else{
                    this->showMessage("Error","Hata Oluştu");
                }
            } catch (mongocxx::exception &e) {
                this->showMessage("Error","Bir Hata Ouştu",e);
            }
        }else {
        }

        this->removeChild(httpclient);
    });

    httpclient->post("http://processor.smsorigin.com/xml/process.aspx",msg);

    mUnuttumButton->setEnabled(false);
}

void Giris::LoginWidget::sendSMS(std::string numara, std::string sms)
{

    //    std::uniform_int_distribution<int> d(10000, 99999);
    //    std::random_device rd1;
    //    verificationCODE = std::to_string(d(rd1));

    //    sms = "Doğrulama Kodu: " + verificationCODE + " . Bu İsteği Siz Bulunmadıysanız Dikkate Almayınız!";

    QByteArray ar;

    ar.clear();

    QXmlStreamWriter stream(&ar);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("MainmsgBody");

    stream.writeTextElement("Command", "0");
    stream.writeTextElement("PlatformID", "1");
    stream.writeTextElement("ChannelCode", "474");
    stream.writeTextElement("UserName", "serikbel");
    stream.writeTextElement("PassWord", "haziran2018");
    stream.writeTextElement("Mesgbody", sms.c_str() );
    stream.writeTextElement("Numbers", numara.c_str());
    stream.writeTextElement("Type", "1");
    stream.writeTextElement("Concat", "1");
    stream.writeTextElement("Originator", "SERiK BLD");

    stream.writeEndElement();

    stream.writeEndDocument();

    auto httpclient = this->addChild(std::make_unique<Http::Client>());

    Http::Message msg;
    msg.setHeader("Content-Type","application/json");

    msg.addBodyText(ar.toStdString());

    httpclient->setSslCertificateVerificationEnabled(false);


    httpclient->done().connect([=](AsioWrapper::error_code code, Http::Message message){
        this->removeChild(httpclient);
    });

    httpclient->post("http://processor.smsorigin.com/xml/process.aspx",msg);

}

void Giris::LoginWidget::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::LoginWidget::showMessage(std::string title, std::string msg, bsoncxx::exception &e)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg+std::string{" - "}+e.what(),
                 Wt::Icon::Information, Wt::StandardButton::Ok));


    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::LoginWidget::showMessage(std::string title, std::string msg, mongocxx::exception &e)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg+std::string{" - "}+e.what(),
                 Wt::Icon::Information, Wt::StandardButton::Ok));


    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

bool Giris::LoginWidget::checkTCCollection(std::string tcno)
{
    auto filter = document{};
    try {
        filter.append(kvp(SBLDKeys::TC::cepTel,tcno.c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",std::string{"filter Oluşturulurken Hata Oluştu: "}+e.what());
        return false;
    }
    try {
        auto val = db->collection(SBLDKeys::TC::collection).count(filter.view());
        if( val )
        {
            return true;
        }else{
            return false;
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("UYARI",std::string{"DB Error: "}+e.what());
        return false;
    }
}

bool Giris::LoginWidget::checkPersonelCollection(std::string tcno)
{
    auto filter = document{};
    try {
        filter.append(kvp(SBLDKeys::Personel::telefon,tcno.c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",std::string{"filter Oluşturulurken Hata Oluştu: "}+e.what());
        return false;
    }
    try {
        auto val = db->collection(SBLDKeys::Personel::collection).count(filter.view());
        if( val )
        {
            return true;
        }else{
            return false;
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("UYARI",std::string{"DB Error: "}+e.what());
        return false;
    }
}

bool Giris::LoginWidget::getUserisVatandas() const
{
    return UserisVatandas;
}

void Giris::LoginWidget::setUserisVatandas(bool value)
{
    UserisVatandas = value;
}

void Giris::LoginWidget::ConfirmLogin()
{



    if( mTelefonNumarasi->text().toUTF8().size() != 11 )
    {
        this->showMessage("Uyarı","Telefon Numarası Uygun Formatta Değil. Lütfen Telefon Numaranızı başında 0 Kullanarak Giriniz");
        return;
    }

    if( mTelefonNumarasi->text().toUTF8().at(0) != '0' || mTelefonNumarasi->text().toUTF8().at(1) != '5' )
    {
        this->showMessage("Uyarı","Telefon Numarası Uygun Formatta Değil. Lütfen Telefon Numaranızı başında 0 Kullanarak Giriniz");
        return;
    }

    if( this->checkTCCollection(mTelefonNumarasi->text().toUTF8() ) )
    {
//        std::cout << "phone is exist TC Collection" << std::endl;
        setUserisVatandas(true);
        setUserisPersonel(false);
        this->ConfirmVatandas();
    }else {
        setUserisVatandas(false);
        if( this->checkPersonelCollection(mTelefonNumarasi->text().toUTF8() ) )
        {
//            std::cout << "Personel" << std::endl;
            setUserisPersonel(true);
            this->ConfirmPersonel();
        }else {
            setUserisPersonel(false);
            this->showMessage("UYARI","Kaydınız Bulunmamaktadır\nLütfen Kayıt Olunuz");
            return;
        }
    }
}

void Giris::LoginWidget::ConfirmVatandas()
{



    std::cout << "Confirm Vatandas" << std::endl;

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::TC::cepTel,mTelefonNumarasi->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Bir Hata Oluştu.",e);
        return;
    }

    bsoncxx::document::value vatandas(document{}.view());

    bool mSucces = false;
    try {
        auto val = db->collection(SBLDKeys::TC::collection).find_one(filter.view());
        if( val )
        {
            if( !val.value().view().empty() )
            {
                //                std::cout << "FIND Vat " << bsoncxx::to_json(val.value().view()) << std::endl;
                vatandas = val.value();

                std::string password;
                try {
                    password = vatandas.view()[SBLDKeys::TC::password].get_utf8().value.to_string();
                } catch (bsoncxx::exception &e) {
                    std::cout << "Password Error: " << e.what() << std::endl;
                }

                std::string tel;
                try {
                    tel = vatandas.view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string();
                } catch (bsoncxx::exception &e) {
                    std::cout << "tel Error: " << e.what() << std::endl;
                }

                if ( password.size() )
                {
                    if( password == mSifre->text().toUTF8() )
                    {
                        mSucces = true;
                    }else{
                        this->showMessage("UYARI","Şifreniz Hatalı Tekrar Deneyiniz");
                    }
                }else{
                    this->CreatePassword(tel);
                }
            }else{
                this->showMessage("UYARI","Kaydınız Bulunmamakta. Lütfen Yeni Kayıt Oluşturunuz!");
            }
        }else{
            this->showMessage("UYARI","Kaydınız Bulunmamakta. Lütfen Yeni Kayıt Oluşturunuz!");
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error","Bir Hata Oluştu",e);
        return;
    }

    if( mSucces )
    {
        _ClickLogicSucces.emit(vatandas);
    }
}

void Giris::LoginWidget::ConfirmPersonel()
{
    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Personel::telefon,mTelefonNumarasi->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Bir Hata Oluştu.",e);
        return;
    }

    bsoncxx::document::value vatandas(document{}.view());


    bool mSucces = false;
    try {
        auto val = db->collection(SBLDKeys::Personel::collection).find_one(filter.view());
        if( val )
        {
            if( !val.value().view().empty() )
            {
                vatandas = val.value();
                try {
                    std::string password = val.value().view()[SBLDKeys::Personel::password].get_utf8().value.to_string();
                    std::cout << "Personel PassWord: " << password <<  "  " << mSifre->text().toUTF8() << std::endl;
                    if ( val.value().view()[SBLDKeys::Personel::password].length() )
                    {
                        if( password == mSifre->text().toUTF8() )
                        {
                            std::cout << "Giris Basariili" << std::endl;
                            mSucces = true;
                        }else{
                            this->showMessage("UYARI","Şifreniz Hatalı Tekrar Deneyiniz");
                        }
                    }else{
                        std::cout << "No PAssword Legnth Create Password" << std::endl;
                        this->CreatePersonelPassword(val.value().view()[SBLDKeys::Personel::telefon].get_utf8().value.to_string());
                    }
                } catch (bsoncxx::exception &e) {
                    std::cout << "Try Catch Create Password" << std::endl;
                    this->CreatePersonelPassword(val.value().view()[SBLDKeys::Personel::telefon].get_utf8().value.to_string());
                }
            }else{
                this->showMessage("UYARI","Kaydınız Bulunmamakta.!");
            }
        }else{
            this->showMessage("UYARI","Kaydınız Bulunmamakta.!");
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error","Bir Hata Oluştu",e);
        return;
    }


    if( mSucces )
    {
        std::cout << "_ClickLogicPersonelSucces" << std::endl;
        _ClickLogicPersonelSucces.emit(vatandas);
    }

}

void Giris::LoginWidget::CreatePassword(std::string numara)
{

    std::cout << "Create Password: " << numara << std::endl;
    dialog = this->addChild(std::make_unique<WDialog>("Şifre oluştur"));

    auto container = dialog->contents();

    auto mainLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


    auto infotTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Hesabınız Bulunmakta Ancak Şifreniz Bulunmamakta"),0,AlignmentFlag::Center);



    auto layoutContainerControl = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    auto layoutControl = layoutContainerControl->setLayout(cpp14::make_unique<WHBoxLayout>());
    auto newpassword = layoutControl->addWidget(cpp14::make_unique<WLineEdit>());
    newpassword->setPlaceholderText("Yeni Şifrenizi Giriniz");
    auto sendbtn = layoutControl->addWidget(cpp14::make_unique<WPushButton>("Gönder"));


    sendbtn->clicked().connect([=](){
        std::uniform_int_distribution<int> d(10000, 99999);
        std::random_device rd1;
        confirmationCODE = std::to_string(d(rd1)).c_str();
        std::string sms = QString("Doğrulama Kodu: ").toStdString() + confirmationCODE.c_str() + QString(" .Bu İsteği Siz Bulunmadıysanız Dikkate Almayınız").toStdString();
        this->sendSMS(numara,sms);
        sendbtn->setEnabled(false);
        newpassword->setEnabled(false);
    });

    auto layoutContainer = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    auto layout = layoutContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    auto dogrulamaKodu = layout->addWidget(cpp14::make_unique<WLineEdit>());
    dogrulamaKodu->setPlaceholderText("Doğrulama Kodunu Giriniz");



    auto okcancelLayout = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    auto okcancellayout = okcancelLayout->setLayout(cpp14::make_unique<WHBoxLayout>());
    auto cancelbtn = okcancellayout->addWidget(cpp14::make_unique<WPushButton>("İptal"),0,AlignmentFlag::Right);
    cancelbtn->clicked().connect(dialog,&WDialog::reject);

    auto okbtn = okcancellayout->addWidget(cpp14::make_unique<WPushButton>("Tamam"),0,AlignmentFlag::Left);
    okbtn->clicked().connect([=](){
        if( confirmationCODE == dogrulamaKodu->text().toUTF8() )
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::TC::cepTel,numara.c_str()));
            } catch (bsoncxx::exception &e) {
                infotTitle->setText(e.what());
            }

            auto setObj = document{};

            try {
                setObj.append(kvp("$set",make_document(kvp(SBLDKeys::TC::password,newpassword->text().toUTF8().c_str()))));
            } catch (bsoncxx::exception &e) {
                infotTitle->setText(e.what());
            }

            try {
                auto upt = db->collection(SBLDKeys::TC::collection).update_one(filter.view(),setObj.view());
                if( upt )
                {
                    if( upt.value().modified_count() )
                    {
                        infotTitle->setText("Şifreniz Başarılı Bir Şekilde Oluşturuldu");
                        okbtn->setEnabled(false);
                        cancelbtn->setEnabled(false);
                        dogrulamaKodu->setEnabled(false);
                        WTimer::singleShot(std::chrono::milliseconds(2000), this,&LoginWidget::removeDialog);
                    }else{
                        infotTitle->setText("Şifreniz Oluşturulamadı");
                    }
                }else{
                    infotTitle->setText("Şifreniz Oluşturulamadı");
                }
            } catch (mongocxx::exception &e) {
                infotTitle->setText("Şifreniz Oluşturulamadı");
            }
        }else{
            infotTitle->setText("<h3>Doğrulama Kodu Hatalı</h3>");
        }
    });
    dialog->show();
}

void Giris::LoginWidget::CreatePersonelPassword(std::string numara)
{
    dialog = this->addChild(std::make_unique<WDialog>("Şifre oluştur"));

    auto container = dialog->contents();

    auto mainLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


    auto infotTitle = mainLayout->addWidget(cpp14::make_unique<WText>("Yani Şifre Tanımla"),0,AlignmentFlag::Center);



    auto layoutContainerControl = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    auto layoutControl = layoutContainerControl->setLayout(cpp14::make_unique<WHBoxLayout>());
    auto newpassword = layoutControl->addWidget(cpp14::make_unique<WLineEdit>());
    newpassword->setPlaceholderText("Yeni Şifrenizi Giriniz");
    auto sendbtn = layoutControl->addWidget(cpp14::make_unique<WPushButton>("Gönder"));


    sendbtn->clicked().connect([=](){
        std::uniform_int_distribution<int> d(10000, 99999);
        std::random_device rd1;
        confirmationCODE = std::to_string(d(rd1)).c_str();
        std::string sms = QString("Doğrulama Kodu: ").toStdString() + confirmationCODE.c_str() + QString(" .Bu İsteği Siz Bulunmadıysanız Dikkate Almayınız").toStdString();
        this->sendSMS(numara,sms);
        sendbtn->setEnabled(false);
        newpassword->setEnabled(false);
    });

    auto layoutContainer = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    auto layout = layoutContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    auto dogrulamaKodu = layout->addWidget(cpp14::make_unique<WLineEdit>());
    dogrulamaKodu->setPlaceholderText("Doğrulama Kodunu Giriniz");



    auto okcancelLayout = mainLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    auto okcancellayout = okcancelLayout->setLayout(cpp14::make_unique<WHBoxLayout>());
    auto cancelbtn = okcancellayout->addWidget(cpp14::make_unique<WPushButton>("İptal"),0,AlignmentFlag::Right);
    cancelbtn->clicked().connect(dialog,&WDialog::reject);

    auto okbtn = okcancellayout->addWidget(cpp14::make_unique<WPushButton>("Tamam"),0,AlignmentFlag::Left);
    okbtn->clicked().connect([=](){
        if( confirmationCODE == dogrulamaKodu->text().toUTF8() )
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Personel::telefon,numara.c_str()));
            } catch (bsoncxx::exception &e) {
                infotTitle->setText(e.what());
            }

            auto setObj = document{};

            try {
                setObj.append(kvp("$set",make_document(kvp(SBLDKeys::Personel::password,newpassword->text().toUTF8().c_str()))));
            } catch (bsoncxx::exception &e) {
                infotTitle->setText(e.what());
            }

            try {
                auto upt = db->collection(SBLDKeys::Personel::collection).update_one(filter.view(),setObj.view());
                if( upt )
                {
                    if( upt.value().modified_count() )
                    {
                        infotTitle->setText("Şifreniz Başarılı Bir Şekilde Oluşturuldu");
                        okbtn->setEnabled(false);
                        cancelbtn->setEnabled(false);
                        dogrulamaKodu->setEnabled(false);
                        WTimer::singleShot(std::chrono::milliseconds(2000), this,&LoginWidget::removeDialog);
                    }else{
                        infotTitle->setText("Şifreniz Oluşturulamadı");
                    }
                }else{
                    infotTitle->setText("Şifreniz Oluşturulamadı");
                }
            } catch (mongocxx::exception &e) {
                infotTitle->setText("Şifreniz Oluşturulamadı");
            }
        }else{
            infotTitle->setText("<h3>Doğrulama Kodu Hatalı</h3>");
        }
    });
    dialog->show();
}

void Giris::LoginWidget::removeDialog()
{

    this->removeChild(dialog);


}

void Giris::LoginWidget::kayitOld()
{

    clear();

    auto row = addWidget(cpp14::make_unique<WContainerWidget>());

    row->addStyleClass(Bootstrap::Grid::row);

    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    container->setContentAlignment(AlignmentFlag::Center);
    auto title = container->addWidget(cpp14::make_unique<WText>("<h4>Kayıt Ekranı</h4>"));
    auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
    _container->setMaximumSize(750,WLength::Auto);

    auto glayout = _container->setLayout(cpp14::make_unique<WGridLayout>());

    int _row = 0;
    {
        glayout->addWidget(cpp14::make_unique<WLabel>("TCNO"),_row,0,AlignmentFlag::Right|AlignmentFlag::Middle);
        lineEditTc = glayout->addWidget(cpp14::make_unique<WLineEdit>(),_row,1,AlignmentFlag::Center);
        lineEditTc->setMargin(25,Side::Right|Side::Left);
        lineEditTc->setInputMask("99999999999");
        _row++;
    }

    {
        glayout->addWidget(cpp14::make_unique<WLabel>("Ad Soyad"),_row,0,AlignmentFlag::Right|AlignmentFlag::Middle);
        lineEditAdSoyad = glayout->addWidget(cpp14::make_unique<WLineEdit>(),_row,1,AlignmentFlag::Center);
        lineEditAdSoyad->setMargin(25,Side::Right|Side::Left);
        _row++;
    }

    {
        glayout->addWidget(cpp14::make_unique<WLabel>("Cep Telefonu"),_row,0,AlignmentFlag::Right|AlignmentFlag::Middle);
        lineEditCepTelefonu = glayout->addWidget(cpp14::make_unique<WLineEdit>(),_row,1,AlignmentFlag::Center);
        lineEditCepTelefonu->setMargin(25,Side::Right|Side::Left);
        lineEditCepTelefonu->setInputMask("05999999999");
        _row++;
    }

    {
        glayout->addWidget(cpp14::make_unique<WLabel>("Normal Telefon"),_row,0,AlignmentFlag::Right|AlignmentFlag::Middle);
        lineEditNormalTelefonu = glayout->addWidget(cpp14::make_unique<WLineEdit>(),_row,1,AlignmentFlag::Center);
        lineEditNormalTelefonu->setMargin(25,Side::Right|Side::Left);
        _row++;
    }



    {
        glayout->addWidget(cpp14::make_unique<WLabel>("Mahalle"),_row,0,AlignmentFlag::Right|AlignmentFlag::Middle);
        comboBoxMahalle = glayout->addWidget(cpp14::make_unique<WComboBox>(),_row,1,AlignmentFlag::Center);
        comboBoxMahalle->setMargin(25,Side::Right|Side::Left);
        _row++;
    }

    {
        glayout->addWidget(cpp14::make_unique<WLabel>("Tam Adres"),_row,0,AlignmentFlag::Right|AlignmentFlag::Middle);
        lineEditTamAdres = glayout->addWidget(cpp14::make_unique<WLineEdit>(),_row,1,AlignmentFlag::Center);
        lineEditTamAdres->setMargin(25,Side::Right|Side::Left);
        _row++;
    }



    // init Mahalleler
    {
        try {
            auto cursor = db->collection(SBLDKeys::Mahalle::collection).find(document{}.view());

            for( auto doc : cursor )
            {
                comboBoxMahalle->addItem(WString("{1}").arg(doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string().c_str()));
            }

        } catch (mongocxx::exception &e) {
            this->showMessage("Code:003",WString("Bir Hata Oluştu. Lütfen Daha Sonra Deneyiniz{1}").arg(e.what()).toUTF8());
            return;
        }
    }

    auto btnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    btnContainer->setMaximumSize(350,WLength::Auto);
    btnContainer->setMargin(25,Side::Top|Side::Bottom);

    WToolBar* toolbar = btnContainer->addWidget(cpp14::make_unique<WToolBar>());

    auto ok = std::make_unique<WPushButton>("Kaydet");
    ok->addStyleClass(Bootstrap::Button::Primary);
    ok->clicked().connect(this,&Giris::LoginWidget::CheckDatabase);
    toolbar->addButton(std::move(ok),AlignmentFlag::Center);

    auto cancel = std::make_unique<WPushButton>("İptal");
    cancel->addStyleClass(Bootstrap::Button::Warning);
    toolbar->addButton(std::move(cancel));

}

void Giris::LoginWidget::CheckDatabase()
{

    auto filter = document{};

    try {
        filter.append(kvp("$or",make_array(make_document(kvp(SBLDKeys::TC::cepTel,lineEditCepTelefonu->text().toUTF8())),
                                           make_document(kvp(SBLDKeys::TC::tcno,lineEditTc->text().toUTF8())))));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg(SBLDKeys::TC::cepTel).arg(e.what()).toUTF8());
        return;
    }


    std::int64_t count = 0;
    try {
        count = db->collection(SBLDKeys::TC::collection).count(filter.view());

    } catch (mongocxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg("DB Count").arg(e.what()).toUTF8());
        return;
    }


    if( count )
    {
        this->showMessage("Hata","Cep Telefonu Yada TCNO Sistemde Kayıtlı. Lütfen Çağrı Merkezini Arayınız. 444 9 722");
        return;
    }else{
        this->ComfirmCepTelefonu();
    }


}

void Giris::LoginWidget::ComfirmCepTelefonu()
{

    if( !lineEditAdSoyad->text().toUTF8().size() )
    {
        this->showMessage("Error",WString("{1}").arg("Ad Soyad Boş Bırakılamaz").toUTF8());
        return;
    }

    if( lineEditTc->text().toUTF8().size() != 11 )
    {
        this->showMessage("Error",WString("{1}").arg("TCNO Eksik Yada Yanlış").toUTF8());
        return;
    }

    if( lineEditCepTelefonu->text().toUTF8().size() != 11 )
    {
        this->showMessage("Error",WString("{1}").arg("Cep Telefonu Eksik Yada Yanlış").toUTF8());
        return;
    }

    if( comboBoxMahalle->currentText().toUTF8() == "NULL" )
    {
        this->showMessage("Error",WString("{1}").arg("Mahalle Seçmediniz").toUTF8());
        return;
    }

    std::uniform_int_distribution<int> d(10000, 99999);
    std::random_device rd1;
    confirmationCODE = std::to_string(d(rd1)).c_str();
    std::string sms = QString("Doğrulama Kodu: ").toStdString() + confirmationCODE.c_str() + QString(" .Bu İsteği Siz Bulunmadıysanız Dikkate Almayınız").toStdString();
    this->sendSMS(lineEditCepTelefonu->text().toUTF8(),sms);


    auto dialog = addChild(cpp14::make_unique<WDialog>("Doğrulama Kodu"));


    auto text = dialog->contents()->addWidget(cpp14::make_unique<WText>("Doğrulama Kodunu Giriniz"));
    auto lineEdit = dialog->contents()->addWidget(cpp14::make_unique<WLineEdit>());
    lineEdit->setPlaceholderText("Doğrulama Kodunu Giriniz");
    lineEdit->setInputMask("99999");

    auto ok = dialog->footer()->addWidget(cpp14::make_unique<WPushButton>("Doğrula"));
    auto cancel = dialog->footer()->addWidget(cpp14::make_unique<WPushButton>("İptal"));

    ok->clicked().connect([=](){
        std::cout << "CONFIRMATION CODE: " << lineEdit->text().toUTF8() << " - " << this->confirmationCODE << std::endl;
        if( lineEdit->text().toUTF8() != this->confirmationCODE )
        {
            text->setText(" ! Doğrulama Kodunu Hatalı Girdiniz ! ");
        }else{
            this->SaveNewTC();
            this->removeChild(dialog);
        }
    });

    cancel->clicked().connect(dialog,&WDialog::reject);


    dialog->finished().connect([=](){

        switch (dialog->result()) {

        case WDialog::Code::Accepted:
            this->SaveNewTC();
            this->removeChild(dialog);
            break;


        case WDialog::Code::Rejected:
            this->removeChild(dialog);
            break;

        default:
            break;

        }


    });

    dialog->show();


}

void Giris::LoginWidget::SaveNewTC()
{

    if( !lineEditAdSoyad->text().toUTF8().size() )
    {
        this->showMessage("Error",WString("{1}").arg("Ad Soyad Boş Bırakılamaz").toUTF8());
        return;
    }

    if( lineEditTc->text().toUTF8().size() != 11 )
    {
        this->showMessage("Error",WString("{1}").arg("TCNO Eksik Yada Yanlış").toUTF8());
        return;
    }

    if( lineEditCepTelefonu->text().toUTF8().size() != 11 )
    {
        this->showMessage("Error",WString("{1}").arg("Cep Telefonu Eksik Yada Yanlış").toUTF8());
        return;
    }

    if( comboBoxMahalle->currentText().toUTF8() == "NULL" )
    {
        this->showMessage("Error",WString("{1}").arg("Mahalle Seçmediniz").toUTF8());
        return;
    }


    auto doc = document{};

    try {
        doc.append(kvp(SBLDKeys::TC::tcno,lineEditTc->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg(SBLDKeys::TC::tcno).arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::isimsoyisim,lineEditAdSoyad->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg(SBLDKeys::TC::isimsoyisim).arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::cepTel,lineEditCepTelefonu->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg(SBLDKeys::TC::cepTel).arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::normalTel,lineEditNormalTelefonu->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg(SBLDKeys::TC::normalTel).arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::mahalle,comboBoxMahalle->currentText().toUTF8()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg(SBLDKeys::TC::mahalle).arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::tamadres,lineEditNormalTelefonu->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg(SBLDKeys::TC::tamadres).arg(e.what()).toUTF8());
        return;
    }




    try {
        auto ins = db->collection(SBLDKeys::TC::collection).insert_one(doc.view());
        if( ins )
        {
            if( ins.value().result().inserted_count() )
            {
                this->initLoginScreen();
            }else{
                this->showMessage("Error",WString("{1}").arg("Kayıt Yapılamadı").toUTF8());
                return;
            }
        }else{
            this->showMessage("Error",WString("{1}").arg("Kayıt başarısız").toUTF8());
            return;
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error",WString("{1} - {2}").arg("DB insert").arg(e.what()).toUTF8());
        return;
    }


}

Signal<bsoncxx::document::value> &Giris::LoginWidget::getClickLogicPersonelSucces()
{
    return _ClickLogicPersonelSucces;
}

void Giris::LoginWidget::initLoginScreen()
{


    clear();
    auto mRow = addWidget(cpp14::make_unique<WContainerWidget>());
    mRow->addStyleClass(Bootstrap::Grid::row);
    mRow->setContentAlignment(AlignmentFlag::Center);
    mRow->setAttributeValue(Style::style,Style::Border::border("1px solid green"));



    {
        auto container = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setContentAlignment(AlignmentFlag::Center);

        container->setMargin(100,Side::Top);

        auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        sContainer->setMaximumSize(500,WLength::Auto);
        sContainer->setAttributeValue(Style::style,Style::Border::border("2px solid gray")+Style::Border::borderRardius("20","20","20","20"));
        sContainer->setPadding(25,AllSides);

        auto gLayout = sContainer->setLayout(cpp14::make_unique<WGridLayout>());

        {
            auto text = gLayout->addWidget(cpp14::make_unique<WText>("Telefon Numarası"),0,0,AlignmentFlag::Center|AlignmentFlag::Middle);
            mTelefonNumarasi = gLayout->addWidget(cpp14::make_unique<WLineEdit>(),0,1,AlignmentFlag::Center);
            mTelefonNumarasi->setPlaceholderText("Telefon Numarasını Giriniz");
            mTelefonNumarasi->setWidth(150);
            mTelefonNumarasi->setInputMask("99999999999");
            if( TEST )
            {
                mTelefonNumarasi->setText("05326778052");
            }
            //
        }
        {
            auto text = gLayout->addWidget(cpp14::make_unique<WText>("Şifreniz"),1,0,AlignmentFlag::Center|AlignmentFlag::Middle);
            mSifre = gLayout->addWidget(cpp14::make_unique<WLineEdit>(),1,1,AlignmentFlag::Center);
            mSifre->setPlaceholderText("Şifrenizi Girniz");
            mSifre->setEchoMode(EchoMode::Password);
            mSifre->setWidth(150);
            if( TEST )
            {
                mSifre->setText("R2587ÇNULL");
            }
            //            mSifre->setText("R2587ÇNULL");
        }
    }


    {
        auto btnWidget = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
        btnWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto layout = btnWidget->setLayout(cpp14::make_unique<WVBoxLayout>());

        mGirisButton = layout->addWidget(cpp14::make_unique<WPushButton>("Giriş"),0,AlignmentFlag::Center);
        mGirisButton->addStyleClass(Bootstrap::Button::Primary);
        mGirisButton->clicked().connect(this,&Giris::LoginWidget::ConfirmLogin);

        mUnuttumButton = layout->addWidget(cpp14::make_unique<WPushButton>("Şifremi Unuttum"),0,AlignmentFlag::Center);
        mUnuttumButton->addStyleClass(Bootstrap::Button::Warning);
        mUnuttumButton->clicked().connect(this,&Giris::LoginWidget::sendtempPasswordSMS);

        mkayitOlButton = layout->addWidget(cpp14::make_unique<WPushButton>("Kayıt Ol"),0,AlignmentFlag::Center);
        mkayitOlButton->addStyleClass(Bootstrap::Button::info);
        mkayitOlButton->clicked().connect(this,&Giris::LoginWidget::kayitOld);
    }

}

bool Giris::LoginWidget::getUserisPersonel() const
{
    return UserisPersonel;
}

Signal<bsoncxx::document::value> &Giris::LoginWidget::getClickLoginSucces()
{
    return this->_ClickLogicSucces;
}

void Giris::LoginWidget::setUserisPersonel(bool value)
{
    UserisPersonel = value;
}

Giris::SivilWidget::SivilWidget(mongocxx::database *_db, bsoncxx::document::value *_user)
    :WContainerWidget(),
      db(_db),
      UserValue(_user)
{

    //    std::cout << "init SivilWidget Constructor" << std::endl;
    WApplication::instance()->setInternalPath("/Sivil",true);

    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mMainContainer->setId("initSivilWidgetId");

    auto mRow = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mRow->addStyleClass(Bootstrap::Grid::row);

    mHeaderContainer = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
    mHeaderContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    this->initHeader();

    mMenuContainer = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
    mMenuContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2);

    mContentContainer = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10);

    auto tempContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    //    tempContainer->setHeight(1200);
    //    tempContainer->setOverflow(Overflow::Scroll);
    mContentWidget = tempContainer->addWidget(cpp14::make_unique<WStackedWidget>());

    mContentWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::CubicBezier,350),true);

    this->initMenu();

}

void Giris::SivilWidget::initMenu()
{

    std::cout << "initSivil Menu" << std::endl;
    auto _mContainer = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _mContainer->setMargin(35,Side::Bottom|Side::Top);

    Wt::WMenu *menu = _mContainer->addWidget(Wt::cpp14::make_unique<Wt::WMenu>(mContentWidget));
    menu->setStyleClass("nav nav-pills nav-stacked");


    // Add menu items using the default lazy loading policy.
    menu->addItem("Bilgilerim", Wt::cpp14::make_unique<Bilgilerim>(db,UserValue));
    menu->addItem("Taleplerim", Wt::cpp14::make_unique<Taleplerim>(db,UserValue));
    menu->addItem("Başvurularım", Wt::cpp14::make_unique<Basvurularim>(db,UserValue));
    menu->addItem("Yeni Talep", Wt::cpp14::make_unique<Talep>(db,UserValue));
    menu->addItem("Yeni Başvuru", Wt::cpp14::make_unique<Basvuru>(db,UserValue));





}

void Giris::SivilWidget::initHeader()
{


    auto _mContainer = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _mContainer->setMargin(10,AllSides);
    _mContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    _mContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AliceBlue));

    auto row = _mContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    auto photoWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    photoWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_2);
    photoWidget->setAttributeValue(Style::style,Style::Border::border("1px solid gray"));
    auto img = photoWidget->addWidget(cpp14::make_unique<WImage>(WLink("img/person.jpg")));


    {
        auto infoContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        infoContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10);

        auto container = infoContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        layout->addStretch(1);

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->UserValue->view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Left);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->UserValue->view()[SBLDKeys::TC::mahalle].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Left);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->UserValue->view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Left);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }
        layout->addStretch(1);
    }





}

Giris::Taleplerim::Taleplerim(mongocxx::database *_db, bsoncxx::document::value *_user)
    :WContainerWidget(),
      db(_db),
      user(_user)
{

    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    auto title = row->addWidget(cpp14::make_unique<WText>("<h3>Taleplerim/Şikayetlerim</h3>"));
    title->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);


    toolBarContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    toolBarContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    Wt::WToolBar *toolBar = toolBarContainer->addWidget(Wt::cpp14::make_unique<Wt::WToolBar>());
    toolBar->setMargin(25,Side::Top|Side::Bottom);

    auto devamBtn = createColorButton("btn-primary", SBLDKeys::SikayetKey::durumType::devamediyor);
    devamBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
    });
    toolBar->addButton(std::move(devamBtn));

    auto iptalBtn = createColorButton("btn-danger", SBLDKeys::SikayetKey::durumType::iptalEdildi);
    iptalBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::iptalEdildi);
    });
    toolBar->addButton(std::move(iptalBtn));


    auto tamamlananBtn = createColorButton("btn-success", SBLDKeys::SikayetKey::durumType::tamamlandi);
    tamamlananBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::tamamlandi);
    });
    toolBar->addButton(std::move(tamamlananBtn));


    auto beklemeBtn = createColorButton("btn-warning", SBLDKeys::SikayetKey::durumType::beklemede);
    beklemeBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::beklemede);
    });
    toolBar->addButton(std::move(beklemeBtn));

    auto hepsiBtn = createColorButton("btn-inverse", "Hepsi");
    hepsiBtn->clicked().connect([=](){
        this->initTalepler("Hepsi");
    });
    toolBar->addButton(std::move(hepsiBtn));


    mContentContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);



    this->_clickTalep.connect(this,&Taleplerim::setDetail);

    this->initTalepler("Hepsi");


}

void Giris::Taleplerim::initTalepler(std::string filterKey)
{

    mContentContainer->clear();
    auto filter = document{};

    if( filterKey != "Hepsi" )
    {
        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,filterKey.c_str()));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error","Bir Hata Oluştu",e);
            return;
        }
    }



    try {
        filter.append(kvp(SBLDKeys::SikayetKey::mainKey::vatandas,user->view()[SBLDKeys::TC::tcno].get_utf8().value));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Bir Hata Oluştu",e);
        return;
    }


    auto table = mContentContainer->addWidget(cpp14::make_unique<WTable>());
    table->setHeaderCount(1);
    table->setWidth(WLength("100%"));

    table->addStyleClass("hover table-hover condensed table-condensed stripes table-striped");

    table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("#"));
    table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Durum"));
    table->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Mahalle"));
    table->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Birim"));
    table->elementAt(0, 4)->addWidget(cpp14::make_unique<WText>("Saat"));
    table->elementAt(0, 5)->addWidget(cpp14::make_unique<WText>("Tarih"));



    oidList.clear();

    try {
        auto cursor = db->collection(SBLDKeys::SikayetKey::collection).find(filter.view());


        for( auto doc : cursor )
        {

            oidListitem item;
            try {
                item.oid = doc[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.oid = "null";
            }
            try {
                item.durum = doc[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.durum = e.what();
            }
            try {
                item.mahalle = doc[SBLDKeys::SikayetKey::mainKey::mahalle].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.mahalle = e.what();
            }

            try {
                item.birim = doc[SBLDKeys::SikayetKey::mainKey::birim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.birim = e.what();
            }

            try {
                item.saat = doc[SBLDKeys::SikayetKey::mainKey::saat].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.saat = e.what();
            }

            try {
                item.tarih = doc[SBLDKeys::SikayetKey::mainKey::tarih].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.tarih = e.what();
            }

            oidList.push_back(item);

        }

    } catch (mongocxx::exception &e) {
        this->showMessage("Error","Database Error",e );
        return;
    }


    int row = 1;
    for( auto item : oidList )
    {

        int col = 0;
        {
            auto container = std::make_unique<WContainerWidget>();
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(row)));
            container->setMargin(10,Side::Top|Side::Bottom);
            table->elementAt(row,col++)->addWidget(std::move(container));
        }
        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.durum));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Durum Atanmamış"));
            }
            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.mahalle));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Mahalle Atanmamış"));
            }
            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.birim));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Birim Atanmamış"));
            }

            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.saat));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Saat Atanmamış"));
            }

            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.tarih));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Tarih Atanmamış"));
            }

            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        row++;
    }


}

void Giris::Taleplerim::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Taleplerim::showMessage(std::string title, std::string msg, bsoncxx::exception &e)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg+std::string{" - "}+e.what(),
                 Wt::Icon::Information, Wt::StandardButton::Ok));


    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Taleplerim::showMessage(std::string title, std::string msg, mongocxx::exception &e)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg+std::string{" - "}+e.what(),
                 Wt::Icon::Information, Wt::StandardButton::Ok));


    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Taleplerim::setDetail(bsoncxx::oid oid)
{

    mContentContainer->clear();


    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::SikayetKey::mainKey::oid,oid));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI","Error Talep oid",e);
        return;
    }


    bsoncxx::document::value val(document{}.view());

    try {
        auto _val = db->collection(SBLDKeys::SikayetKey::collection).find_one(filter.view());
        if( _val )
        {
            if( _val.value().view().empty() )
            {
                this->showMessage("UYARI","İçerik Boş");
                return;
            }else{
                val = _val.value();
            }
        }else{
            this->showMessage("UYARI","DB Herhangi Bir İçerik Göndermedi");
            return;
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("UYARI","Bir Hata Oluştu",e);
        return;
    }



    {
        auto view = val.view();

        // Talep Başlığı
        {
            std::string konu,tarih,saat,mahalle,tamadres,durum,birim,cagriMerkeziPersonli;

            try {
                konu = view[SBLDKeys::SikayetKey::mainKey::konu].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                konu = e.what();
            }

            try {
                tarih = view[SBLDKeys::SikayetKey::mainKey::tarih].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                tarih = e.what();
            }
            try {
                saat = view[SBLDKeys::SikayetKey::mainKey::saat].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                saat = e.what();
            }
            try {
                mahalle = view[SBLDKeys::SikayetKey::mainKey::mahalle].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                mahalle = e.what();
            }
            try {
                tamadres = view[SBLDKeys::SikayetKey::mainKey::tamadres].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                tamadres = e.what();
            }
            try {
                durum = view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                durum = e.what();
            }
            try {
                birim = view[SBLDKeys::SikayetKey::mainKey::birim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                birim = e.what();
            }

            try {
                cagriMerkeziPersonli = "";
            } catch (bsoncxx::exception &e) {
                cagriMerkeziPersonli = e.what();
            }

            auto header = std::make_unique<TalepHeader>(konu,tarih,saat,mahalle,tamadres,durum,birim,cagriMerkeziPersonli);
            mContentContainer->addWidget(std::move(header));
        }

        // Talep Aşamaları
        mAsamaContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        try {
            auto arlist = view[SBLDKeys::SikayetKey::mainKey::asama].get_array().value;

            for( auto doc : arlist )
            {
                try {
                    if( doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::degisiklik )
                    {
                        auto degisim = std::make_unique<DegisimWidget>((doc.get_document()));
                        mAsamaContainer->addWidget(std::move(degisim));
                    }
                } catch (bsoncxx::exception &e) {

                }

                try {
                    if( doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::aciklama )
                    {
                        auto aciklama = std::make_unique<AciklamaWidget>((doc.get_document()));
                        mAsamaContainer->addWidget(std::move(aciklama));
                    }
                } catch (bsoncxx::exception &e) {

                }

                try {
                    if( doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::gorsel )
                    {
                        auto aciklama = std::make_unique<GorselWidget>(db,doc.get_document());
                        mAsamaContainer->addWidget(std::move(aciklama));
                    }
                } catch (bsoncxx::exception &e) {

                }

            }

            mContentContainer->addWidget(cpp14::make_unique<AciklamaEkle>(db,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),mAsamaContainer,user));


        } catch (bsoncxx::exception &e) {
            std::cout << "No Array in Element" << std::endl;
        }


    }




}

Giris::Bilgilerim::Bilgilerim(mongocxx::database *_db, bsoncxx::document::value *_user)
    :WContainerWidget(),
      db(_db),
      user(_user)
{


    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);


    // Photo
    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

        auto photoContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

        auto layout = photoContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

        layout->addStretch(1);

        auto img = layout->addWidget(cpp14::make_unique<WImage>(WLink("img/person.jpg")));

        layout->addStretch(1);
    }


    // Diğer Bilgiler
    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        std::string _isim;
        try {
            _isim = user->view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string();
        } catch (bsoncxx::exception &e) {
            _isim = e.what();
        }
        auto isim = layout->addWidget(cpp14::make_unique<WText>(_isim),0,AlignmentFlag::Center);
        isim->setAttributeValue(Style::style,Style::font::size::s18px);

        std::string _tel;
        try {
            _tel = user->view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string();
        } catch (bsoncxx::exception &e) {
            _tel = e.what();
        }
        layout->addWidget(cpp14::make_unique<WText>(_tel),0,AlignmentFlag::Center);



        std::string _mahalle;
        try {
            _mahalle = user->view()[SBLDKeys::TC::mahalle].get_utf8().value.to_string();
        } catch (bsoncxx::exception &e) {
            _mahalle = e.what();
        }

        layout->addWidget(cpp14::make_unique<WText>(_mahalle),0,AlignmentFlag::Center);

        std::string _tamadres{"adres: "};
        try {
            _tamadres = user->view()[SBLDKeys::TC::tamadres].get_utf8().value.to_string();
        } catch (bsoncxx::exception &e) {
            _tamadres += e.what();
        }

        layout->addWidget(cpp14::make_unique<WText>(_tamadres),0,AlignmentFlag::Center);


    }

}

Giris::Taleplerim::TalepHeader::TalepHeader(std::string konu, std::string tarih, std::string saat, std::string mahalle, std::string adres, std::string durum, std::string birim, std::string cagriMerkeziPersoneli)
{

    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AntiqueWhite));

    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

    {
        auto text = layout->addWidget(cpp14::make_unique<WText>(konu));
        text->setAttributeValue(Style::style,Style::font::weight::bold);
    }
    {
        auto text = layout->addWidget(cpp14::make_unique<WText>(tarih));
    }
    {
        auto text = layout->addWidget(cpp14::make_unique<WText>(saat));
    }
    {
        auto text = layout->addWidget(cpp14::make_unique<WText>(mahalle));
    }

    {
        auto text = layout->addWidget(cpp14::make_unique<WText>(durum));
    }
    {
        auto text = layout->addWidget(cpp14::make_unique<WText>(birim));
    }

    {
        auto text = layout->addWidget(cpp14::make_unique<WText>(adres));
    }
    {
        auto text = layout->addWidget(cpp14::make_unique<WText>("<b>Çarğı Merkezi Personeli: "+cagriMerkeziPersoneli+"</b>"));
    }



}

Giris::Taleplerim::DegisimWidget::DegisimWidget(bsoncxx::document::view view)
{

    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::FloralWhite)+Style::Border::bottom::border("1px solid black"));


    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

    try {
        layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    } catch (bsoncxx::exception &e) {
        layout->addWidget(cpp14::make_unique<WText>(WString("tip Error {1}").arg(e.what())),0,AlignmentFlag::Left);
    }
    try {
        layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::saat].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    } catch (bsoncxx::exception &e) {
        layout->addWidget(cpp14::make_unique<WText>(WString("saat Error {1}").arg(e.what())),0,AlignmentFlag::Left);
    }
    try {
        layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::tarih].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    } catch (bsoncxx::exception &e) {
        layout->addWidget(cpp14::make_unique<WText>(WString("tarih Error {1}").arg(e.what())),0,AlignmentFlag::Left);
    }
    try {
        layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::degisim].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    } catch (bsoncxx::exception &e) {
        layout->addWidget(cpp14::make_unique<WText>(WString("degisim Error {1}").arg(e.what())),0,AlignmentFlag::Left);
    }


}

Giris::Taleplerim::AciklamaWidget::AciklamaWidget(bsoncxx::document::view view)
{
    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);

    try {
        if( view[SBLDKeys::SikayetKey::asamakey::birim].get_utf8().value.to_string() == "Vatandaş" )
        {
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)+Style::Border::bottom::border("5px solid red"));
        }else{
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::LavenderBlush)+Style::Border::bottom::border("1px solid black"));
        }
    } catch (bsoncxx::exception &e) {
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::LavenderBlush)+Style::Border::bottom::border("1px solid black"));
    }



    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


    try {
        if( view[SBLDKeys::SikayetKey::asamakey::birim].get_utf8().value.to_string() == "Vatandaş" )
        {
            layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::personel].get_document().view()[SBLDKeys::SikayetKey::asamakey::Personel::adSoyad].get_utf8().value.to_string()),0,AlignmentFlag::Left);
        }
    } catch (bsoncxx::exception &e) {

    }

    layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::saat].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::tarih].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::aciklama].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);






}

Giris::Taleplerim::AciklamaWidget::AciklamaWidget(std::string aciklama)
{
    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)+Style::Border::bottom::border("1px solid red"));


    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

    layout->addWidget(cpp14::make_unique<WText>(SBLDKeys::SikayetKey::asamakey::tipKey::aciklama),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(QTime::currentTime().toString("hh:mm").toStdString().c_str()),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(QDate::currentDate().toString("dddd - dd/MM/yyyy").toStdString().c_str()),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(aciklama),0,AlignmentFlag::Left);
}

Giris::Taleplerim::GorselWidget::GorselWidget(mongocxx::database *db, bsoncxx::document::view view)
{
    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::MistyRose)+Style::Border::bottom::border("1px solid black"));


    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

    layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::saat].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::asamakey::tarih].get_utf8().value.to_string().c_str()),0,AlignmentFlag::Left);
    auto bucket = db->gridfs_bucket();
    std::string img = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::SikayetKey::asamakey::gorsel].get_oid().value.to_string());
    auto imgContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>());
    imgContainer->addWidget(cpp14::make_unique<WImage>(WLink(img)));

}

Giris::Taleplerim::AciklamaEkle::AciklamaEkle(mongocxx::database *_db, std::string _oid,WContainerWidget* widget, bsoncxx::document::value* user, bool isVatandas)
    :WContainerWidget(),
      db(_db),
      oid(_oid)
{


    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    container->setMargin(25,Side::Top);

    auto text = container->addWidget(cpp14::make_unique<WText>("Açıklama Ekle"));
    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s20px);


    textEdit = container->addWidget(cpp14::make_unique<WTextEdit>());
    textEdit->setHeight(250);

    auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Ekle"));

    btn->clicked().connect([=](){



        auto filter  = document{};

        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::oid,bsoncxx::oid{_oid}));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("Filter Error: {1}").arg(e.what()).toUTF8());
            return;
        }


        auto doc = document{};
        try {
            if( isVatandas )
            {
                doc.append(kvp(SBLDKeys::SikayetKey::asamakey::personel,make_document(kvp(SBLDKeys::SikayetKey::asamakey::Personel::adSoyad,user->view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string().c_str()))));
            }else{
                doc.append(kvp(SBLDKeys::SikayetKey::asamakey::personel,make_document(kvp(SBLDKeys::SikayetKey::asamakey::Personel::adSoyad,user->view()[SBLDKeys::Personel::ad].get_utf8().value.to_string().c_str()))));
            }
        } catch (bsoncxx::exception &e) {
            if( isVatandas )
            {
                this->showMessage("UYARI",WString("Doc Sivil: {1}").arg(e.what()).toUTF8());
            }else{
                this->showMessage("UYARI",WString("Doc Personel: {1}").arg(e.what()).toUTF8());
            }
            return;
        }


        try {
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::tip,SBLDKeys::SikayetKey::asamakey::tipKey::aciklama));
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::tarih,QDate::currentDate().toString("dddd - dd/MM/yyyy").toStdString().c_str()));
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::birim,"Vatandaş"));
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::aciklama,textEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("Doc Error: {1}").arg(e.what()).toUTF8());
            return;
        }



        auto pushDoc = document{};

        try {
            pushDoc.append(kvp("$push",make_document(kvp(SBLDKeys::SikayetKey::mainKey::asama,doc))));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("$push Error: {1}").arg(e.what()).toUTF8());
            return;
        }

        try {
            auto upt = db->collection(SBLDKeys::SikayetKey::collection).update_one(filter.view(),pushDoc.view());
            if( upt )
            {
                if( upt.value().modified_count() )
                {
                    auto aciklama = std::make_unique<AciklamaWidget>(textEdit->text().toUTF8());
                    widget->addWidget(std::move(aciklama));
                }else{
                    this->showMessage("UYARI","Mesajınız Eklenemedi");
                }
            }else{
                this->showMessage("UYARI","Mesajınız Eklenemedi");
            }
        } catch (mongocxx::exception &e) {
            this->showMessage("UYARI",WString("DB Error: {1}").arg(e.what()).toUTF8());
        }
    });
}

Giris::Taleplerim::AciklamaEkle::AciklamaEkle(mongocxx::database *_db, std::string _oid, WContainerWidget *widget, bsoncxx::document::value &user, bool isVatandas)
    :WContainerWidget(),
      db(_db),
      oid(_oid)
{
    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    container->setMargin(25,Side::Top);

    auto text = container->addWidget(cpp14::make_unique<WText>("Açıklama Ekle"));
    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s20px);

    textEdit = container->addWidget(cpp14::make_unique<WTextEdit>());
    textEdit->setHeight(250);

    auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Ekle"));
    btn->addStyleClass(Bootstrap::Button::Primary);

    btn->clicked().connect([=](){

        auto filter  = document{};

        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::oid,bsoncxx::oid{_oid}));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("Filter Error: {1}").arg(e.what()).toUTF8());
            return;
        }

        std::cout << "filter Created" << std::endl;

        auto doc = document{};
        try {
            if( isVatandas )
            {
                doc.append(kvp(SBLDKeys::SikayetKey::asamakey::personel,make_document(kvp(SBLDKeys::SikayetKey::asamakey::Personel::adSoyad,user.view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string()))));
                doc.append(kvp(SBLDKeys::SikayetKey::asamakey::birim,"Vatandaş"));
            }else{
                //                doc.append(kvp(SBLDKeys::SikayetKey::asamakey::birim,user.view()[SBLDKeys::Personel::birimi].get_utf8().value));
                doc.append(kvp(SBLDKeys::SikayetKey::asamakey::personel,make_document(kvp(SBLDKeys::SikayetKey::asamakey::Personel::adSoyad,user.view()[SBLDKeys::Personel::ad].get_utf8().value.to_string()))));
            }
        } catch (bsoncxx::exception &e) {
            if( isVatandas )
            {
                this->showMessage("UYARI",WString("Doc Sivil: {1}").arg(e.what()).toUTF8());
            }else{
                this->showMessage("UYARI",WString("Doc Personel: {1}").arg(e.what()).toUTF8());
            }
            return;
        }

        std::cout << "Personel / Vatandas Appended" << std::endl;

        try {
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::tip,SBLDKeys::SikayetKey::asamakey::tipKey::aciklama));
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::tarih,QDate::currentDate().toString("dddd - dd/MM/yyyy").toStdString().c_str()));
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
            doc.append(kvp(SBLDKeys::SikayetKey::asamakey::aciklama,textEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("Doc Error: {1}").arg(e.what()).toUTF8());
            return;
        }

        std::cout << "FullDoc Created Appended" << std::endl;



        auto pushDoc = document{};

        try {
            pushDoc.append(kvp("$push",make_document(kvp(SBLDKeys::SikayetKey::mainKey::asama,doc))));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("$push Error: {1}").arg(e.what()).toUTF8());
            return;
        }

        std::cout << "PushDoc Ctreayed" << std::endl;


        try {
            if( db )
            {
                auto upt = db->collection(SBLDKeys::SikayetKey::collection).update_one(filter.view(),pushDoc.view());

                if( upt )
                {
                    if( upt.value().modified_count() )
                    {
                        std::cout << "aciklama " <<  textEdit->text().toUTF8() << std::endl;

                        auto aciklama = std::make_unique<AciklamaWidget>(textEdit->text().toUTF8());
                        widget->addWidget(std::move(aciklama));

                    }else{
                        this->showMessage("UYARI","Mesajınız Eklenemedi");
                    }
                }else{
                    this->showMessage("UYARI","Mesajınız Eklenemedi");
                }
            }else{
                this->showMessage("Error","NO Database Pointer");
            }

        } catch (mongocxx::exception &e) {
            this->showMessage("UYARI",WString("DB Error: {1}").arg(e.what()).toUTF8());
        }
    });
}

void Giris::Taleplerim::AciklamaEkle::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

Giris::Basvurularim::Basvurularim(mongocxx::database *_db, bsoncxx::document::value *_user)
    :WContainerWidget(),
      db(_db),
      user(_user)
{


    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    auto title = row->addWidget(cpp14::make_unique<WText>("<h3>Başvurularım</h3>"));
    title->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);


    toolBarContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    toolBarContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    Wt::WToolBar *toolBar = toolBarContainer->addWidget(Wt::cpp14::make_unique<Wt::WToolBar>());
    toolBar->setMargin(25,Side::Top|Side::Bottom);

    auto devamBtn = createColorButton("btn-primary", " Cevaplananlar  ");
    devamBtn->clicked().connect([=](){
        this->initBasvurular(cevaplandi);
    });
    toolBar->addButton(std::move(devamBtn));

    auto iptalBtn = createColorButton("btn-danger", "Cevaplanmayanlar");
    iptalBtn->clicked().connect([=](){
        this->initBasvurular(cevaplanmadi);
    });
    toolBar->addButton(std::move(iptalBtn));

    auto hepsiBtn = createColorButton("btn-warning", "     Hepsi     ");
    hepsiBtn->clicked().connect([=](){
        this->initBasvurular(hepsi);
    });
    toolBar->addButton(std::move(hepsiBtn));


    mContentContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);


    this->_clickBasvuruDetail.connect(this,&Basvurularim::setBasvuruDetail);

    this->initBasvurular();

}

std::unique_ptr<WPushButton> Giris::Basvurularim::createColorButton(const char *className, const WString &text)
{
    auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
    button->setTextFormat(Wt::TextFormat::XHTML);
    button->setText(text);
    button->addStyleClass(className);
    return button;
}

void Giris::Basvurularim::initBasvurular(Giris::Basvurularim::Cevaplanma durum)
{


    mContentContainer->clear();

    auto filter = document{};


    try {
        filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::tcno,user->view()[SBLDKeys::TC::tcno].get_utf8().value));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("TCNO nunuz Sistemde Kayıtlı Değil.Lütfen Bilgilerinizi Güncelleyiniz:\n{1}").arg(e.what()).toUTF8());

        return;
    }

    switch (durum) {
    case cevaplandi:
        try {
        filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::geridonus,SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapildi));
    } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("Bir Hata Oluştu: {1}").arg(e.what()).toUTF8());

            return;
        }
        break;
    case cevaplanmadi:
        try {
        filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::geridonus,SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapilmadi));
    } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("Bir Hata Oluştu: {1}").arg(e.what()).toUTF8());

            return;
        }
        break;
    case hepsi:
        break;
    default:
        break;
    }



    auto table = mContentContainer->addWidget(cpp14::make_unique<WTable>());
    table->setHeaderCount(1);
    table->setWidth(WLength("100%"));
    table->setMargin(25,Side::Bottom);

    table->addStyleClass("hover table-hover condensed table-condensed stripes table-striped");

    table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("#"));
    table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Durum"));
    table->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Konu"));
    table->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Saat"));
    table->elementAt(0, 4)->addWidget(cpp14::make_unique<WText>("Tarih"));



    try {
        auto cursor = db->collection(SBLDKeys::BilgiEdinme::Basvuru::collection).find(filter.view());

        oidList.clear();

        for( auto doc : cursor )
        {
            std::string durum;
            std::string konu;
            std::string saat;
            std::string tarih;

            oidListItem item;

            try {
                if( doc[SBLDKeys::BilgiEdinme::Basvuru::geridonus].get_bool() == SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapildi )
                {
                    item.durum = "Cevaplandı";
                }else{
                    item.durum= "Cevaplanmadı";
                }
            } catch (bsoncxx::exception &e) {
                item.durum = e.what();
            }
            try {
                item.oid = doc[SBLDKeys::BilgiEdinme::Basvuru::oid].get_oid().value;
            } catch (bsoncxx::exception &e) {
                item.oid = bsoncxx::oid{"null"};
            }

            try {
                item.konu = doc[SBLDKeys::BilgiEdinme::Basvuru::konu].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.konu = e.what();
            }

            try {
                item.saat = doc[SBLDKeys::BilgiEdinme::Basvuru::saat].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.saat = e.what();
            }

            try {
                item.tarih = doc[SBLDKeys::BilgiEdinme::Basvuru::tarih].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.tarih = e.what();
            }

            oidList.push_back(item);

        }
    } catch (mongocxx::exception &e) {
        this->showMessage("UYARI",WString("Database Error: {1}").arg(e.what()).toUTF8());
        return;
    }

    int row = 1;
    for( auto item : oidList )
    {
        int col = 0;

        {
            auto text = cpp14::make_unique<WText>(WString("{1}").arg(row));
            text->setMargin(10,Side::Top|Side::Bottom);
            text->decorationStyle().setCursor(Cursor::PointingHand);
            text->clicked().connect([=](){
                if( item.oid.to_string() != "null" )
                {
                    _clickBasvuruDetail.emit(bsoncxx::oid{item.oid});
                }
            });
            table->elementAt(row,col++)->addWidget(std::move(text));
        }

        {
            auto text = cpp14::make_unique<WText>(item.durum);
            text->setMargin(5,Side::Top|Side::Bottom);
            text->decorationStyle().setCursor(Cursor::PointingHand);
            text->clicked().connect([=](){
                if( item.oid.to_string() != "null" )
                {
                    _clickBasvuruDetail.emit(bsoncxx::oid{item.oid});
                }
            });
            table->elementAt(row,col++)->addWidget(std::move(text));
        }
        {
            auto text = cpp14::make_unique<WText>(item.konu);
            text->setMargin(5,Side::Top|Side::Bottom);
            text->decorationStyle().setCursor(Cursor::PointingHand);
            text->clicked().connect([=](){
                if( item.oid.to_string() != "null" )
                {
                    _clickBasvuruDetail.emit(bsoncxx::oid{item.oid});
                }
            });
            table->elementAt(row,col++)->addWidget(std::move(text));
        }
        {
            auto text = cpp14::make_unique<WText>(item.saat);
            text->setMargin(5,Side::Top|Side::Bottom);
            text->decorationStyle().setCursor(Cursor::PointingHand);
            text->clicked().connect([=](){
                if( item.oid.to_string() != "null" )
                {
                    _clickBasvuruDetail.emit(bsoncxx::oid{item.oid});
                }
            });
            table->elementAt(row,col++)->addWidget(std::move(text));
        }
        {
            auto text = cpp14::make_unique<WText>(item.tarih);
            text->setMargin(5,Side::Top|Side::Bottom);
            text->decorationStyle().setCursor(Cursor::PointingHand);
            text->clicked().connect([=](){
                if( item.oid.to_string() != "null" )
                {
                    _clickBasvuruDetail.emit(bsoncxx::oid{item.oid});
                }
            });
            table->elementAt(row,col++)->addWidget(std::move(text));
        }
        row++;
    }




}

void Giris::Basvurularim::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Basvurularim::setBasvuruDetail(bsoncxx::oid oid)
{

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::oid,oid));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("Filter oid Error: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        auto val = db->collection(SBLDKeys::BilgiEdinme::Basvuru::collection).find_one(filter.view());
        if( val )
        {
            if( !val.value().view().empty() )
            {

                mContentContainer->clear();

                auto containerFluid = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                containerFluid->addStyleClass(Bootstrap::Grid::container_fluid);

                auto row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
                row->addStyleClass(Bootstrap::Grid::row);

                try {
                    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                    if( val.value().view()[SBLDKeys::BilgiEdinme::Basvuru::geridonus].get_bool().value )
                    {
                        auto title = container->addWidget(cpp14::make_unique<WText>("Cevaplanmış"));
                        title->setMargin(25,Side::Bottom|Side::Top);
                        title->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::Green::ForestGreen));
                    }else{
                        auto title = container->addWidget(cpp14::make_unique<WText>("Cevaplanmamış"));
                        title->setMargin(25,Side::Bottom|Side::Top);
                        title->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::Orange::Tomato));
                    }
                } catch (bsoncxx::exception &e) {
                    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                    auto title = container->addWidget(cpp14::make_unique<WText>("Cevaplanmamış"));
                    title->setMargin(25,Side::Bottom|Side::Top);
                    title->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::Orange::Tomato));
                }

                try {
                    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                    auto title = container->addWidget(cpp14::make_unique<WText>(val.value().view()[SBLDKeys::BilgiEdinme::Basvuru::konu].get_utf8().value.to_string()));
                    title->setMargin(25,Side::Bottom|Side::Top);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {

                }


                try {
                    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                    auto text = container->addWidget(cpp14::make_unique<WText>(val.value().view()[SBLDKeys::BilgiEdinme::Basvuru::mesaj].get_utf8().value.to_string(),TextFormat::UnsafeXHTML));
                    text->setMargin(25,Side::Top|Side::Bottom);
                } catch (bsoncxx::exception &e) {

                }


                try {
                    auto arList = val.value().view()[SBLDKeys::BilgiEdinme::Basvuru::aciklama].get_array().value;

                    for( auto _doc : arList )
                    {
                        auto doc = _doc.get_document().value;


                        {
                            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Pink::LightPink));
                            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                            auto text = container->addWidget(cpp14::make_unique<WText>("<b>Açıklama<b>",TextFormat::UnsafeXHTML));
                            text->setMargin(25,Side::Top|Side::Bottom);
                        }


                        try {
                            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Pink::LightPink));
                            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                            auto text = container->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(doc[SBLDKeys::BilgiEdinme::Basvuru::ACIKLAMA::tarih].get_int64().value).toString("dddd dd/MM/yyyy").toStdString().c_str(),TextFormat::UnsafeXHTML));
                            text->setMargin(25,Side::Top|Side::Bottom);
                        } catch (bsoncxx::exception &e) {
                            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                            auto text = container->addWidget(cpp14::make_unique<WText>(WString("Tarih : {1}").arg(e.what()),TextFormat::UnsafeXHTML));
                            text->setMargin(25,Side::Top|Side::Bottom);
                        }

                        try {
                            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Pink::LightPink));
                            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                            auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::BilgiEdinme::Basvuru::ACIKLAMA::saat].get_utf8().value.to_string(),TextFormat::UnsafeXHTML));
                            text->setMargin(25,Side::Top|Side::Bottom);
                        } catch (bsoncxx::exception &e) {
                            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                            auto text = container->addWidget(cpp14::make_unique<WText>(WString("Tarih : {1}").arg(e.what()),TextFormat::UnsafeXHTML));
                            text->setMargin(25,Side::Top|Side::Bottom);
                        }

                        try {
                            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Pink::LightPink));
                            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                            auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::BilgiEdinme::Basvuru::ACIKLAMA::aciklama].get_utf8().value.to_string(),TextFormat::UnsafeXHTML));
                            text->setMargin(25,Side::Top|Side::Bottom);
                        } catch (bsoncxx::exception &e) {
                            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                            auto text = container->addWidget(cpp14::make_unique<WText>(WString("Aciklama : {1}").arg(e.what()),TextFormat::UnsafeXHTML));
                            text->setMargin(25,Side::Top|Side::Bottom);
                        }
                    }

                } catch (bsoncxx::exception &e) {
                    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                    auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what()),TextFormat::UnsafeXHTML));
                    text->setMargin(25,Side::Top|Side::Bottom);
                }



                if( val.value().view()[SBLDKeys::BilgiEdinme::Basvuru::geridonus].get_bool().value )
                {
                    try {
                        auto cevap = val.value().view()[SBLDKeys::BilgiEdinme::Basvuru::cevap].get_document().view();
                        auto bucket = this->db->gridfs_bucket();
                        auto filePath = SBLDKeys::downloadifNotExist(&bucket,cevap[SBLDKeys::BilgiEdinme::Basvuru::CEVAP::cevapDosyasiOid].get_oid().value.to_string());

                        std::string link = "<iframe src=\""+filePath+"\"  height=\"1200px\" width=\"100%\" allowfullscreen></iframe>";

                        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                        auto Cevaptext = container->addWidget(cpp14::make_unique<WText>(WString("<h3>{1}</h3>").arg("Cevap"),TextFormat::UnsafeXHTML));
                        Cevaptext->setMargin(25,Side::Top|Side::Bottom);

                        auto text = container->addWidget(cpp14::make_unique<WText>(link,TextFormat::UnsafeXHTML));





                    } catch (bsoncxx::exception &e) {

                    }
                }



            }else{
                this->showMessage("Error","İçerik Boş");
            }
        }else{
            this->showMessage("Error","İçerik Yok");
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error",WString("Query Error: {1}").arg(e.what()).toUTF8());
        return;
    }
}

Giris::Basvuru::Basvuru(mongocxx::database *_db, bsoncxx::document::value *_user)
    :WContainerWidget(),
      db(_db),
      user(_user)
{
    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    {
        auto _container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());

        // Başvuru Title
        {
            auto container = _layout->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto text = container->addWidget(cpp14::make_unique<WText>("Bilgi Edinme Başvurusu"));
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
        }

        //        _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
        ////                this->_ClickBack.emit(NoClass());
        //        });
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);


    //Açıklamalar
    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("Bilgi Edinme kanunun tanıdığı hakkı belediyemiz ile ilgili konularda kullanmak için İnternet üzerinden Bilgi Edinme Formlarını doldurmanız yeterli olacaktır."));
    }

    // Bilgi Edinme Hakkı Kanunu
    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::row);

        auto textlink = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        textlink->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto pdfanchor2 = std::make_unique<WAnchor>(WLink("http://www.serik.bel.tr/file/4982-sayili-Bilgi-Edinme-Hakki-Kanunu.pdf"));
        pdfanchor2->setText("4982 Sayılı Bilgi Edinme Hakkı Kanunu");
        pdfanchor2->setAttributeValue(Style::style,Style::font::size::s18px);
        textlink->addWidget(std::move(pdfanchor2));
    }

    // Bilgi Edinme Hakkı Kanunu
    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::row);

        auto textlink = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        textlink->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto link2 = WLink("http://www.serik.bel.tr/file/Bilgi-Edinme-Uygulamasina-Iliskin-Yonetmelik.pdf");
        link2.setTarget(LinkTarget::NewWindow);
        auto pdfanchor2 = std::make_unique<WAnchor>(link2);
        pdfanchor2->setText("Bilgi Edinme Uygulamasına İlişkin Yönetmenlik");
        pdfanchor2->setAttributeValue(Style::style,Style::font::size::s18px);
        textlink->addWidget(std::move(pdfanchor2));
    }

    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        //        container->setHeight(600);

        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        int _row = 0;

        {
            layout->addWidget(cpp14::make_unique<WText>("Aşağıdaki Bilgileri Doldurarak Kaydet Tuşuna Basınız"),0,AlignmentFlag::Center);
            lineEdittc = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
            lineEdittc->setPlaceholderText("TCNO Zorunlu");
            lineEdittc->setInputMask("99999999999");
            lineEdittc->setText(user->view()[SBLDKeys::TC::tcno].get_utf8().value.to_string());
            lineEdittc->setEnabled(false);
            _row++;
        }


        {
            //            layout->addWidget(cpp14::make_unique<WText>("Adınız & Soyadınız"),0,AlignmentFlag::Center);
            lineEditad = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
            lineEditad->setPlaceholderText("Ad Soyad Zorunlu");
            lineEditad->setText(user->view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string());
            lineEditad->setEnabled(false);
            _row++;
        }

        {
            //            layout->addWidget(cpp14::make_unique<WText>("E-Posta"),0,AlignmentFlag::Center);
            lineEditmail = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
            lineEditmail->setPlaceholderText("E-Posta isteğe bağlı");
            _row++;
        }

        {
            //            layout->addWidget(cpp14::make_unique<WText>("Telefon"),0,AlignmentFlag::Center);
            lineEditTel = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
            lineEditTel->setPlaceholderText("Telefon Numarası Zorunlu");
            lineEditTel->setText(user->view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string());
            lineEditTel->setEnabled(false);
            _row++;
        }

        {
            //            layout->addWidget(cpp14::make_unique<WText>("Adres"),0,AlignmentFlag::Center);
            lineEditAdres = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
            lineEditAdres->setPlaceholderText("Adres isteğe bağlı");
            _row++;
        }

        //        {
        ////            layout->addWidget(cpp14::make_unique<WText>("Adres"),0,AlignmentFlag::Center);
        //            comboBoxGeriDonus = layout->addWidget(cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Justify);
        //            comboBoxGeriDonus->addItem("Geri Dönüş Yolu Seçilmedi");
        ////            comboBoxGeriDonus->addItem(SBLDKeys::BilgiEdinme::Basvuru::geriDonusTipiVar::hesaptan);
        //            comboBoxGeriDonus->addItem(SBLDKeys::BilgiEdinme::Basvuru::geriDonusTipiVar::mail);
        //            comboBoxGeriDonus->addItem(SBLDKeys::BilgiEdinme::Basvuru::geriDonusTipiVar::posta);
        //            comboBoxGeriDonus->addItem(SBLDKeys::BilgiEdinme::Basvuru::geriDonusTipiVar::sms);
        //            comboBoxGeriDonus->addItem(SBLDKeys::BilgiEdinme::Basvuru::geriDonusTipiVar::telefon);

        //            comboBoxGeriDonus->sactivated().connect(this,&Basvuru::ShowGeriDonusUyari);
        //            _row++;
        //        }

        {
            //            layout->addWidget(cpp14::make_unique<WText>("Konu"),0,AlignmentFlag::Center);
            lineEditKonu = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
            lineEditKonu->setPlaceholderText("Konu Zorunlu");
            _row++;
        }

        {
            //            layout->addWidget(cpp14::make_unique<WText>("Mesaj"),0,AlignmentFlag::Center);
            mesajEdit = layout->addWidget(cpp14::make_unique<WTextEdit>(),1,AlignmentFlag::Justify);
            mesajEdit->setHeight(250);
            mesajEdit->setText("Mesajınızı Buraya Yazınız");
            _row++;
        }

    }

    {
        auto _container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto saveBtn = _layout->addWidget(cpp14::make_unique<WPushButton>("Kaydet"),0,AlignmentFlag::Center);
        saveBtn->addStyleClass(Bootstrap::Button::Primary);
        saveBtn->clicked().connect(this,&Basvuru::save);
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }
}

Signal<NoClass> &Giris::Basvuru::clickBasvuru()
{
    return this->_mClickBasvuru_;
}

void Giris::Basvuru::save()
{

    if( lineEdittc->text().toUTF8().size() != 11 )
    {
        this->showMessage("UYARI","TCNO Girmek Zorunlu");
        return;
    }

    if( lineEditad->text().toUTF8().size() < 5 )
    {
        this->showMessage("UYARI","Ad Soyad Kısmını Tam Doldurunuz");
        return;
    }

    std::cout << "Telefon NUmarası Size: "<<lineEditTel->text().toUTF8().size() <<" " << lineEditTel->text().toUTF8() << std::endl;

    if( lineEditTel->text().toUTF8().size() != 11 )
    {
        this->showMessage("UYARI","Telefon Numaranızı İstenilen Formatta Giriniz");
        return;
    }

    if( lineEditKonu->text().toUTF8().size() < 5 )
    {
        this->showMessage("UYARI","Konu Başlığını Yeterli Değil. Lütfen Konu Başlığını Kontrol Ediniz");
        return;
    }

    if( mesajEdit->text().toUTF8().size() < 50 )
    {
        this->showMessage("UYARI",WString("Mesajınız 50 Karakterden Az Olamaz. Girdiğiniz Karakter Sayısı {1}").arg(mesajEdit->text().toUTF8().size()).toUTF8());
        return;
    }


    auto doc = document{};

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document saat: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::tarih,QDate::currentDate().toString("dddd - dd/MM/yyyy").toStdString().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document tarih: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::geridonus,SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapilmadi));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document tarih: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::julianDate,QDate::currentDate().toJulianDay()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document julianDate: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::tcno,lineEdittc->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document tcno: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::adsoyad,lineEditad->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document adsoyad: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::eposta,lineEditmail->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document mail: {1}").arg(e.what()).toUTF8());
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::telefon,lineEditTel->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document telefon: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::adres,lineEditAdres->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document adres: {1}").arg(e.what()).toUTF8());
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::konu,lineEditKonu->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document konu: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::mesaj,mesajEdit->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document mesaj: {1}").arg(e.what()).toUTF8());
        return;
    }

    try {
        doc.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::mesaj,mesajEdit->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI",WString("Error Document mesaj: {1}").arg(e.what()).toUTF8());
        return;
    }


    try {
        auto ins = db->collection(SBLDKeys::BilgiEdinme::Basvuru::collection).insert_one(doc.view());
        if( ins )
        {
            if( !ins.value().result().inserted_count() )
            {
                this->showMessage("UYARI","Başvurunuz Kayıt Edilemedi. Lütfen Daha Sonra Tekrar Deneyiniz");
            }else{
                this->showMessage("Bilgi","Başvurunuz Alındı. En Kısa Zaman tarafınıza Dönüş Yapılacaktır. Bu Telefona Bağlı Diğer İstek/Talep/Şikayetler ve Başvurular için Giriş Yapınız");
                this->resetForm();
            }
        }else{
            this->showMessage("UYARI","Başvurunuz Alınamadı. Lütfen Daha Sonra Tekrar Deneyiniz");
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("UYARI",WString("DB insert Document Error: {1}").arg(e.what()).toUTF8());
    }
}

void Giris::Basvuru::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Basvuru::ShowGeriDonusUyari(WString mesaj)
{
    if( mesaj.toUTF8() == SBLDKeys::BilgiEdinme::Basvuru::geriDonusTipiVar::hesaptan )
    {

    }
}

void Giris::Basvuru::resetForm()
{
    lineEditAdres->setText("");
    lineEditKonu->setText("");
    //    lineEditTel->setText("");
    //    lineEditad->setText("");
    lineEditmail->setText("");
    //    lineEdittc->setText("");
    mesajEdit->setText("Mesajınızı Buraya Yazınız");
}

Giris::Talep::Talep(mongocxx::database *_db, bsoncxx::document::value *_user)
    :WContainerWidget(),
      db(_db),
      user(_user)
{
    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());

    mMainContainer->setMargin(25,Side::Top);

    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    {
        auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("Talepte/Şikayette Bulun"));
            text->setAttributeValue(Style::style,Style::font::size::s20px);
        }

    }


    {
        auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("Kişisel Bilgileriniz"));
        }

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_6);
            auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

            auto layout = layoutContainer->setLayout(cpp14::make_unique<WGridLayout>());

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("TC NO"),0,0,AlignmentFlag::Middle);
                mTcNO = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,1);
                mTcNO->setPlaceholderText("TCNO Kayıtlı ise Bilgileriniz Otomatik Dolacaktır");
                mTcNO->setText(user->view()[SBLDKeys::TC::tcno].get_utf8().value.to_string().c_str());
                mTcNO->setEnabled(false);
                //                mTcNO->textInput().connect(this,&Talep::TCChanged);
            }

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Ad Soyad"),1,0);
                mAdSoyad = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,1);
                mAdSoyad->setPlaceholderText("Adınızı ve Soyadınızı Giriniz");
                mAdSoyad->setText(user->view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string().c_str());
                mAdSoyad->setEnabled(false);

            }

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Mahalle"),2,0,AlignmentFlag::Middle);
                mMahalle = layout->addWidget(cpp14::make_unique<WComboBox>(),2,1);


                mMahalle->addItem(user->view()[SBLDKeys::TC::mahalle].get_utf8().value.to_string().c_str());
                mMahalle->setEnabled(false);


            }
            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Telefon"),3,0,AlignmentFlag::Middle);
                mTelefon = layout->addWidget(cpp14::make_unique<WLineEdit>(),3,1);
                mTelefon->setPlaceholderText("Cep Telefonu Numaranızı Giriniz(05321234567)");
                mTelefon->setText(user->view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string().c_str());
                mTelefon->setEnabled(false);
            }

        }






        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_6);
            auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

            auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Mahalle"),0,AlignmentFlag::Middle);
                mTalepMahalle = layout->addWidget(cpp14::make_unique<WComboBox>(),0);


                auto collection = db->collection(SBLDKeys::Mahalle::collection);

                try {
                    auto cursor = collection.find(document{}.view());

                    for( auto doc : cursor )
                    {
                        mTalepMahalle->addItem(doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string().c_str());
                    }

                } catch (mongocxx::exception &e) {
                    mTalepMahalle->addItem(WString("Error: {1}").arg(e.what()));
                }

            }


            {
                auto labelAdSoyad = layout->addWidget(cpp14::make_unique<WText>("Talebiniz"));
                mEdit = layout->addWidget(cpp14::make_unique<WTextEdit>());


                mEdit->setHeight(250);
                auto labelCount = layout->addWidget(cpp14::make_unique<WText>(WString("{1} - Min(61) Karakter").arg(mEdit->text().toUTF8().size())));
                mEdit->changed().connect([=](){
                    labelCount->setText(WString("{1} - Min(61) Karakter").arg(mEdit->text().toUTF8().size()));
                });
                mEdit->setText("Şikayetinizi/Talebinizi Detaylı Olarak Buraya Yazın");
            }

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Adres"),1,AlignmentFlag::Left);
                mAdres = layout->addWidget(cpp14::make_unique<WLineEdit>(),1);
                mAdres->setPlaceholderText("Şikayet/Talep Adresi Giriniz");
            }



        }

    }


    {
        auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("<h3>Talebinizin Tam Olarak Yerine Getirilebilmesi için "
                                                                    "Lütfen Bilgilerinizi Eksiksiz Doldurunuz</h3>"));
            mSave = layout->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
            mSave->addStyleClass(Bootstrap::Button::Primary);
            mSave->clicked().connect(this,&Talep::Save);
        }



    }
}

void Giris::Talep::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Talep::Save()
{

    if( mAdres->text().empty() )
    {
        this->showMessage("UYARI","Adres Bilgisi Girmediniz");
        return;
    }

    if( mEdit->text().toUTF8().size() <= 61 )
    {
        this->showMessage("UYARI","Talep Kısmına Yeterli Bilgi Girmediniz");
        return;
    }



    auto collection = db->collection(SBLDKeys::SikayetKey::collection);

    auto doc = document{};

    try {

        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::vatandasadSoyad,mAdSoyad->text().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::vatandas,mTcNO->text().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::konu,mEdit->text().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::mahalle,mTalepMahalle->currentText().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::tamadres,mAdres->text().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::devamediyor));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::tarih,QDate::currentDate().toString("dddd - dd/MM/yyyy").toStdString().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,QDate::currentDate().toJulianDay()));

    } catch (bsoncxx::exception &e) {
        showMessage("UYARI",WString("Talebiniz Alınamadı: %1").arg(e.what()).toUTF8().c_str());
        return;
    }


    try {
        auto ins = collection.insert_one(doc.view());

        if( !ins )
        {
            showMessage("UYARI",WString("Talebiniz Alınamadı. Daha Sonra Tekrar Deneyiniz").toUTF8().c_str());
            return;
        }

        if( ins.value().result().inserted_count() )
        {
            showMessage("Bilgi",WString("Talep Kaydınız Başarılı Bir Şekilde Alındı. En Kısa Zaman Tarafınıza Geri Dönüş Yapılacaktır.").toUTF8().c_str());
            mEdit->setText("Şikayetinizi/Talebinizi Detaylı Olarak Buraya Yazın");
            mTalepMahalle->setCurrentIndex(0);
            mAdres->setText("");

        }else{
            showMessage("UYARI",WString("Talebiniz Alınamadı. Daha Sonra Tekrar Deneyiniz").toUTF8().c_str());
        }

    } catch (mongocxx::exception &e) {
        showMessage("UYARI",WString("Talebiniz Alınamadı: %1").arg(e.what()).toUTF8().c_str());
        return;
    }
}

Giris::Personel::PersonelWidget::PersonelWidget(mongocxx::database *_db, std::string tel)
    :Giris::Personel::BaseWidget (_db,tel)
{

    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto mRow = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mRow->addStyleClass(Bootstrap::Grid::row);


    {

        mHeaderContainer = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderContainer->addStyleClass( Bootstrap::Grid::Large::col_lg_2 + Bootstrap::Grid::Medium::col_md_12 + Bootstrap::Grid::Small::col_sm_12 +  Bootstrap::Grid::ExtraSmall::col_xs_12 );
        mHeaderContainer->setPadding(0,AllSides);

        auto containerFluid = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        containerFluid->addStyleClass(Bootstrap::Grid::container_fluid);

        auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
        _row->addStyleClass(Bootstrap::Grid::row);

        this->initHeader(_row);

        mMenuContainer = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        mMenuContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);

        mMenuContainer->setPadding(0,AllSides);
    }


    mContentContainer = mRow->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    mContentContainer->setPadding(0,AllSides);

    auto tempContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentWidget = tempContainer->addWidget(cpp14::make_unique<WStackedWidget>());

    mContentWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromLeft,TimingFunction::CubicBezier,500),true);
    mContentWidget->setPadding(0,AllSides);

//    std::cout << "this->initMenu();" << std::endl;
    this->initMenu();


}

void Giris::Personel::PersonelWidget::initMenu()
{

    auto _mContainer = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _mContainer->setPadding(35,Side::Bottom|Side::Top);

    Wt::WMenu *menu = _mContainer->addWidget(Wt::cpp14::make_unique<Wt::WMenu>(mContentWidget));
    menu->setStyleClass("nav nav-pills nav-stacked");


    menu->addItem("Kod Repository", Wt::cpp14::make_unique<Giris::Personel::Yenilikler>());

    menu->addItem("Bilgilerim", Wt::cpp14::make_unique<Bilgilerim>(db(),User()));

    menu->addItem(WString::fromUTF8("Arıza Kaydı"), Wt::cpp14::make_unique<ArizaKaydi>(db(),User()));

    if(this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::baskan ||
            this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string() == "Bilgi İşlem Müdürlüğü" )
    {
    }

    if(this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::baskan ||
            this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::baskanyrd ||
            this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string() == "Kadın ve Aile Hizmetleri Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("Stok Yardım"), Wt::cpp14::make_unique<KadinAileStock>(db(),User()));
    }

    if(this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::baskan ||
            this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::baskanyrd ||
            this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string() == "İnsan Kaynakları ve Eğitim Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("IK"), Wt::cpp14::make_unique<IK>(db(),User()));
    }


    menu->addItem("Anons Sistemi", Wt::cpp14::make_unique<AnonsWidget>(db(),User()));


    if(this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::baskan ||
            this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::baskanyrd )
    {
        menu->addItem(WString::fromUTF8("Giriş Çıkış"), Wt::cpp14::make_unique<GirisCikisWidget>(db(),User()));

        menu->addItem("Mesajlar", Wt::cpp14::make_unique<BaskanMesajlar>(db(),User()));
        menu->addItem("Projeler", Wt::cpp14::make_unique<ProjeWidget>(db(),User()));
        menu->addItem(WString::fromUTF8("Çalışmalar"), Wt::cpp14::make_unique<CalismaWidget>(db(),User()));

        menu->addItem(WString::fromUTF8("Çağrı Merkezi"), Wt::cpp14::make_unique<CagriMerkezi>(db(),User()));

        //        menu->addItem("Muhasebe", Wt::cpp14::make_unique<WText>("Muhasebe Yakın Zamanda Devreye Alınacaktır"));
        //        menu->addItem("Birimler", Wt::cpp14::make_unique<WText>("Birimler"));

        menu->addSeparator();

        menu->addItem(" ", Wt::cpp14::make_unique<WText>("Bu Sayfa Bilerek Boş Bırakılmıştır"));
    }else{
        // Add menu items using the default lazy loading policy.
        menu->addItem("Gelen Mesajlar", Wt::cpp14::make_unique<PersonelV2::GelenMesajlarWidget>(db(),User()));
        menu->addItem("Taleplerim", Wt::cpp14::make_unique<Taleplerim>(db(),User()));
        menu->addItem(WString::fromUTF8("Evrak Arşivi"), Wt::cpp14::make_unique<EvrakArsiv>(db(),User()));

        menu->addItem(WString::fromUTF8("Başvurularım"), Wt::cpp14::make_unique<BilgiEdinmeClient>(db(),User()));

        menu->addItem(WString::fromUTF8("Çalışmalar"), Wt::cpp14::make_unique<CalismaGirWidget>(db(),User()));
        menu->addItem(WString::fromUTF8("Giriş Çıkışlarım"), Wt::cpp14::make_unique<GirisCikisWidget>(db(),User()));
    }

    if( this->UserYetki().contains("Çağrı Merkezi") )
    {
        menu->addItem(WString::fromUTF8("Çağrı Merkezi/Y"), Wt::cpp14::make_unique<SikayetimYonetim>(db(),User()));
    }







    //    menu->addItem("Hata Bildir", Wt::cpp14::make_unique<Bilgilerim>(db(),User()))->addStyleClass(Bootstrap::Label::Danger);





    //    menu->addItem("Yeni Talep", Wt::cpp14::make_unique<WText>("Yeni Talep"));
    //    menu->addItem("Yeni Başvuru", Wt::cpp14::make_unique<WText>("Yeni Başvuru"));



}

void Giris::Personel::PersonelWidget::initHeader(WContainerWidget* _row)
{


    auto tempContainer = _row->addWidget(cpp14::make_unique<WContainerWidget>());
    tempContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    tempContainer->setAttributeValue(Style::style,Style::background::color::rgba(120,155,175));


    auto photoWidget = tempContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    auto bucket = db()->gridfs_bucket();
    std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,User().view()[SBLDKeys::Personel::fotoOid].get_oid().value.to_string());
    auto img = photoWidget->addWidget(cpp14::make_unique<WImage>(WLink(imgPath)));
    img->setWidth(120);


    {
        auto infoContainer = tempContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        auto container = infoContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        layout->addStretch(1);

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->User().view()[SBLDKeys::Personel::ad].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Center);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Center);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->User().view()[SBLDKeys::Personel::telefon].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Center);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }
        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Center);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }
        layout->addStretch(1);
    }

}

Giris::Personel::BaseWidget::BaseWidget(mongocxx::database *_db, std::string tel)
    :WContainerWidget(),
      __db(_db),
      _User(document{}.view())
{
    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Personel::telefon,tel.c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Hata Oluştu",e);
        return;
    }

    try {
        auto val = __db->collection(SBLDKeys::Personel::collection).find_one(filter.view());
        if( val )
        {
            if( val.value().view().empty() )
            {
                this->showMessage("Error","Dosya Boş");
                return;
            }else{
                this->setUser(val.value());
            }
        }else{
            this->showMessage("Error","DB cevap vermiyor");
            return;
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error","Hata Oluştu",e);
        return;
    }
}

Giris::Personel::BaseWidget::BaseWidget(mongocxx::database *_db, bsoncxx::document::value _user)
    :WContainerWidget(),
      __db(_db),
      _User(_user)
{

}

void Giris::Personel::BaseWidget::showMessage(std::string title, std::string msg, std::string btnText)
{


    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, StandardButton::Ok));
    if( btnText != "OK" )
    {
        auto btn = messageBox->button(StandardButton::Ok);
        btn->setText(btnText);
    }


    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Personel::BaseWidget::showMessage(std::string title, std::string msg, bsoncxx::exception &e, Icon icon)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 WString("{1} : {2}").arg(msg.c_str()).arg(e.what()).toUTF8(),
                 icon, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

void Giris::Personel::BaseWidget::showMessage(std::string title, std::string msg, mongocxx::exception &e, Icon icon)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 WString("{1} : {2}").arg(msg.c_str()).arg(e.what()).toUTF8(),
                 icon, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

mongocxx::database *Giris::Personel::BaseWidget::db() const
{
    return this->__db;
}

bsoncxx::document::value Giris::Personel::BaseWidget::User() const
{
    return _User;
}

QVector<QString> Giris::Personel::BaseWidget::UserYetki() const
{
    QVector<QString> yetkiler;

    try {
        auto _array = _User.view()["Yetkiler"].get_array().value;

        for( auto item : _array )
        {
            try {
                yetkiler.push_back(item.get_utf8().value.to_string().c_str());
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
            }
        }
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
    }
    return yetkiler;
}

std::unique_ptr<WPushButton> Giris::Personel::BaseWidget::createColorButton(const char *className, const WString &text)
{
    auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
    button->setTextFormat(Wt::TextFormat::XHTML);
    button->setText(text);
    button->addStyleClass(className);
    return button;
}

mongocxx::cursor Giris::Personel::BaseWidget::find(std::string collection, const bsoncxx::document::view &view, mongocxx::options::find options)
{
    try {
        auto cursor = this->db()->collection(collection).find(view,options);
        return cursor;
    } catch (mongocxx::exception &e) {
        this->showMessage("Error",e.what());
    }
}

mongocxx::stdx::optional<bsoncxx::document::value> Giris::Personel::BaseWidget::find_one(std::string collection, const bsoncxx::document::view &view, mongocxx::options::find options)
{

    try {
        auto val = this->db()->collection(collection).find_one(view,options);
        return val;
    } catch (mongocxx::exception &e) {
        this->showMessage("Error",e.what());
    }


}

int64_t Giris::Personel::BaseWidget::count(std::string collection, bsoncxx::document::view &filter)
{
    std::int64_t count = 0;
    try {
        count = this->db()->collection(collection).count(filter);
    } catch (mongocxx::exception &e) {

    }
    return count;
}



int64_t Giris::Personel::BaseWidget::count(std::string collection, bsoncxx::builder::basic::document &filter)
{
    std::int64_t count = 0;
    try {
        count = this->db()->collection(collection).count(filter.view());
    } catch (mongocxx::exception &e) {

    }
    return count;
}

int64_t Giris::Personel::BaseWidget::count(std::string collection, bsoncxx::document::value val)
{
    std::int64_t count = 0;
    try {
        count = this->db()->collection(collection).count(val.view());
    } catch (mongocxx::exception &e) {

    }
    return count;
}

void Giris::Personel::BaseWidget::setUser(const bsoncxx::document::value &User)
{
    _User = User;
}

Giris::Personel::Bilgilerim::Bilgilerim(mongocxx::database *_db, bsoncxx::document::value _user)
    :Giris::Personel::BaseWidget (_db,_user)
{

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        {
            auto container = _layout->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto text = container->addWidget(cpp14::make_unique<WText>("Personel Bilgileri"));
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
        }
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }


    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        layout->addStretch(1);
        auto bucket = this->db()->gridfs_bucket();
        auto img = layout->addWidget(cpp14::make_unique<WImage>(WLink(SBLDKeys::downloadifNotExist(&bucket,this->User().view()[SBLDKeys::Personel::fotoOid].get_oid().value.to_string()))));
        layout->addStretch(1);
    }



    {
        auto text = row->addWidget(cpp14::make_unique<WText>(this->User().view()[SBLDKeys::Personel::ad].get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    }

    {
        auto text = row->addWidget(cpp14::make_unique<WText>(this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    }

    {
        auto text = row->addWidget(cpp14::make_unique<WText>(this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    }

    {
        auto text = row->addWidget(cpp14::make_unique<WText>(this->User().view()[SBLDKeys::Personel::telefon].get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    }

}

Giris::Personel::Taleplerim::Taleplerim(mongocxx::database *_db, bsoncxx::document::value _user)
    :Giris::Personel::BaseWidget (_db,_user)
{

    skip = 0;

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        {
            auto container = _layout->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto text = container->addWidget(cpp14::make_unique<WText>("Gelen Talepler/Şikayetler"));
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
        }
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }



    toolBarContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    toolBarContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    this->toolbarReFresh();

    mContentContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());

    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    this->_clickTalep.connect(this,&Personel::Taleplerim::setDetail);

    this->initTalepler();

}

void Giris::Personel::Taleplerim::toolbarReFresh()
{

    toolBarContainer->clear();

    Wt::WToolBar *toolBar = toolBarContainer->addWidget(Wt::cpp14::make_unique<Wt::WToolBar>());
    toolBar->setMargin(25,Side::Top|Side::Bottom);

    auto devamBtn = createColorButton("btn-primary", SBLDKeys::SikayetKey::durumType::devamediyor+" "+Bootstrap::Badges::badget(std::to_string(this->countTalepler(SBLDKeys::SikayetKey::durumType::devamediyor))));
    devamBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
    });
    toolBar->addButton(std::move(devamBtn));

    auto tekraracildiBtn = createColorButton("btn-info", SBLDKeys::SikayetKey::durumType::tekrarAcildi+" "+Bootstrap::Badges::badget(std::to_string(this->countTalepler(SBLDKeys::SikayetKey::durumType::tekrarAcildi))));
    tekraracildiBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::tekrarAcildi);
    });
    toolBar->addButton(std::move(tekraracildiBtn));

    auto iptalBtn = createColorButton("btn-danger", SBLDKeys::SikayetKey::durumType::iptalEdildi+" "+Bootstrap::Badges::badget(std::to_string(this->countTalepler(SBLDKeys::SikayetKey::durumType::iptalEdildi))));
    iptalBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::iptalEdildi);
    });
    toolBar->addButton(std::move(iptalBtn));


    auto tamamlananBtn = createColorButton("btn-success", SBLDKeys::SikayetKey::durumType::tamamlandi+" "+Bootstrap::Badges::badget(std::to_string(this->countTalepler(SBLDKeys::SikayetKey::durumType::tamamlandi))));
    tamamlananBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::tamamlandi);
    });
    toolBar->addButton(std::move(tamamlananBtn));

    auto teyitEdilmemisBtn = createColorButton("btn-info", SBLDKeys::SikayetKey::durumType::teyitEdilmemis+" "+Bootstrap::Badges::badget(std::to_string(this->countTalepler(SBLDKeys::SikayetKey::durumType::teyitEdilmemis))));
    teyitEdilmemisBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::teyitEdilmemis);
    });
    toolBar->addButton(std::move(teyitEdilmemisBtn));


    auto beklemeBtn = createColorButton("btn-warning", SBLDKeys::SikayetKey::durumType::beklemede+" "+Bootstrap::Badges::badget(std::to_string(this->countTalepler(SBLDKeys::SikayetKey::durumType::beklemede))));
    beklemeBtn->clicked().connect([=](){
        this->initTalepler(SBLDKeys::SikayetKey::durumType::beklemede);
    });
    toolBar->addButton(std::move(beklemeBtn));

    //    toolBar->addSeparator();
    auto hepsiBtn = createColorButton("btn-default", "Hepsi "+Bootstrap::Badges::badget(std::to_string(this->countTalepler())));
    hepsiBtn->clicked().connect([=](){
        this->initTalepler("Hepsi");
    });
    toolBar->addButton(std::move(hepsiBtn));

}

void Giris::Personel::Taleplerim::initTalepler(std::string filterKey)
{

    mContentContainer->clear();
    auto filter = document{};

    if( filterKey != "Hepsi" )
    {
        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,filterKey.c_str()));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error","Bir Hata Oluştu durum",e);
            return;
        }
    }


    if( this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::mudur )
    {
        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error","Bir Hata Oluştu vatandas",e);
            return;
        }
    }else{

        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::gorevli,make_document(kvp("$elemMatch",make_document(kvp(SBLDKeys::SikayetKey::GorevliType::personelid,this->User().view()[SBLDKeys::Personel::oid].get_oid().value))))));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error","Bir Hata Oluştu vatandas",e);
            return;
        }

    }





    auto table = mContentContainer->addWidget(cpp14::make_unique<WTable>());
    table->setHeaderCount(1);
    table->setWidth(WLength("100%"));

    table->addStyleClass("table form-inline");

    table->addStyleClass("table-hover table-condensed table-striped");

    table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("#"));
    table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Durum"));
    table->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Mahalle"));
    table->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Birim"));
    table->elementAt(0, 4)->addWidget(cpp14::make_unique<WText>("Saat"));
    table->elementAt(0, 5)->addWidget(cpp14::make_unique<WText>("Tarih"));


    mongocxx::options::find findOptions;

    findOptions.limit(limit);
    findOptions.skip(skip);


    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::SikayetKey::mainKey::oid,-1));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Uyarı","Sıralama Hatası Oluştu");
    }

    findOptions.sort(sortDoc.view());

    try {
        TotalTalep = this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view());
    } catch (mongocxx::exception &e) {
        this->showMessage("Error","Database Error",e );
        return;
    }


    oidList.clear();

    try {
        auto cursor = this->db()->collection(SBLDKeys::SikayetKey::collection).find(filter.view(),findOptions);


        for( auto doc : cursor )
        {

            oidListitem item;
            try {
                item.oid = doc[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.oid = "null";
            }
            try {
                item.durum = doc[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.durum = e.what();
            }
            try {
                item.mahalle = doc[SBLDKeys::SikayetKey::mainKey::mahalle].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.mahalle = e.what();
            }

            try {
                item.birim = doc[SBLDKeys::SikayetKey::mainKey::birim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.birim = e.what();
            }

            try {
                item.saat = doc[SBLDKeys::SikayetKey::mainKey::saat].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.saat = e.what();
            }

            try {
                item.tarih = doc[SBLDKeys::SikayetKey::mainKey::tarih].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.tarih = e.what();
            }

            oidList.push_back(item);

        }

    } catch (mongocxx::exception &e) {
        this->showMessage("Error","Database Error",e );
        return;
    }


    int row = 1;
    for( auto item : oidList )
    {

        int col = 0;
        {
            auto container = std::make_unique<WContainerWidget>();
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            container->addWidget(cpp14::make_unique<WText>(WString("<strong>{1}</strong>").arg(row+skip)));
            container->setMargin(10,Side::Top|Side::Bottom);
            //            container->setAttributeValue(Style::style,Style::Border::border("1px solid gray"));
            table->elementAt(row,col++)->addWidget(std::move(container));
        }
        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.durum));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Durum Atanmamış"));
            }
            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.mahalle));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Mahalle Atanmamış"));
            }
            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.birim));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Birim Atanmamış"));
            }
            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.saat));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Saat Atanmamış"));
            }
            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        {
            auto container = std::make_unique<WContainerWidget>();
            container->setMargin(10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                _clickTalep.emit(bsoncxx::oid{item.oid});
            });
            container->decorationStyle().setCursor(Cursor::PointingHand);
            try {
                container->addWidget(cpp14::make_unique<WText>(item.tarih));
            } catch (bsoncxx::exception &e) {
                container->addWidget(cpp14::make_unique<WText>("Tarih Atanmamış"));
            }
            table->elementAt(row,col++)->addWidget(std::move(container));
        }

        row++;
    }


    auto btnContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto btnLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

    btnLayout->addStretch(1);
    auto backbtn = btnLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
    auto label = btnLayout->addWidget( cpp14::make_unique<WLabel>( Bootstrap::Badges::badget( std::to_string(skip) + " - " + std::to_string( skip + limit ) , std::to_string(TotalTalep) )));
    label->setAttributeValue(Style::style,Style::font::size::s16px);
    auto nextbtn = btnLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));
    btnLayout->addStretch(1);

    label->addStyleClass(Bootstrap::Label::info);
    backbtn->addStyleClass(Bootstrap::Button::Primary);
    nextbtn->addStyleClass(Bootstrap::Button::Primary);

    backbtn->clicked().connect([=](){
        if( skip >= limit )
        {
            skip -= limit;
            this->initTalepler(filterKey);
        }
    });

    nextbtn->clicked().connect([=](){
        if( skip + limit < TotalTalep )
        {
            skip += limit;
            this->initTalepler(filterKey);
        }
    });


}

int64_t Giris::Personel::Taleplerim::countTalepler(std::string filterKey)
{


    auto filter = document{};

    if( this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::mudur )
    {
        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error","Bir Hata Oluştu vatandas",e);
            return 0;
        }
    }else{

        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::gorevli,make_document(kvp("$elemMatch",make_document(kvp(SBLDKeys::SikayetKey::GorevliType::personelid,this->User().view()[SBLDKeys::Personel::oid].get_oid().value))))));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error","Bir Hata Oluştu vatandas",e);
            return 0;
        }

    }

    if( filterKey != "Hepsi" )
    {
        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,filterKey.c_str()));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error","Bir Hata Oluştu durum",e);
            return 0 ;
        }
    }

    try {
        auto cursor = this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view());
        return cursor;
    } catch (mongocxx::exception &e) {
        return 0;
    }
}

void Giris::Personel::Taleplerim::setDetail(bsoncxx::oid oid)
{


    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::SikayetKey::mainKey::oid,oid));
    } catch (bsoncxx::exception &e) {
        this->showMessage("UYARI","Error Talep oid",e);
        return;
    }




    bsoncxx::document::value val(document{}.view());

    try {
        auto _val = this->db()->collection(SBLDKeys::SikayetKey::collection).find_one(filter.view());
        if( _val )
        {
            if( _val.value().view().empty() )
            {
                this->showMessage("UYARI","İçerik Boş");
                return;
            }else{
                val = _val.value();
            }
        }else{
            this->showMessage("UYARI","DB Herhangi Bir İçerik Göndermedi");
            return;
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("UYARI","Bir Hata Oluştu",e);
        return;
    }

    mContentContainer->clear();

    {
        auto view = val.view();

        auto vatandasFilter = document{};
        try {
            vatandasFilter.append(kvp(SBLDKeys::TC::tcno,view[SBLDKeys::SikayetKey::mainKey::vatandas].get_utf8().value));
        } catch (bsoncxx::exception &e) {
            auto title = mContentContainer->addWidget(cpp14::make_unique<WText>(WString("Filtre Hatalı: {1}").arg(e.what())));
            title->addStyleClass(Bootstrap::Label::Danger);
            title->setAttributeValue(Style::style,Style::font::size::s12px);
        }

        try {
            auto var = this->db()->collection(SBLDKeys::TC::collection).find_one(vatandasFilter.view());
            if( var )
            {
                if( !var.value().view().empty() )
                {
                    auto title = mContentContainer->addWidget(cpp14::make_unique<WText>(var.value().view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string()));
                    title->addStyleClass(Bootstrap::Label::Primary);
                    title->setAttributeValue(Style::style,Style::font::size::s16px);
                    auto tel = mContentContainer->addWidget(cpp14::make_unique<WText>(var.value().view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string()));
                    tel->addStyleClass(Bootstrap::Label::info);
                    tel->setAttributeValue(Style::style,Style::font::size::s16px);
                }else{
                    auto title = mContentContainer->addWidget(cpp14::make_unique<WText>("Döküman Boş"));
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s12px);
                }
            }else{
                auto title = mContentContainer->addWidget(cpp14::make_unique<WText>("Vatandaş Kaydı Yok"));
                title->addStyleClass(Bootstrap::Label::Danger);
                title->setAttributeValue(Style::style,Style::font::size::s12px);
            }
        } catch (mongocxx::exception &e) {
            auto title = mContentContainer->addWidget(cpp14::make_unique<WText>(e.what()));
            title->addStyleClass(Bootstrap::Label::Danger);
            title->setAttributeValue(Style::style,Style::font::size::s12px);

        }
    }



    {
        auto view = val.view();

        // Talep Başlığı
        {
            std::string konu,tarih,saat,mahalle,tamadres,durum,birim,cagirMerkeziPersoneli;



            try {
                konu = view[SBLDKeys::SikayetKey::mainKey::konu].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                konu = e.what();
            }

            try {
                tarih = view[SBLDKeys::SikayetKey::mainKey::tarih].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                tarih = e.what();
            }
            try {
                saat = view[SBLDKeys::SikayetKey::mainKey::saat].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                saat = e.what();
            }
            try {
                mahalle = view[SBLDKeys::SikayetKey::mainKey::mahalle].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                mahalle = e.what();
            }
            try {
                tamadres = view[SBLDKeys::SikayetKey::mainKey::tamadres].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                tamadres = e.what();
            }
            try {
                durum = view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                durum = e.what();
            }
            try {
                birim = view[SBLDKeys::SikayetKey::mainKey::birim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                birim = e.what();
            }

            try {
                cagirMerkeziPersoneli = view[SBLDKeys::SikayetKey::mainKey::cagriMerkeziPersoneli].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                cagirMerkeziPersoneli = e.what();
            }

            auto header = std::make_unique<Giris::Taleplerim::TalepHeader>(konu,tarih,saat,mahalle,tamadres,durum,birim,cagirMerkeziPersoneli);
            mContentContainer->addWidget(std::move(header));


            // Varsa Fotoğraf ve Konum Bilgisi
            {
                try {
                    auto bucket = db()->gridfs_bucket();

                    auto mobildoc = view[SBLDKeys::SikayetKey::mainKey::mobilDoc].get_document().value;

                    auto latitude = mobildoc[SBLDKeys::SikayetKey::mobil::latitude].get_double().value;
                    auto longtitude = mobildoc[SBLDKeys::SikayetKey::mobil::longtitude].get_double().value;

                    auto imglist = mobildoc[SBLDKeys::SikayetKey::mobil::fotooidlist].get_array().value;

                    auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::container_fluid);
                    auto row = container->addWidget(cpp14::make_unique<WContainerWidget>());
                    row->addStyleClass(Bootstrap::Grid::row);
                    for( auto img : imglist )
                    {
                        auto filename = SBLDKeys::downloadifNotExist(&bucket,img.get_oid().value.to_string());

                        auto mimg = row->addWidget(cpp14::make_unique<WImage>(WLink(filename)));
                        mimg->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                    }


                } catch (bsoncxx::exception &e) {

                }

            }


            {
                auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::container_fluid);
                auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addWidget(cpp14::make_unique<WText>("<h3>Görevli Personeller</h3>"),0,AlignmentFlag::Center);

                try {
                    auto perList = view[SBLDKeys::SikayetKey::mainKey::gorevli].get_array().value;

                    for( auto doc : perList )
                    {

                        auto per = layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
                        auto perLayout = per->setLayout(cpp14::make_unique<WHBoxLayout>());

                        try {
                            auto iconWidget = perLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                            auto bucket = this->db()->gridfs_bucket();
                            std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,
                                                                               doc[SBLDKeys::SikayetKey::GorevliType::photooid].get_oid().value.to_string());
                            iconWidget->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::repeat::norepeat+Style::background::size::contain);
                            iconWidget->setHeight(64);
                            iconWidget->setWidth(64);
                        } catch (bsoncxx::exception &e) {
                            auto iconWidget = perLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                            iconWidget->setAttributeValue(Style::style,Style::background::url("img/person.jpg")+Style::background::repeat::norepeat+Style::background::size::contain+Style::Border::border("1px solid gray"));
                            iconWidget->setHeight(64);
                            iconWidget->setWidth(64);


                            //                            perLayout->addWidget(iconWidget);
                        }

                        auto textCOntainer = perLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
                        auto textLayout = textCOntainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                        try {
                            auto text = textLayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::GorevliType::adsoyad].get_utf8().value.to_string()));
                        } catch (bsoncxx::exception &e) {
                            auto text = textLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
                        }

                        try {
                            auto text = textLayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::GorevliType::tel].get_utf8().value.to_string()));
                        } catch (bsoncxx::exception &e) {
                            auto text = textLayout->addWidget(cpp14::make_unique<WText>(WString("Telefon Yok: {1}").arg(e.what())));
                        }
                    }
                } catch (bsoncxx::exception &e) {
                    layout->addWidget(cpp14::make_unique<WText>("<h4>Görevli Personel Yok</h4>"));
                }
            }
        }

        // Talep Aşamaları
        mAsamaContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        try {
            auto arlist = view[SBLDKeys::SikayetKey::mainKey::asama].get_array().value;

            for( auto doc : arlist )
            {
                try {
                    if( doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::degisiklik )
                    {
                        auto degisim = std::make_unique<Giris::Taleplerim::DegisimWidget>((doc.get_document()));
                        mAsamaContainer->addWidget(std::move(degisim));
                    }
                } catch (bsoncxx::exception &e) {

                }

                try {
                    if( doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::aciklama )
                    {
                        auto aciklama = std::make_unique<Giris::Taleplerim::AciklamaWidget>((doc.get_document()));
                        mAsamaContainer->addWidget(std::move(aciklama));
                    }
                } catch (bsoncxx::exception &e) {

                }

                try {
                    if( doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::gorsel )
                    {
                        auto aciklama = std::make_unique<Giris::Taleplerim::GorselWidget>(db(),doc.get_document());
                        mAsamaContainer->addWidget(std::move(aciklama));
                    }
                } catch (bsoncxx::exception &e) {

                }

            }

            mContentContainer->addWidget(addControlPanel(val));

        } catch (bsoncxx::exception &e) {
        }

        try {
            if( view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string() != SBLDKeys::SikayetKey::durumType::tamamlandi
                    && view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string() != SBLDKeys::SikayetKey::durumType::teyitEdilmemis
                    && view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string() != SBLDKeys::SikayetKey::durumType::iptalEdildi
                    && view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string() != SBLDKeys::SikayetKey::durumType::beklemede )
            {
                mContentContainer->addWidget(cpp14::make_unique<Giris::Taleplerim::AciklamaEkle>(
                                                 this->db(),
                                                 view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                                             mAsamaContainer,
                                             this->User(),
                                             false));
            }
        } catch (bsoncxx::exception &e) {

        }

    }



}

std::unique_ptr<WContainerWidget> Giris::Personel::Taleplerim::addControlPanel(bsoncxx::document::value &value)
{
    auto _container = std::make_unique<WContainerWidget>();
    auto container = _container.get();

    container->addStyleClass(Bootstrap::Grid::container_fluid);

    Wt::WToolBar *toolBar = container->addWidget(Wt::cpp14::make_unique<Wt::WToolBar>());
    toolBar->setMargin(25,Side::Top|Side::Bottom);

    auto view = value.view();

    std::string durum;

    try {
        durum = view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        durum = e.what();
        auto iptalBtn = createColorButton(Bootstrap::Button::Danger.c_str(), "Geçersiz Durum");
        iptalBtn->clicked().connect([=](){
            std::cout << "İptal Edilmiş Olurak Değiştirildi" << std::endl;
        });
        toolBar->addButton(std::move(iptalBtn));
        return _container;
    }


    if( durum == SBLDKeys::SikayetKey::durumType::devamediyor || durum == SBLDKeys::SikayetKey::durumType::tekrarAcildi )
    {
        if( this->User().view()[SBLDKeys::Personel::statu].get_utf8().value.to_string() == SBLDKeys::Personel::statuType::mudur )
        {
            auto gorevlendirBtn = createColorButton(Bootstrap::Button::info.c_str(), "Görevlendir");
            gorevlendirBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
            gorevlendirBtn->clicked().connect([=](){
                this->setPersonel(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string());
            });
            toolBar->addButton(std::move(gorevlendirBtn));
        }


        auto teyitEdilmemisBtn = createColorButton(Bootstrap::Button::Primary.c_str(), "Tamamla");
        teyitEdilmemisBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        teyitEdilmemisBtn->clicked().connect([=](){
            this->setNewDurum(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                    durum,SBLDKeys::SikayetKey::durumType::teyitEdilmemis);
        });
        toolBar->addButton(std::move(teyitEdilmemisBtn));

        auto iptalBtn = createColorButton(Bootstrap::Button::Danger.c_str(), "İptal Et");
        iptalBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        iptalBtn->clicked().connect([=](){
            this->setNewDurum(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                    durum,SBLDKeys::SikayetKey::durumType::iptalEdildi);
        });
        toolBar->addButton(std::move(iptalBtn));

        auto beklemeBtn = createColorButton(Bootstrap::Button::Warning.c_str(), "Beklemeye Al");
        beklemeBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        beklemeBtn->clicked().connect([=](){
            this->setNewDurum(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                    durum,SBLDKeys::SikayetKey::durumType::beklemede);
        });
        toolBar->addButton(std::move(beklemeBtn));
    }

    if( durum == SBLDKeys::SikayetKey::durumType::iptalEdildi )
    {
        auto teyitEdilmemisBtn = createColorButton(Bootstrap::Button::Primary.c_str(), "Tekrar Aç");
        teyitEdilmemisBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        teyitEdilmemisBtn->clicked().connect([=](){
            this->setNewDurum(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                    durum,SBLDKeys::SikayetKey::durumType::tekrarAcildi);
        });
        toolBar->addButton(std::move(teyitEdilmemisBtn));


        auto beklemeBtn = createColorButton(Bootstrap::Button::Warning.c_str(), "Beklemeye Al");
        beklemeBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        beklemeBtn->clicked().connect([=](){
            this->setNewDurum(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                    durum,SBLDKeys::SikayetKey::durumType::beklemede);
        });
        toolBar->addButton(std::move(beklemeBtn));
    }

    if( durum == SBLDKeys::SikayetKey::durumType::tamamlandi )
    {
        //        auto teyitEdilmemisBtn = createColorButton(Bootstrap::Button::Primary.c_str(), "Tekrar Aç");
        //        teyitEdilmemisBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        //        teyitEdilmemisBtn->clicked().connect([=](){
        //            std::cout << "Tekrar Açıldı(devam ediyor) Olurak Değiştirildi" << std::endl;
        //        });
        //        toolBar->addButton(std::move(teyitEdilmemisBtn));
    }

    if( durum == SBLDKeys::SikayetKey::durumType::teyitEdilmemis )
    {
        auto teyitEdilmemisBtn = createColorButton(Bootstrap::Button::Primary.c_str(), "Tekrar Aç");
        teyitEdilmemisBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        teyitEdilmemisBtn->clicked().connect([=](){
            this->setNewDurum(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                    durum,SBLDKeys::SikayetKey::durumType::tekrarAcildi);
        });
        toolBar->addButton(std::move(teyitEdilmemisBtn));
    }

    if( durum == SBLDKeys::SikayetKey::durumType::beklemede )
    {
        auto teyitEdilmemisBtn = createColorButton(Bootstrap::Button::Primary.c_str(), "Tekrar Aç");
        teyitEdilmemisBtn->setAttributeValue(Style::dataoid,view[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string().c_str());
        teyitEdilmemisBtn->clicked().connect([=](){
            this->setNewDurum(value.view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value.to_string(),
                    durum,SBLDKeys::SikayetKey::durumType::tekrarAcildi);
        });
        toolBar->addButton(std::move(teyitEdilmemisBtn));
    }

    return _container;
}

void Giris::Personel::Taleplerim::setNewDurum(std::string oid, std::string oldPos, std::string newPos)
{

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::SikayetKey::mainKey::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Durum Filtre Hatası",e);
        return;
    }

    auto pushDoc = document{};

    try {
        pushDoc.append(kvp(SBLDKeys::SikayetKey::asamakey::tip,SBLDKeys::SikayetKey::asamakey::tipKey::degisiklik));
        pushDoc.append(kvp(SBLDKeys::SikayetKey::asamakey::tarih,QDate::currentDate().toString("dddd - dd/MM/yyyy").toStdString().c_str()));
        pushDoc.append(kvp(SBLDKeys::SikayetKey::asamakey::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
        pushDoc.append(kvp(SBLDKeys::SikayetKey::asamakey::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
        pushDoc.append(kvp(SBLDKeys::SikayetKey::asamakey::degisim,oldPos+" ==> " + newPos + " Olarak Değiştirildi"));
        pushDoc.append(kvp(SBLDKeys::SikayetKey::asamakey::personel,make_document(kvp(SBLDKeys::SikayetKey::asamakey::Personel::adSoyad,this->User().view()[SBLDKeys::Personel::ad].get_utf8().value))));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Append Error",e);
        return;
    }



    auto setDoc = document{};

    try {
        setDoc.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,newPos));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","setAppend Error",e);
        return;
    }



    auto updateDoc = document{};

    try {
        updateDoc.append(kvp("$push",make_document(kvp(SBLDKeys::SikayetKey::mainKey::asama,pushDoc))));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","$pushDoc Hatası",e);
        return;
    }

    try {
        updateDoc.append(kvp("$set",setDoc));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","$set Hatası",e);
        return;
    }


    mongocxx::options::find_one_and_update findOptions;

    findOptions.return_document(mongocxx::options::return_document::k_after);




    try {
        auto upt = this->db()->collection(SBLDKeys::SikayetKey::collection).find_one_and_update(filter.view(),updateDoc.view(),findOptions);
        if( upt )
        {
            if( !upt.value().view().empty() )
            {
                this->toolbarReFresh();
                this->setDetail(upt.value().view()[SBLDKeys::SikayetKey::mainKey::oid].get_oid().value);
                //                auto degisim = std::make_unique<Giris::Taleplerim::DegisimWidget>((pushDoc));
                //                mAsamaContainer->addWidget(std::move(degisim));
            }else{
                this->showMessage("Error","No Document Updated");
            }
        }else{
            this->showMessage("Error","No Server Responding");
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error","$pushDoc Hatası",e);
        return;
    }

}

void Giris::Personel::Taleplerim::setPersonel(std::string oid)
{

    auto dialog = addChild(Wt::cpp14::make_unique<Wt::WDialog>("Personel Görevlendir"));

    auto table = dialog->contents()->addWidget(cpp14::make_unique<WTable>());
    dialog->contents()->setOverflow(Overflow::Scroll);
    dialog->contents()->setHeight(450);

    table->setHeaderCount(1);

    table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("#"));
    table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Ad Soyad"));


    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Personel::birimi,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Personel Birim Filter",e);
        return;
    }


    try {
        auto cursor = this->db()->collection(SBLDKeys::Personel::collection).find(filter.view());

        int row = 1;

        for( auto doc : cursor )
        {
            table->elementAt(row,0)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(row)));


            std::string personelAdi;
            try {
                personelAdi = doc[SBLDKeys::Personel::ad].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                personelAdi = e.what();
            }

            std::string personelTel;
            try {
                personelTel = doc[SBLDKeys::Personel::telefon].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                personelTel = e.what();
            }


            std::string iconPath;

            try {
                auto bucket = this->db()->gridfs_bucket();
                iconPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Personel::fotoOid].get_oid().value.to_string());
            } catch (bsoncxx::exception &e) {
                iconPath = e.what();
            }

            auto perWidget = std::make_unique<PersonelGorevlendirWidget>(this->db(),personelTel,doc[SBLDKeys::Personel::fotoOid].get_oid().value,bsoncxx::oid{oid},personelAdi,doc[SBLDKeys::Personel::oid].get_oid().value,iconPath,this->User());



            perWidget->clickOk().connect(dialog,&WDialog::accept);
            table->elementAt(row,1)->addWidget(std::move(perWidget));

            row++;
        }

    } catch (mongocxx::exception &e) {
        this->showMessage("Error","DB Error",e);
    }



    table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped");

    dialog->contents()->addStyleClass("form-group");

    Wt::WPushButton *cancel =
            dialog->footer()->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Cancel"));
    dialog->rejectWhenEscapePressed();

    cancel->clicked().connect(dialog, &Wt::WDialog::reject);


    dialog->finished().connect([=] {
        removeChild(dialog);
        this->setDetail(bsoncxx::oid{oid});
    });

    dialog->show();

}

Giris::Personel::PersonelGorevlendirWidget::PersonelGorevlendirWidget(mongocxx::database *db,std::string tel , bsoncxx::oid photoOid,bsoncxx::oid docOid ,  std::string personelAdi, bsoncxx::oid personelOid, std::string iconPath, bsoncxx::document::value user)
    :Giris::Personel::BaseWidget (db,user),
      _personelOid(personelOid),
      _personelAdi(personelAdi),
      _docoid(docOid),
      _photoOid(photoOid),
      _tel(tel)
{


    auto layout = this->setLayout(cpp14::make_unique<WHBoxLayout>());
    auto iconContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
    iconContainer->setAttributeValue(Style::style,Style::background::url(iconPath)+Style::background::repeat::norepeat+Style::background::size::contain);
    iconContainer->setHeight(40);
    iconContainer->setWidth(40);
    iconContainer->setMargin(0,AllSides);
    iconContainer->setPadding(0,AllSides);
    layout->addWidget(cpp14::make_unique<WText>(personelAdi),0,AlignmentFlag::Left|AlignmentFlag::Middle);
    layout->addStretch(1);

    auto btn = layout->addWidget(cpp14::make_unique<WPushButton>("Görevlendir"),0,AlignmentFlag::Right);
    btn->addStyleClass(Bootstrap::Button::Primary);

    btn->clicked().connect([=](){


        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::SikayetKey::mainKey::oid,_docoid));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI","Error Building Filter",e);
            return;
        }


        auto pushDoc = document{};

        try {
            pushDoc.append(kvp(SBLDKeys::SikayetKey::GorevliType::adsoyad,_personelAdi));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI","Error Building $push personelAdi",e);
            return;
        }
        try {
            pushDoc.append(kvp(SBLDKeys::SikayetKey::GorevliType::personelid,_personelOid));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI","Error Building $push personelOID",e);
            return;
        }
        try {
            pushDoc.append(kvp(SBLDKeys::SikayetKey::GorevliType::photooid,_photoOid));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI","Error Building $push photooid",e);
            return;
        }
        try {
            pushDoc.append(kvp(SBLDKeys::SikayetKey::GorevliType::tel,_tel));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI","Error Building $push tel",e);
            return;
        }


        auto updateDoc = document{};

        try {
            updateDoc.append(kvp("$push",make_document(kvp(SBLDKeys::SikayetKey::mainKey::gorevli,pushDoc))));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI","$push set Operator",e);
            return;
        }


        try {
            auto upt = this->db()->collection(SBLDKeys::SikayetKey::collection).update_one(filter.view(),updateDoc.view());
            if( upt )
            {
                if( upt.value().modified_count() )
                {
                    _clickOk.emit(NoClass());
                }else{
                    this->showMessage("Error","No Document Updated");
                }
            }else{
                this->showMessage("Error","DB Not Responding");
            }
        } catch (mongocxx::exception &e) {
            this->showMessage("UYARI","DB Error",e);
            return;
        }


    });

}

Signal<NoClass> &Giris::Personel::PersonelGorevlendirWidget::clickOk()
{
    return _clickOk;
}

Giris::Personel::EvrakArsiv::EvrakArsiv(mongocxx::database *_db, bsoncxx::document::value _user)
    :Giris::Personel::BaseWidget (_db,_user)
{
    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        {
            auto container = _layout->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto text = container->addWidget(cpp14::make_unique<WText>("Evrak Arşivi"));
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
        }
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }

    mContentContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    mContentContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));



    mEvrakContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    mEvrakContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    mEvrakContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
    this->initMenu();

    // Connection
    _initMenu.connect(this,&EvrakArsiv::initMenu);
    _initEvrakArama.connect(this,&EvrakArsiv::initEvrakArama);
    _initTumEvraklar.connect(this,&EvrakArsiv::initTumEvraklar);

    skip = 0;

}

void Giris::Personel::EvrakArsiv::initMenu()
{

    mContentContainer->clear();
    mEvrakContainer->clear();

    auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = container->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto aramaContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        aramaContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        aramaContainer->setContentAlignment(AlignmentFlag::Center);
        auto AramaBtn = aramaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        AramaBtn->setWidth(100);
        AramaBtn->setHeight(100);
        AramaBtn->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::DarkMagenta));
        AramaBtn->decorationStyle().setCursor(Cursor::PointingHand);
        auto layout = AramaBtn->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Evrak Arama"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        AramaBtn->clicked().connect(this,&EvrakArsiv::initEvrakArama);
    }


    {
        auto aramaContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        aramaContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        aramaContainer->setContentAlignment(AlignmentFlag::Center);
        auto AramaBtn = aramaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        AramaBtn->setWidth(200);
        AramaBtn->setHeight(100);
        //        AramaBtn->setAttributeValue(Style::style,Style::background::url("icon/3D-Logo.png")+Style::background::repeat::norepeat+
        //                                    Style::background::size::contain+
        //                                    Style::background::position::center_center);
        AramaBtn->decorationStyle().setCursor(Cursor::PointingHand);
        auto layout = AramaBtn->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->setContentsMargins(0,0,0,0);

        auto imgContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Center|AlignmentFlag::Top);
        imgContainer->setHeight(75);
        imgContainer->setWidth(75);
        imgContainer->setAttributeValue(Style::style,Style::background::url("icon/3D-Logo.png")+Style::background::repeat::norepeat+
                                        Style::background::size::contain+
                                        Style::background::position::center_center);

        Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/EArsiv/releases");
        link.setTarget(Wt::LinkTarget::NewWindow);

        std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                    "EArşiv v0.2");

        //        container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● Kod Kütüphanesi</b>"));

        auto text = layout->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● E-Arşiv Uygulaması</b>"),0,AlignmentFlag::Center|AlignmentFlag::Bottom);
        //        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        //        AramaBtn->clicked().connect(this,&EvrakArsiv::initTumEvraklar);
    }
}

void Giris::Personel::EvrakArsiv::initEvrakTip(WString kategoriName)
{


    auto filter = document{};

    if( kategoriName.toUTF8() != "Hepsi" )
    {
        try {
            filter.append(kvp(SBLDKeys::Arsiv::Kategori::kategoriAdi,kategoriName.toUTF8().c_str()));
        } catch (bsoncxx::exception &e) {
            Tipcombobox->addItem(e.what());
            return;
        }
    }


    try {
        filter.append(kvp(SBLDKeys::Arsiv::Kategori::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
    } catch (bsoncxx::exception &e) {
        Tipcombobox->addItem(e.what());
        return;
    }

    auto val = this->find_one(SBLDKeys::Arsiv::Kategori::collection,filter.view());

    if( val )
    {
        if( val.value().view().empty() )
        {
            this->showMessage("Error","Document Empty");
            return;
        }

        filter.clear();

        if( kategoriName.toUTF8() == "Hepsi" )
        {
            try {
                filter.append(kvp(SBLDKeys::Arsiv::Tip::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error",WString("User Department {1}").arg(e.what()).toUTF8());
                return;
            }
        }else{
            try {
                filter.append(kvp(SBLDKeys::Arsiv::Tip::kategorioid,val.value().view()[SBLDKeys::oid].get_oid().value));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error",WString("KategorOid {1}").arg(e.what()).toUTF8());
                return;
            }
        }


        auto cursor = this->find(SBLDKeys::Arsiv::Tip::collection,filter.view());
        Tipcombobox->clear();
        Tipcombobox->addItem("Hepsi");
        for( auto doc : cursor )
        {
            Tipcombobox->addItem(doc[SBLDKeys::Arsiv::Tip::adi].get_utf8().value.to_string().c_str());
        }

    }



}

void Giris::Personel::EvrakArsiv::initEvrakArama()
{

    mContentContainer->clear();

    auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);
    container->setPadding(0,Side::Right|Side::Left);

    auto row = container->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    // Back Main Menu
    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_12);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::SlateBlue));
        auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Ana Menü"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        backContainer->setHeight(128);
        backContainer->clicked().connect(this,&EvrakArsiv::initMenu);
        backContainer->decorationStyle().setCursor(Cursor::PointingHand);

    }


    // Kategori Select
    {
        kategoriContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        kategoriContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_12);
        kategoriContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::Fuchsia));
        auto kategorilayout = kategoriContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto kategoritext = kategorilayout->addWidget(cpp14::make_unique<WText>("Kategori"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        kategoritext->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        kateogoricombobox = kategorilayout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        kateogoricombobox->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::Grey::DimGray));

        auto filter = document{};
        try {
            filter.append(kvp(SBLDKeys::Arsiv::Kategori::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
        } catch (bsoncxx::exception &e) {
            kateogoricombobox->addItem(e.what());
            return;
        }
        auto cursor = this->find(SBLDKeys::Arsiv::Kategori::collection,filter.view());

        kateogoricombobox->addItem("Hepsi");
        for( auto doc : cursor )
        {
            kateogoricombobox->addItem(doc[SBLDKeys::Arsiv::Kategori::kategoriAdi].get_utf8().value.to_string().c_str());
        }
        kateogoricombobox->sactivated().connect(this,&EvrakArsiv::initEvrakTip);
        kategoriContainer->setHeight(128);
    }

    // Tip Select
    {
        TipContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        TipContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_12);
        TipContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::BlueViolet));
        auto layout = TipContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Evrak Tipi"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        Tipcombobox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        Tipcombobox->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::Grey::DimGray));
        TipContainer->setHeight(128);
    }

    // Arşiv Adı , Anahtar Kelime , Sayı
    {
        ArsivKeysContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        ArsivKeysContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_12);
        ArsivKeysContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::Indigo));
        auto layout = ArsivKeysContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto container = layout->addWidget(cpp14::make_unique<WContainerWidget>());
            auto _layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            lineEditArsivAdi = _layout->addWidget(cpp14::make_unique<WLineEdit>());
            lineEditArsivAdi->setPlaceholderText("Arşiv Adı");
        }
        {
            auto container = layout->addWidget(cpp14::make_unique<WContainerWidget>());
            auto _layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            {
                lineEditAnahtar = _layout->addWidget(cpp14::make_unique<WLineEdit>());
                lineEditAnahtar->setPlaceholderText("Anahtar Kelime");
            }
            {
                auto text = _layout->addWidget(cpp14::make_unique<WText>("Sayı"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setAttributeValue(Style::style,Style::font::size::s10px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                spinValueSayi = _layout->addWidget(cpp14::make_unique<WSpinBox>());
            }
        }

    }

    // ARA
    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_12);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::RebeccaPurple));
        auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Ara!"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        backContainer->setHeight(128);
        backContainer->decorationStyle().setCursor(Cursor::PointingHand);
        backContainer->clicked().connect(this,&EvrakArsiv::initList);
    }




    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_6);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::ForestGreen));
        auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Geri"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        backContainer->setHeight(40);
        backContainer->decorationStyle().setCursor(Cursor::PointingHand);
        backContainer->clicked().connect([=](){

            if( skip >= spinValueLimitValue->value() )
            {
                skip -= spinValueLimitValue->value();
                this->initList();
            }else{
                if( skip > 0 )
                {
                    skip = 0;
                }else{
                    this->showMessage("Bilgi","Off! Başka Yok mu?","YOK");
                }

            }
        });

    }


    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::OliveDrab));
        auto layout = backContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Arşiv Sayısı"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        spinValueLimitValue = layout->addWidget(cpp14::make_unique<WSpinBox>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        spinValueLimitValue->setValue(20);
        spinValueLimitValue->setSingleStep(10);
        spinValueLimitValue->setMaximum(100);
        spinValueLimitValue->setMinimum(20);
        spinValueLimitValue->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::Grey::Black));
        backContainer->setHeight(40);
        spinValueLimitValue->changed().connect(this,&EvrakArsiv::initList);
    }

    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::LightGreen));
        auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Sayfa"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::Grey::Black));
        backContainer->setHeight(40);
    }

    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_6);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::ForestGreen));
        auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("İleri"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        backContainer->setHeight(40);
        backContainer->decorationStyle().setCursor(Cursor::PointingHand);
        backContainer->clicked().connect([=](){
            auto count = this->listCount(this->filter().view());

            if( skip + spinValueLimitValue->value() < count )
            {
                skip += spinValueLimitValue->value();
                this->initList();
            }else{
                this->showMessage("Bilgi","Bu Kadar mı Evrak Var?","Evet!");
            }

        });

    }


}

void Giris::Personel::EvrakArsiv::initList()
{

    mEvrakContainer->clear();
    auto table = mEvrakContainer->addWidget(cpp14::make_unique<WTable>());
    table->setMargin(15,Side::Top|Side::Bottom);

    table->setHeaderCount(1);
    table->setWidth(WLength("100%"));

    table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("#"));
    table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Arşiv Adı"));
    table->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Anahtar Kelime"));
    table->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Sayı"));
    table->elementAt(0, 4)->addWidget(cpp14::make_unique<WText>("Birim"));
    table->elementAt(0, 5)->addWidget(cpp14::make_unique<WText>("İncele"));
    table->elementAt(0, 6)->addWidget(cpp14::make_unique<WText>("İndir"));

    table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped");


    auto filter = document{};

    if( kateogoricombobox->currentText().toUTF8() != "Hepsi" )
    {
        if( Tipcombobox->currentText().toUTF8() != "Hepsi" )
        {

            try {
                filter.append(kvp(SBLDKeys::Arsiv::Tip::adi,Tipcombobox->currentText().toUTF8().c_str()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error",WString("Tip Element: {1}").arg(e.what()).toUTF8());
            }

            try {
                filter.append(kvp(SBLDKeys::Arsiv::Tip::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Error",WString("Department Element: {1}").arg(e.what()).toUTF8());
            }

            //            std::cout << " T BSON: " << bsoncxx::to_json(filter.view()) << std::endl;

            try {
                auto val = this->db()->collection(SBLDKeys::Arsiv::Tip::collection).find_one(filter.view());
                filter.clear();
                if( val )
                {
                    if( !val.value().view().empty() )
                    {
                        std::cout << "VAL: " << bsoncxx::to_json(val.value().view()) << std::endl;
                        try {
                            filter.append(kvp(SBLDKeys::Arsiv::Arsiv::tipoid,val.value().view()[SBLDKeys::oid].get_oid().value));
                        } catch (bsoncxx::exception &e) {

                        }
                    }else{

                    }
                }else{

                }

            } catch (mongocxx::exception &e) {
            }
        }
    }


    //    std::cout << "BSON: " << bsoncxx::to_json(filter.view()) << std::endl;





    try {
        filter.append(kvp(SBLDKeys::Arsiv::Arsiv::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("Department Element: {1}").arg(e.what()).toUTF8());
    }

    if( !lineEditArsivAdi->text().empty() )
    {
        auto regexDoc = document{};

        try {
            regexDoc.append(kvp("$regex",".*"+lineEditArsivAdi->text().toUTF8()+".*"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("regex : {1}").arg(e.what()).toUTF8());
        }
        try {
            regexDoc.append(kvp("$options","i"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("options : {1}").arg(e.what()).toUTF8());

        }

        try {
            filter.append(kvp(SBLDKeys::Arsiv::Arsiv::arsivAdi,regexDoc));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Arşiv Adı : {1}").arg(e.what()).toUTF8());
        }
    }

    if( !lineEditAnahtar->text().empty() )
    {
        auto regexDoc = document{};

        try {
            regexDoc.append(kvp("$regex",".*"+lineEditAnahtar->text().toUTF8()+".*"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("regex : {1}").arg(e.what()).toUTF8());
        }
        try {
            regexDoc.append(kvp("$options","i"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("options : {1}").arg(e.what()).toUTF8());

        }

        try {
            filter.append(kvp(SBLDKeys::Arsiv::Arsiv::anahtarKelime,regexDoc));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Anahtar Kelime : {1}").arg(e.what()).toUTF8());
        }
    }

    if( spinValueSayi->value() )
    {
        try {
            filter.append(kvp(SBLDKeys::Arsiv::Arsiv::sayi,bsoncxx::types::b_int32{spinValueSayi->value()}));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Sayı : {1}").arg(e.what()).toUTF8());
        }
    }


    mongocxx::options::find findOptions;

    findOptions.limit(spinValueLimitValue->value());


    try {
        auto count = this->listCount(filter.view());

        if( count < this->skip )
        {
            this->skip = 0;
        }

    } catch (mongocxx::exception &e) {
        std::cout << "Skip Errror: " << e.what() << std::endl;
    }


    findOptions.skip(this->skip);



    auto cursor = this->find(SBLDKeys::Arsiv::Arsiv::collection,filter.view(),findOptions);

    int row = 1;
    oidList.clear();
    for( auto doc : cursor )
    {
        table->elementAt(row,0)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(skip+row)));
        table->elementAt(row,1)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Arsiv::Arsiv::arsivAdi].get_utf8().value.to_string().c_str())));
        table->elementAt(row,2)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Arsiv::Arsiv::anahtarKelime].get_utf8().value.to_string().c_str())));
        table->elementAt(row,3)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Arsiv::Arsiv::sayi].get_int32().value)));
        table->elementAt(row,4)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Arsiv::Arsiv::birim].get_utf8().value.to_string().c_str())));
        oidList.push_back(doc[SBLDKeys::oid].get_oid().value);

        {
            auto btn = cpp14::make_unique<WPushButton>("Buna Bakayım!");
            btn->addStyleClass(Bootstrap::Button::Primary);
            btn->clicked().connect([=](){
                auto oid = oidList.at(row-1);
                this->setArsiv(oid);
            });
            table->elementAt(row,5)->addWidget(std::move(btn));
        }

        {
            //            oidList.push_back(doc[SBLDKeys::oid].get_oid().value);
            auto btn = cpp14::make_unique<WPushButton>("indir");
            btn->addStyleClass(Bootstrap::Button::Link);
            btn->clicked().connect([=](){
                auto oid = oidList.at(row-1);
                this->setForDownload(oid);
            });
            table->elementAt(row,6)->addWidget(std::move(btn));
        }


        row++;
    }

}

int64_t Giris::Personel::EvrakArsiv::listCount(const bsoncxx::document::view &filter)
{
    try {
        auto count = this->db()->collection(SBLDKeys::Arsiv::Arsiv::collection).count(filter);
        return count;
    } catch (mongocxx::exception &e) {
        this->showMessage("Error",e.what());
        return 0;
    }
}

const bsoncxx::document::value Giris::Personel::EvrakArsiv::filter()
{

    auto filter = document{};


    try {
        filter.append(kvp(SBLDKeys::Arsiv::Arsiv::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error",WString("Department Element: {1}").arg(e.what()).toUTF8());
    }

    if( !lineEditArsivAdi->text().empty() )
    {
        auto regexDoc = document{};

        try {
            regexDoc.append(kvp("$regex",".*"+lineEditArsivAdi->text().toUTF8()+".*"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("regex : {1}").arg(e.what()).toUTF8());
        }
        try {
            regexDoc.append(kvp("$options","i"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("options : {1}").arg(e.what()).toUTF8());

        }

        try {
            filter.append(kvp(SBLDKeys::Arsiv::Arsiv::arsivAdi,regexDoc));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Arşiv Adı : {1}").arg(e.what()).toUTF8());
        }
    }

    if( !lineEditAnahtar->text().empty() )
    {
        auto regexDoc = document{};

        try {
            regexDoc.append(kvp("$regex",".*"+lineEditAnahtar->text().toUTF8()+".*"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("regex : {1}").arg(e.what()).toUTF8());
        }
        try {
            regexDoc.append(kvp("$options","i"));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("options : {1}").arg(e.what()).toUTF8());

        }

        try {
            filter.append(kvp(SBLDKeys::Arsiv::Arsiv::anahtarKelime,regexDoc));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Anahtar Kelime : {1}").arg(e.what()).toUTF8());
        }
    }

    if( spinValueSayi->value() )
    {
        try {
            filter.append(kvp(SBLDKeys::Arsiv::Arsiv::sayi,bsoncxx::types::b_int32{spinValueSayi->value()}));
        } catch (bsoncxx::exception &e) {
            this->showMessage("Error",WString("Sayı : {1}").arg(e.what()).toUTF8());
        }
    }

    return bsoncxx::document::value(filter.view());

}

void Giris::Personel::EvrakArsiv::setArsiv(bsoncxx::oid oid)
{

    mEvrakContainer->clear();

    auto containerFluid = mEvrakContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    containerFluid->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());

    row->addStyleClass(Bootstrap::Grid::row);

    auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    titleContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    titleContainer->setHeight(50);

    {
        auto Backcontainer = cpp14::make_unique<WContainerWidget>();
        Backcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
        Backcontainer->setPadding(2,AllSides);
        auto container = Backcontainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setMargin(0,AllSides);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::LightCoral));
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Ana Listeye\nDön!\n."));
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
        Backcontainer->decorationStyle().setCursor(Cursor::PointingHand);
        Backcontainer->clicked().connect([=](){
            this->initList();
        });
        row->addWidget(std::move(Backcontainer));
    }


    std::unique_ptr<WContainerWidget> PreviousEvrak;
    std::unique_ptr<WContainerWidget> NextEvrak;

    {
        PreviousEvrak = cpp14::make_unique<WContainerWidget>();
        PreviousEvrak->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
        PreviousEvrak->setPadding(2,AllSides);
        auto container = PreviousEvrak->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setMargin(0,AllSides);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::DarkOrchid+Style::color::color(Style::color::White::AliceBlue)));
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("◀Bir Önceki\nArşiv"));
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
        PreviousEvrak->decorationStyle().setCursor(Cursor::PointingHand);
    }

    {
        NextEvrak = cpp14::make_unique<WContainerWidget>();
        NextEvrak->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
        NextEvrak->setPadding(2,AllSides);
        auto container = NextEvrak->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setMargin(0,AllSides);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::DarkOrchid));
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Bir Sonraki Arşiv▶"));
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        NextEvrak->decorationStyle().setCursor(Cursor::PointingHand);
    }

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,oid));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Hata","Vala Endeki Dosya Filtresinde Bir Hata Oldu.","Napalım, Nasip Değilmiş!");
        return;
    }


    auto val = this->find_one(SBLDKeys::Arsiv::Arsiv::collection,filter.view());

    std::string Currentoid = "null";

    if( val )
    {
        if( !val.value().view().empty() )
        {
            try {
                auto view = val.value().view();
                Currentoid = view[SBLDKeys::oid].get_oid().value.to_string();

                //                std::cout << "Current OID: " << Currentoid << " Title: " <<view[SBLDKeys::Arsiv::Arsiv::arsivAdi].get_utf8().value.to_string() <<  std::endl;

                {
                    PreviousEvrak->clicked().connect([=](){
                        if( Currentoid != "null" )
                        {
                            auto filter_ = document{};
                            try {
                                filter_.append(kvp(SBLDKeys::oid,make_document(kvp("$lt",bsoncxx::oid{Currentoid.c_str()}))));
                            } catch (bsoncxx::exception &e) {
                                this->showMessage("Error",WString("Bence Sen Aç Mabel Matizden Öyle Kolaysayı,Boşver Evrak Mevrak İşini\n {1}").arg(e.what()).toUTF8(),"Kes Lan!");
                                return;
                            }
                            try {
                                filter_.append(kvp(SBLDKeys::Arsiv::Arsiv::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string().c_str()));
                            } catch (bsoncxx::exception &e) {
                                this->showMessage("Error",WString("Bence Sen Aç Mabel Matizden Öyle Kolaysayı,Boşver Evrak Mevrak İşini\n {1}").arg(e.what()).toUTF8(),"Kes Lan!");
                                return;
                            }
                            std::cout << "PREVIOUS ID: " << Currentoid << std::endl;

                            auto count = this->listCount(filter_.view());

                            mongocxx::options::find _findOptions;

                            if( count > 1 )
                            {
                                _findOptions.skip(count-1);
                            }

                            auto val = this->find_one(SBLDKeys::Arsiv::Arsiv::collection,filter_.view(),_findOptions);
                            if( val )
                            {
                                if( val->view().empty() )
                                {
                                    this->showMessage("Hata","Boyna Hata Çıkıyor Arkadaş!\nNeyse, Sen İstersen Başka Dosyaya Bak Olur mu?:)","Olur!");
                                }else{
                                    bsoncxx::oid _oid = val.value().view()[SBLDKeys::oid].get_oid().value;
                                    this->setArsiv(std::move(_oid));
                                }
                            }else{
                                this->showMessage("Hata","Yaw Hep Sana Denk Geliyor Haaa!","Eveeet!");
                            }
                        }
                    });
                    row->addWidget(std::move(PreviousEvrak));
                }

                {
                    NextEvrak->clicked().connect([=](){
                        if( Currentoid != "null" )
                        {
                            auto filter_ = document{};
                            try {
                                filter_.append(kvp(SBLDKeys::oid,make_document(kvp("$gt",bsoncxx::oid{Currentoid.c_str()}))));
                            } catch (bsoncxx::exception &e) {
                                this->showMessage("Error",WString("Bence Sen Aç Mabel Matizden Öyle Kolaysayı,Boşver Evrak Mevrak İşini\n {1}").arg(e.what()).toUTF8(),"Kafam Karışık!");
                                return;
                            }

                            try {
                                filter_.append(kvp(SBLDKeys::Arsiv::Arsiv::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string().c_str()));
                            } catch (bsoncxx::exception &e) {
                                this->showMessage("Error",WString("Bence Sen Aç Mabel Matizden Öyle Kolaysayı,Boşver Evrak Mevrak İşini\n {1}").arg(e.what()).toUTF8(),"Kafam Karışık!");
                                return;
                            }

                            std::cout << "NEXT ID: " << Currentoid << std::endl;
                            auto val = this->find_one(SBLDKeys::Arsiv::Arsiv::collection,filter_.view());
                            if( val )
                            {
                                if( val->view().empty() )
                                {
                                    this->showMessage("Hata","Boyna Hata Çıkıyor Arkadaş!\nNeyse, Sen İstersen Başka Dosyaya Bak Olur mu?:)","Olur!");
                                }else{
                                    bsoncxx::oid _oid = val.value().view()[SBLDKeys::oid].get_oid().value;
                                    this->setArsiv(std::move(_oid));
                                }
                            }else{
                                this->showMessage("Hata","Bundan Sonra Evrak Yok Yok!","Tamam Anladık!");
                            }
                        }
                    });
                    row->addWidget(std::move(NextEvrak));
                }


                auto __Layout = titleContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto title = __Layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Arsiv::Arsiv::arsivAdi].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
                title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
                title->setMargin(10,Side::Bottom|Side::Top);

                for( auto doc : view[SBLDKeys::Arsiv::Arsiv::dosyalar].get_array().value )
                {
                    auto Backcontainer = cpp14::make_unique<WContainerWidget>();
                    Backcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
                    Backcontainer->setPadding(2,AllSides);
                    auto container = Backcontainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->setMargin(0,AllSides);
                    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray));
                    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                    QFileInfo info(doc[SBLDKeys::Arsiv::Arsiv::dosya::adi].get_utf8().value.to_string().c_str());
                    auto text = layout->addWidget(cpp14::make_unique<WText>(info.fileName().toStdString()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    Backcontainer->decorationStyle().setCursor(Cursor::PointingHand);
                    bsoncxx::oid oid = doc[SBLDKeys::Arsiv::Arsiv::dosya::oid].get_oid().value;
                    Backcontainer->clicked().connect([=](){

                        this->setEvrak(std::move(oid));
                    });
                    row->addWidget(std::move(Backcontainer));
                }

            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Vala Ne Oldu Anlamadım!","Bende Anlamadım!");
            }

        }else{
            this->showMessage("Hata","Allah Allaahhh!","Bak Sen Şu İşe Yaw!");
        }
    }else{
        this->showMessage("Hata","Kim Bilir Ne Oldu?","off!");
    }

    msetEvrakWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    msetEvrakWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
}

void Giris::Personel::EvrakArsiv::setEvrak(bsoncxx::oid fileOid)
{

    if( !msetEvrakWidget )
    {
        this->showMessage("Hata","Boyna Hata Çıkıyor Arkadaş!\nNeyse, Sen İstersen Başka Dosyaya Bak Olur mu?:)","Olur!");
        return;
    }

    auto bucket = this->db()->gridfs_bucket();
    std::string filePath = SBLDKeys::downloadifNotExist(&bucket,fileOid.to_string());

    auto list = msetEvrakWidget->children();
    bool found = false;

    for( auto id : list )
    {
        if( id->id() == "pathViewer" )
        {
            found = true;
        }
    }

    if( found )
    {

        QFileInfo info(filePath.c_str());
        if( info.suffix() == "pdf" )
        {
            std::string link = "<iframe src=\""+filePath+"\"  height=\"1200px\" width=\"100%\"; allowfullscreen></iframe>";
            msetEvrakWidget->clear();
            auto TExtid = msetEvrakWidget->addWidget(cpp14::make_unique<WText>(link,TextFormat::UnsafeXHTML));
            TExtid->setAttributeValue(Style::style,"max-width:950px; width:100%;");
        }else{
            mEvrakCanvas->setImageLink(WLink(filePath));
            mEvrakCanvas->setAttributeValue(Style::style,"max-width:950px; width:100%;");
        }

    }else{

        msetEvrakWidget->clear();

        QFileInfo info(filePath.c_str());
        if( info.suffix() == "pdf" )
        {
            std::string link = "<iframe src=\""+filePath+"\"  height=\"1200px\" width=\"100%\"; allowfullscreen></iframe>";
            auto TExtid = msetEvrakWidget->addWidget(cpp14::make_unique<WText>(link,TextFormat::UnsafeXHTML));
            TExtid->setAttributeValue(Style::style,"max-width:950px; width:100%;");
        }else{
            mEvrakCanvas = msetEvrakWidget->addWidget(cpp14::make_unique<WImage>(WLink(filePath)));
            mEvrakCanvas->setAttributeValue(Style::style,"max-width:950px; width:100%;");
            mEvrakCanvas->setId("pathViewer");
        }

    }

}

void Giris::Personel::EvrakArsiv::setForDownload(bsoncxx::oid oid)
{

    //    std::cout << oid.to_string().c_str()<<std::endl;



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,oid));
    } catch (bsoncxx::exception &e) {
        std::cout << "Exception Error: " << e.what() << std::endl;
    }


    auto projectFilter = document{};

    try {
        projectFilter.append(kvp(SBLDKeys::Arsiv::Arsiv::dosyalar,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "Exception Error: " << e.what() << std::endl;
    }

    try {
        projectFilter.append(kvp(SBLDKeys::Arsiv::Arsiv::anahtarKelime,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "Exception Error: " << e.what() << std::endl;
    }

    mongocxx::options::find findOptions;

    findOptions.projection(projectFilter.view());

    try {
        auto val = this->db()->collection(SBLDKeys::Arsiv::Arsiv::collection).find_one(filter.view(),findOptions);

        if( !val.value().view().empty() )
        {

            auto arsivadi = val.value().view()[SBLDKeys::Arsiv::Arsiv::anahtarKelime].get_utf8().value.to_string();
            auto filelist = val.value().view()[SBLDKeys::Arsiv::Arsiv::dosyalar].get_array().value;

            auto bucket = this->db()->gridfs_bucket();

            std::vector<std::string> filenameList;

            for( auto element : filelist )
            {

                auto fileOid = element.get_document().view()[SBLDKeys::Arsiv::Arsiv::dosya::oid].get_oid().value;
                std::string filePath = SBLDKeys::downloadifNotExist(&bucket,fileOid.to_string());
                filenameList.push_back(filePath);
            }



            std::string scriptString;

            scriptString +="var urls = [";

            for( auto url : filenameList )
            {
                scriptString += "\""+url+"\",";
            }
            scriptString += "];"
                            "var name = \""+std::string(QLatin1String(arsivadi.c_str()).latin1())+".zip\";"
                                                                                                  "compressed_img(urls,name);";


            this->doJavaScript(scriptString);


        }

    } catch (mongocxx::exception &e) {

    }



}

void Giris::Personel::EvrakArsiv::initTumEvraklar()
{

    mContentContainer->clear();

    auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = container->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::Magenta));
        auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Ana Menü"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        backContainer->setWidth(75);
        backContainer->setHeight(75);
        backContainer->clicked().connect(this,&EvrakArsiv::initMenu);
        backContainer->decorationStyle().setCursor(Cursor::PointingHand);
    }

    {
        auto backContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        backContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkGray));
        auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Bu Fonksiyon Kullanılabilir Değil!"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        //        backContainer->setWidth(75);
        backContainer->setHeight(75);
        backContainer->clicked().connect(this,&EvrakArsiv::initMenu);
        backContainer->decorationStyle().setCursor(Cursor::PointingHand);
    }
}

//Giris::Personel::Basvurularim::Basvurularim(mongocxx::database *_db, bsoncxx::document::value _user)
//    :Giris::Personel::BaseWidget (_db,_user)
//{

//    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
//    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

//    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    row->addWidget(cpp14::make_unique<WContainerWidget>());
//    row->addStyleClass(Bootstrap::Grid::row);

//    {
//        auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
//        {
//            auto container = _layout->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//            auto text = container->addWidget(cpp14::make_unique<WText>("Gelen Başvurular"));
//            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
//        }
//        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
//                                      Style::background::color::color(Style::color::Grey::Gainsboro));
//    }


//    toolBarContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
//    toolBarContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

//    mContentContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
//    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
//    mContentContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));

//    this->toolbarReFresh();




//    this->initList();

//}

//void Giris::Personel::Basvurularim::toolbarReFresh()
//{

//    toolBarContainer->clear();

//    Wt::WToolBar *toolBar = toolBarContainer->addWidget(Wt::cpp14::make_unique<Wt::WToolBar>());
//    toolBar->setMargin(25,Side::Top|Side::Bottom);


//    auto tamamlananBtn = createColorButton("btn-success", "Cevaplananlar "+Bootstrap::Badges::badget(std::to_string(this->countTalepler("Cevaplandı"))));
//    tamamlananBtn->clicked().connect([=](){
//        this->initList("Cevaplandı");
//    });
//    toolBar->addButton(std::move(tamamlananBtn));



//    auto beklemeBtn = createColorButton("btn-warning", "Cevaplanmayanlar "+Bootstrap::Badges::badget(std::to_string(this->countTalepler("Cevaplanmayanlar"))));
//    beklemeBtn->clicked().connect([=](){
//        this->initList("Cevaplanmayanlar");
//    });
//    toolBar->addButton(std::move(beklemeBtn));

//    toolBar->addSeparator();
//    auto hepsiBtn = createColorButton("btn-default", "Hepsi "+Bootstrap::Badges::badget(std::to_string(this->countTalepler())));
//    hepsiBtn->clicked().connect([=](){
//        this->initList();
//    });
//    toolBar->addButton(std::move(hepsiBtn));
//}

//int64_t Giris::Personel::Basvurularim::countTalepler(std::string filterKey)
//{

//    auto filter = document{};

//    try {
//        filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
//    } catch (bsoncxx::exception &e) {
//        this->showMessage("Error","Bir Hata Oluştu vatandas",e);
//        return 0;
//    }




//    if( filterKey != "Hepsi" )
//    {
//        if( filterKey == "Cevaplandı" )
//        {
//            try {
//                filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::geridonus,SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapildi));
//            } catch (bsoncxx::exception &e) {
//                this->showMessage("Error","Bir Hata Oluştu durum",e);
//                return 0 ;
//            }
//        }else {
//            try {
//                filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::geridonus,SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapilmadi));
//            } catch (bsoncxx::exception &e) {
//                this->showMessage("Error","Bir Hata Oluştu durum",e);
//                return 0 ;
//            }
//        }
//    }


//    //    std::cout << "BSON: " << bsoncxx::to_json(filter.view())  << std::endl;

//    try {
//        auto cursor = this->db()->collection(SBLDKeys::BilgiEdinme::Basvuru::collection).count(filter.view());
//        return cursor;
//    } catch (mongocxx::exception &e) {
//        return 0;
//    }
//}

//void Giris::Personel::Basvurularim::initList(std::string filterKey )
//{

//    mContentContainer->clear();
//    auto table = mContentContainer->addWidget(cpp14::make_unique<WTable>());
//    table->setMargin(15,Side::Top|Side::Bottom);

//    table->setHeaderCount(1);
//    table->setWidth(WLength("100%"));

//    table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("#"));
//    table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Başvuru Sahibi"));
//    table->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Konu"));
//    table->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Tarih"));
//    table->elementAt(0, 4)->addWidget(cpp14::make_unique<WText>("Cevap"));
//    table->elementAt(0, 5)->addWidget(cpp14::make_unique<WText>("İncele"));

//    table->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped");


//    auto filter = document{};

//    if( filterKey != "Hepsi" )
//    {
//        if( filterKey == "Cevaplandı" )
//        {
//            try {
//                filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::geridonus,SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapildi));
//            } catch (bsoncxx::exception &e) {
//                std::cout << "Error: " << e.what() << std::endl;
//            }
//        }else{
//            try {
//                filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::geridonus,SBLDKeys::BilgiEdinme::Basvuru::geriDonusVar::yapilmadi));
//            } catch (bsoncxx::exception &e) {
//                std::cout << "Error: " << e.what() << std::endl;
//            }
//        }
//    }

//    try {
//        filter.append(kvp(SBLDKeys::BilgiEdinme::Basvuru::birim,this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value));
//    } catch (bsoncxx::exception &e) {
//        std::cout << "Error: " << e.what() << std::endl;
//    }


//    try {
//        auto cursor = this->db()->collection(SBLDKeys::BilgiEdinme::Basvuru::collection).find(filter.view());
//        list.clear();
//        for( auto doc : cursor )
//        {
//            basvuruStruct basvuru;
//            basvuru.oid = doc[SBLDKeys::oid].get_oid().value.to_string();
//            basvuru.konu = doc[SBLDKeys::BilgiEdinme::Basvuru::konu].get_utf8().value.to_string();
//            basvuru.basvuruSahibi = doc[SBLDKeys::BilgiEdinme::Basvuru::adsoyad].get_utf8().value.to_string();
//            if( doc[SBLDKeys::BilgiEdinme::Basvuru::geridonus].get_bool().value )
//            {
//                basvuru.cevap = "Cevap Var";
//            }else{
//                basvuru.cevap = "Cevap Yok";
//            }
//            basvuru.tarih = doc[SBLDKeys::BilgiEdinme::Basvuru::tarih].get_utf8().value.to_string();
//            list.push_back(basvuru);
//        }

//    } catch (mongocxx::exception &e) {

//    }


//    int row = 1;
//    for( std::size_t i = 0 ; i < list.size() ; i++ )
//    {
//        basvuruStruct item = list.at(i);
//        table->elementAt(row, 0)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(row)));
//        table->elementAt(row, 1)->addWidget(cpp14::make_unique<WText>(item.basvuruSahibi));
//        table->elementAt(row, 2)->addWidget(cpp14::make_unique<WText>(item.konu));
//        table->elementAt(row, 3)->addWidget(cpp14::make_unique<WText>(item.tarih));
//        table->elementAt(row, 4)->addWidget(cpp14::make_unique<WText>(item.cevap));


//        auto btn = std::make_unique<WPushButton>("İncele");
//        btn->addStyleClass(Bootstrap::Button::Primary);
//        std::string oid = item.oid;
//        btn->clicked().connect([=](){
//            this->setBasvuruDetail(oid);
//        });
//        table->elementAt(row, 5)->addWidget(std::move(btn));
//    }



//}


//// TODO: Başvuru Detayları BUrada Gözükecek
//void Giris::Personel::Basvurularim::setBasvuruDetail(std::string oid)
//{


//    std::cout << "Oid " << oid << std::endl;

//}



Giris::Personel::ContainerWidget::ContainerWidget(mongocxx::database *_db, bsoncxx::document::value _user, std::string ContainerTitle)
    :BaseWidget::BaseWidget(_db,_user)
{
    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    mRowContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mRowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mRowContainer->addStyleClass(Bootstrap::Grid::row);

    if( ContainerTitle != "NOCONTAINER" ){
        auto _container = mRowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        {
            auto container = _layout->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto text = container->addWidget(cpp14::make_unique<WText>(ContainerTitle));
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
        }
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }

}



Giris::Personel::ProjeWidget::ProjeWidget(mongocxx::database *_database, bsoncxx::document::value _user)
    :ContainerWidget::ContainerWidget(_database,_user,"Projeler")
{

    // Proje Durumu Toplam , Devam Eden  , Tamamlanan , Yapılacak Projeler (Text Formatında)
    {
        this->initProjectState();
    }


    this->initMenu();




    {
        mMainContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mMainContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        mMainContainer->setPadding(0,AllSides);
    }







}

void Giris::Personel::ProjeWidget::initProjectState()
{


    auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    container->setPadding(0,AllSides);

    auto fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    //Toplam Proje Sayısı
    {
        auto toplamCOntainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        toplamCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Toplam Proje: {1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count(document{}.view()))),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(150);
        toplamCOntainer->decorationStyle().setCursor(Cursor::PointingHand);
        toplamCOntainer->clicked().connect([=](){
            this->setList();
        });
    }

    //Tamamlanan Proje Sayısı
    {
        auto toplamCOntainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        toplamCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tamamlanan Projeler: {1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count(make_document(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Calismalar::DURUM::tamamlandi))))),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(150);
        toplamCOntainer->decorationStyle().setCursor(Cursor::PointingHand);
        toplamCOntainer->clicked().connect([=](){
            this->setList(SBLDKeys::Projeler::DURUM::tamamlandi);
        });
    }

    //Devam Eden Proje Sayısı
    {
        auto toplamCOntainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        toplamCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Devam Eden Projeler: {1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count(make_document(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Calismalar::DURUM::devamediyor))))),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(150);
        toplamCOntainer->decorationStyle().setCursor(Cursor::PointingHand);
        toplamCOntainer->clicked().connect([=](){
            this->setList(SBLDKeys::Projeler::DURUM::devamediyor);
        });
    }

    //Yapılacak Proje Sayısı
    {
        auto toplamCOntainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        toplamCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Yapılacak Projeler: {1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count(make_document(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Calismalar::DURUM::yapilacak))))),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(150);
        toplamCOntainer->decorationStyle().setCursor(Cursor::PointingHand);
        toplamCOntainer->clicked().connect([=](){
            this->setList(SBLDKeys::Projeler::DURUM::yapilacak);
        });
    }

}

void Giris::Personel::ProjeWidget::initMenu()
{



    auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    container->setPadding(0,AllSides);


    auto fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);



    //Mahalle Mahalle
    {
        auto imgContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->setAttributeValue(Style::style,Style::background::url("img/2.jpg")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
        imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        imgContainer->setPadding(0,AllSides);

        auto toplamCOntainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Mahalle/Mahalle"),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(100);
        imgContainer->clicked().connect(this,&ProjeWidget::initMahallelerdeProje);
        imgContainer->decorationStyle().setCursor(Cursor::PointingHand);
    }

    //Birim Birim
    {
        auto imgContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->setAttributeValue(Style::style,Style::background::url("img/aspendos.jpg")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
        imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        imgContainer->setPadding(0,AllSides);

        auto toplamCOntainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Birim/Birim"),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(100);
        imgContainer->clicked().connect(this,&ProjeWidget::initBirimlerdeProje);
        imgContainer->decorationStyle().setCursor(Cursor::PointingHand);
    }

    //Birim Mahalle
    {
        auto imgContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->setAttributeValue(Style::style,Style::background::url("img/asfalt.jpg")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
        imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        imgContainer->setPadding(0,AllSides);

        auto toplamCOntainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Birim/Mahalle"),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(100);
        imgContainer->clicked().connect(this,&ProjeWidget::initBirimMahalleProje);
        imgContainer->decorationStyle().setCursor(Cursor::PointingHand);
    }

    //Mahalle YIL
    {
        auto imgContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->setAttributeValue(Style::style,Style::background::url("img/kopru.jpg")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
        imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        imgContainer->setPadding(0,AllSides);

        auto toplamCOntainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        toplamCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
        toplamCOntainer->setPadding(5,AllSides);
        auto _container = toplamCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Mahalle/Yıl"),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        _container->setHeight(100);
        imgContainer->clicked().connect(this,&ProjeWidget::initMahalleYilProje);
        imgContainer->decorationStyle().setCursor(Cursor::PointingHand);
    }






}

void Giris::Personel::ProjeWidget::initMahallelerdeProje()
{

    mMainContainer->clear();
    //    mMainContainer->setPadding(5,AllSides);

    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);



    auto mahList = this->getMahalleList();


    for( decltype (mahList.size()) i = 0 ; i < mahList.size() ; i++ )
    {

        auto mah = mahList.at(i);

        auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        _container->decorationStyle().setCursor(Cursor::PointingHand);
        _container->setMargin(5,Side::Top|Side::Bottom);
        _container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
            } catch (bsoncxx::exception &e) {

            }
            this->setProjeList(filter.view());
        });


        auto filter = document{};
        try {
            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
        } catch (bsoncxx::exception &e) {

        }

        std::int64_t toplamProjeSayisi_ = this->count(SBLDKeys::Projeler::collection,filter.view());
        std::int64_t toplamProjeSayisi = toplamProjeSayisi_;
        if( toplamProjeSayisi * 20 > 255 )
        {
            toplamProjeSayisi = 255;
        }else{
            toplamProjeSayisi *= 20;
        }

        auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(20,toplamProjeSayisi,20,0.85));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        QString birim = mah.mahalle.c_str();
        if( birim.count() > 27 )
        {
            birim = birim.mid(0,25);
            birim.append("...");
        }

        auto mahText = vLayout->addWidget(cpp14::make_unique<WText>(birim.toStdString()),0,AlignmentFlag::Center);
        mahText->addStyleClass(Bootstrap::Label::Warning);
        mahText->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));



        auto toplamText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Toplam: {1}").arg(toplamProjeSayisi_)),0,AlignmentFlag::Center);
        toplamText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

        try {
            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }

        auto devamText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Devam Eden: {1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Center);
        devamText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);

        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }

        auto tamamlananText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tamamlanan: {1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Center);
        tamamlananText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
        } catch (bsoncxx::exception &e) {

        }

        auto yapilacakText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Yapılacak: {1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Center);
        yapilacakText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

    }



}

std::vector<Giris::Personel::ProjeWidget::MahalleItem> Giris::Personel::ProjeWidget::getMahalleList() const
{
    std::vector<MahalleItem> MahalleList;

    MahalleList.clear();
    try {
        auto cursor = this->db()->collection(SBLDKeys::Mahalle::collection).find(document{}.view());
        for( auto doc : cursor )
        {
            MahalleItem item;
            try {
                item.mahalle = doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.mahalle = e.what();
            }
            try {
                item.oid = doc[SBLDKeys::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.oid = e.what();
            }
            if( item.mahalle != "NULL" )
            {
                MahalleList.push_back(item);
            }

        }
    } catch (mongocxx::exception &e) {

    }

    return MahalleList;

}

void Giris::Personel::ProjeWidget::initBirimlerdeProje()
{
    mMainContainer->clear();
    //    mMainContainer->setPadding(5,AllSides);

    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);



    auto birimList = this->getBirimList();



    for( decltype (birimList.size()) i = 0 ; i < birimList.size() ; i++ )
    {

        auto mah = birimList.at(i);

        auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);


        auto filter = document{};
        try {
            filter.append(kvp(SBLDKeys::Projeler::birim,mah.birim));
        } catch (bsoncxx::exception &e) {

        }

        std::int64_t toplamProjeSayisi_ = this->count(SBLDKeys::Projeler::collection,filter);
        std::int64_t toplamProjeSayisi = toplamProjeSayisi_;
        if( toplamProjeSayisi * 20 > 255 )
        {
            toplamProjeSayisi = 255;
        }else{
            toplamProjeSayisi *= 20;
        }

        auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(20,toplamProjeSayisi,20,0.85));
        container->decorationStyle().setCursor(Cursor::PointingHand);

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        QString birim = mah.birim.c_str();
        if( birim.count() > 25 )
        {
            birim = birim.mid(0,22);
            birim.append("...");
        }

        auto mahText = vLayout->addWidget(cpp14::make_unique<WText>(birim.toStdString()),0,AlignmentFlag::Center);
        mahText->addStyleClass(Bootstrap::Label::Warning);
        mahText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));



        auto toplamText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Toplam: {1}").arg(toplamProjeSayisi_)),0,AlignmentFlag::Center);
        toplamText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

        try {
            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }

        auto devamText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Devam Eden: {1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Center);
        devamText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.birim));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }

        auto tamamlananText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tamamlanan: {1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Center);
        tamamlananText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.birim));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
        } catch (bsoncxx::exception &e) {

        }

        auto yapilacakText = vLayout->addWidget(cpp14::make_unique<WText>(WString("Yapılacak: {1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Center);
        yapilacakText->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

    }
}

std::vector<Giris::Personel::ProjeWidget::BirimItem> Giris::Personel::ProjeWidget::getBirimList() const
{

    auto _filter = document{};

    try {
        _filter.append(kvp(SBLDKeys::Birimler::haberlesmekodu,make_document(kvp("$ne","0"))));
    } catch (bsoncxx::exception &e) {

    }



    std::vector<BirimItem> BirimList;
    try {


        auto cursor = this->db()->collection(SBLDKeys::Birimler::collection).find(_filter.view());
        for( auto doc : cursor )
        {
            BirimItem item;
            try {
                item.birim = doc[SBLDKeys::Birimler::birim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.birim = e.what();
            }
            try {
                item.oid = doc[SBLDKeys::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.oid = e.what();
            }
            if( item.birim != "NULL" )
            {
                BirimList.push_back(item);
            }

        }
    } catch (mongocxx::exception &e) {

    }


    return BirimList;


}

void Giris::Personel::ProjeWidget::initBirimMahalleProje()
{


    mMainContainer->clear();
    //    mMainContainer->setPadding(5,AllSides);

    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);


    auto birimList = this->getBirimList();




    // PROJESI OLANLAR
    for( decltype (birimList.size()) i = 0 ; i < birimList.size() ; i++ )
    {

        auto birim = birimList.at(i);

        auto ProjeSayisi = this->count(SBLDKeys::Projeler::collection,make_document(kvp(SBLDKeys::Projeler::birim,birim.birim)));

        if( ProjeSayisi )
        {

            auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            _container->setPadding(5,Side::Top|Side::Bottom);

            auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,50),this->getRandom(0,50),this->getRandom(0,50),0.75));


            auto vlayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


            // Birim Başlığı
            {
                auto titleContainer = vlayout->addWidget(cpp14::make_unique<WContainerWidget>());
                auto titleLayout = titleContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
                auto birimTitle = titleLayout->addWidget(cpp14::make_unique<WText>(birim.birim),0,AlignmentFlag::Center);
                birimTitle->addStyleClass(Bootstrap::Label::Warning);
                birimTitle->setAttributeValue(Style::style,Style::font::size::s20px+Style::font::weight::bold);
            }

            // Projes Sayısı
            {
                auto titleContainer = vlayout->addWidget(cpp14::make_unique<WContainerWidget>());
                auto titleLayout = titleContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
                auto birimTitle = titleLayout->addWidget(cpp14::make_unique<WText>(WString("Proje Sayısı: {1}").arg(ProjeSayisi)),0,AlignmentFlag::Center);
                birimTitle->addStyleClass(Bootstrap::Label::Success);
                birimTitle->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
            }

            auto containerChart = vlayout->addWidget(cpp14::make_unique<WContainerWidget>());

            containerChart->addStyleClass(Bootstrap::Grid::container_fluid);

            auto rowContainer = containerChart->addWidget(cpp14::make_unique<WContainerWidget>());
            rowContainer->addStyleClass(Bootstrap::Grid::row);


            auto _chart = rowContainer->addWidget(cpp14::make_unique<BChart::DonutChart>());
            _chart->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

            std::vector<BChart::DonutChart::item> itemList;

            // Tamamlanan GRAFIK
            {
                BChart::DonutChart::item item;
                item.label = "Tamamlanan";
                item.rgb = QString("rgb(%1,%2,%3)").arg(this->getRandom()).arg(this->getRandom()).arg(this->getRandom()).toStdString();
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
                } catch (bsoncxx::exception &e) {

                }
                item.value = this->count(SBLDKeys::Projeler::collection,filter);
                item.label = QString("Tamamlanan: %1").arg(item.value).toStdString();

                itemList.push_back(item);
            }

            // DEVAM EDEN GRAFIK
            {
                BChart::DonutChart::item item;
                item.rgb = QString("rgb(%1,%2,%3)").arg(this->getRandom()).arg(this->getRandom()).arg(this->getRandom()).toStdString();
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
                } catch (bsoncxx::exception &e) {

                }
                item.value = this->count(SBLDKeys::Projeler::collection,filter);
                item.label = QString("Devam Eden: %1").arg(item.value).toStdString();

                itemList.push_back(item);
            }

            // YAPILACAK
            {
                BChart::DonutChart::item item;
                item.label = "Yapılacak";
                item.rgb = QString("rgb(%1,%2,%3)").arg(this->getRandom()).arg(this->getRandom()).arg(this->getRandom()).toStdString();
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
                } catch (bsoncxx::exception &e) {

                }
                item.value = this->count(SBLDKeys::Projeler::collection,filter);
                item.label = QString("Yapılacak: %1").arg(item.value).toStdString();
                itemList.push_back(item);
            }

            _chart->setDataList(itemList);


            // YIL YIL
            {
                auto MahListContainer = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());

                MahListContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                auto table = MahListContainer->addWidget(cpp14::make_unique<WTable>());
                table->setHeaderCount(1);
                table->setWidth(WLength("100%"));

                {
                    auto text = table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("Yıl"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Tamamlanan"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Devam Eden"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Yapılacak"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }


                table->addStyleClass("table form-inline table-bordered");




                QStringList mahalleList;

                try {
                    auto cursor = this->db()->collection(SBLDKeys::Projeler::collection).find(make_document(kvp(SBLDKeys::Projeler::birim,birim.birim)).view());

                    for( auto doc : cursor )
                    {


                        try {
                            mahalleList.append(QString::number(doc[SBLDKeys::Projeler::year].get_int32().value));
                        } catch (bsoncxx::exception &e) {

                        }

                    }
                } catch (mongocxx::exception &e) {

                }

                mahalleList.removeDuplicates();

                int row = 1;
                for( auto mah : mahalleList )
                {
                    auto filter = document{};
                    {
                        filter.clear();
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{mah.toInt()}));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
                        } catch (bsoncxx::exception &e) {

                        }


                        {
                            auto text = (cpp14::make_unique<WText>(mah.toStdString()));
                            text->addStyleClass(Bootstrap::Label::Primary);
                            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            table->elementAt(row, 0)->addWidget(std::move(text));
                        }
                        {
                            auto text = (cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter))));
                            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            table->elementAt(row, 1)->addWidget(std::move(text));
                        }
                    }

                    {
                        filter.clear();
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{mah.toInt()}));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
                        } catch (bsoncxx::exception &e) {

                        }


                        auto text = cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter)));
                        //                        text->addStyleClass(Bootstrap::Label::info);
                        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        table->elementAt(row, 2)->addWidget(std::move(text));
                    }

                    {
                        filter.clear();
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{mah.toInt()}));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
                        } catch (bsoncxx::exception &e) {

                        }


                        auto text = cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter)));
                        //                        text->addStyleClass(Bootstrap::Label::info);
                        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        table->elementAt(row, 3)->addWidget(std::move(text));
                    }
                    row++;
                }
            }




            // Mahalle Mahalle
            {
                auto MahListContainer = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());

                MahListContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                auto table = MahListContainer->addWidget(cpp14::make_unique<WTable>());
                table->setHeaderCount(1);
                table->setWidth(WLength("100%"));

                {
                    auto text = table->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("Mahalle"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Toplam"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Tamamlanan"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Devam Eden"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, 4)->addWidget(cpp14::make_unique<WText>("Yapılacak"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }


                table->addStyleClass("table form-inline table-bordered");




                QStringList mahalleList;

                try {
                    auto cursor = this->db()->collection(SBLDKeys::Projeler::collection).find(make_document(kvp(SBLDKeys::Projeler::birim,birim.birim)).view());

                    for( auto doc : cursor )
                    {


                        try {
                            mahalleList.append(doc[SBLDKeys::Projeler::mahalle].get_utf8().value.to_string().c_str());
                        } catch (bsoncxx::exception &e) {

                        }

                    }
                } catch (mongocxx::exception &e) {

                }

                mahalleList.removeDuplicates();

                int row = 1;
                for( auto mah : mahalleList )
                {
                    auto filter = document{};
                    {
                        filter.clear();
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.toStdString()));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }


                        {
                            auto text = (cpp14::make_unique<WText>(mah.toStdString()));
                            text->addStyleClass(Bootstrap::Label::info);
                            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            table->elementAt(row, 0)->addWidget(std::move(text));
                        }
                        {
                            auto text = (cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter.view()))));
                            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            table->elementAt(row, 1)->addWidget(std::move(text));
                        }
                    }
                    {
                        filter.clear();
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.toStdString()));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
                        } catch (bsoncxx::exception &e) {

                        }

                        {
                            auto text = (cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter.view()))));
                            text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            table->elementAt(row, 2)->addWidget(std::move(text));
                        }
                    }

                    {
                        filter.clear();
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.toStdString()));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
                        } catch (bsoncxx::exception &e) {

                        }


                        auto text = cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter.view())));
                        //                        text->addStyleClass(Bootstrap::Label::info);
                        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        table->elementAt(row, 3)->addWidget(std::move(text));
                    }

                    {
                        filter.clear();
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.toStdString()));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
                        } catch (bsoncxx::exception &e) {

                        }


                        auto text = cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter.view())));
                        //                        text->addStyleClass(Bootstrap::Label::info);
                        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        table->elementAt(row, 4)->addWidget(std::move(text));
                    }
                    row++;
                }
            }






            // PROJE PROJE
            {
                auto MahListContainer = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());

                MahListContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                auto table = MahListContainer->addWidget(cpp14::make_unique<WTable>());
                table->setHeaderCount(1);
                table->setWidth(WLength("100%"));

                int col = 0;
                {
                    auto text = table->elementAt(0, col++)->addWidget(cpp14::make_unique<WText>("#"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, col++)->addWidget(cpp14::make_unique<WText>("Yıl"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, col++)->addWidget(cpp14::make_unique<WText>("Proje"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, col++)->addWidget(cpp14::make_unique<WText>("Mahalle"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, col++)->addWidget(cpp14::make_unique<WText>("Durum"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }
                {
                    auto text = table->elementAt(0, col++)->addWidget(cpp14::make_unique<WText>("####"));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                }



                table->addStyleClass("table form-inline table-bordered");



                auto bucket = this->db()->gridfs_bucket();
                try {
                    auto cursor = this->db()->collection(SBLDKeys::Projeler::collection).find(make_document(kvp(SBLDKeys::Projeler::birim,birim.birim)).view());

                    int row = 1;
                    for( auto doc : cursor )
                    {
                        int col = 0;
                        {
                            try {
                                auto iconContainer = (cpp14::make_unique<WContainerWidget>());
                                auto imgPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Projeler::icon].get_oid().value.to_string());
                                iconContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
                                iconContainer->setHeight(50);
                                iconContainer->setWidth(75);
                                table->elementAt(row, col)->addWidget(std::move(iconContainer));
                                table->elementAt(row, col++)->setWidth(85);
                            } catch (bsoncxx::exception &e) {
                                auto text = (cpp14::make_unique<WText>(e.what()));
                                text->addStyleClass(Bootstrap::Label::Danger);
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            }

                            try {
                                auto text = (cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::year].get_int32().value)));
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            } catch (bsoncxx::exception &e) {
                                auto text = (cpp14::make_unique<WText>(e.what()));
                                text->addStyleClass(Bootstrap::Label::Danger);
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            }

                            try {
                                auto text = (cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::title].get_utf8().value.to_string().c_str())));
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            } catch (bsoncxx::exception &e) {
                                auto text = (cpp14::make_unique<WText>(e.what()));
                                text->addStyleClass(Bootstrap::Label::Danger);
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            }

                            try {
                                auto text = (cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::mahalle].get_utf8().value.to_string().c_str())));
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            } catch (bsoncxx::exception &e) {
                                auto text = (cpp14::make_unique<WText>(e.what()));
                                text->addStyleClass(Bootstrap::Label::Danger);
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            }

                            try {
                                auto text = (cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::durum].get_utf8().value.to_string().c_str())));
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            } catch (bsoncxx::exception &e) {
                                auto text = (cpp14::make_unique<WText>(e.what()));
                                text->addStyleClass(Bootstrap::Label::Danger);
                                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                                table->elementAt(row, col++)->addWidget(std::move(text));
                            }

                            table->elementAt(row, col)->setAttributeValue(Style::style,Style::background::color::color(Style::color::Purple::DarkViolet));
                            auto inceleLayout = table->elementAt(row, col)->setLayout(cpp14::make_unique<WVBoxLayout>());
                            inceleLayout->addStretch(1);

                            auto incelebtnContainer = inceleLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                            auto incelebtn = incelebtnContainer->addWidget(cpp14::make_unique<WText>("İncele"));
                            incelebtn->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

                            inceleLayout->addStretch(1);

                            incelebtnContainer->decorationStyle().setCursor(Cursor::PointingHand);
                            incelebtnContainer->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string().c_str());
                            incelebtnContainer->clicked().connect([=](){
                                this->setProjeDetail(incelebtnContainer->attributeValue(Style::dataoid).toUTF8());
                            });



                            row++;

                        }

                    }
                } catch (mongocxx::exception &e) {

                }


            }



        }
    }



    // Projesi Olmayanlari Yazdir
    for( decltype (birimList.size()) i = 0 ; i < birimList.size() ; i++ )
    {

        auto birim = birimList.at(i);

        auto ProjeSayisi = this->count(SBLDKeys::Projeler::collection,make_document(kvp(SBLDKeys::Projeler::birim,birim.birim)));


        if( !ProjeSayisi )
        {
            auto _container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            _container->setPadding(5,Side::Top|Side::Bottom);

            auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,50),this->getRandom(0,50),this->getRandom(0,50),0.75));
            container->decorationStyle().setCursor(Cursor::PointingHand);


            auto vlayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

            // Birim Başlığı
            {
                auto titleContainer = vlayout->addWidget(cpp14::make_unique<WContainerWidget>());
                auto titleLayout = titleContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
                auto birimTitle = titleLayout->addWidget(cpp14::make_unique<WText>(birim.birim),0,AlignmentFlag::Center);
                birimTitle->addStyleClass(Bootstrap::Label::Warning);
                birimTitle->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
            }

            // Birim Başlığı
            {
                auto titleContainer = vlayout->addWidget(cpp14::make_unique<WContainerWidget>());
                auto titleLayout = titleContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
                auto birimTitle = titleLayout->addWidget(cpp14::make_unique<WText>("Proje Yok"),0,AlignmentFlag::Center);
                birimTitle->addStyleClass(Bootstrap::Label::Danger);
                birimTitle->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
            }
        }
    }
}

void Giris::Personel::ProjeWidget::initMahalleYilProje()
{
    mMainContainer->clear();


    std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";

    auto fluidContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fluidContainer->setMargin(5,Side::Top);
    fluidContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);

    auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);
    rowContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(175,255),this->getRandom(175,255),this->getRandom(175,255),0.75));


    //TOPLAM YILLAR
    {
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        // Title
        {
            auto tContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto title = hLayout->addWidget(cpp14::make_unique<WText>("YIl Bazında Projeler"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
            title->addStyleClass(Bootstrap::Label::info);
            title->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
        }

        // Coloum Name
        {
            auto _fContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

            auto _rContainer = _fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer->addStyleClass(Bootstrap::Grid::row);


            {
                // YIL
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Yıl"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Toplam
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Toplam"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Tamamlanan
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Tamamlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Devam Eden
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Devam Eden"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Yapılacak
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Yapılacak"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }
            }
        }


        for( int i = 2019 ; i >=2014 ; i-- )
        {

            auto _fContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            _fContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120))+Style::background::blend_mode::color_dodge);

            auto _rContainer = _fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer->addStyleClass(Bootstrap::Grid::row);

            {

                // YIL
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(i)),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Default);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Toplam
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,make_document(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}))))),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Default);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Tamamlanan
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Default);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Devam Eden
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Default);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Yapılacak
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,filter))),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Default);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }
            }

        }




        // Title
        {
            auto tContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto title = hLayout->addWidget(cpp14::make_unique<WText>("Mahalle Bazında Yıl/Yıl Projeler"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
            title->addStyleClass(Bootstrap::Label::info);
            title->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
        }


        {
            auto _fContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            //            _fContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120))+Style::background::blend_mode::color_dodge);

            auto _rContainer = _fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer->addStyleClass(Bootstrap::Grid::row);


            // Coloum Name
            {
                // YIL
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Mahalle"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // YIL
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Yıl"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Toplam
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Toplam"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Tamamlanan
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Tamamlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Devam Eden
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Devam Eden"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }

                // Yapılacak
                {
                    auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto title = hLayout->addWidget(cpp14::make_unique<WText>("Yapılacak"),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                    title->addStyleClass(Bootstrap::Label::Danger);
                    title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                }
            }
        }

        auto mahList = this->getMahalleList();

        for( auto mah : mahList )
        {


            auto _fContainer_ = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fContainer_->addStyleClass(Bootstrap::Grid::container_fluid);

            std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
            _fContainer_->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);

            auto _rContainer_ = _fContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer_->addStyleClass(Bootstrap::Grid::row);
            _rContainer_->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(50,100),this->getRandom(50,100),this->getRandom(50,100),0.85));


            // Mahalle
            {
                auto tContainer = _rContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                QString str = mah.mahalle.c_str();
                str = str.remove(str.count()-9,9);
                auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(str.toStdString().c_str())),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                title->addStyleClass(Bootstrap::Label::Primary);
                title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                tContainer->clicked().connect([=](){
                    auto filter = document{};

                    try {
                        filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
                    } catch (bsoncxx::exception &e) {

                    }
                    this->setProjeList(filter.view());

                });
                tContainer->decorationStyle().setCursor(Cursor::PointingHand);
            }



            for( int i = 2019 ; i >=2014 ; i-- )
            {

                auto _fContainer = _rContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                _fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

                auto _rContainer = _fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                _rContainer->addStyleClass(Bootstrap::Grid::row);

                bool ToplamProje = false;

                {

                    // BLACK WIDGET
                    {
                        auto tContainer = _rContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                        tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                        auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("")),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                        title->addStyleClass(Bootstrap::Label::Primary);
                        title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                    }


                    // YIL
                    {
                        auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                        auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(i)),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                        title->addStyleClass(Bootstrap::Label::Warning);
                        title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                        tContainer->clicked().connect([=](){
                            auto filter = document{};

                            try {
                                filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
                            } catch (bsoncxx::exception &e) {

                            }
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                            } catch (bsoncxx::exception &e) {

                            }
                            this->setProjeList(filter.view(),i);

                        });
                        tContainer->decorationStyle().setCursor(Cursor::PointingHand);
                    }

                    // Toplam
                    {
                        auto filter = document{};
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                        } catch (bsoncxx::exception &e) {

                        }
                        auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                        auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        auto _ToplamProje = this->count(SBLDKeys::Projeler::collection,filter);
                        if( _ToplamProje ) ToplamProje = true;
                        auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(_ToplamProje)),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                        title->addStyleClass(Bootstrap::Label::Default);
                        title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                    }

                    // Tamamlanan
                    {
                        auto _ToplamProje = 0;
                        if( ToplamProje )
                        {
                            auto filter = document{};
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
                            } catch (bsoncxx::exception &e) {

                            }
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
                            } catch (bsoncxx::exception &e) {

                            }
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                            } catch (bsoncxx::exception &e) {

                            }
                            _ToplamProje = this->count(SBLDKeys::Projeler::collection,filter);
                        }

                        auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                        auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());



                        auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(_ToplamProje)),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                        title->addStyleClass(Bootstrap::Label::Default);
                        title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                    }

                    // Devam Eden
                    {
                        auto _ToplamProje = 0;
                        if( ToplamProje )
                        {
                            auto filter = document{};
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
                            } catch (bsoncxx::exception &e) {

                            }
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
                            } catch (bsoncxx::exception &e) {

                            }
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                            } catch (bsoncxx::exception &e) {

                            }
                            _ToplamProje = this->count(SBLDKeys::Projeler::collection,filter);
                        }


                        auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                        auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(_ToplamProje)),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                        title->addStyleClass(Bootstrap::Label::Default);
                        title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                    }

                    // Yapılacak
                    {
                        auto _ToplamProje = 0;
                        if( ToplamProje )
                        {
                            auto filter = document{};
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
                            } catch (bsoncxx::exception &e) {

                            }
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
                            } catch (bsoncxx::exception &e) {

                            }
                            try {
                                filter.append(kvp(SBLDKeys::Projeler::year,bsoncxx::types::b_int32{i}));
                            } catch (bsoncxx::exception &e) {

                            }
                            _ToplamProje = this->count(SBLDKeys::Projeler::collection,filter);
                        }
                        auto tContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        tContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                        auto hLayout = tContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        auto title = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(_ToplamProje)),0,AlignmentFlag::Middle|AlignmentFlag::Left);
                        title->addStyleClass(Bootstrap::Label::Default);
                        title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::Azure));
                    }
                }

            }
        }

    }






}

void Giris::Personel::ProjeWidget::setProjeDetail(std::string oid)
{

    mMainContainer->clear();



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->db()->collection(SBLDKeys::Projeler::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Projeler::title].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(view[SBLDKeys::Projeler::julianDate].get_int64().value)),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Projeler::durum].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    //    {
    //        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
    //        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
    //        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Kategori {1}").arg(view[SBLDKeys::Calismalar::kategori].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
    //        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    //    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Projeler::birim].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    //    {
    //        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
    //        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
    //        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tanım: <u>{1}</u>").arg(view[SBLDKeys::Calismalar::tanim].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
    //        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    //    }


    {
        auto text = TitleContainerColor->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(view[SBLDKeys::Projeler::mahalle].get_utf8().value.to_string().c_str())));
        text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
    }


    //    {
    //        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
    //        container->addStyleClass(Bootstrap::Grid::row);
    //        container->setPadding(10,AllSides);
    //        for( auto mah : view[SBLDKeys::Calismalar::mahalleler].get_array().value )
    //        {
    //            auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mah.get_utf8().value.to_string().c_str())));
    //            text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
    //            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
    //        }
    //    }


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto bucket = this->db()->gridfs_bucket();
        std::string iconPaht = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Projeler::icon].get_oid().value.to_string());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }


    {



        //        mMainContainer->setPadding(5,Side::Top);
        auto asamaContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto bucket = this->db()->gridfs_bucket();
        try {
            auto asamaList = view[SBLDKeys::Projeler::slide].get_array().value;

            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());


                auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                slideCOntainer->setHeight(350);


                std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Projeler::slideItem::img].get_oid().value.to_string());

                auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                frontimgContainer->setHeight(350);
                frontimgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                auto imgContainer = cpp14::make_unique<WContainerWidget>();
                imgContainer->setHeight(550);
                imgContainer->setWidth(WLength("100%"));
                imgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::contain+Style::background::position::center_center+"z-index:-1; ");
                imgContainer->addStyleClass("CommanFilter");
                imgContainer->setPositionScheme(PositionScheme::Absolute);
                //                imgContainer->setOffsets(0,Side::Top);
                slideCOntainer->addWidget(std::move(imgContainer));

                container->addWidget(std::move(slideCOntainer));

                //                auto StackWidget = container->addWidget(cpp14::make_unique<WStackedWidget>());
                //                StackWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::EaseInOut,250),true);
                //                std::vector<std::string> imgPathList;
                //                for( auto id : doc[SBLDKeys::Calismalar::Asama::Gorseller].get_array().value )
                //                {
                //                    std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,id.get_oid().value.to_string());
                //                    imgPathList.push_back(imgPath);
                //                }

                //                for( auto str : imgPathList )
                //                {




                //                }


                //                auto btnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                //                btnContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));


                //                auto hLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

                //                auto backBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
                //                backBtn->clicked().connect([=](){
                //                    int indexCount = StackWidget->count();
                //                    int CurrentIndex = StackWidget->currentIndex();

                //                    if( CurrentIndex > 0 )
                //                    {
                //                        StackWidget->setCurrentIndex(--CurrentIndex );
                //                    }else{
                //                        StackWidget->setCurrentIndex( indexCount-1 );
                //                    }
                //                });

                //                hLayout->addStretch(1);
                //                int imgCounter = 0;

                //                for( auto str : imgPathList )
                //                {
                //                    imgCounter++;
                //                }

                //                {
                //                    auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Resim").arg(imgCounter)),0,AlignmentFlag::Center);
                //                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s24px);
                //                }
                //                hLayout->addStretch(1);



                //                auto nextBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));

                //                nextBtn->clicked().connect([=](){
                //                    int indexCount = StackWidget->count();
                //                    int CurrentIndex = StackWidget->currentIndex();

                //                    if( CurrentIndex < indexCount-1 )
                //                    {
                //                        StackWidget->setCurrentIndex(++CurrentIndex );
                //                    }else{
                //                        StackWidget->setCurrentIndex( 0 );
                //                    }
                //                });

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setPadding(15,Side::Bottom|Side::Top);
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.75));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::slideItem::text].get_utf8().value.to_string().c_str())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s16px);
            }
        } catch (bsoncxx::exception &e) {

        }


    }



}

void Giris::Personel::ProjeWidget::setProjeList(bsoncxx::document::view view, int32_t year)
{

    mMainContainer->clear();



    std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";

    auto fluidContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fluidContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
    fluidContainer->setMargin(25,Side::Top);
    fluidContainer->setPadding(25,Side::Top|Side::Bottom);


    auto rContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(50,100),this->getRandom(50,100),this->getRandom(50,100),0.85));


    // Mahalle Adı
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setMargin(WLength::Auto,Side::Left|Side::Right);
        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Projeler::mahalle].get_utf8().value.to_string().c_str()));
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s18px+Style::font::weight::bold);
    }

    if( year == 0 )
    {
        // Tüm Zamanlar
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(WLength::Auto,Side::Left|Side::Right);
            auto text = container->addWidget(cpp14::make_unique<WText>("Tüm Projeler"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s18px+Style::font::weight::bold);
        }
    }else{
        // YIL
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(WLength::Auto,Side::Left|Side::Right);
            auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1} Yılına Ait Projeler").arg(year)));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s18px+Style::font::weight::bold);
        }
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setMargin(0,Side::Left|Side::Right);

        auto _fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto _rContainer = _fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _rContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Yıl"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Proje Adı"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Birim"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Durum"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Tarih"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
    }


    try {
        auto cursor = this->db()->collection(SBLDKeys::Projeler::collection).find(view);

        for( auto doc : cursor )
        {

            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(0,Side::Left|Side::Right);
            container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid white"));
            container->setPadding(10,Side::Top|Side::Bottom);

            auto _fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

            auto _rContainer = _fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer->addStyleClass(Bootstrap::Grid::row);

            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::year].get_int32().value)));
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            }
            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Projeler::title].get_utf8().value.to_string().c_str()));
                text->addStyleClass(Bootstrap::Label::Primary);
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                text->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string().c_str());
                text->decorationStyle().setCursor(Cursor::PointingHand);
                text->clicked().connect([=](){
                    std::string oid = text->attributeValue(Style::dataoid).toUTF8();
                    this->setProjeDetail(oid);
                });
            }
            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Projeler::birim].get_utf8().value.to_string().c_str()));
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            }
            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Projeler::durum].get_utf8().value.to_string().c_str()));
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            }
            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(doc[SBLDKeys::Projeler::julianDate].get_int64().value).toString("dd MMMM yyyy").toStdString().c_str()));
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            }
        }
    } catch (mongocxx::exception &e) {

    }
}

void Giris::Personel::ProjeWidget::setList( std::string projectDurum )
{

    mMainContainer->clear();

    auto fluidContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);


    auto rContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    auto mahList = this->getMahalleList();

    for( auto mah : mahList )
    {
        auto filter = document{};
        if( projectDurum != "Toplam" )
        {
            try {
                filter.append(kvp(SBLDKeys::Projeler::durum,projectDurum));
            } catch (bsoncxx::exception &e) {

            }
        }

        try {
            filter.append(kvp(SBLDKeys::Projeler::mahalle,mah.mahalle));
        } catch (bsoncxx::exception) {

        }

        this->addList(rContainer,filter.view());

    }

}

void Giris::Personel::ProjeWidget::addList(WContainerWidget *mrContainer, bsoncxx::document::view filter)
{

    auto projeCount = this->count(SBLDKeys::Projeler::collection,filter);

    if( !projeCount ) return;


    std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";


    auto _rCOntainer = mrContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _rCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto fluidContainer = _rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());


    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fluidContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
    fluidContainer->setMargin(25,Side::Top);
    fluidContainer->setPadding(25,Side::Top|Side::Bottom);


    auto rContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(50,100),this->getRandom(50,100),this->getRandom(50,100),0.85));


    // Mahalle Adı
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setMargin(WLength::Auto,Side::Left|Side::Right);
        auto text = container->addWidget(cpp14::make_unique<WText>(filter[SBLDKeys::Projeler::mahalle].get_utf8().value.to_string().c_str()));
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s18px+Style::font::weight::bold);
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setMargin(0,Side::Left|Side::Right);
        container->setPadding(20,Side::Top);


        auto _fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto _rContainer = _fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _rContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Yıl"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Proje Adı"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Birim"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
        {
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>("Durum"));
            text->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        }
    }


    try {
        auto cursor = this->db()->collection(SBLDKeys::Projeler::collection).find(filter);

        for( auto doc : cursor )
        {

            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(0,Side::Left|Side::Right);
            container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid white"));
            container->setPadding(20,Side::Top|Side::Bottom);

            auto _fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

            auto _rContainer = _fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer->addStyleClass(Bootstrap::Grid::row);

            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::year].get_int32().value)));
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            }
            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Projeler::title].get_utf8().value.to_string().c_str()));
                //                text->addStyleClass(Bootstrap::Label::Primary);
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

            }
            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Projeler::birim].get_utf8().value.to_string().c_str()));
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            }
            {
                auto text = _rContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Projeler::durum].get_utf8().value.to_string().c_str()));
                text->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            }

            _fluidContainer->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string().c_str());
            _fluidContainer->decorationStyle().setCursor(Cursor::PointingHand);
            _fluidContainer->clicked().connect([=](){
                std::string oid = _fluidContainer->attributeValue(Style::dataoid).toUTF8();
                this->setProjeDetail(oid);
            });
        }
    } catch (mongocxx::exception &e) {

    }



}

Giris::Personel::CalismaWidget::CalismaWidget(mongocxx::database *_database, bsoncxx::document::value _user)
    :ContainerWidget::ContainerWidget(_database,_user,"Çalışmalar")
{

    this->TotalState();

    this->ZamanDongusu();

    this->initMenu();

    mMainContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());

    mMainContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

}

void Giris::Personel::CalismaWidget::TotalState()
{

    // Tüm Çalışmalar
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(10,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Tüm Çalışmalar"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(document{}.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};
            this->listCalismalar(filter.view(),"Tüm Çalışmalar");
        });
    }


    // Tamamlanan Çalışmalar
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(10,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Tamamlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
            } catch (bsoncxx::exception &e) {

            }

            this->listCalismalar(filter.view(),"Tamamlanan");
        });
    }


    // Devam Eden Çalışmalar
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(10,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Devam Eden"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
            } catch (bsoncxx::exception &e) {

            }

            this->listCalismalar(filter.view(),"Devam Eden");
        });
    }

}

void Giris::Personel::CalismaWidget::ZamanDongusu()
{

    auto __container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    __container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    //    __container->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));

    auto fluidContainer = __container->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fluidContainer->setMargin(10,Side::Top|Side::Bottom);
    fluidContainer->setMargin(0,Side::Left|Side::Right);

    auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);


    // Bugün Başlanan Çalışmalar
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(120);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Bugün Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,QDate::currentDate().toJulianDay()));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,QDate::currentDate().toJulianDay()));
            } catch (bsoncxx::exception &e) {

            }

            this->listCalismalar(filter.view(),"Bugün Başlanan Çalışmalar");

        });

    }

    // Bu Hafta
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(120);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Bu Hafta Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-7))));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-7))));
            } catch (bsoncxx::exception &e) {

            }

            this->listCalismalar(filter.view(),"Bu Haftaki Çalışmalar");
        });
    }

    // Bu Ay Son 30 Gün
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(120);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son Bir Ayda Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-30))));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-30))));
            } catch (bsoncxx::exception &e) {

            }

            this->listCalismalar(filter.view(),"Son Bir Aylık Çalışmalar");
        });
    }

    // Bu Ay Son 90 Gün
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(120);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son 6 Ayda Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-180))));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-180))));
            } catch (bsoncxx::exception &e) {

            }

            this->listCalismalar(filter.view(),"Son 6 Aylık Çalışmalar");
        });
    }
}

void Giris::Personel::CalismaWidget::initMenu()
{


    auto __container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    __container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    //    __container->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));

    auto fluidContainer = __container->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fluidContainer->setMargin(10,Side::Top|Side::Bottom);
    fluidContainer->setMargin(0,Side::Left|Side::Right);

    auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);


    // Zaman Tüneli
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setMargin(0,AllSides);
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,25),this->getRandom(0,50),this->getRandom(0,75),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Zaman Tüneli"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->clicked().connect(this,&CalismaWidget::zamanTuneli);
        container->decorationStyle().setCursor(Cursor::PointingHand);
    }

    // Birim/Birim
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setMargin(0,AllSides);
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,25),this->getRandom(0,50),this->getRandom(0,75),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Birim/Birim"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->clicked().connect(this,&CalismaWidget::birimDongusu);
        container->decorationStyle().setCursor(Cursor::PointingHand);
    }

    // Mahalle/Mahalle
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setMargin(0,AllSides);
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,25),this->getRandom(0,50),this->getRandom(0,75),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Mahalle/Mahalle"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->clicked().connect(this,&CalismaWidget::mahalleMahalle);
        container->decorationStyle().setCursor(Cursor::PointingHand);
    }

    // Raporlar
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,8))+".jpg";
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setMargin(0,AllSides);
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,25),this->getRandom(0,50),this->getRandom(0,75),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Raporlar"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->clicked().connect(this,&CalismaWidget::setReportMenu);
        container->decorationStyle().setCursor(Cursor::PointingHand);
    }






}

void Giris::Personel::CalismaWidget::birimDongusu()
{
    mMainContainer->clear();
    auto birimList = this->getBirimList();

    auto _fluidCOntainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _fluidCOntainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto _rowContainer = _fluidCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _rowContainer->addStyleClass(Bootstrap::Grid::row);

    skip = 0;
    this->birimDongusuAdd(_rowContainer,birimList);

}

void Giris::Personel::CalismaWidget::birimDongusuAdd(WContainerWidget *_rowContainer, const std::vector<Giris::Personel::CalismaWidget::BirimItem> birimList)
{

    int Total = skip+5;
    if( skip < birimList.size() && skip+5 >= birimList.size() )
    {
        Total = birimList.size();
    }

    if( skip >= birimList.size() ) return;


    for( auto i = skip ; i < Total ; i++ )
    {

        auto birim = birimList.at(i);

        auto __container = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        __container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto fluidContainer = __container->addWidget(cpp14::make_unique<WContainerWidget>());
        fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);
        fluidContainer->setMargin(10,Side::Top|Side::Bottom);
        fluidContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));

        auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rowContainer->addStyleClass(Bootstrap::Grid::row);


        // TITLE BIRIM
        {
            auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setPadding(WLength::Auto,Side::Left|Side::Right);

            auto text = container->addWidget(cpp14::make_unique<WText>(birim.birim));
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        }


        // CHART
        {

            auto _chartContainer = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _chartContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_9+Bootstrap::Grid::Medium::col_md_9+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

            auto _chart = _chartContainer->addWidget(cpp14::make_unique<BChart::DonutChart>());
            _chart->setMargin(WLength::Auto,Side::Left|Side::Right);

            std::vector<BChart::DonutChart::item> itemList;

            // Tamamlanan GRAFIK
            {
                BChart::DonutChart::item item;
                item.label = "Tamamlanan";
                item.rgb = QString("rgb(%1,%2,%3)").arg(this->getRandom()).arg(this->getRandom()).arg(this->getRandom()).toStdString();
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
                } catch (bsoncxx::exception &e) {

                }
                item.value = this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view());
                item.label = QString("Tamamlanan").toStdString();

                itemList.push_back(item);
            }

            // DEVAM EDEN GRAFIK
            {
                BChart::DonutChart::item item;
                item.rgb = QString("rgb(%1,%2,%3)").arg(this->getRandom()).arg(this->getRandom()).arg(this->getRandom()).toStdString();
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
                } catch (bsoncxx::exception &e) {

                }
                item.value = this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view());;
                item.label = QString("Devam Eden").toStdString();

                itemList.push_back(item);
            }

            _chart->setDataList(itemList);
        }



        {
            auto _container_ = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container_->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            _container_->setPadding(WLength::Auto,Side::Left|Side::Right);

            auto _fluidContainer_ = _container_->addWidget(cpp14::make_unique<WContainerWidget>());
            _fluidContainer_->addStyleClass(Bootstrap::Grid::container_fluid);

            auto _rContainer_ = _fluidContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer_->addStyleClass(Bootstrap::Grid::row);

            // Bugün Başlanan Çalışmalar
            {
                auto container = _rContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120)));
                container->setPadding(5,AllSides);
                auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                borderContainer->setHeight(75);
                borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
                borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
                auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Bugün Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,QDate::currentDate().toJulianDay()));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                    container->decorationStyle().setCursor(Cursor::PointingHand);
                    container->clicked().connect([=](){
                        auto filter = document{};
                        try {
                            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,QDate::currentDate().toJulianDay()));
                        } catch (bsoncxx::exception &e) {

                        }
                        try {
                            filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                        } catch (bsoncxx::exception &e) {

                        }
                        this->listCalismalar(filter.view(),birim.birim);
                    });
                }
            }

            // Bu Hafta
            {
                auto container = _rContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120)));
                container->setPadding(5,AllSides);
                auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                borderContainer->setHeight(75);
                borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
                borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
                auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Bu Hafta Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-7))));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                container->decorationStyle().setCursor(Cursor::PointingHand);
                container->clicked().connect([=](){
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-7))));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    this->listCalismalar(filter.view(),birim.birim);
                });
            }


            // Devam Eden
            {
                auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120)));
                container->setPadding(5,AllSides);
                auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                borderContainer->setHeight(75);
                borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
                borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
                auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Toplam Devam Eden"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                container->decorationStyle().setCursor(Cursor::PointingHand);
                container->clicked().connect([=](){
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    this->listCalismalar(filter.view(),birim.birim);
                });
            }

            // Bu Ay Son 30 Gün
            {
                auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120)));
                container->setPadding(5,AllSides);
                auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                borderContainer->setHeight(75);
                borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
                borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
                auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son Bir Ayda Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-30))));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                container->decorationStyle().setCursor(Cursor::PointingHand);
                container->clicked().connect([=](){
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-30))));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    this->listCalismalar(filter.view(),birim.birim);
                });
            }

            // Bu Ay Son 90 Gün
            {
                auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120)));
                container->setPadding(5,AllSides);
                auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                borderContainer->setHeight(75);
                borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
                borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
                auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son 6 Ayda Başlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-90))));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                container->decorationStyle().setCursor(Cursor::PointingHand);
                container->clicked().connect([=](){
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-90))));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    this->listCalismalar(filter.view(),birim.birim);
                });
            }

            // Devam Eden
            {
                auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120)));
                container->setPadding(5,AllSides);
                auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                borderContainer->setHeight(75);
                borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
                borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
                auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>("Toplam Tamamlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                {
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
                }
                container->decorationStyle().setCursor(Cursor::PointingHand);
                container->clicked().connect([=](){
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,birim.birim));
                    } catch (bsoncxx::exception &e) {

                    }
                    this->listCalismalar(filter.view(),birim.birim);
                });
            }
        }
    }

    {

        auto container = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));

        btn->setMargin(WLength::Auto,Side::Left|Side::Right);
        btn->addStyleClass(Bootstrap::Button::Primary);

        btn->clicked().connect([=](){
            skip += 5;
            this->birimDongusuAdd(_rowContainer,birimList);
            _rowContainer->removeWidget(container);
        });

    }
}

std::vector<Giris::Personel::CalismaWidget::BirimItem> Giris::Personel::CalismaWidget::getBirimList() const
{
    auto _filter = document{};

    try {
        _filter.append(kvp(SBLDKeys::Birimler::haberlesmekodu,make_document(kvp("$ne","0"))));
    } catch (bsoncxx::exception &e) {

    }

    std::vector<BirimItem> BirimList;
    try {
        auto cursor = this->db()->collection(SBLDKeys::Birimler::collection).find(_filter.view());
        for( auto doc : cursor )
        {
            BirimItem item;
            try {
                item.birim = doc[SBLDKeys::Birimler::birim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.birim = "null";
            }
            try {
                item.oid = doc[SBLDKeys::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.oid = "null";
            }
            if( item.birim != "NULL" )
            {
                BirimList.push_back(item);
            }

        }
    } catch (mongocxx::exception &e) {

    }


    return BirimList;
}

std::vector<Giris::Personel::CalismaWidget::MahalleItem> Giris::Personel::CalismaWidget::getMahalleList() const
{
    std::vector<MahalleItem> MahalleList;

    MahalleList.clear();
    try {
        auto cursor = this->db()->collection(SBLDKeys::Mahalle::collection).find(document{}.view());
        for( auto doc : cursor )
        {
            MahalleItem item;
            try {
                item.mahalle = doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.mahalle = e.what();
            }
            try {
                item.oid = doc[SBLDKeys::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.oid = e.what();
            }
            if( item.mahalle != "NULL" )
            {
                MahalleList.push_back(item);
            }

        }
    } catch (mongocxx::exception &e) {

    }

    return MahalleList;
}

void Giris::Personel::CalismaWidget::zamanTuneli()
{

    mMainContainer->clear();

    auto _fluidCOntainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _fluidCOntainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto _rowContainer = _fluidCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _rowContainer->addStyleClass(Bootstrap::Grid::row);
    _rowContainer->setMargin(25,Side::Bottom|Side::Top);


    {
        auto container = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setMargin(WLength::Auto,Side::Left|Side::Right);
        container->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
        auto text = container->addWidget(cpp14::make_unique<WText>("Bugün: " + QDate::currentDate().toString("dddd dd/MMMM/yyyy").toStdString()));
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s20px+Style::background::color::rgb(0,77,153));
        text->setPadding(15,AllSides);
        text->setMargin(0,Side::Top);
        container->setPadding(0,Side::Top);
        container->setMargin(0,Side::Top);
        container->setZIndex(11);
        container->setAttributeValue(Style::style,Style::background::color::rgb(0,77,153));

    }


    auto _LeftContainer = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _LeftContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    _LeftContainer->setPadding(3,Side::Left);
    auto LeftFluidContainer = _LeftContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    LeftFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto LeftRowContainer = LeftFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    LeftRowContainer->addStyleClass(Bootstrap::Grid::row);
    LeftRowContainer->setAttributeValue(Style::customData,"left");

    // Time Line
    {
        auto TimeLineContainer = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        TimeLineContainer->setAttributeValue(Style::style,Style::Border::border("1px solid #004d99"));
        TimeLineContainer->setMargin(0,Side::Top|Side::Bottom);
        TimeLineContainer->setPositionScheme(PositionScheme::Absolute);
        TimeLineContainer->setOffsets(25,Side::Top|Side::Bottom);
        TimeLineContainer->setOffsets(WLength("50%"),Side::Left);
        TimeLineContainer->setZIndex(10);
    }


    // Right Side Wall

    auto _RightContainer = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _RightContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    _RightContainer->setPadding(3,Side::Right);
    auto RightFluidContainer = _RightContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    RightFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto RightRowContainer = RightFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    RightRowContainer->addStyleClass(Bootstrap::Grid::row);
    RightRowContainer->setAttributeValue(Style::customData,"right");

    skip = 0;

    this->addZamanTuneli(_rowContainer,LeftRowContainer,RightRowContainer);


}

void Giris::Personel::CalismaWidget::addZamanTuneli(WContainerWidget *_rowContainer, WContainerWidget* LeftRowContainer, WContainerWidget*  RightRowContainer )
{

    mongocxx::options::find findOptions;

    findOptions.skip(skip);

    findOptions.limit(10);

    auto sortDoc = document{};

    //    try {
    //        sortDoc.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,-1));
    //    } catch (bsoncxx::exception &e) {

    //    }


    try {
        sortDoc.append(kvp(SBLDKeys::Calismalar::updateDate,-1));
    } catch (bsoncxx::exception &e) {

    }

    findOptions.sort(sortDoc.view());


    LeftSide = true;

    try {
        auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).find(document{}.view(),findOptions);


        for( auto doc : cursor )
        {
            if( LeftSide )
            {
                this->addCalismaItemToZamanTuneli(LeftRowContainer,doc);
            }else{
                this->addCalismaItemToZamanTuneli(RightRowContainer,doc);
            }
            LeftSide = !LeftSide;
        }


    } catch (mongocxx::exception &e) {

    }


    {
        auto container = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setZIndex(11);

        auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));
        btn->addStyleClass(Bootstrap::Button::Primary);
        btn->setMargin(WLength::Auto,Side::Left|Side::Right);
        btn->clicked().connect([=](){
            skip += 10;
            _rowContainer->removeWidget(container);
            this->addZamanTuneli(_rowContainer ,LeftRowContainer,RightRowContainer);
        });
    }

}

void Giris::Personel::CalismaWidget::addCalismaItemToZamanTuneli(WContainerWidget *widget, bsoncxx::document::view doc)
{
    auto bucket = this->db()->gridfs_bucket();
    auto container = widget->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    container->setMargin(25,Side::Bottom|Side::Top);
    container->setPadding(15,Side::Top|Side::Bottom);
    container->decorationStyle().setCursor(Cursor::PointingHand);

    auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    auto imgContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto imgPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Calismalar::icon].get_oid().value.to_string());
    imgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
    imgContainer->setHeight(this->getRandom(180,250));
    imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    if( LeftSide )
    {
        container->setAttributeValue(Style::style,Style::background::color::color("#c2c2d6; ")+Style::Border::borderRardius("15","0","15","15"));
    }else{
        container->setAttributeValue(Style::style,Style::background::color::color("#c2c2d6; ")+Style::Border::borderRardius("0","15","15","15"));
    }
    {
        auto textContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto text = textContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Calismalar::baslik].get_utf8().value.to_string().c_str()));
        text->setAttributeValue(Style::style,Style::font::weight::bold);
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
    }


    {
        auto mahList = doc[SBLDKeys::Calismalar::mahalleler].get_array().value;
        for( auto mah : mahList )
        {
            auto textContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            QString str = mah.get_utf8().value.to_string().c_str();
            str = str.remove(str.count()-9,9);
            auto text = textContainer->addWidget(cpp14::make_unique<WText>(str.toStdString().c_str()));
            text->addStyleClass(Bootstrap::Label::Warning);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
            text->setMargin(WLength::Auto,Side::Left|Side::Right);
        }
    }

    container->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string());

    container->clicked().connect([=](){
        this->setCalismaDetail(container->attributeValue(Style::dataoid).toUTF8());
    });



    {
        auto dateContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        dateContainer->setPositionScheme(PositionScheme::Absolute);
        dateContainer->setHeight(25);
        dateContainer->setPadding(0,AllSides);

        {
            auto _fluidContainer = dateContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fluidContainer->setPadding(0,AllSides);
            _fluidContainer->setMargin(0,AllSides);
            auto hLayout = _fluidContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
            hLayout->setContentsMargins(0,0,0,0);


            if( LeftSide )
            {
                auto _colorContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                //                _colorContainer->setAttributeValue(Style::style,Style::background::color::rgba(0,77,153,0.85));
                //                _colorContainer->setHeight(25);
                _colorContainer->setMargin(0,Side::Right);


                auto text = _colorContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(QDate::fromJulianDay(doc[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dddd dd/MMMM/yyyy").toStdString())));
                text->addStyleClass(Bootstrap::Label::Primary);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::lighter+Style::font::size::s14px);
                text->addStyleClass("ZamanTuneliFont");


                auto _imgContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                _imgContainer->setAttributeValue(Style::style,Style::background::url("img/leftConnect.png")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
                _imgContainer->setHeight(20);
                _imgContainer->setWidth(30);
                _imgContainer->setMargin(0,Side::Left);


            }else{
                auto _imgContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                _imgContainer->setAttributeValue(Style::style,Style::background::url("img/rightConnect.png")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
                _imgContainer->setHeight(20);
                _imgContainer->setWidth(30);
                _imgContainer->setMargin(0,Side::Right);

                auto _colorContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                //                _colorContainer->setAttributeValue(Style::style,Style::background::color::rgba(0,77,153,0.85));
                //                _colorContainer->setHeight(25);
                _colorContainer->setMargin(0,Side::Left);

                auto text = _colorContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(QDate::fromJulianDay(doc[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dddd dd/MMMM/yyyy").toStdString())));
                text->addStyleClass(Bootstrap::Label::Primary);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::lighter+Style::font::size::s14px);
                text->addStyleClass("ZamanTuneliFont");
            }

        }

        dateContainer->setOffsets(-10,Side::Top);

        if( LeftSide )
        {
            dateContainer->setOffsets(-5,Side::Right);
        }else{
            dateContainer->setOffsets(-5,Side::Left);
        }
    }

    {
        auto dateContainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        dateContainer->setPositionScheme(PositionScheme::Absolute);
        dateContainer->setHeight(25);
        dateContainer->setPadding(0,AllSides);

        {
            auto _fluidContainer = dateContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fluidContainer->setPadding(0,AllSides);
            _fluidContainer->setMargin(0,AllSides);
            auto hLayout = _fluidContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
            hLayout->setContentsMargins(0,0,0,0);

            auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Calismalar::birim].get_utf8().value.to_string())));
            text->addStyleClass(Bootstrap::Label::Success);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s14px);
            text->addStyleClass("ZamanTuneliFont");
        }

        dateContainer->setOffsets(WLength("85%"),Side::Top);

        if( LeftSide )
        {
            dateContainer->setOffsets(-15,Side::Left);
        }else{
            dateContainer->setOffsets(-15,Side::Right);
        }
    }

    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void Giris::Personel::CalismaWidget::setCalismaDetail(std::string oid)
{


    mMainContainer->clear();


    //    mTableContainer->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(oid.c_str())));

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->db()->collection(SBLDKeys::Calismalar::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Calismalar::baslik].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(WDate::fromJulianDay(view[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MM/yyyy"))),0,AlignmentFlag::Middle);
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Calismalar::durum].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Kategori {1}").arg(view[SBLDKeys::Calismalar::kategori].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Calismalar::birim].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tanım: <u>{1}</u>").arg(view[SBLDKeys::Calismalar::tanim].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }



    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        container->setPadding(10,AllSides);
        for( auto mah : view[SBLDKeys::Calismalar::mahalleler].get_array().value )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mah.get_utf8().value.to_string().c_str())));
            text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
            //            text->setMargin(5,AllSides);
        }
    }


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto bucket = this->db()->gridfs_bucket();
        std::string iconPaht = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Calismalar::icon].get_oid().value.to_string());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }
    {


        auto asamaContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto bucket = this->db()->gridfs_bucket();
        try {
            auto asamaList = view[SBLDKeys::Calismalar::asamalar].get_array().value;

            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                //                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto StackWidget = container->addWidget(cpp14::make_unique<WStackedWidget>());
                StackWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::EaseInOut,250),true);
                std::vector<std::string> imgPathList;
                for( auto id : doc[SBLDKeys::Calismalar::Asama::Gorseller].get_array().value )
                {
                    std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,id.get_oid().value.to_string());
                    imgPathList.push_back(imgPath);
                }

                for( auto str : imgPathList )
                {
                    auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                    slideCOntainer->setHeight(350);



                    auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    frontimgContainer->setHeight(350);
                    frontimgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                    auto imgContainer = cpp14::make_unique<WContainerWidget>();
                    imgContainer->setHeight(350);
                    imgContainer->setWidth(WLength("100%"));
                    imgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+"z-index:-1; ");
                    imgContainer->addStyleClass("CommanFilter");
                    imgContainer->setPositionScheme(PositionScheme::Absolute);
                    imgContainer->setOffsets(0,Side::Top);
                    slideCOntainer->addWidget(std::move(imgContainer));

                    StackWidget->addWidget(std::move(slideCOntainer));

                }


                auto btnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                btnContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));


                auto hLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

                auto backBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
                backBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex > 0 )
                    {
                        StackWidget->setCurrentIndex(--CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( indexCount-1 );
                    }
                });

                hLayout->addStretch(1);
                int imgCounter = 0;

                for( auto str : imgPathList )
                {
                    imgCounter++;
                }

                {
                    auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Resim").arg(imgCounter)),0,AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s24px);
                }
                hLayout->addStretch(1);



                auto nextBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));

                nextBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex < indexCount-1 )
                    {
                        StackWidget->setCurrentIndex(++CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( 0 );
                    }
                });

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Calismalar::Asama::Aciklama].get_utf8().value.to_string().c_str())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
                TextCOntainer->setPadding(10,Side::Top|Side::Bottom);
            }
        } catch (bsoncxx::exception &e) {

        }


    }




}

void Giris::Personel::CalismaWidget::mahalleMahalle()
{

    mMainContainer->clear();

    auto fluidContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);




    auto mahList = this->getMahalleList();


    skip = 0;
    this->mahalleMahalleAdd(rowContainer,mahList);




}

void Giris::Personel::CalismaWidget::mahalleMahalleAdd(WContainerWidget *rowContainer, const std::vector<MahalleItem> mahList)
{


    int Total = skip+5;
    if( skip < mahList.size() && skip+5 >= mahList.size() )
    {
        Total = mahList.size();
    }

    if( skip >= mahList.size() ) return;

    for( auto i = skip ; i < Total ; i++ )
    {

        auto mah = mahList.at(i);

        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
        fContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,55),this->getRandom(0,55),this->getRandom(0,55))+Style::Border::bottom::border("1px solid white"));
        auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);
        rContainer->setMargin(15,Side::Top|Side::Bottom);

        {

            auto mahContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            mahContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_5+Bootstrap::Grid::Medium::col_md_5+Bootstrap::Grid::Small::col_sm_5+Bootstrap::Grid::ExtraSmall::col_xs_12);

            auto _fContainer = mahContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

            auto _rContainer = _fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer->addStyleClass(Bootstrap::Grid::row);

            QString str = mah.mahalle.c_str();
            str = str.remove(str.count()-9,9);
            auto text = _rContainer->addWidget(cpp14::make_unique<WText>(str.toStdString()));
            text->setMargin(WLength::Auto,Side::Left|Side::Right);
            text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);



            auto _chart = _rContainer->addWidget(cpp14::make_unique<BChart::DonutChart>());
            _chart->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

            std::vector<BChart::DonutChart::item> itemList;

            // Tamamlanan GRAFIK
            {
                BChart::DonutChart::item item;
                item.label = "Tamamlanan";
                item.rgb = QString("rgb(%1,%2,%3)").arg(this->getRandom()).arg(this->getRandom()).arg(this->getRandom()).toStdString();
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::mahalleler,mah.mahalle));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
                } catch (bsoncxx::exception &e) {

                }
                item.value = this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view());
                item.label = QString("Tamamlanan: %1").arg(item.value).toStdString();

                itemList.push_back(item);
            }

            // DEVAM EDEN GRAFIK
            {
                BChart::DonutChart::item item;
                item.rgb = QString("rgb(%1,%2,%3)").arg(this->getRandom()).arg(this->getRandom()).arg(this->getRandom()).toStdString();
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::mahalleler,mah.mahalle));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
                } catch (bsoncxx::exception &e) {

                }
                item.value = this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view());
                item.label = QString("Devam Eden: %1").arg(item.value).toStdString();

                itemList.push_back(item);
            }

            _chart->setDataList(itemList);

            {
                auto devamedenContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                devamedenContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);

                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::mahalleler,mah.mahalle));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
                } catch (bsoncxx::exception &e) {

                }


                auto text = devamedenContainer->addWidget(cpp14::make_unique<WText>(WString("Devam Eden: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                devamedenContainer->decorationStyle().setCursor(Cursor::PointingHand);
                devamedenContainer->clicked().connect([=](){

                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::mahalleler,mah.mahalle));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
                    } catch (bsoncxx::exception &e) {

                    }

                    this->listCalismalar(filter.view(),mah.mahalle+ " - " + SBLDKeys::Calismalar::DURUM::devamediyor);

                });
            }

            {
                auto tamamlananContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                tamamlananContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);

                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::mahalleler,mah.mahalle));
                } catch (bsoncxx::exception &e) {

                }
                try {
                    filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
                } catch (bsoncxx::exception &e) {

                }


                auto text = tamamlananContainer->addWidget(cpp14::make_unique<WText>(WString("Tamamlanan: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
                text->addStyleClass(Bootstrap::Label::Success);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                tamamlananContainer->decorationStyle().setCursor(Cursor::PointingHand);
                tamamlananContainer->clicked().connect([=](){

                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::mahalleler,mah.mahalle));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
                    } catch (bsoncxx::exception &e) {

                    }

                    this->listCalismalar(filter.view(),mah.mahalle+ " - " + SBLDKeys::Calismalar::DURUM::tamamlandi);

                });
            }

        }


        {
            auto tableContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            tableContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_7+Bootstrap::Grid::Medium::col_md_7+Bootstrap::Grid::Small::col_sm_7+Bootstrap::Grid::ExtraSmall::col_xs_12);

            auto _fContainer = tableContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

            auto _rContainer = _fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _rContainer->addStyleClass(Bootstrap::Grid::row);

            //YIL
            {
                auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                auto text = yilcontainer->addWidget(cpp14::make_unique<WText>("Yıl"));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
            }

            //Toplam
            {
                auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                auto text = yilcontainer->addWidget(cpp14::make_unique<WText>("Toplam"));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
            }

            //Tamamlanan
            {
                auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                auto text = yilcontainer->addWidget(cpp14::make_unique<WText>("Tamamlanan"));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
            }

            //Devam Eden
            {
                auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                auto text = yilcontainer->addWidget(cpp14::make_unique<WText>("Devam Eden"));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
            }

            for( int i = 2019 ; i >=2014 ; i-- )
            {

                //YIL
                {
                    auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                    auto text = yilcontainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(i)));
                    text->setMargin(WLength::Auto,Side::Left|Side::Right);
                    text->addStyleClass(Bootstrap::Label::Primary);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);

                }

                //Toplam
                {
                    auto filter = document{};

                    auto yilDoc = document{};

                    try {
                        yilDoc.append(kvp("$gte",QDate::fromString(QString("0101%1").arg(i),"ddMMyyyy").toJulianDay()));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        yilDoc.append(kvp("$lte",QDate::fromString(QString("0101%1").arg(i+1),"ddMMyyyy").toJulianDay()));
                    } catch (bsoncxx::exception &e) {

                    }

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,yilDoc));
                    } catch (bsoncxx::exception &e) {

                    }

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::mahalleler,make_document(kvp("$elemMatch",make_document(kvp("$eq",mah.mahalle))))));
                    } catch (bsoncxx::exception &e) {

                    }

                    auto toplam = this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view());

                    auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                    auto text = yilcontainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(toplam)));
                    text->setMargin(WLength::Auto,Side::Left|Side::Right);
                    text->addStyleClass(Bootstrap::Label::Default);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
                }

                //Tamamlanan
                {
                    auto filter = document{};

                    auto yilDoc = document{};


                    try {
                        yilDoc.append(kvp("$gte",QDate::fromString(QString("0101%1").arg(i),"ddMMyyyy").toJulianDay()));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        yilDoc.append(kvp("$lte",QDate::fromString(QString("0101%1").arg(i+1),"ddMMyyyy").toJulianDay()));
                    } catch (bsoncxx::exception &e) {

                    }

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,yilDoc));
                    } catch (bsoncxx::exception &e) {

                    }

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::mahalleler,make_document(kvp("$elemMatch",make_document(kvp("$eq",mah.mahalle))))));
                    } catch (bsoncxx::exception &e) {

                    }

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto toplam = this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view());
                    auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                    auto text = yilcontainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(toplam)));
                    text->setMargin(WLength::Auto,Side::Left|Side::Right);
                    text->addStyleClass(Bootstrap::Label::Default);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
                }

                //Devam Eden
                {
                    auto filter = document{};

                    auto yilDoc = document{};


                    try {
                        yilDoc.append(kvp("$gte",QDate::fromString(QString("0101%1").arg(i),"ddMMyyyy").toJulianDay()));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        yilDoc.append(kvp("$lte",QDate::fromString(QString("0101%1").arg(i+1),"ddMMyyyy").toJulianDay()));
                    } catch (bsoncxx::exception &e) {

                    }

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,yilDoc));
                    } catch (bsoncxx::exception &e) {

                    }

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::mahalleler,make_document(kvp("$elemMatch",make_document(kvp("$eq",mah.mahalle))))));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
                    } catch (bsoncxx::exception &e) {

                    }
                    auto toplam = this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view());
                    auto yilcontainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    yilcontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
                    auto text = yilcontainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(toplam)));
                    text->setMargin(WLength::Auto,Side::Left|Side::Right);
                    text->addStyleClass(Bootstrap::Label::Default);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
                }
            }

        }



    }




    {

        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));

        btn->setMargin(WLength::Auto,Side::Left|Side::Right);
        btn->addStyleClass(Bootstrap::Button::Primary);

        btn->clicked().connect([=](){
            skip += 5;
            this->mahalleMahalleAdd(rowContainer,mahList);
            rowContainer->removeWidget(container);
        });

    }

}

void Giris::Personel::CalismaWidget::listCalismalar(bsoncxx::document::view filterView, std::string title)
{


    mMainContainer->clear();

    auto fluidContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);

    // Title
    {
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127)));
        auto text = container->addWidget(cpp14::make_unique<WText>(title));
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s20px+Style::color::color(Style::color::White::AliceBlue));
        container->setPadding(25,Side::Top|Side::Bottom);
    }

    {
        mongocxx::pipeline stage;

        stage.match(filterView).group(make_document(kvp(SBLDKeys::oid,"$"+SBLDKeys::Calismalar::birim),kvp("count",make_document(kvp("$sum",1)))));

        try {
            auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).aggregate(stage);



            for( auto doc : cursor )
            {
                auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
                container->setAttributeValue(Style::style,Style::Border::border("1px solid black")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,50),this->getRandom(0,30)));
                auto vlayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

                auto text = vlayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::oid].get_utf8().value.to_string()),0,AlignmentFlag::Center);
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

                {
                    auto text1 = vlayout->addWidget(cpp14::make_unique<WText>(WString("<b>{1}</b> - Adet Başlattı").arg(doc["count"].get_int32().value).toUTF8()),0,AlignmentFlag::Center);
                    text1->setMargin(WLength::Auto,Side::Left|Side::Right);
                    text1->setAttributeValue(Style::style,Style::font::weight::lighter+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
                }
                container->setPadding(10,Side::Top|Side::Bottom);
                container->setHeight(75);

                container->decorationStyle().setCursor(Cursor::PointingHand);

                container->clicked().connect([=](){
                    auto filter = document{};
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,text->text().toUTF8()));
                    } catch (bsoncxx::exception &e) {

                    }

                    for( auto it = filterView.begin() ; it != filterView.end() ; it++ )
                    {
                        try {
                            filter.append(kvp(it->key().to_string(),it->get_value()));
                        } catch (bsoncxx::exception &e) {

                        }
                    }

                    this->listCalismalar(filter.view(),text->text().toUTF8());
                });
            }

        } catch (mongocxx::exception &e) {

        }



    }




    skip = 0;

    this->addCalismalartoList(rowContainer,bsoncxx::document::value(filterView));



}

void Giris::Personel::CalismaWidget::addCalismalartoList(WContainerWidget *rowContainer, bsoncxx::document::value filterValue)
{

    mongocxx::options::find findOptions;

    findOptions.skip(skip);

    findOptions.limit(20);

    auto sortDoc = document{};

    //    try {
    //        sortDoc.append(kvp(SBLDKeys::oid,-1));
    //    } catch (bsoncxx::exception &e) {

    //    }


    try {
        sortDoc.append(kvp(SBLDKeys::Calismalar::updateDate,-1));
    } catch (bsoncxx::exception &e) {

    }

    findOptions.sort(sortDoc.view());


    auto projectView = document{};

    try {
        projectView.append(kvp(SBLDKeys::Calismalar::baslik,true));
    } catch (bsoncxx::exception &e) {

    }
    try {
        projectView.append(kvp(SBLDKeys::Calismalar::birim,true));
    } catch (bsoncxx::exception &e) {

    }
    try {
        projectView.append(kvp(SBLDKeys::Calismalar::icon,true));
    } catch (bsoncxx::exception &e) {

    }

    try {
        projectView.append(kvp(SBLDKeys::Calismalar::mahalleler,true));
    } catch (bsoncxx::exception &e) {

    }
    try {
        projectView.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,true));
    } catch (bsoncxx::exception &e) {

    }
    try {
        projectView.append(kvp(SBLDKeys::Calismalar::durum,true));
    } catch (bsoncxx::exception &e) {

    }

    findOptions.projection(projectView.view());

    auto bucket = this->db()->gridfs_bucket();
    try {
        auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).find(filterValue.view(),findOptions);


        int row = 1;
        for( auto doc : cursor )
        {
            auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(50,127),this->getRandom(75,127),this->getRandom(80,127),.7));

            auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            fContainer->setMargin(15,Side::Top|Side::Bottom);
            fContainer->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid white"));

            auto rCOntainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            rCOntainer->addStyleClass(Bootstrap::Grid::row);

            // icon
            {
                auto imgPaht = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Calismalar::icon].get_oid().value.to_string());
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);

                imgContainer->setAttributeValue(Style::style,Style::background::url(imgPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);

                auto countContainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                countContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                countContainer->setHeight(100);
                auto vLayout = countContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                vLayout->addStretch(1);
                auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(row+this->skip)));
                vLayout->addStretch(1);
                text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                row++;
            }

            //title
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                QString str;
                try {
                    str = doc[SBLDKeys::Calismalar::baslik].get_utf8().value.to_string().c_str();
                } catch (bsoncxx::exception &e) {
                    str = "Başlık Yok";
                }
                if( str.count() > 40 )
                {
                    str = str.mid(0,37);
                    str.append("...");
                }
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(str.toStdString()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }

            //Birim
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Calismalar::birim].get_utf8().value.to_string().c_str()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }
            //Mahalleler
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                auto vLayout = imgContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto arlist = doc[SBLDKeys::Calismalar::mahalleler].get_array().value;

                int count = 0;
                for( auto item : arlist )
                {
                    if( count < 3 )
                    {

                        QString str = item.get_utf8().value.to_string().c_str();
                        str = str.remove(str.count()-9,9);
                        auto text = vLayout->addWidget(cpp14::make_unique<WText>(str.toStdString()));
                        text->addStyleClass(Bootstrap::Label::Warning);
                        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    }
                    count++;
                }

                if( count > 3 )
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("+{1} Daha").arg(count-3)));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                }
                vLayout->addStretch(1);
            }

            //Tanım
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                QString str;
                try {
                    str = doc[SBLDKeys::Calismalar::durum].get_utf8().value.to_string().c_str();
                } catch (bsoncxx::exception &e) {
                    str = "Durum Yok";
                }

                if( str.count() > 40 )
                {
                    str = str.mid(0,37);
                    str.append("...");
                }
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(str.toStdString().c_str()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }

            //Tanım
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(doc[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MMMM/yyyy").toStdString()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->addStyleClass(Bootstrap::Label::Primary);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

                rCOntainer->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string().c_str());
                rCOntainer->decorationStyle().setCursor(Cursor::PointingHand);
                rCOntainer->clicked().connect([=](){
                    this->setCalismaDetail(rCOntainer->attributeValue(Style::dataoid).toUTF8());
                });
            }



        }

    } catch (mongocxx::exception &e) {

    }

    if( this->db()->collection(SBLDKeys::Calismalar::collection).count(filterValue.view()) > this->skip+20 ){





        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        {
            auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));
            btn->setMargin(WLength::Auto,Side::Left|Side::Right);
            btn->addStyleClass(Bootstrap::Button::Primary);

            btn->clicked().connect([=](){
                skip += 20;
                rowContainer->removeWidget(container);
                this->addCalismalartoList(rowContainer,filterValue);
            });
        }
    }


}

void Giris::Personel::CalismaWidget::setReportMenu()
{

    mMainContainer->clear();

    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContanier = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContanier->addStyleClass(Bootstrap::Grid::row);

    {
        auto container = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto text = container->addWidget(cpp14::make_unique<WText>("Dünün Raporu"));
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        container->setPadding(5,Side::Bottom|Side::Top);
        text->decorationStyle().setCursor(Cursor::PointingHand);
        text->clicked().connect([=](){

            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::updateDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-1}));
            } catch (bsoncxx::exception &e) {

            }

            this->reportlist(filter.view(),WDate::fromJulianDay(WDate::currentDate().toJulianDay()-1).toString("dd/MM/yyyy").toUTF8() + " Gününe Ait Çalışmalar");

        });
    }


    {
        auto container = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto text = container->addWidget(cpp14::make_unique<WText>("Bugünün Raporu"));
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        container->setPadding(5,Side::Bottom|Side::Top);
        text->decorationStyle().setCursor(Cursor::PointingHand);
        text->clicked().connect([=](){

            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::updateDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
            } catch (bsoncxx::exception &e) {

            }

            this->reportlist(filter.view(),WDate::currentDate().toString("dd/MM/yyyy").toUTF8() + " Gününe Ait Çalışmalar");

        });
    }

    {
        auto container = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto text = container->addWidget(cpp14::make_unique<WText>("Son Bir Haftanın Raporu"));
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        container->setPadding(5,Side::Bottom|Side::Top);
        text->decorationStyle().setCursor(Cursor::PointingHand);
        text->clicked().connect([=](){

            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::updateDate,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-7}))));
            } catch (bsoncxx::exception &e) {

            }

            this->reportlist(filter.view(),"Son 7 Günün Çalışmaları");

        });
    }

    for( int i = 1 ; i < 9 ; i++ )
    {
        {
            auto container = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            std::string tarih = WDate::fromJulianDay(WDate::currentDate().toJulianDay()-i).toString("dd/MM/yyyy").toUTF8();
            auto text = container->addWidget(cpp14::make_unique<WText>(tarih));
            text->addStyleClass(Bootstrap::Label::Primary);
            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
            text->setMargin(WLength::Auto,Side::Left|Side::Right);
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
            container->setPadding(5,Side::Bottom|Side::Top);
            text->decorationStyle().setCursor(Cursor::PointingHand);
            text->clicked().connect([=](){

                auto filter = document{};

                try {
                    filter.append(kvp(SBLDKeys::Calismalar::updateDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-i}));
                } catch (bsoncxx::exception &e) {

                }

                this->reportlist(filter.view(),tarih + " Gününe Ait Çalışmalar");
            });
        }
    }





}

void Giris::Personel::CalismaWidget::reportlist(bsoncxx::document::view filterView , std::string title )
{

    mMainContainer->clear();

    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContanier = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContanier->addStyleClass(Bootstrap::Grid::row);

    {
        auto LayContainer = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
        LayContainer->addStyleClass(Bootstrap::Grid::col_full_12);
        //        LayContainer->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));

        auto vLayout = LayContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

        {
            auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<h2>Günlük Çalışma Raporu</h2>"),0,AlignmentFlag::Center);
        }

        {
            auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<h4>"+title+"</h4>"),0,AlignmentFlag::Center);
        }
        {
            auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<h4>Birimlerin Girdiği Çalışma Sayıları</h4>"),0,AlignmentFlag::Center);
        }
    }


    {
        mongocxx::pipeline stage;


        stage.match(filterView)
                .group(make_document(kvp("_id","$"+SBLDKeys::Calismalar::birim),kvp("count",make_document(kvp("$sum",1)))));


        //        stage.match(make_document(kvp(SBLDKeys::Calismalar::updateDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()})))
        //                .group(make_document(kvp("_id","$"+SBLDKeys::Calismalar::birim),kvp("count",make_document(kvp("$sum",1)))));


        std::vector<bsoncxx::document::view> viewList;

        try {
            auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).aggregate(stage);

            for( auto doc : cursor )
            {
                viewList.push_back(doc);
            }

        } catch (mongocxx::exception &e) {

        }

        auto birimList = this->getBirimList();

        auto layoutContainer = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
        layoutContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto gLayout = layoutContainer->setLayout(cpp14::make_unique<WGridLayout>());


        int i = 0;
        int j = 0;
        for( auto birim : birimList )
        {
            auto container = gLayout->addWidget(cpp14::make_unique<WContainerWidget>(),i,j,AlignmentFlag::Center|AlignmentFlag::Justify);
            container->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
            auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            QString str = birim.birim.c_str();
            if( str.count() > 9 )
            {
                str = str.mid(0,str.count()-9);
            }
            vLayout->addWidget(cpp14::make_unique<WText>("<b>"+str.toStdString()+"</b>"),0,AlignmentFlag::Center);
            vLayout->addStretch(1);
            bool exist = false;
            for( auto view : viewList )
            {
                std::cout << "BIRIM: " << birim.birim << std::endl;
                std::cout << "BSON: " << bsoncxx::to_json(view) << std::endl;
                try {
                    if( view["_id"].get_utf8().value.to_string() == birim.birim )
                    {
                        try {
                            vLayout->addWidget(cpp14::make_unique<WText>(WString("<b><span style=\"color:green\">{1}</span></b>").arg(view["count"].get_int32().value)),0,AlignmentFlag::Center);
                        } catch (bsoncxx::exception &e) {
                            vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())),0,AlignmentFlag::Center);
                        }
                        exist = true;
                    }
                } catch (bsoncxx::exception &e) {
                    std::cout << "Error _id: " << e.what() << std::endl;
                }

            }

            if ( !exist ) {
                try {
                    vLayout->addWidget(cpp14::make_unique<WText>(WString("<b><span style=\"color:red\">!{1}!</span></b>").arg(0)),0,AlignmentFlag::Center);
                } catch (bsoncxx::exception &e) {
                    vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())),0,AlignmentFlag::Center);
                }
            }

            j++;
            if( j >= 4 )
            {
                j = 0;
                i++;
            }

        }
    }



    try {


        auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).find(filterView);


        auto LayContainer = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
        LayContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto vLayout = LayContainer->setLayout(cpp14::make_unique<WVBoxLayout>());


        {
            auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<h4>"+title+"ın Detayları</h4>"),0,AlignmentFlag::Center);
        }

        int row = 1;
        for( auto doc : cursor )
        {
            {
                auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<b>"+WString("{1} - ").arg(row++).toUTF8()+doc[SBLDKeys::Calismalar::birim].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
            }

            {
                auto titlebaslik = vLayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Calismalar::baslik].get_utf8().value.to_string()),0,AlignmentFlag::Center);
            }

            {
                auto tanimText = vLayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Calismalar::tanim].get_utf8().value.to_string()),0,AlignmentFlag::Left|AlignmentFlag::Justify);
            }

            {
                try {
                    auto asamaDoc = doc[SBLDKeys::Calismalar::asamalar].get_array().value;
                    for( auto item : asamaDoc )
                    {
                        try {
                            vLayout->addWidget(cpp14::make_unique<WText>("● "+item.get_document().view()[SBLDKeys::Calismalar::Asama::Aciklama].get_utf8().value.to_string()
                                               + " - " + WDate::fromJulianDay(item.get_document().view()[SBLDKeys::Calismalar::Asama::Tarih].get_int64().value).toString("dd/MM/yyyy").toUTF8()),0,AlignmentFlag::Left);
                        } catch (bsoncxx::exception &e) {

                        }
                    }
                } catch (bsoncxx::exception &e) {

                }

            }

            {
                auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
            }
        }




    } catch (mongocxx::exception &e) {

    }







    {

        auto rContanier_ = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rContanier_->addStyleClass(Bootstrap::Grid::row);


        auto imgContainer = rContanier_->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        imgContainer->setHeight(100);

        auto btn = imgContainer->addWidget(cpp14::make_unique<WPushButton>("Yazdır"));
        btn->clicked().connect([=](){


            WString str = WString("var prtContent = document.getElementById(\"{1}\");"
                                  "var WinPrint = window.open('', '', 'left=0,top=0,width=1024,height=900,toolbar=0,scrollbars=0,status=0');"
                                  "WinPrint.document.write(prtContent.innerHTML);"
                                  "WinPrint.document.close();"
                                  "WinPrint.focus();"
                                  "WinPrint.print();"
                                  "WinPrint.close();").arg(fContainer->id().c_str());

            //                WString str = WString("var prtContent = document.getElementById(\"{1}\");"
            //                "var WinPrint = window.open('', '', 'left=0,top=0,width=1024,height=900,toolbar=0,scrollbars=0,status=0');"
            //                "WinPrint.document.write(prtContent.innerHTML);"
            //                "WinPrint.document.close();"
            //                "WinPrint.focus();"
            //                "WinPrint.print();"
            //                "WinPrint.close();").arg(rowContainer->id().c_str());

            imgContainer->doJavaScript(str.toUTF8());
        });
    }

}

Giris::Personel::CalismaGirWidget::CalismaGirWidget(mongocxx::database *_database, bsoncxx::document::value _user)
    :ContainerWidget::ContainerWidget(_database,_user,"Birim Çalışmaları")
{

    this->CurrentBirim = this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string();


    this->initTotalState();



    mMainContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    mMainContainer->setMargin(25,Side::Top);





}

void Giris::Personel::CalismaGirWidget::initTotalState()
{

    // Toplam Çalışma
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Toplam: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(make_document(kvp(SBLDKeys::Calismalar::birim,CurrentBirim))))));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::White::AliceBlue));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }
            this->ListCalismalar(bsoncxx::document::value(filter.view()));
        });
    }


    // Devam Eden
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(50,150),this->getRandom(50,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Devam Eden: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::White::AliceBlue));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }

            try {
                filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }
            this->ListCalismalar(bsoncxx::document::value(filter.view()));
        });
    }


    // Tamamlanan
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(75,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tamamlanan: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::White::AliceBlue));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }

            try {
                filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }
            this->ListCalismalar(bsoncxx::document::value(filter.view()));
        });
    }


    // Bugün Başlanan
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(75,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
        } catch (bsoncxx::exception &e) {

        }

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Bugün Başlanan: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::Grey::DarkSlateGray));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }

            try {
                filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }
            this->ListCalismalar(bsoncxx::document::value(filter.view()));
        });
    }

    // Son 7 Gün Başlanan
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(75,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-7}))));
        } catch (bsoncxx::exception &e) {

        }

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Son 7 Gün Başlanan: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::Grey::DarkSlateGray));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-7}))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }

            try {
                filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }
            this->ListCalismalar(bsoncxx::document::value(filter.view()));
        });
    }


    // Son 30 Gün Başlanan
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(75,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-30}))));
        } catch (bsoncxx::exception &e) {

        }

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Son 30 Gün Başlanan: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::Grey::DarkSlateGray));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-30}))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }

            try {
                filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }
            this->ListCalismalar(bsoncxx::document::value(filter.view()));
        });
    }

    // Son 90 Gün Başlanan
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(75,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-90}))));
        } catch (bsoncxx::exception &e) {

        }

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Son 90 Gün Başlanan: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()))));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::Grey::DarkSlateGray));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-90}))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }

            try {
                filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Uyarı","filter Error",e);
                return;
            }
            this->ListCalismalar(bsoncxx::document::value(filter.view()));
        });
    }

    // Yeni Çalışma Ekle
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(75,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Yeni Çalışma Ekle")));
        text->addStyleClass(Bootstrap::Label::Danger);
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::White::AliceBlue));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect(this,&CalismaGirWidget::YeniEkle);
    }

    // Ayarlar
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(75,150),this->getRandom(0,127)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Ayarlar")));
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::White::AliceBlue));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect(this,&CalismaGirWidget::Ayarlar);
    }



    // Bilgi
    {
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(127,160),this->getRandom(127,250),this->getRandom(127,255)));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("! Çalışmalar'a Aşama Eklediğiniz de, Bugün Yapılan Çalışmalara Düşecek Şekilde Düzenleme Yapılmıştır."
                                                                         "Örnek: 3 Gün Önce Başlattığınız Çalışmaya Bugün Aşama Eklerseniz, Bugünun Raporların Çalışmanız "
                                                                         "Gözükür !")));
        //        text->addStyleClass(Bootstrap::Label::Danger);
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::Red::Crimson));

        vLayout->addStretch(1);

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect(this,&CalismaGirWidget::Ayarlar);
    }




    {


        auto fluidContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto rContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);


        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
        } catch (bsoncxx::exception &e) {

        }



        mongocxx::pipeline stage;
        stage.match(filter.view()).group(make_document(kvp(SBLDKeys::oid,"$"+SBLDKeys::Calismalar::birim),kvp("count",make_document(kvp("$sum",1)))));
        try {
            auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).aggregate(stage);
            for( auto doc : cursor )
            {
                auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_4);
                container->setAttributeValue(Style::style,Style::Border::border("1px solid black")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,50),this->getRandom(0,30)));
                auto vlayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = vlayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::oid].get_utf8().value.to_string()),0,AlignmentFlag::Center);
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
                try {
                    {
                        auto text1 = vlayout->addWidget(cpp14::make_unique<WText>(WString("<b>{1}</b> - Adet Başlattı").arg(doc["count"].get_int32().value).toUTF8()),0,AlignmentFlag::Center);
                        text1->setMargin(WLength::Auto,Side::Left|Side::Right);
                        text1->setAttributeValue(Style::style,Style::font::weight::lighter+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
                    }
                } catch (bsoncxx::exception &e) {
                    {
                        auto text1 = vlayout->addWidget(cpp14::make_unique<WText>(WString("<b>0</b> - Adet Başlattı")),0,AlignmentFlag::Center);
                        text1->setMargin(WLength::Auto,Side::Left|Side::Right);
                        text1->setAttributeValue(Style::style,Style::font::weight::lighter+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
                    }
                }

                container->setPadding(10,Side::Top|Side::Bottom);
                container->setHeight(75);
            }
        } catch (mongocxx::exception &e) {
        }
    }

    {


        auto fluidContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto rContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);


        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::updateDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
        } catch (bsoncxx::exception &e) {

        }

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$ne",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}))));
        } catch (bsoncxx::exception &e) {

        }



        mongocxx::pipeline stage;
        stage.match(filter.view()).group(make_document(kvp(SBLDKeys::oid,"$"+SBLDKeys::Calismalar::birim),kvp("count",make_document(kvp("$sum",1)))));
        try {
            auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).aggregate(stage);
            for( auto doc : cursor )
            {
                auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_4);
                container->setAttributeValue(Style::style,Style::Border::border("1px solid black")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,50),this->getRandom(0,30)));
                auto vlayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = vlayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::oid].get_utf8().value.to_string()),0,AlignmentFlag::Center);
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
                try {
                    {
                        auto text1 = vlayout->addWidget(cpp14::make_unique<WText>(WString("<b>{1}</b> - Adet Güncelledi").arg(doc["count"].get_int32().value).toUTF8()),0,AlignmentFlag::Center);
                        text1->setMargin(WLength::Auto,Side::Left|Side::Right);
                        text1->setAttributeValue(Style::style,Style::font::weight::lighter+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
                    }
                } catch (bsoncxx::exception &e) {
                    {
                        auto text1 = vlayout->addWidget(cpp14::make_unique<WText>(WString("<b>0</b> - Adet Güncelledi")),0,AlignmentFlag::Center);
                        text1->setMargin(WLength::Auto,Side::Left|Side::Right);
                        text1->setAttributeValue(Style::style,Style::font::weight::lighter+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
                    }
                }

                container->setPadding(10,Side::Top|Side::Bottom);
                container->setHeight(75);
            }
        } catch (mongocxx::exception &e) {
        }
    }


}

void Giris::Personel::CalismaGirWidget::ListCalismalar(bsoncxx::document::value filterValue)
{

    mMainContainer->clear();

    mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);



    skip = 0;
    this->ListCalismalarAdd(rContainer,filterValue);


}

void Giris::Personel::CalismaGirWidget::ListCalismalarAdd(WContainerWidget *rowContainer, bsoncxx::document::value filterValue)
{


    mongocxx::options::find findOptions;

    findOptions.limit(limit);
    findOptions.skip(skip);

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,-1));
    } catch (bsoncxx::exception &e) {

    }

    try {
        sortDoc.append(kvp(SBLDKeys::Calismalar::updateDate,-1));
    } catch (bsoncxx::exception &e) {

    }


    findOptions.sort(sortDoc.view());

    auto _fluidContainer = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto _rowContainer = _fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _rowContainer->addStyleClass(Bootstrap::Grid::row);

    auto bucket = this->db()->gridfs_bucket();
    try {
        auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).find(filterValue.view(),findOptions);

        for( auto doc : cursor )
        {
            auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(50,127),this->getRandom(75,127),this->getRandom(80,127),.7));

            auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            fContainer->setMargin(15,Side::Top|Side::Bottom);

            auto rCOntainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            rCOntainer->addStyleClass(Bootstrap::Grid::row);



            // icon
            {
                auto imgPaht = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Calismalar::icon].get_oid().value.to_string());
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                imgContainer->setAttributeValue(Style::style,Style::background::url(imgPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
            }



            //title
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                QString str = doc[SBLDKeys::Calismalar::kategori].get_utf8().value.to_string().c_str();
                if( str.count() > 40 )
                {
                    str = str.mid(0,37);
                    str.append("...");
                }
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(str.toStdString()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }

            //Birim
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Calismalar::birim].get_utf8().value.to_string().c_str()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }
            //Mahalleler
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                auto vLayout = imgContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto arlist = doc[SBLDKeys::Calismalar::mahalleler].get_array().value;

                int count = 0;
                for( auto item : arlist )
                {
                    if( count < 3 )
                    {

                        QString str = item.get_utf8().value.to_string().c_str();
                        str = str.remove(str.count()-9,9);
                        auto text = vLayout->addWidget(cpp14::make_unique<WText>(str.toStdString()));
                        text->addStyleClass(Bootstrap::Label::Warning);
                        text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                    }
                    count++;
                }

                if( count > 3 )
                {
                    auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("+{1} Daha").arg(count-3)));
                    text->addStyleClass(Bootstrap::Label::Warning);
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
                }
                vLayout->addStretch(1);
            }

            //Tanım
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                QString str = doc[SBLDKeys::Calismalar::baslik].get_utf8().value.to_string().c_str();
                if( str.count() > 40 )
                {
                    str = str.mid(0,37);
                    str.append("...");
                }
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(str.toStdString().c_str()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }

            //Tanım
            {
                auto imgContainer = rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_4);
                imgContainer->setHeight(100);
                auto text = imgContainer->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(doc[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MMMM/yyyy").toStdString()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->addStyleClass(Bootstrap::Label::Primary);
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

                rCOntainer->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string().c_str());
                rCOntainer->decorationStyle().setCursor(Cursor::PointingHand);
                rCOntainer->clicked().connect([=](){
                    this->setCalismaDetail(rCOntainer->attributeValue(Style::dataoid).toUTF8());
                });
            }
        }




    } catch (mongocxx::exception &e) {
        this->showMessage("Uyarı","Hata",e);
    }



    {
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));
        btn->setMargin(WLength::Auto,Side::Left|Side::Right);
        btn->addStyleClass(Bootstrap::Button::Primary);

        btn->clicked().connect([=](){
            skip += limit;
            rowContainer->removeWidget(container);
            this->ListCalismalarAdd(rowContainer,filterValue);
        });
    }

}

void Giris::Personel::CalismaGirWidget::setCalismaDetail(std::string oid)
{
    mMainContainer->clear();


    //    mTableContainer->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(oid.c_str())));

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->db()->collection(SBLDKeys::Calismalar::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));





    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Calismalar::baslik].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(WDate::fromJulianDay(view[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MM/yyyy").toUTF8().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Calismalar::durum].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Kategori {1}").arg(view[SBLDKeys::Calismalar::kategori].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Calismalar::birim].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tanım: <u>{1}</u>").arg(view[SBLDKeys::Calismalar::tanim].get_utf8().value.to_string().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }



    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        container->setPadding(10,AllSides);
        for( auto mah : view[SBLDKeys::Calismalar::mahalleler].get_array().value )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mah.get_utf8().value.to_string().c_str())));
            text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
        }
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        if( view[SBLDKeys::Calismalar::yayin].get_utf8().value.to_string() == SBLDKeys::Calismalar::Yayinda::yayinda )
        {
            auto btn = vLayout->addWidget(cpp14::make_unique<WPushButton>("Yayından Kaldır"),0,AlignmentFlag::Middle);
            btn->addStyleClass(Bootstrap::Button::Primary);
            btn->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            btn->clicked().connect([=](){
                auto filter = document{};

                try {
                    filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid.c_str()}));
                } catch (bsoncxx::exception &e) {

                }

                auto doc = document{};

                try {
                    doc.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayindaDegil));
                } catch (bsoncxx::exception &e) {

                }

                try {
                    auto upt = this->db()->collection(SBLDKeys::Calismalar::collection).update_one(filter.view(),make_document(kvp("$set",doc)).view());
                    if( upt )
                    {
                        if( upt.value().modified_count() )
                        {
                            this->showMessage("Bilgi","Çalışma Yayından Kaldırıldı","Tamam");
                            this->setCalismaDetail(oid);
                        }else{
                            this->showMessage("Hata","Olmadı. Napsak?","Bilmem?");
                        }
                    }else{
                        this->showMessage("Hata","Sitede Yayından Kaldıramadık. Napsak?","Bilmem?");
                    }
                } catch (mongocxx::exception &e) {
                    this->showMessage("Hata","Bir Hata Oluştu.",e);
                }

            });
        }else{
            auto btn = vLayout->addWidget(cpp14::make_unique<WPushButton>("Yayınla"),0,AlignmentFlag::Middle);
            btn->addStyleClass(Bootstrap::Button::Success);
            btn->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            btn->clicked().connect([=](){
                auto filter = document{};

                try {
                    filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid.c_str()}));
                } catch (bsoncxx::exception &e) {

                }

                auto doc = document{};

                try {
                    doc.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
                } catch (bsoncxx::exception &e) {

                }

                try {
                    auto upt = this->db()->collection(SBLDKeys::Calismalar::collection).update_one(filter.view(),make_document(kvp("$set",doc)).view());
                    if( upt )
                    {
                        if( upt.value().modified_count() )
                        {
                            this->showMessage("Bilgi","Çalışma Yayınladı","Tamam");
                            this->setCalismaDetail(oid);
                        }else{
                            this->showMessage("Hata","Sitede Yayınlayamadık. Ne Yapsak Acaba?","Bilmem?");
                        }
                    }else{
                        this->showMessage("Hata","Sitede Yayınlayamadık. Napsak?","Bilmem?");
                    }
                } catch (mongocxx::exception &e) {
                    this->showMessage("Hata","Bir Hata Oluştu.",e);
                }

            });
        }

    }


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto bucket = this->db()->gridfs_bucket();
        std::string iconPaht = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Calismalar::icon].get_oid().value.to_string());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }
    {


        auto asamaContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto bucket = this->db()->gridfs_bucket();
        try {
            auto asamaList = view[SBLDKeys::Calismalar::asamalar].get_array().value;

            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                //                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto StackWidget = container->addWidget(cpp14::make_unique<WStackedWidget>());
                StackWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::EaseInOut,250),true);
                std::vector<std::string> imgPathList;
                for( auto id : doc[SBLDKeys::Calismalar::Asama::Gorseller].get_array().value )
                {
                    std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,id.get_oid().value.to_string());
                    imgPathList.push_back(imgPath);
                }

                for( auto str : imgPathList )
                {
                    auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                    slideCOntainer->setHeight(350);



                    auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    frontimgContainer->setHeight(350);
                    frontimgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                    auto imgContainer = cpp14::make_unique<WContainerWidget>();
                    imgContainer->setHeight(350);
                    imgContainer->setWidth(WLength("100%"));
                    imgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+"z-index:-1; ");
                    imgContainer->addStyleClass("CommanFilter");
                    imgContainer->setPositionScheme(PositionScheme::Absolute);
                    imgContainer->setOffsets(0,Side::Top);
                    slideCOntainer->addWidget(std::move(imgContainer));

                    StackWidget->addWidget(std::move(slideCOntainer));

                }


                auto btnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                btnContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));


                auto hLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

                auto backBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
                backBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex > 0 )
                    {
                        StackWidget->setCurrentIndex(--CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( indexCount-1 );
                    }
                });

                hLayout->addStretch(1);
                int imgCounter = 0;

                for( auto str : imgPathList )
                {
                    imgCounter++;
                }

                {
                    auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Resim").arg(imgCounter)),0,AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s24px);
                }
                hLayout->addStretch(1);



                auto nextBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));

                nextBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex < indexCount-1 )
                    {
                        StackWidget->setCurrentIndex(++CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( 0 );
                    }
                });

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Calismalar::Asama::Aciklama].get_utf8().value.to_string().c_str())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            }
        } catch (bsoncxx::exception &e) {

        }


    }


    bool ended = false;
    try {
        ended = view[SBLDKeys::Calismalar::durum].get_utf8().value.to_string() == SBLDKeys::Calismalar::DURUM::devamediyor ;
    } catch (bsoncxx::exception &e) {
        ended = false;
    }




    {
        auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
        fContainer->setMargin(25,Side::Top);
        fContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(75,150),this->getRandom(125,175),this->getRandom(100,150)));
        fContainer->setPadding(25,Side::Top|Side::Bottom);

        auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        if( !ended )
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

            auto text = container->addWidget(cpp14::make_unique<WText>("Bu Çalışma Tamamlandı"));
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px+Style::color::color(Style::color::White::AliceBlue));
        }else
        {
            {

                auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                auto text = container->addWidget(cpp14::make_unique<WText>("Aşama Ekle"));
                text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px+Style::color::color(Style::color::White::AliceBlue));

            }


            auto dateContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            dateContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
            dateContainer->setMargin(5,Side::Bottom);

            auto dateEdit = dateContainer->addWidget(cpp14::make_unique<WDateEdit>());
            dateEdit->setMargin(WLength::Auto,Side::Left|Side::Right);
            dateEdit->setDate(WDate::currentDate());
            dateEdit->setPopup(true);
            dateEdit->setFormat("dd.MM.yyyy");
            dateEdit->setEnabled(false);

            auto baslikContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            baslikContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
            baslikContainer->setMargin(5,Side::Bottom);

            auto aciklamaLineEdit = baslikContainer->addWidget(cpp14::make_unique<WLineEdit>());
            aciklamaLineEdit->setMargin(WLength::Auto,Side::Left|Side::Right);
            aciklamaLineEdit->setPlaceholderText("Aşama Açıklamasını Buraya Giriniz");

            auto mahalleContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            mahalleContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
            mahalleContainer->setMargin(5,Side::Bottom);

            auto mahalleComboBox = mahalleContainer->addWidget(cpp14::make_unique<WComboBox>());
            mahalleComboBox->addItem("Seçiniz...");
            mahalleComboBox->addItem("Genel Çalışma..");
            try {
                auto cursor = this->db()->collection(SBLDKeys::Mahalle::collection).find(document{}.view());

                for( auto doc : cursor )
                {
                    try {
                        if( doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string() != "NULL" )
                        {
                            mahalleComboBox->addItem(doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string().c_str());
                        }
                    } catch (bsoncxx::exception &e) {

                    }
                }

            } catch (mongocxx::exception &e) {

            }


            filePathList.clear();

            this->addUploader(rContainer);


            auto btnContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            btnContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            btnContainer->setMargin(5,Side::Bottom);


            Wt::WPushButton *saveButton = btnContainer->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Bu Aşamayı Kaydet"));
            saveButton->addStyleClass(Bootstrap::Button::Primary);
            saveButton->setMargin(WLength::Auto, Wt::Side::Left | Wt::Side::Right);



            saveButton->clicked().connect([=](){

                if( !filePathList.size() )
                {
                    this->showMessage("Hata","Hiç Resim Koymadın, Bu İyi Olmadı!","Evet Olmadı!");
                    return;
                }

                if( aciklamaLineEdit->text().empty() )
                {
                    this->showMessage("Hata","Hiç Açıklama Yazmadın!","Evet Yazmadım!");
                    return;
                }


                auto pushDoc = document{};

                try {
                    pushDoc.append(kvp(SBLDKeys::Calismalar::Asama::Aciklama,aciklamaLineEdit->text().toUTF8()));
                } catch (bsoncxx::exception &e) {
                    this->showMessage("Hata","Aciklama",e);
                    return;
                }
                try {
                    pushDoc.append(kvp(SBLDKeys::Calismalar::Asama::Tarih,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
                } catch (bsoncxx::exception &e) {
                    this->showMessage("Hata","Tarih",e);
                    return;
                }

                auto iconArray = array{};

                auto bucket = this->db()->gridfs_bucket();
                for( auto item : filePathList )
                {
                    auto id = SBLDKeys::uploadfile(&bucket,QString(item.c_str()));
                    iconArray.append(id);
                }


                try {
                    pushDoc.append(kvp(SBLDKeys::Calismalar::Asama::Gorseller,iconArray));
                } catch (bsoncxx::exception &e) {
                    this->showMessage("Hata","Gorseller",e);
                    return;
                }

                auto doc = document{};

                auto _pushDoc = document{};




                auto filter = document{};

                try {
                    filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid.c_str()}));
                } catch (bsoncxx::exception &e) {
                    this->showMessage("Hata","filter",e);
                    return;
                }


                bool addMahalle = true;
                try {
                    auto val = this->db()->collection(SBLDKeys::Calismalar::collection).find_one(filter.view());
                    if( val )
                    {
                        if( !val.value().view().empty() )
                        {
                            try {
                                auto arList = val.value().view()[SBLDKeys::Calismalar::mahalleler].get_array().value;
                                for( auto item : arList )
                                {
                                    if( item.get_utf8().value.to_string() == mahalleComboBox->currentText().toUTF8() )
                                    {
                                        addMahalle = false;
                                    }
                                }
                            } catch (bsoncxx::exception &e) {
                            }
                        }
                    }
                } catch (mongocxx::exception &e) {
                }

                if( addMahalle )
                {
                    try {
                        _pushDoc.append(kvp(SBLDKeys::Calismalar::mahalleler,mahalleComboBox->currentText().toUTF8().c_str()));
                    } catch (bsoncxx::exception &e) {
                        this->showMessage("Hata","mahalleler",e);
                        return;
                    }
                }

                try {
                    _pushDoc.append(kvp(SBLDKeys::Calismalar::asamalar,pushDoc));
                } catch (bsoncxx::exception &e) {
                    this->showMessage("Hata","asamalar",e);
                    return;
                }

                try {
                    doc.append(kvp("$push",_pushDoc));
                } catch (bsoncxx::exception &e) {
                    this->showMessage("Hata","doc $push",e);
                    return;
                }

                try {
                    doc.append(kvp("$set",make_document(kvp(SBLDKeys::Calismalar::updateDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}))));
                } catch (bsoncxx::exception &e) {
                    this->showMessage("Hata","doc $push",e);
                    return;
                }




                try {
                    auto upt = this->db()->collection(SBLDKeys::Calismalar::collection).update_one(filter.view(),doc.view());
                    if( upt )
                    {
                        if( upt.value().modified_count() )
                        {
                            this->showMessage("Bilgi","Aşama Başarılı Bir Şekilde Eklendi");
                            this->setCalismaDetail(oid);
                        }else{
                            this->showMessage("Hata","Aşama Eklenemedi O Modified");
                        }
                    }else{
                        this->showMessage("Hata","Aşama Eklenemedi Null Document");
                    }
                } catch (mongocxx::exception &e) {
                    this->showMessage("Hata","Aşama Eklenemedi",e);
                }
            });


            {
                auto btnSetCloseContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                btnSetCloseContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                btnSetCloseContainer->setMargin(5,Side::Bottom);


                Wt::WPushButton *closeButton = btnSetCloseContainer->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Bu Çalışmayı Sonlandır"));
                closeButton->addStyleClass(Bootstrap::Button::Success);
                closeButton->setMargin(WLength::Auto, Wt::Side::Left | Wt::Side::Right);

                closeButton->clicked().connect([=](){
                    auto filter = document{};

                    try {
                        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid.c_str()}));
                    } catch (bsoncxx::exception &e) {
                        this->showMessage("Hata","filter",e);
                        return;
                    }

                    auto setDoc = document{};

                    try {
                        setDoc.append(kvp("$set",make_document(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi))));
                    } catch (bsoncxx::exception &e) {
                        this->showMessage("Hata","$set",e);
                        return;
                    }

                    try {
                        auto upt = this->db()->collection(SBLDKeys::Calismalar::collection).update_one(filter.view(),setDoc.view());
                        if( upt )
                        {
                            if( upt.value().modified_count() )
                            {
                                this->showMessage("Hata","Bu Çalışma Sonlandırıldı");
                            }else{
                                this->showMessage("Hata","Sonlandırılamadı 0 Document");
                            }
                        }else{
                            this->showMessage("Hata","Sonlandırılamadı Null Document");
                        }
                    } catch (mongocxx::exception &e) {
                        this->showMessage("Hata","mongocxx::exception",e);
                    }

                });
            }
        }







    }





}

void Giris::Personel::CalismaGirWidget::addUploader(WContainerWidget *rContainer)
{



    auto progresBarContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    progresBarContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    auto progresBar = progresBarContainer->addWidget(cpp14::make_unique<WProgressBar>());




    auto iconContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    iconContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    iconContainer->setMargin(5,Side::Bottom);




    Wt::WFileUpload *fu = iconContainer->addWidget(Wt::cpp14::make_unique<Wt::WFileUpload>());
    fu->setFileTextSize(6000); // Set the maximum file size to 2048 kB.
    fu->setProgressBar(Wt::cpp14::make_unique<Wt::WProgressBar>());
    fu->setMargin(10, Wt::Side::Right);
    fu->setFilters("image/*");
    fu->setMultiple(true);
    fu->setProgressBar(progresBar);




    // Provide a button to start uploading.
    Wt::WPushButton *uploadButton = iconContainer->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Resimleri Yükle"));
    uploadButton->setMargin(10, Wt::Side::Left | Wt::Side::Right);



    Wt::WText *out = iconContainer->addWidget(Wt::cpp14::make_unique<Wt::WText>());



    // Upload when the button is clicked.
    uploadButton->clicked().connect([=] {
        fu->upload();
        fu->enable();
    });

    // React to a file upload problem.
    fu->fileTooLarge().connect([=] {
        std::cout << fu->fileTextSize() << std::endl;
        std::cout << "Request Size: " << wApp->maximumRequestSize() << std::endl;
        this->showMessage("Hata","Dosya Boyutu Fazla Büyük. Max: 6MB Olmalı");
    });

    fu->uploaded().connect([=] {
        out->setText("Yükleme Tamamlandı.");

        auto list = fu->uploadedFiles();

        for( auto item : list )
        {
            auto icon_Container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            icon_Container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_4);
            icon_Container->setPadding(3,AllSides);
            auto imgContainer = icon_Container->addWidget(cpp14::make_unique<WContainerWidget>());


            QString _fileName = QString("%1%2").arg(QUuid::createUuid().toString()).arg(item.clientFileName().c_str());
            rootnewFileName = QString("docroot/tempfile/")+_fileName;
            QString newFileName = QString("tempfile/")+ _fileName;
            if( QFile::copy(item.spoolFileName().c_str(),rootnewFileName) )
            {
                QImage img;
                if( img.load(rootnewFileName) )
                {
                    img = img.scaledToWidth(1024);
                    if( img.save(rootnewFileName,"JPG",75) )
                    {
                        std::cout << "FILE SAVE MIN" << std::endl;
                    }else{
                        std::cout << "FILE NOT SAVE MIN" << std::endl;
                    }
                }else{
                    std::cout << "FILE NOT OPEN" << std::endl;
                }
                imgContainer->setAttributeValue(Style::style,Style::background::url(newFileName.toStdString().c_str())+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
                imgContainer->setHeight(150);
                filePathList.push_back(rootnewFileName.toStdString());
            }else{
                imgContainer->setAttributeValue(Style::style,Style::background::url("img/error.png")+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
                imgContainer->setHeight(150);
            }

            std::cout << "clientFileName: " << item.clientFileName() << std::endl;
            std::cout << "spoolFileName: " << item.spoolFileName() << std::endl;
        }
        rContainer->removeWidget(progresBarContainer);
        rContainer->removeWidget(iconContainer);
        this->addUploader(rContainer);


    });


}

void Giris::Personel::CalismaGirWidget::YeniEkle()
{

    mMainContainer->clear();


    newFileNamePath.clear();


    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("Yeni Çalışma Ekle"));
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127)));
        container->setMargin(25,Side::Bottom);
    }


    auto dateContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    dateContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
    dateContainer->setMargin(5,Side::Bottom);

    auto dateEdit = dateContainer->addWidget(cpp14::make_unique<WDateEdit>());
    dateEdit->setMargin(WLength::Auto,Side::Left|Side::Right);
    dateEdit->setDate(WDate::currentDate());
    dateEdit->setPopup(true);
    dateEdit->setFormat("dd.MM.yyyy");
    //    dateEdit->setEnabled(false);


    auto baslikContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    baslikContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
    baslikContainer->setMargin(5,Side::Bottom);

    auto baslikLineEdit = baslikContainer->addWidget(cpp14::make_unique<WLineEdit>());
    baslikLineEdit->setMargin(WLength::Auto,Side::Left|Side::Right);
    baslikLineEdit->setPlaceholderText("Çalışma Başlığını Giriniz");

    auto adresContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    adresContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
    adresContainer->setMargin(5,Side::Bottom);

    auto adresLineEdit = adresContainer->addWidget(cpp14::make_unique<WLineEdit>());
    adresLineEdit->setMargin(WLength::Auto,Side::Left|Side::Right);
    adresLineEdit->setPlaceholderText("Adresi Buraya Giriniz");



    auto kategoriContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    kategoriContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    kategoriContainer->setMargin(5,Side::Bottom);

    auto kategoriComboBox = kategoriContainer->addWidget(cpp14::make_unique<WComboBox>());
    kategoriComboBox->addItem("Seçiniz");

    try {
        auto cursor = this->db()->collection(SBLDKeys::Calismalar::Ayarlar::Kategori::collection).find(make_document(kvp(SBLDKeys::Calismalar::Ayarlar::Kategori::birim,CurrentBirim)).view());

        for( auto doc : cursor )
        {
            try {
                kategoriComboBox->addItem(doc[SBLDKeys::Calismalar::Ayarlar::Kategori::kategoriAdi].get_utf8().value.to_string().c_str());
            } catch (bsoncxx::exception &e) {

            }
        }

    } catch (mongocxx::exception &e) {

    }


    auto mahalleContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mahalleContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    mahalleContainer->setMargin(5,Side::Bottom);

    auto mahalleComboBox = mahalleContainer->addWidget(cpp14::make_unique<WComboBox>());

    mahalleComboBox->addItem("Seçiniz");
    mahalleComboBox->addItem("Genel Çalışma  ");
    try {
        auto cursor = this->db()->collection(SBLDKeys::Mahalle::collection).find(document{}.view());

        for( auto doc : cursor )
        {
            try {
                if( doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string() != "NULL" )
                {
                    mahalleComboBox->addItem(doc[SBLDKeys::Mahalle::mahalle].get_utf8().value.to_string().c_str());
                }
            } catch (bsoncxx::exception &e) {

            }
        }

    } catch (mongocxx::exception &e) {

    }

    auto progresBarContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    progresBarContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    progresBarContainer->setMargin(5,Side::Bottom);

    auto progresBar = progresBarContainer->addWidget(cpp14::make_unique<WProgressBar>());

    auto iconContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    iconContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    iconContainer->setMargin(5,Side::Bottom);


    Wt::WFileUpload *fu = iconContainer->addWidget(Wt::cpp14::make_unique<Wt::WFileUpload>());
    fu->setProgressBar(progresBar);
    fu->setFileTextSize(256); // Set the maximum file size to 50 kB.
    fu->setProgressBar(Wt::cpp14::make_unique<Wt::WProgressBar>());
    fu->setMargin(10, Wt::Side::Right);
    fu->setFilters("image/*");


    // Provide a button to start uploading.
    Wt::WPushButton *uploadButton =
            iconContainer->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Yükle"));
    uploadButton->setMargin(10, Wt::Side::Left | Wt::Side::Right);

    Wt::WText *out = iconContainer->addWidget(Wt::cpp14::make_unique<Wt::WText>());

    rootnewFileName.clear();
    // Upload when the button is clicked.
    uploadButton->clicked().connect([=] {
        fu->upload();
    });

    // React to a succesfull upload.
    fu->uploaded().connect([=] {
        out->setText("File upload is finished.");
        rootnewFileName = QString("docroot/tempfile/%1").arg(fu->clientFileName().toUTF8().c_str());
        QString newFileName = QString("tempfile/")+ fu->clientFileName().toUTF8().c_str();
        if( QFile::copy(fu->spoolFileName().c_str(),rootnewFileName) )
        {

            QImage img;
            if( img.load(rootnewFileName) )
            {
                img = img.scaledToWidth(400);
                if( img.save(rootnewFileName,"JPG",75) )
                {
                    std::cout << "FILE SAVE MIN" << std::endl;
                }else{
                    std::cout << "FILE NOT SAVE MIN" << std::endl;
                    this->showMessage("Hata","Resim Dosyası Kayıt Edilemedi");
                    uploadButton->enable();
                    return;
                }
            }else{
                std::cout << "FILE NOT OPEN: " << rootnewFileName.toStdString() << std::endl;
                this->showMessage("Hata","Resim Dosyası Açılamadı: "  + rootnewFileName.toStdString());
                uploadButton->enable();
                return;
            }

            auto containericon = iconContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            containericon->setAttributeValue(Style::style,Style::background::url(newFileName.toStdString().c_str())+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
            containericon->setHeight(350);
            uploadButton->disable();
        }else{
            this->showMessage("Hata","Resim Dosyası Yüklenemedi");
            uploadButton->enable();
        }

    });

    // React to a file upload problem.
    fu->fileTooLarge().connect([=] {
        this->showMessage("Hata","Dosya Boyutu Fazla Büyük. Max: 256KB Olmalı");
    });




    auto tanimContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    tanimContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    tanimContainer->setMargin(5,Side::Bottom);

    auto tanimtextEdit = tanimContainer->addWidget(cpp14::make_unique<WTextEdit>());
    tanimtextEdit->setMargin(WLength::Auto,Side::Left|Side::Right);
    tanimtextEdit->setText("Tanımı Buraya Giriniz");
    tanimtextEdit->setHeight(350);




    auto saveContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    saveContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    saveContainer->setMargin(5,Side::Bottom);


    auto savebtn = saveContainer->addWidget(cpp14::make_unique<WPushButton>("Başlat"));
    savebtn->setMargin(WLength::Auto,Side::Left|Side::Right);


    savebtn->clicked().connect([=](){


        if( rootnewFileName.isEmpty() )
        {
            this->showMessage("Hata","Resim Yüklemedin!, Hayırdır?","Tamam Yükleyecem");
            return;
        }

        if( !baslikLineEdit->text().toUTF8().size() )
        {
            this->showMessage("Hata","Başlıkta Yok","Anladık!");
            return;
        }

        if( !adresLineEdit->text().toUTF8().size() )
        {
            this->showMessage("Hata","Adres Tam Değil","Tamamlayayım");
            return;
        }

        if( !mahalleComboBox->currentIndex() )
        {
            this->showMessage("Hata","Mahalle Seçmedin","Seçeyim");
            return;
        }

        if( !kategoriComboBox->currentIndex() )
        {
            this->showMessage("Hata","Kategori Seçmedin","Seçeyim O Zaman");
            return;
        }

        std::cout << "Current mahalleComboBox Index: " << mahalleComboBox->currentIndex() << std::endl;

        if( WDate::currentDate().toJulianDay() - dateEdit->date().toJulianDay() > 3 )
        {
            this->showMessage("Hata","En Fazla 3 Gün Öncesinin Çalışmasını Kayıt Edebilirsiniz","Tamam");
            return;
        }

        if( dateEdit->date().toJulianDay() - WDate::currentDate().toJulianDay() > 3 )
        {
            this->showMessage("Hata","En Fazla 3 Gün Sonrasına Çalışmasını Kayıt Edebilirsiniz","Tamam");
            return;
        }


        auto doc = document{};

        try {
            doc.append(kvp(SBLDKeys::Calismalar::updateDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,bsoncxx::types::b_int64{dateEdit->date().toJulianDay()}));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::baslik,baslikLineEdit->text().toUTF8().c_str()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::kategori,kategoriComboBox->currentText().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::mahalleler,make_array(mahalleComboBox->currentText().toUTF8())));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::tanim,tanimtextEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }

        // Tam Adres Yok
        try {
            doc.append(kvp(SBLDKeys::Calismalar::tamadres,adresLineEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayindaDegil));
        } catch (bsoncxx::exception &e) {

        }

        auto bucket = this->db()->gridfs_bucket();
        auto id = SBLDKeys::uploadfile(&bucket,rootnewFileName);
        try {
            doc.append(kvp(SBLDKeys::Calismalar::icon,id));
        } catch (bsoncxx::exception &e) {

        }

        try {
            auto ins = this->db()->collection(SBLDKeys::Calismalar::collection).insert_one(doc.view());
            if( ins )
            {
                if( ins.value().result().inserted_count() )
                {
                    this->showMessage("Holaa!","Başarılı Bir Şekilde Çalışmayı Başlattınız","Devam Et");
                    std::cout << "Deleted File: " << QFile::remove(rootnewFileName) << std::endl;
                    this->setCalismaDetail(ins.value().inserted_id().get_oid().value.to_string());
                }else{
                    this->showMessage("Off","İçimde Bir Sıkıntı Var. Çalışmada Yüklenmedi","Anladık!");
                }
            }else{
                this->showMessage("Off","Bugün Keyfim Yok","Hımm");
            }
        } catch (mongocxx::exception &e) {
            this->showMessage("Off","Veri Tabanına Atamadık",e);
        }



    });


}

void Giris::Personel::CalismaGirWidget::Ayarlar()
{

    mMainContainer->clear();

    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("Ayarlar"));
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
    }



    auto LineEditContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    LineEditContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto lineEdit = LineEditContainer->addWidget(cpp14::make_unique<WLineEdit>());
    lineEdit->setPlaceholderText("Kategori Adını Giriniz");


    auto AddBtnContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    AddBtnContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto addbtn = AddBtnContainer->addWidget(cpp14::make_unique<WPushButton>("Yeni Ekle"));
    addbtn->addStyleClass(Bootstrap::Button::Primary);


    auto listContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    listContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    listContainer->setMargin(25,Side::Top|Side::Bottom);

    this->ListKategori(listContainer);



    addbtn->clicked().connect([=](){


        if( lineEdit->text().empty() )
        {
            this->showMessage("Hata","Vala Kategori Adını Girmeden Kayıt Yapamam","İyi Be! Yapmazsan Yapma");
            return;
        }

        auto doc = document{};

        try {
            doc.append(kvp(SBLDKeys::Calismalar::Ayarlar::Kategori::kategoriAdi,lineEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Calismalar::Ayarlar::Kategori::birim,CurrentBirim));
        } catch (bsoncxx::exception &e) {

        }


        try {
            auto ins = this->db()->collection(SBLDKeys::Calismalar::Ayarlar::Kategori::collection).insert_one(doc.view());

            if( ins )
            {
                if( ins.value().result().inserted_count() )
                {
                    this->ListKategori(listContainer);
                }else{
                    this->showMessage("Hata","Kayıt Edilmedi","Tamam");
                }
            }else{
                this->showMessage("Hata","Kayıt Edilmedi","Tamam");
            }
        } catch (mongocxx::exception &e) {
            this->showMessage("Hata","Error",e);
        }
    });







}

void Giris::Personel::CalismaGirWidget::ListKategori(WContainerWidget *listContainer)
{

    listContainer->clear();

    auto fContainer = listContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);



    try {
        auto cursor = this->db()->collection(SBLDKeys::Calismalar::Ayarlar::Kategori::collection).find(make_document(kvp(SBLDKeys::Calismalar::Ayarlar::Kategori::birim,CurrentBirim)).view());

        for( auto item : cursor )
        {

            {
                auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>("Kategori Adı"));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                container->setMargin(10,Side::Top|Side::Bottom);
            }

            {
                auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_5+Bootstrap::Grid::Medium::col_md_5+Bootstrap::Grid::Small::col_sm_5+Bootstrap::Grid::ExtraSmall::col_xs_5);
                auto text = container->addWidget(cpp14::make_unique<WText>(item[SBLDKeys::Calismalar::Ayarlar::Kategori::kategoriAdi].get_utf8().value.to_string().c_str()));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                container->setMargin(10,Side::Top|Side::Bottom);
            }

            {
                auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);
                auto text = container->addWidget(cpp14::make_unique<WText>("Sil"));
                text->setMargin(WLength::Auto,Side::Left|Side::Right);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue));
                std::string kategoriAdi = item[SBLDKeys::Calismalar::Ayarlar::Kategori::kategoriAdi].get_utf8().value.to_string().c_str();
                container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::DarkRed));
                container->decorationStyle().setCursor(Cursor::PointingHand);
                container->setMargin(10,Side::Top|Side::Bottom);
                container->clicked().connect([=](){

                    auto filter = document{};

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::kategori,kategoriAdi));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::birim,CurrentBirim));
                    } catch (bsoncxx::exception &e) {

                    }

                    if( this->db()->collection(SBLDKeys::Calismalar::collection).count(filter.view()) )
                    {
                        std::cout << "Bu Kategoriye Ait Çalışma Var" << std::endl;
                        this->showMessage("Dikkat","Bu Kategoriye Ait Çalışmalar Var.","Tamam");
                        return;
                    }

                    filter.clear();

                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::Ayarlar::Kategori::kategoriAdi,kategoriAdi));
                    } catch (bsoncxx::exception &e) {

                    }
                    try {
                        filter.append(kvp(SBLDKeys::Calismalar::Ayarlar::Kategori::birim,CurrentBirim));
                    } catch (bsoncxx::exception &e) {

                    }


                    try {
                        auto result = this->db()->collection(SBLDKeys::Calismalar::Ayarlar::Kategori::collection).delete_one(filter.view());

                        if( result )
                        {
                            if( result.value().deleted_count() )
                            {
                                this->showMessage("Bilgi","Silindi","Tamam");
                                this->ListKategori(listContainer);
                            }else{
                                this->showMessage("Hata","Silme Başarısız","Tamam");
                            }
                        }else{
                            this->showMessage("Hata","Silme Başarısız","Tamam");
                        }
                    } catch (mongocxx::exception &e) {
                        this->showMessage("Hata","Error",e);
                    }

                });
            }


        }


    } catch (mongocxx::exception &e) {

    }



}

Giris::Personel::CagriMerkezi::CagriMerkezi(mongocxx::database* _database , bsoncxx::document::value _user)
    :ContainerWidget::ContainerWidget(_database,_user,"Şikayetler / Çağrı Merkezi")
{

    this->initialState();

    mMainContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);



}

void Giris::Personel::CagriMerkezi::initialState()
{

    //    static std::string devamediyor{"Devam Ediyor"};
    //    static std::string tamamlandi{"Tamamlandı"};
    //    static std::string iptalEdildi{"İptal Edildi"};
    //    static std::string tekrarAcildi{"Tekrar Açıldı"};
    //    static std::string beklemede{"Beklemede"};
    //    static std::string teyitEdilmemis{"Teyit Edilmemiş"};

    // Toplam Şikayet
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(10,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Toplam Şikayet"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(document{}.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            this->listSikayet(document{}.view());
        });
    }



    // Devam Eden Şikayet
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(10,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Devam Eden Şikayet"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::devamediyor));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::devamediyor));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }


    // Tamamlanan Şikayet
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(10,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Tamamlanan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::tamamlandi));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::tamamlandi));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }

    // Raporlar Menü
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(10,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(100);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Raporlar"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            this->initReportMenu();
        });
    }


    // İptal Edilen Şikayet
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("İptal Edilen"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::iptalEdildi));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::iptalEdildi));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }

    // Teyit Edilmemiş Şikayet
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Teyit Edilmemiş"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::teyitEdilmemis));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::teyitEdilmemis));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }

    // Beklemede Şikayet
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Beklemede"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::beklemede));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::beklemede));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }


    // Tekrar Açılmış Şikayet
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Tekrar Açılmış"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::tekrarAcildi));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::tekrarAcildi));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }


    // Bugün Gelen
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Bugün Gelen"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }

    // Son Bir Haftada Gelen
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son Bir Haftada Gelen"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-7}))));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-7}))));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }


    // Son Bir Ayda Gelen
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son Bir Ayda Gelen"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-30}))));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-30}))));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }

    // Son Bir Haftada Gelen
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(0,AllSides);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son 3 Ayda Gelen"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-90}))));
            } catch (bsoncxx::exception &e) {

            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::SikayetKey::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,make_document(kvp("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()-90}))));
            } catch (bsoncxx::exception &e) {

            }

            this->listSikayet(filter.view());
        });
    }

}

void Giris::Personel::CagriMerkezi::listSikayet(bsoncxx::document::view filterView)
{

    mMainContainer->clear();

    auto fluidContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);


    {
        auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::container_fluid);

        auto row = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            layout->addStretch(1);
            auto text = layout->addWidget(cpp14::make_unique<WText>("Sıra"));
            layout->addStretch(1);
            text->addStyleClass(Bootstrap::Label::Primary);
            text->setAttributeValue(Style::style,Style::font::size::s14px);
        }

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            layout->addStretch(1);
            auto text = layout->addWidget(cpp14::make_unique<WText>("Tarih"));
            layout->addStretch(1);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s14px);
        }

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            layout->addStretch(1);
            auto text = layout->addWidget(cpp14::make_unique<WText>("Durum"));
            layout->addStretch(1);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s14px);
        }

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            layout->addStretch(1);
            auto text = layout->addWidget(cpp14::make_unique<WText>("Konu"));
            layout->addStretch(1);
            text->addStyleClass(Bootstrap::Label::Danger);
            text->setAttributeValue(Style::style,Style::font::size::s14px);
        }

    }


    skip = 0;
    this->addlist(rContainer,bsoncxx::document::value(filterView));



}

void Giris::Personel::CagriMerkezi::initReportMenu()
{

    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);


    auto arrayList = array{};

    try {
        arrayList.append(SBLDKeys::SikayetKey::durumType::beklemede);
    } catch (bsoncxx::exception &e) {

    }

    try {
        arrayList.append(SBLDKeys::SikayetKey::durumType::devamediyor);
    } catch (bsoncxx::exception &e) {

    }

    try {
        arrayList.append(SBLDKeys::SikayetKey::durumType::tekrarAcildi);
    } catch (bsoncxx::exception &e) {

    }



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,make_document(kvp("$in",arrayList))));
    } catch (bsoncxx::exception &e) {

    }

    this->initReportList(rContainer,filter.view());

}

void Giris::Personel::CagriMerkezi::initReportList(WContainerWidget* rowContainer , bsoncxx::document::view filterView)
{

    rowContainer->clear();

    auto fContainer = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContanier = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContanier->addStyleClass(Bootstrap::Grid::row);

    {
        auto LayContainer = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());
        LayContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto vLayout = LayContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

        {
            auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>Gelen Şikayetler Raporu</h2>")),0,AlignmentFlag::Center);
        }
    }


    try {


        mongocxx::options::find findOptions;

        findOptions.limit(50);

        auto sortDoc = document{};

        try {
            sortDoc.append(kvp(SBLDKeys::oid,-1));
        } catch (bsoncxx::exception &e) {

        }


        findOptions.sort(sortDoc.view());


        auto cursor = this->db()->collection(SBLDKeys::SikayetKey::collection).find(filterView,findOptions);

        auto LayContainer = rContanier->addWidget(cpp14::make_unique<WContainerWidget>());

        LayContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto vLayout = LayContainer->setLayout(cpp14::make_unique<WVBoxLayout>());


        int row = 0;
        for( auto doc : cursor )
        {
            row++;

            {
                auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<h2>"+WString("{1}").arg(row).toUTF8()+"</h2>"),0,AlignmentFlag::Center);
            }

            try {
                auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<h4>"+doc[SBLDKeys::SikayetKey::mainKey::tarih].get_utf8().value.to_string()+"</h4>"),0,AlignmentFlag::Center);
            } catch (bsoncxx::exception &e) {
                auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<h4>Tarih Bilgisi Yok</h4>"),0,AlignmentFlag::Center);
            }

            try {

                auto filter = document{};

                try {
                    filter.append(kvp(SBLDKeys::TC::tcno,doc[SBLDKeys::SikayetKey::mainKey::vatandas].get_utf8().value));
                } catch (bsoncxx::exception &e) {

                }

                auto val = this->db()->collection(SBLDKeys::TC::collection).find_one(filter.view());

                if( val )
                {
                    if( !val.value().view().empty() )
                    {
                        try {
                            auto titlebaslik = vLayout->addWidget(cpp14::make_unique<WText>("<b> AdSoyad: "+val.value().view()[SBLDKeys::TC::isimsoyisim].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
                        } catch (bsoncxx::exception &e) {
                            auto titlebaslik = vLayout->addWidget(cpp14::make_unique<WText>("AdSoyad Yok"),0,AlignmentFlag::Center);
                        }

                        try {
                            auto titlebaslik = vLayout->addWidget(cpp14::make_unique<WText>("<b> Telefon: "+val.value().view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
                        } catch (bsoncxx::exception &e) {
                            auto titlebaslik = vLayout->addWidget(cpp14::make_unique<WText>("Telefon Yok"),0,AlignmentFlag::Center);
                        }
                    }
                }

            } catch (mongocxx::exception &e) {

            }


            try {
                auto titlebaslik = vLayout->addWidget(cpp14::make_unique<WText>("<b>"+doc[SBLDKeys::SikayetKey::mainKey::konu].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
            } catch (bsoncxx::exception &e) {
                auto titlebaslik = vLayout->addWidget(cpp14::make_unique<WText>("Konu Yok"),0,AlignmentFlag::Center);
            }

            try {
                auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("<b>"+doc[SBLDKeys::SikayetKey::mainKey::birim].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
            } catch (bsoncxx::exception &e) {
                auto titleBirim = vLayout->addWidget(cpp14::make_unique<WText>("Birim Atanmamış"),0,AlignmentFlag::Center);
            }

            try {
                auto mahalle = vLayout->addWidget(cpp14::make_unique<WText>("<b>"+doc[SBLDKeys::SikayetKey::mainKey::mahalle].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
            } catch (bsoncxx::exception &e) {
                auto mahalle = vLayout->addWidget(cpp14::make_unique<WText>("Mahalle Yok"),0,AlignmentFlag::Center);
            }


            {
                auto tanimText = vLayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string()),0,AlignmentFlag::Left|AlignmentFlag::Justify);
            }

            {
                auto tanimText = vLayout->addWidget(cpp14::make_unique<WText>("<b>ilgili Personel(ler)</b>"),0,AlignmentFlag::Left|AlignmentFlag::Justify);
            }
            try {
                auto gorevliViewList = doc[SBLDKeys::SikayetKey::mainKey::gorevli].get_array().value;


                for( auto gorevli : gorevliViewList )
                {
                    auto gorevliView = gorevli.get_document().view();
                    {
                        auto mahalle = vLayout->addWidget(cpp14::make_unique<WText>("<b>"+gorevliView[SBLDKeys::SikayetKey::GorevliType::adsoyad].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
                    }
                    {
                        auto mahalle = vLayout->addWidget(cpp14::make_unique<WText>("<b>"+gorevliView[SBLDKeys::SikayetKey::GorevliType::tel].get_utf8().value.to_string()+"</b>"),0,AlignmentFlag::Center);
                    }
                }

            } catch (bsoncxx::exception &e) {
                auto mahalle = vLayout->addWidget(cpp14::make_unique<WText>("Görevli Personel Yok"),0,AlignmentFlag::Center);
            }

            {
                auto tanimText = vLayout->addWidget(cpp14::make_unique<WText>("Cevaplar - Açıklamalar"),0,AlignmentFlag::Left);
            }

            {
                auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                container->setAttributeValue(Style::style,Style::Border::bottom::border("1px dotted black"));
            }
            {
                bool cevapExist = false;
                try {
                    auto asamaDoc = doc[SBLDKeys::SikayetKey::mainKey::asama].get_array().value;
                    for( auto item : asamaDoc )
                    {

                        if( item.get_document().view()[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::degisiklik)
                        {
                            try {
                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::degisim].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Değişim Yok")),0,AlignmentFlag::Left);
                                }

                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::tarih].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Tarih Yok")),0,AlignmentFlag::Left);
                                }

                                try {
                                    auto personelOid = item.get_document().view()[SBLDKeys::SikayetKey::asamakey::personel].get_document().value;
                                    vLayout->addWidget(cpp14::make_unique<WText>(personelOid[SBLDKeys::SikayetKey::asamakey::Personel::adSoyad].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Adsoyad Yok")),0,AlignmentFlag::Left);
                                }

                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::saat].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Saat Yok")),0,AlignmentFlag::Left);
                                }


                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::birim].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Birim Yok")),0,AlignmentFlag::Left);
                                }


                                cevapExist = true;

                                {
                                    auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                                    container->setAttributeValue(Style::style,Style::Border::bottom::border("1px dotted black"));
                                }

                            } catch (bsoncxx::exception &e) {
                                vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Değişiklik Yok")),0,AlignmentFlag::Left);
                                {
                                    auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                                    container->setAttributeValue(Style::style,Style::Border::bottom::border("1px dotted black"));
                                }
                            }
                        }

                        if( item.get_document().view()[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string() == SBLDKeys::SikayetKey::asamakey::tipKey::aciklama)
                        {
                            try {



                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::aciklama].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Açıklama Yok")),0,AlignmentFlag::Left);
                                }

                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::tarih].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Tarih Yok")),0,AlignmentFlag::Left);
                                }

                                try {
                                    auto personelOid = item.get_document().view()[SBLDKeys::SikayetKey::asamakey::personel].get_document().value;
                                    vLayout->addWidget(cpp14::make_unique<WText>(personelOid[SBLDKeys::SikayetKey::asamakey::Personel::adSoyad].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Adsoyad Yok")),0,AlignmentFlag::Left);
                                }

                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::saat].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Saat Yok")),0,AlignmentFlag::Left);
                                }


                                try {
                                    vLayout->addWidget(cpp14::make_unique<WText>(item.get_document().view()[SBLDKeys::SikayetKey::asamakey::birim].get_utf8().value.to_string()),0,AlignmentFlag::Left);
                                } catch (bsoncxx::exception &e) {
                                    vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Birim Yok")),0,AlignmentFlag::Left);
                                }
                                cevapExist = true;

                                {
                                    auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                                    container->setAttributeValue(Style::style,Style::Border::bottom::border("1px dotted black"));
                                }

                            } catch (bsoncxx::exception &e) {
                                vLayout->addWidget(cpp14::make_unique<WText>(e.what()+std::string(": Açıklama Yok")),0,AlignmentFlag::Left);
                                {
                                    auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                                    container->setAttributeValue(Style::style,Style::Border::bottom::border("1px dotted black"));
                                }
                            }
                        }




                    }
                } catch (bsoncxx::exception &e) {

                }

                if( !cevapExist )
                {
                    auto tanimText = vLayout->addWidget(cpp14::make_unique<WText>("<b>Henüz Cevap Yazılmamış</b>"),0,AlignmentFlag::Left);

                }

            }

            {
                auto container = vLayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
            }
        }




    } catch (mongocxx::exception &e) {

    }







    {

        auto rContanier_ = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rContanier_->addStyleClass(Bootstrap::Grid::row);


        auto imgContainer = rContanier_->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        imgContainer->setHeight(100);

        auto btn = imgContainer->addWidget(cpp14::make_unique<WPushButton>("Yazdır"));
        btn->clicked().connect([=](){


            WString str = WString("var prtContent = document.getElementById(\"{1}\");"
                                  "var WinPrint = window.open('', '', 'left=0,top=0,width=1024,height=900,toolbar=0,scrollbars=0,status=0');"
                                  "WinPrint.document.write(prtContent.innerHTML);"
                                  "WinPrint.document.close();"
                                  "WinPrint.focus();"
                                  "WinPrint.print();"
                                  "WinPrint.close();").arg(fContainer->id().c_str());

            //                WString str = WString("var prtContent = document.getElementById(\"{1}\");"
            //                "var WinPrint = window.open('', '', 'left=0,top=0,width=1024,height=900,toolbar=0,scrollbars=0,status=0');"
            //                "WinPrint.document.write(prtContent.innerHTML);"
            //                "WinPrint.document.close();"
            //                "WinPrint.focus();"
            //                "WinPrint.print();"
            //                "WinPrint.close();").arg(rowContainer->id().c_str());

            imgContainer->doJavaScript(str.toUTF8());
        });
    }


}

void Giris::Personel::CagriMerkezi::addlist(WContainerWidget *rContainer, bsoncxx::document::value filterValue)
{

    mongocxx::options::find findOptions;

    findOptions.limit(limit);
    findOptions.skip(skip);

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,-1));
    } catch (bsoncxx::exception &e) {

    }

    findOptions.sort(sortDoc.view());

    try {
        auto cursor = this->db()->collection(SBLDKeys::SikayetKey::collection).find(filterValue.view(),findOptions);

        int index = 0;
        for( auto doc : cursor )
        {
            index++;
            auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::container_fluid);
            _container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,124),this->getRandom(0,124),this->getRandom(0,124),.7));


            auto row = _container->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);
            row->decorationStyle().setCursor(Cursor::PointingHand);
            row->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string());
            row->clicked().connect([=](){
                this->setDetail(row->attributeValue(Style::dataoid).toUTF8());
            });

            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
                auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                auto text = layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(index + skip)));
                layout->addStretch(1);
                text->addStyleClass(Bootstrap::Label::Primary);
                text->setAttributeValue(Style::style,Style::font::size::s14px);
            }

            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
                auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                try {
                    auto text = layout->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(doc[SBLDKeys::SikayetKey::mainKey::julianDay].get_int64().value).toString("dd/MM/yyyy")));
                    text->addStyleClass(Bootstrap::Label::Warning);
                } catch (bsoncxx::exception &e) {
                    auto text = layout->addWidget(cpp14::make_unique<WText>(e.what()));
                    text->addStyleClass(Bootstrap::Label::Warning);
                }
                layout->addStretch(1);

            }

            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
                auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                try {
                    auto text = layout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {
                    auto text = layout->addWidget(cpp14::make_unique<WText>(e.what()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
                }


                layout->addStretch(1);
            }

            {
                auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                try {
                    QTextDocument textEdit;
                    textEdit.setHtml(doc[SBLDKeys::SikayetKey::mainKey::konu].get_utf8().value.to_string().c_str());
                    QString str = textEdit.toPlainText();
                    if( str.count() > 300 )
                    {
                        str = str.mid(0,300);
                        str.append("...");
                    }
                    auto text = layout->addWidget(cpp14::make_unique<WText>(str.toStdString().c_str()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
                } catch (bsoncxx::exception &e) {
                    auto text = layout->addWidget(cpp14::make_unique<WText>(e.what()));
                    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
                }
                layout->addStretch(1);
            }



        }


    } catch (mongocxx::exception &e) {
        std::cout << "CXX Exception: " << e.what() << std::endl;
    }

    if( this->count(SBLDKeys::SikayetKey::collection,filterValue ) > skip )
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));
        btn->addStyleClass(Bootstrap::Button::Primary);
        btn->clicked().connect([=](){


            skip += limit;
            this->addlist(rContainer,filterValue);

            rContainer->removeWidget(container);
        });

    }
}

void Giris::Personel::CagriMerkezi::setDetail(std::string oid)
{




    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->db()->collection(SBLDKeys::SikayetKey::collection).find_one(make_document(kvp(SBLDKeys::oid,bsoncxx::oid{oid})));

        if( val_ )
        {
            if( val_.value().view().empty() )
            {
                this->showMessage("Hata","No Document");
                return;
            }else{
                val = val_.value();
            }
        }else{
            this->showMessage("Hata","No Document");
            return;
        }

    } catch (mongocxx::exception &e) {
        this->showMessage("HATA","Error: " , e);
        return;
    }


    mMainContainer->clear();

    auto fluidContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fluidContainer->setMargin(20,Side::Top|Side::Bottom);

    auto rContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    auto view = val.view();


    // Vatandaş
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Vatandaş"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Danger);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        try {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::mainKey::vatandasadSoyad].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }
    }

    // Telefon Numarası
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Telefon"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Danger);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        std::string tcno = "null";
        try {
            tcno = view[SBLDKeys::SikayetKey::mainKey::vatandas].get_utf8().value.to_string();
        } catch (bsoncxx::exception &e) {
        }

        if( tcno != "null" )
            try {
            auto val = this->db()->collection(SBLDKeys::TC::collection).find_one(make_document(kvp(SBLDKeys::TC::tcno,tcno)));
            if( val )
            {
                if( !val.value().view().empty() )
                {
                    try {
                        auto text = vLayout->addWidget(cpp14::make_unique<WText>(val.value().view()[SBLDKeys::TC::cepTel].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
                    } catch (bsoncxx::exception &e) {
                        auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
                    }
                }
            }
        } catch (mongocxx::exception &e) {

        }
    }

    //Tarih
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Tarih"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Primary);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        try {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::mainKey::tarih].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }
    }

    //Mahalle
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Mahalle"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Primary);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        try {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::mainKey::mahalle].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }
    }

    //Durum
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Durum"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Primary);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        try {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::mainKey::durum].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }
    }

    //Birim
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Birim"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Primary);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        try {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::mainKey::birim].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Konu"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Danger);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        try {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::mainKey::konu].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Adres"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Default);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        try {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::SikayetKey::mainKey::tamadres].get_utf8().value.to_string()),0,AlignmentFlag::Middle);
        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black"));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Cevaplar"),0,AlignmentFlag::Middle);
        title->addStyleClass(Bootstrap::Label::Warning);
        title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);

        try {
            auto arList = view[SBLDKeys::SikayetKey::mainKey::asama].get_array().value;

            for( auto ar : arList )
            {
                auto doc = ar.get_document().value;

                auto fContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
                fContainer->setMargin(20,Side::Top|Side::Bottom);


                auto row = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                row->addStyleClass(Bootstrap::Grid::row);
                row->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.75));

                {
                    auto CContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    CContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    auto _layout = CContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto _title = _layout->addWidget(cpp14::make_unique<WText>("Tip"),0,AlignmentFlag::Middle);
                    _title->addStyleClass(Bootstrap::Label::Success);
                    _title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
                    try {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string()));
                        _text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    } catch (bsoncxx::exception &e) {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(e.what()));
                    }
                }

                {
                    auto CContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    CContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    auto _layout = CContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto _title = _layout->addWidget(cpp14::make_unique<WText>("Tarih"),0,AlignmentFlag::Middle);
                    _title->addStyleClass(Bootstrap::Label::Success);
                    _title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
                    try {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::asamakey::tarih].get_utf8().value.to_string()));
                        _text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    } catch (bsoncxx::exception &e) {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(e.what()));
                    }
                }

                {
                    auto CContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    CContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    auto _layout = CContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto _title = _layout->addWidget(cpp14::make_unique<WText>("Saat"),0,AlignmentFlag::Middle);
                    _title->addStyleClass(Bootstrap::Label::Success);
                    _title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
                    try {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::asamakey::saat].get_utf8().value.to_string()));
                        _text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    } catch (bsoncxx::exception &e) {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(e.what()));
                    }
                }

                {
                    auto CContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                    CContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    auto _layout = CContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                    auto _title = _layout->addWidget(cpp14::make_unique<WText>("Personel"),0,AlignmentFlag::Middle);
                    _title->addStyleClass(Bootstrap::Label::Success);
                    _title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
                    try {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::asamakey::personel].get_document().view()[SBLDKeys::SikayetKey::asamakey::Personel::adSoyad].get_utf8().value.to_string()));
                        _text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                    } catch (bsoncxx::exception &e) {
                        auto _text = _layout->addWidget(cpp14::make_unique<WText>(e.what()));
                    }
                }


                try {
                    auto tip = doc[SBLDKeys::SikayetKey::asamakey::tip].get_utf8().value.to_string();

                    if( tip == SBLDKeys::SikayetKey::asamakey::tipKey::aciklama )
                    {
                        auto CContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                        CContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                        auto _layout = CContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        auto _title = _layout->addWidget(cpp14::make_unique<WText>("Açıklama"),0,AlignmentFlag::Middle);
                        _title->addStyleClass(Bootstrap::Label::info);
                        _title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
                        try {
                            auto _text = _layout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::asamakey::aciklama].get_utf8().value.to_string()));
                            _text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        } catch (bsoncxx::exception &e) {
                            auto _text = _layout->addWidget(cpp14::make_unique<WText>(e.what()));
                        }
                    }

                    if( tip == SBLDKeys::SikayetKey::asamakey::tipKey::degisiklik )
                    {
                        auto CContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                        CContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                        auto _layout = CContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                        auto _title = _layout->addWidget(cpp14::make_unique<WText>("Değişim"),0,AlignmentFlag::Middle);
                        _title->addStyleClass(Bootstrap::Label::info);
                        _title->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
                        try {
                            auto _text = _layout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::asamakey::degisim].get_utf8().value.to_string()));
                            _text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        } catch (bsoncxx::exception &e) {
                            auto _text = _layout->addWidget(cpp14::make_unique<WText>(e.what()));
                        }
                    }

                } catch (bsoncxx::exception &e) {

                }

            }


        } catch (bsoncxx::exception &e) {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Middle);
        }

    }

}

Giris::Personel::BaskanMesajlar::BaskanMesajlar(mongocxx::database *_database, bsoncxx::document::value _user)
    :ContainerWidget::ContainerWidget(_database,_user,"Mesajlar")
{
    this->init();
}

void Giris::Personel::BaskanMesajlar::init()
{

    this->getRowContainer()->clear();

    // Bugün Gelen Mesajlar
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(5,Side::Left|Side::Right);
        container->setPadding(30,Side::Top|Side::Bottom);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Bugün Gelen Mesajlar"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::julianDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
            } catch (bsoncxx::exception &e) {

            }

            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){

            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::julianDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e );
                return;
            }
            this->listMesajlar(filter.view(),"Bugün Gelen Mesajlar");
        });
    }

    // Cevaplanmayanlar
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(5,Side::Left|Side::Right);
        container->setPadding(30,Side::Top|Side::Bottom);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Cevaplanmayan"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,make_document(kvp("$exists",false))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e );
                return;
            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){

            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,make_document(kvp("$exists",false))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e );
                return;
            }

            this->listMesajlar(filter.view(),"Cevaplanmayan Mesajlar");
        });
    }

    // Bekleyenler
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(5,Side::Left|Side::Right);
        container->setPadding(30,Side::Top|Side::Bottom);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Sizde Bekleyenler"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,make_document(kvp("$exists",false))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e );
                return;
            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::birim,make_document(kvp("$exists",false))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e );
                return;
            }
            this->listMesajlar(filter.view(),"Sizde Bekleyenler");
        });
    }

    // Bekleyenler
    {
        std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setPadding(5,Side::Left|Side::Right);
        container->setPadding(30,Side::Top|Side::Bottom);
        auto borderContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        borderContainer->setHeight(75);
        borderContainer->setPadding(WLength::Auto,Side::Bottom|Side::Top);
        borderContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white")+Style::background::color::rgba(this->getRandom(0,127),this->getRandom(0,127),this->getRandom(0,127),0.7));
        auto vLayout = borderContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Cevaplananlar"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        {
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,make_document(kvp("$exists",true))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e );
                return;
            }
            auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Adet").arg(this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).count(filter.view()))),0,AlignmentFlag::Middle|AlignmentFlag::Center);
            text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(WLength::Auto,Side::Top|Side::Bottom);
        }
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            auto filter = document{};
            try {
                filter.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,make_document(kvp("$exists",true))));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e );
                return;
            }
            this->listMesajlar(filter.view(),"Cevaplananlar");
        });
    }

    mMainContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);


}

void Giris::Personel::BaskanMesajlar::listMesajlar( bsoncxx::document::view filterView,std::string titleText)
{
    mMainContainer->clear();
    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        container->setHeight(50);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        auto title = vLayout->addWidget(cpp14::make_unique<WText>(titleText));
        vLayout->addStretch(1);
        title->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }
    skip = 0 ;
    this->addList(rContainer,filterView);
}

void Giris::Personel::BaskanMesajlar::addList(WContainerWidget *rContainer, bsoncxx::document::view filterView)
{


    mongocxx::options::find findOptions;

    findOptions.limit(this->limit);

    findOptions.skip(skip);

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::oid,-1));
    } catch (bsoncxx::exception &e) {

    }

    findOptions.sort(sortDoc.view());


    try {
        auto cursor = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).find(filterView,findOptions);

        auto fContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto _rCOntainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _rCOntainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto fContainer_ = _rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
            fContainer_->addStyleClass(Bootstrap::Grid::container_fluid);

            auto _rCOntainer_ = fContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
            _rCOntainer_->addStyleClass(Bootstrap::Grid::row);
            _rCOntainer_->setPadding(5,Side::Top|Side::Bottom);


            // TARIH
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>("Tarih"));
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
            }

            // Saat
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>("Saat"));
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
            }

            // Ad Soyad
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>("Ad Soyad"));
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
            }

            // Telefon
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>("Telefon"));
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
            }

            // Mesaj
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                auto text = container->addWidget(cpp14::make_unique<WText>("Mesaj"));
                text->addStyleClass(Bootstrap::Label::Danger);
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
            }
        }

        for( auto doc : cursor )
        {

            auto fContainer_ = _rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
            fContainer_->addStyleClass(Bootstrap::Grid::container_fluid);
            fContainer_->setMargin(5,Side::Top|Side::Bottom);
            fContainer_->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string());
            fContainer_->clicked().connect([=](){
                this->setMesajDetail(fContainer_->attributeValue(Style::dataoid).toUTF8());
            });
            fContainer_->decorationStyle().setCursor(Cursor::PointingHand);

            auto _rCOntainer_ = fContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
            _rCOntainer_->addStyleClass(Bootstrap::Grid::row);
            _rCOntainer_->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));


            // TARIH
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(doc[SBLDKeys::Yonetim::Baskan::MESAJ::julianDate].get_int64().value).toString("dd/MM/yyyy")));
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

            }

            // Saat
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::saat].get_utf8().value.to_string()));
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));

            }

            // Ad Soyad
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::ad].get_utf8().value.to_string().c_str()));
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }

            // Telefon
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::tel].get_utf8().value.to_string().c_str()));
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue));
            }

            // Mesaj
            {
                auto container = _rCOntainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                container->setAttributeValue(Style::style,Style::Border::top::border("1px solid white"));
                auto text = container->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Yonetim::Baskan::MESAJ::mesaj].get_utf8().value.to_string().c_str()));
                text->setMargin(WLength::Auto,Side::Right|Side::Left);
                text->setAttributeValue(Style::style,Style::font::size::s12px+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            }

        }

        if( this->count(SBLDKeys::Yonetim::Baskan::MESAJ::collection,filterView) > skip+limit ){
            auto container = _rCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

            auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));
            btn->setMargin(WLength::Auto,Side::Left|Side::Right);
            btn->setMargin(15,Side::Top|Side::Bottom);
            btn->addStyleClass(Bootstrap::Button::Primary);
            btn->clicked().connect([=](){
                skip += limit;
                this->addList(rContainer,filterView);
                _rCOntainer->removeWidget(container);
            });
        }


    } catch (mongocxx::exception &e) {

    }



}

void Giris::Personel::BaskanMesajlar::setMesajDetail(std::string oid)
{


    mMainContainer->clear();
    auto fContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    fContainer->setMargin(15,Side::Top|Side::Bottom);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {

    }



    try {
        auto val = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).find_one(filter.view());

        if( val )
        {
            if( !val.value().view().empty() )
            {
                auto view = val.value().view();

                // Tarih
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(view[SBLDKeys::Yonetim::Baskan::MESAJ::julianDate].get_int64().value).toString("dd/MM/yyyy").toUTF8()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // Saat
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::saat].get_utf8().value.to_string()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // AD
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::ad].get_utf8().value.to_string()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // TEL
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::tel].get_utf8().value.to_string()));
                        text->addStyleClass(Bootstrap::Label::Primary);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }


                // Mesaj
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->setPadding(25,Side::Top|Side::Bottom);
                    container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::MESAJ::mesaj].get_utf8().value.to_string()));
                        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s12px+Style::font::weight::bold);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>(e.what()));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // Birim
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->setPadding(15,Side::Top|Side::Bottom);
                    container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                    try {
                        auto text = container->addWidget(cpp14::make_unique<WText>("Görevli Birim: "+view[SBLDKeys::Yonetim::Baskan::MESAJ::birim].get_utf8().value.to_string()));
                        text->addStyleClass(Bootstrap::Label::Warning);
                        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s12px+Style::font::weight::bold);
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>("Birime Yönlendirilmemiş"));
                        text->addStyleClass(Bootstrap::Label::Danger);
                    }
                }

                // Cevap Var mı
                bool cevapExist = false;
                bsoncxx::document::value cevapValue(document{}.view());
                {
                    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    container->setPadding(25,Side::Top|Side::Bottom);
                    container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                    try {
                        auto cevapDoc = view[SBLDKeys::Yonetim::Baskan::MESAJ::cevap].get_document().view();
                        cevapValue = bsoncxx::document::value(cevapDoc);
                        cevapExist = true;


                        auto _fContainer_ = container->addWidget(cpp14::make_unique<WContainerWidget>());
                        _fContainer_->addStyleClass(Bootstrap::Grid::container_fluid);

                        auto _rContainer_ = _fContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
                        _rContainer_->addStyleClass(Bootstrap::Grid::row);


                        {
                            auto TarihText = _rContainer_->addWidget(cpp14::make_unique<WText>("Tarih: "+WDate::fromJulianDay(cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::julianDate].get_int64().value).toString("dd/MM/yyyy").toUTF8()));
                            TarihText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s10px+Style::font::weight::bold);
                            TarihText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                        }
                        {
                            auto SaatText = _rContainer_->addWidget(cpp14::make_unique<WText>("Saat: "+cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::saat].get_utf8().value.to_string()));
                            SaatText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s10px+Style::font::weight::bold);
                            SaatText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                        }
                        {
                            auto YazarText = _rContainer_->addWidget(cpp14::make_unique<WText>("Yazar: "+cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::personelAdSoyad].get_utf8().value.to_string()));
                            YazarText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s10px+Style::font::weight::bold);
                            YazarText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                        }
                        {
                            auto CevapText = _rContainer_->addWidget(cpp14::make_unique<WText>(cevapDoc[SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::cevap].get_utf8().value.to_string()));
                            CevapText->setAttributeValue(Style::style,Style::color::color(Style::color::White::FloralWhite)+Style::font::size::s12px+Style::font::weight::bold);
                            CevapText->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
                            CevapText->setMargin(10,Side::Top|Side::Bottom);
                        }
                    } catch (bsoncxx::exception &e) {
                        auto text = container->addWidget(cpp14::make_unique<WText>("Cevaplanmamış Mesaj"));
                        text->addStyleClass(Bootstrap::Label::Danger);
                        text->setAttributeValue(Style::style,Style::font::size::s14px);
                    }
                }

                if( cevapExist )
                {


                }else{

                    auto cevapStack = rContainer->addWidget(cpp14::make_unique<WStackedWidget>());
                    cevapStack->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::CubicBezier,250));




                    // Cevap Ekleme Container
                    {
                        auto container = cpp14::make_unique<WContainerWidget>();
                        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                        vLayout->addStretch(1);
                        auto text = vLayout->addWidget(cpp14::make_unique<CevapYazWidget>(this->db(),this->User(),view[SBLDKeys::oid].get_oid().value.to_string()));
                        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                        text->SuccessCevap().connect([=](){
                            cevapStack->setCurrentIndex(2);
                        });
                        vLayout->addStretch(1);
                        cevapStack->insertWidget(0,std::move(container));
                    }

                    // Birime Yönlendirme Container
                    {
                        auto container= cpp14::make_unique<WContainerWidget>();
                        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                        vLayout->addStretch(1);
                        auto birimGorevList = vLayout->addWidget(cpp14::make_unique<BirimGorevlendirmeList>(this->db(),this->User()));
                        birimGorevList->ClickBack().connect([=](){
                            cevapStack->setCurrentIndex(2);
                        });
                        birimGorevList->setAttributeValue(Style::dataoid,view[SBLDKeys::oid].get_oid().value.to_string());

                        birimGorevList->ClickBirim().connect([=](std::string birim){

                            auto _filter = document{};

                            try {
                                _filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{birimGorevList->attributeValue(Style::dataoid).toUTF8()}));
                            } catch (bsoncxx::exception &e) {

                            }

                            std::cout << "_FILTER: " << bsoncxx::to_json(_filter.view()) << std::endl;

                            auto setDoc = document{};

                            try {
                                setDoc.append(kvp("$set",make_document(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::birim,birim))));
                            } catch (bsoncxx::exception &e) {

                            }

                            try {
                                auto upt = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).update_one(_filter.view(),setDoc.view());
                                if( upt )
                                {
                                    if( upt.value().modified_count() )
                                    {
                                        this->showMessage("Bilgi",WString("{1} Görevlendirmesi Yapıldı").arg(birim.c_str()).toUTF8());
                                        cevapStack->setCurrentIndex(2);
                                    }else{
                                        this->showMessage("Hata","Dosya Değiştirilemedi");
                                    }
                                }else{
                                    this->showMessage("Hata","Server Cevap Vermedi");
                                }
                            } catch (mongocxx::exception &e) {
                                this->showMessage("Hata","Error: ", e);
                            }


                        });
                        vLayout->addStretch(1);
                        cevapStack->insertWidget(1,std::move(container));
                    }

                    // Seçenekler Container
                    {
                        auto container= cpp14::make_unique<WContainerWidget>();

                        auto fluidContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                        fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

                        auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                        rowContainer->addStyleClass(Bootstrap::Grid::row);

                        {
                            auto birimContanier = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                            birimContanier->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                            birimContanier->setHeight(40);
                            birimContanier->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                            auto vLayout = birimContanier->setLayout(cpp14::make_unique<WVBoxLayout>());
                            vLayout->addStretch(1);
                            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Birime Yönlendir"));
                            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            vLayout->addStretch(1);
                            birimContanier->clicked().connect([=](){
                                cevapStack->setCurrentIndex(1);
                            });
                            birimContanier->decorationStyle().setCursor(Cursor::PointingHand);
                        }
                        {
                            auto birimContanier = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                            birimContanier->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
                            birimContanier->setHeight(40);
                            birimContanier->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom(),0.7));
                            auto vLayout = birimContanier->setLayout(cpp14::make_unique<WVBoxLayout>());
                            vLayout->addStretch(1);
                            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Cevap Yaz"));
                            text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
                            vLayout->addStretch(1);
                            birimContanier->clicked().connect([=](){
                                cevapStack->setCurrentIndex(0);
                            });
                            birimContanier->decorationStyle().setCursor(Cursor::PointingHand);
                        }
                        cevapStack->insertWidget(2,std::move(container));
                    }
                    cevapStack->setCurrentIndex(2);
                    cevapStack->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

                }

            }
        }


    } catch (mongocxx::exception &e) {

    }

}

Giris::Personel::BaskanMesajlar::BirimGorevlendirmeList::BirimGorevlendirmeList(mongocxx::database *_database, bsoncxx::document::value _user)
    :ContainerWidget::ContainerWidget(_database,_user)
{


    {
        auto mContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
        auto btn = mContainer->addWidget(cpp14::make_unique<WPushButton>("Geri"));
        btn->addStyleClass(Bootstrap::Button::Primary);
        btn->setMargin(WLength::Auto,AllSides);
        mContainer->setHeight(40);
        btn->clicked().connect([=](){
            _mClickBack.emit(NoClass());
        });
    }
    {
        auto mContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_10+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto text = mContainer->addWidget(cpp14::make_unique<WText>("Birime Yönlendir"));
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        text->setMargin(WLength::Auto,AllSides);
        mContainer->setHeight(40);
    }



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Birimler::alici,make_document(kvp("$ne","NULL"))));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
    }



    auto birimList = this->getBirimList();

    for( decltype (birimList.size()) i = 0 ; i < birimList.size() ; i++ )
    {
        auto item = birimList.at(i);
        auto mContainer = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto fContainer = mContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);


        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(item.birim));
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s14px+Style::color::color(Style::color::White::AliceBlue));
        auto btn = vLayout->addWidget(cpp14::make_unique<WPushButton>("Yönlendir"));
        btn->addStyleClass(Bootstrap::Button::Warning);

        btn->clicked().connect([=](){
            mClickBirim.emit(item.birim);
        });
        vLayout->addStretch(1);
        mContainer->setHeight(100);
        mContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white"));

    }




}

std::vector<Giris::Personel::BaskanMesajlar::BirimGorevlendirmeList::BirimItem> Giris::Personel::BaskanMesajlar::BirimGorevlendirmeList::getBirimList() const
{
    auto _filter = document{};

    auto notSelect = array{};

    try {
        notSelect.append(make_document(kvp(SBLDKeys::Birimler::haberlesmekodu,"0")));
    } catch (bsoncxx::exception &e) {

    }

    try {
        notSelect.append(make_document(kvp(SBLDKeys::Birimler::haberlesmekodu,"-1")));
    } catch (bsoncxx::exception &e) {

    }

    try {
        _filter.append(kvp("$nor",notSelect));
    } catch (bsoncxx::exception &e) {

    }

    std::vector<BirimItem> BirimList;
    try {
        auto cursor = this->db()->collection(SBLDKeys::Birimler::collection).find(_filter.view());
        for( auto doc : cursor )
        {
            BirimItem item;
            try {
                item.birim = doc[SBLDKeys::Birimler::birim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.birim = e.what();
            }
            try {
                item.oid = doc[SBLDKeys::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                item.oid = e.what();
            }
            if( item.birim != "NULL" )
            {
                BirimList.push_back(item);
            }

        }
    } catch (mongocxx::exception &e) {

    }


    return BirimList;
}

Giris::Personel::BaskanMesajlar::CevapYazWidget::CevapYazWidget(mongocxx::database *_database, bsoncxx::document::value _user , std::string mesajoid)
    :ContainerWidget::ContainerWidget(_database,_user),mesajOid(mesajoid)
{
    this->getRowContainer()->addWidget(cpp14::make_unique<WText>("Cevap Yaz"))->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    auto textEdit = this->getRowContainer()->addWidget(cpp14::make_unique<WTextEdit>());
    textEdit->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    textEdit->setText("Cevabı, Bu Yazıyı Silip Buraya Yazınız");

    auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Cevabı Ekle"));
    btn->addStyleClass(Bootstrap::Button::Primary);

    btn->setMargin(WLength::Auto,Side::Right|Side::Left);

    btn->clicked().connect([=](){

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{mesajOid}));
        } catch (bsoncxx::exception &e) {

        }

        auto doc = document{};
        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::cevap,textEdit->text().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::julianDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
        } catch (bsoncxx::exception &e) {

        }



        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::saat,WTime::currentTime().toString("hh:mm").toUTF8()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::CEVAP::personelAdSoyad,this->User().view()[SBLDKeys::Personel::ad].get_utf8().value.to_string()));
        } catch (bsoncxx::exception &e) {

        }

        auto setDoc = document{};

        try {
            setDoc.append(kvp("$set",make_document(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::cevap,doc))));
        } catch (bsoncxx::exception &e) {

        }

        try {
            auto upt = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).update_one(filter.view(),setDoc.view());
            if( upt )
            {
                if( upt.value().modified_count() )
                {
                    this->showMessage("Bilgi","Cevap Eklendi");
                    this->_SuccessCevap.emit(NoClass());
                }else{
                    this->showMessage("Hata","Dosya Set Edilemedi");
                }
            }else{
                this->showMessage("Hata","Server Cevap Vermedi");
            }
        } catch (mongocxx::exception &e) {
            this->showMessage("Hata","Error: " , e);
        }


    });

}

Giris::Personel::ArizaKaydi::ArizaKaydi(mongocxx::database *_database, bsoncxx::document::value _user)
    :ContainerWidget::ContainerWidget( _database , _user,"Arıza Kayıtları")
{

    addWidget(cpp14::make_unique<WText>("Ariza Kaydı"));

    addWidget(cpp14::make_unique<WText>(this->User().view()[SBLDKeys::Personel::birimi].get_utf8().value.to_string().c_str()));



}















