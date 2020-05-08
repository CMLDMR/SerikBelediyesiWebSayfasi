



#include "giriswidget.h"
#include "yenilikler.h"
#include <QLatin1String>

#include "anonswidget.h"
#include "../url.h"

#include "SerikBelediyesiWebSayfasi/srcV2/kadinailestock.h"

#include "SerikBelediyesiWebSayfasi/srcV2/girisCikisWidget/giriscikiswidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetimyonetim.h"
#include "SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeclient.h"
#include "SerikBelediyesiWebSayfasi/srcV2/dilekceyonetim.h"
#include "SerikBelediyesiWebSayfasi/srcV2/talepler/talepyonetim.h"
#include "SerikBelediyesiWebSayfasi/srcV2/talepler/talepvatandasarayuz.h"
#include "SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcetcyonetim.h"
#include "SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/bilgiedinmebasvuruwidget.h"
#include "tc.h"
#include "SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/basvurularim.h"

#include "SerikBelediyesiWebSayfasi/srcV2/meclis/meclispage.h"
#include "SerikBelediyesiWebSayfasi/srcV2/personel/personelmanagerpage.h"

#include "SerikBelediyesiWebSayfasi/srcV2/stok/stokkategoripage.h"



Giris::GirisWidget::GirisWidget(mongocxx::database *_db)
    :WContainerWidget(),
      db(_db),
      User(document{}.view()),
      mBirimManager( new SerikBLDCore::BirimManager(new SerikBLDCore::DB(_db)))
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
    mContentContainer->addStyleClass (Bootstrap::Grid::row);

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

//    auto loginWidgetv2 = mContentContainer->addWidget(cpp14::make_unique<LoginWidgetV2>(new SerikBLDCore::DB(db)));
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

    std::cout << "Direl Perosnel" << std::endl;

    mBirimManager->setLimit (100);
    mBirimManager->UpdateList ();


    mContentContainer->clear();
    this->User = vatandas;

    mPageTitle->setText("www.serik.bel.tr");

    mUser = new SerikBLDCore::User(this->db,this->User);

    if( mUser->Statu () == SerikBLDCore::IK::Statu::Mudur )
    {
        if( mUser->mudurlukList ().count () )
        {
            mContentContainer->clear();

            auto container = mContentContainer->addWidget (cpp14::make_unique<WText>("<h5>Müdürlüğünü Yaptığınız Birimler</h5>",TextFormat::UnsafeXHTML));
            container->addStyleClass (Bootstrap::Grid::col_full_12);
            container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
            container->setMargin (25,Side::Top|Side::Bottom);
            mContentContainer->setMargin (100,Side::Bottom);


            {
                auto _container = mContentContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                           Bootstrap::Grid::Medium::col_md_3+
                                           Bootstrap::Grid::Small::col_sm_4+
                                           Bootstrap::Grid::ExtraSmall::col_xs_6);
                auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                vLayout->addStretch (1);
                vLayout->addWidget (cpp14::make_unique<WText>("<h5>"+ mUser->Birimi ()+"</h5>",TextFormat::UnsafeXHTML));
                vLayout->addStretch (1);
                container->addStyleClass ("boxShadow boxRadius");
                container->setHeight (150);
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::DeepPink)+
                                              Style::color::color (Style::color::White::Snow));

                container->decorationStyle ().setCursor (Cursor::PointingHand);

                container->clicked().connect ([=](){
                    mContentContainer->clear();
                    User = mUser->Value ();
                    mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,&User));
                });
            }

            for( auto birimOid : mUser->mudurlukList () )
            {
                for( auto item : mBirimManager->List () )
                {
                    if( birimOid.to_string () == item.oid ().value ().to_string () )
                    {

                        auto _container = mContentContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                                  Bootstrap::Grid::Medium::col_md_3+
                                                  Bootstrap::Grid::Small::col_sm_4+
                                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
                        auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                        vLayout->addStretch (1);
                        vLayout->addWidget (cpp14::make_unique<WText>("<h5>"+item.birimAdi ().toStdString ()+"</h5>",TextFormat::UnsafeXHTML));
                        vLayout->addStretch (1);
                        container->addStyleClass ("boxShadow boxRadius");
                        container->setHeight (150);
                        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::DodgerBlue)+
                                                      Style::color::color (Style::color::White::Snow));

                        container->decorationStyle ().setCursor (Cursor::PointingHand);

                        container->clicked().connect ([=](){
                            mContentContainer->clear();
                            mUser->setWorkingBirim (item.birimAdi ());
                            User = mUser->Value ();
                            mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,&User));
                        });

                    }

                }
            }
        }else{
            mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,&User));
        }

    }else{
        mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,&User));
    }


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
    mContentContainer->clear();
    mContentContainer->addWidget(cpp14::make_unique<SivilWidget>(db,&User));
}

void Giris::GirisWidget::initPersonel()
{

    mContentContainer->clear();

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
                    mUser = new SerikBLDCore::User(this->db,var.value ());
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


    std::cout << "Müdürlük Sayısı: " << mUser->mudurlukList ().count () << std::endl;

    mBirimManager->setLimit (100);
    mBirimManager->UpdateList ();

    if( mUser->mudurlukList ().count () )
    {

        auto container = mContentContainer->addWidget (cpp14::make_unique<WText>("<h5>Müdürlüğünü Yaptığınız Birimler</h5>",TextFormat::UnsafeXHTML));
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        container->setMargin (25,Side::Top|Side::Bottom);
        mContentContainer->setMargin (100,Side::Bottom);

        {
            auto _container = mContentContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
            auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            vLayout->addStretch (1);
            vLayout->addWidget (cpp14::make_unique<WText>("<h5>"+mUser->Birimi ()+"</h5>",TextFormat::UnsafeXHTML));
            vLayout->addStretch (1);
            container->addStyleClass ("boxShadow boxRadius");
            container->setHeight (150);
            container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::DeepPink)+
                                          Style::color::color (Style::color::White::Snow));

            container->decorationStyle ().setCursor (Cursor::PointingHand);

            container->clicked().connect ([=](){
                mContentContainer->clear();
                User = mUser->Value ();
                mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,&User));
            });
        }


        for( auto birimOid : mUser->mudurlukList () )
        {
            for( auto item : mBirimManager->List () )
            {
                if( birimOid.to_string () == item.oid ().value ().to_string () )
                {

                    auto _container = mContentContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                    _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                              Bootstrap::Grid::Medium::col_md_3+
                                              Bootstrap::Grid::Small::col_sm_4+
                                              Bootstrap::Grid::ExtraSmall::col_xs_6);
                    auto container = _container->addWidget(cpp14::make_unique<WContainerWidget>());
                    auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
                    vLayout->addStretch (1);
                    vLayout->addWidget (cpp14::make_unique<WText>("<h5>"+item.birimAdi ().toStdString ()+"</h5>",TextFormat::UnsafeXHTML));
                    vLayout->addStretch (1);
                    container->addStyleClass ("boxShadow boxRadius");
                    container->setHeight (150);
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::DodgerBlue)+
                                                  Style::color::color (Style::color::White::Snow));

                    container->decorationStyle ().setCursor (Cursor::PointingHand);

                    container->clicked().connect ([=](){
                        mContentContainer->clear();
                        mUser->setWorkingBirim (item.birimAdi ());
                        User = mUser->Value ();
                        mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,&User));
                    });

                }

            }
        }
    }else{
        mContentContainer->addWidget(cpp14::make_unique<Personel::PersonelWidget>(db,&User));
    }




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

    stream.writeTextElement("Command", SMSCONFIG::Command.c_str());
    stream.writeTextElement("PlatformID", SMSCONFIG::PlatformID.c_str());
    stream.writeTextElement("ChannelCode", SMSCONFIG::ChannelCode.c_str ());
    stream.writeTextElement("UserName", SMSCONFIG::UserName.c_str ());
    stream.writeTextElement("PassWord", SMSCONFIG::Password.c_str ());
    stream.writeTextElement("Mesgbody", sms );
    stream.writeTextElement("Numbers", mTelefonNumarasi->text().toUTF8().c_str());
    stream.writeTextElement("Type", SMSCONFIG::Type.c_str ());
    stream.writeTextElement("Concat", SMSCONFIG::Concat.c_str());
    stream.writeTextElement("Originator", SMSCONFIG::Originator.c_str ());

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

    stream.writeTextElement("Command" , SMSCONFIG::Command.c_str());
    stream.writeTextElement("PlatformID" , SMSCONFIG::PlatformID.c_str());
    stream.writeTextElement("ChannelCode" , SMSCONFIG::ChannelCode.c_str ());
    stream.writeTextElement("UserName" , SMSCONFIG::UserName.c_str ());
    stream.writeTextElement("PassWord" , SMSCONFIG::Password.c_str ());
    stream.writeTextElement("Mesgbody" , sms.c_str() );
    stream.writeTextElement("Numbers" , numara.c_str() );
    stream.writeTextElement("Type" , SMSCONFIG::Type.c_str () );
    stream.writeTextElement("Concat" , SMSCONFIG::Concat.c_str() );
    stream.writeTextElement("Originator" , SMSCONFIG::Originator.c_str () );

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
            gLayout->addWidget(cpp14::make_unique<WText>("Telefon Numarası"),0,0,AlignmentFlag::Center|AlignmentFlag::Middle);
            mTelefonNumarasi = gLayout->addWidget(cpp14::make_unique<WLineEdit>(),0,1,AlignmentFlag::Center);
            mTelefonNumarasi->setPlaceholderText("Telefon Numarasını Giriniz");
            mTelefonNumarasi->setWidth(150);
            mTelefonNumarasi->setInputMask("99999999999");
        }
        {
            gLayout->addWidget(cpp14::make_unique<WText>("Şifreniz"),1,0,AlignmentFlag::Center|AlignmentFlag::Middle);
            mSifre = gLayout->addWidget(cpp14::make_unique<WLineEdit>(),1,1,AlignmentFlag::Center);
            mSifre->setPlaceholderText("Şifrenizi Giriniz");
            mSifre->setEchoMode(EchoMode::Password);
            mSifre->setWidth(150);

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
    :ContainerWidget("",ContainerWidget::Horizontal),
      db(_db),
      UserValue(_user),
      mTCUser(_user)
{

    this->initHeader();

    this->initMenu();

}

void Giris::SivilWidget::initMenu()
{

    Wt::WMenu *menu = this->Header ()->addWidget(Wt::cpp14::make_unique<WMenu>());
    menu->setStyleClass("nav nav-pills nav-stacked");

    menu->addItem("Bilgilerim")->clicked ().connect ([=](){
        this->Content ()->clear();
        this->Content ()->addWidget (Wt::cpp14::make_unique<VatandasWidget>(new SerikBLDCore::DB(db),new SerikBLDCore::TC(UserValue)));
    });


    menu->addItem("Dilekçelerim")->clicked ().connect ([=](){
        this->Content ()->clear();
        this->Content ()->addWidget (Wt::cpp14::make_unique<DilekceTCYonetim>(db,UserValue));
    });



    menu->addItem("Yeni Talep")->clicked ().connect ([=](){
        this->Content ()->clear();
        auto TalepWidgetItem = cpp14::make_unique<TalepWidget::TalepWidget>(db);
        SerikBLDCore::TC tcItem(UserValue);
        TalepWidgetItem->tcWidget->setTCItem (tcItem);
        this->Content ()->addWidget (std::move(TalepWidgetItem));
    });



//    if( QDate::currentDate ().toJulianDay () < QDate(2020,2,1).toJulianDay () )
//    {
//        menu->addItem("Taleplerim")->clicked ().connect ([=](){
//            this->Content ()->clear();
//            this->Content ()->addWidget (Wt::cpp14::make_unique<Taleplerim>(db,UserValue));
//        });
//    }



    menu->addItem("Taleplerim")->clicked ().connect ([=](){
        this->Content ()->clear();
        this->Content ()->addWidget (Wt::cpp14::make_unique<TalepVatandasArayuz>(db,UserValue));
    });

//    if( QDate::currentDate ().toJulianDay () < QDate(2020,2,1).toJulianDay () )
//    {
//        menu->addItem("Başvurularım")->clicked ().connect ([=](){
//            this->Content ()->clear();
//            this->Content ()->addWidget (Wt::cpp14::make_unique<Basvurularim>(db,UserValue));
//        });
//    }


    menu->addItem("Başvurularım")->clicked ().connect ([=](){
        this->Content ()->clear();
        this->Content ()->addWidget (Wt::cpp14::make_unique<V2::Basvurularim>(new SerikBLDCore::DB(db),mTCUser));
    });


    menu->addItem("Yeni Başvuru")->clicked ().connect ([=](){
        this->Content ()->clear();
        this->Content ()->addWidget (Wt::cpp14::make_unique<BilgiEdinmeBasvuruWidget>(new SerikBLDCore::DB(db),&mTCUser));
    });


    menu->addItem("Mimari Proje Başvurusu")->clicked ().connect ([=](){
        this->Content ()->clear();
//        this->Content ()->addWidget (Wt::cpp14::make_unique<BilgiEdinmeBasvuruWidget>(new SerikBLDCore::DB(db),&mTCUser));
    });

}

void Giris::SivilWidget::initHeader()
{
    auto _mContainer = this->Header ()->addWidget(cpp14::make_unique<WContainerWidget>());
    _mContainer->setMargin(10,AllSides);
    _mContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    _mContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AliceBlue));
    _mContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    auto row = _mContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    auto photoWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    photoWidget->addStyleClass(Bootstrap::Grid::col_full_12);
    photoWidget->setContentAlignment (AlignmentFlag::Center);
    photoWidget->addStyleClass (Bootstrap::ImageShape::img_circle);
    auto __db = new SerikBLDCore::DB(db);
    auto fotoUrl = __db->downloadFileWeb (mTCUser.FotoOid ());
    if( fotoUrl != "NULL" )
    {
        photoWidget->setAttributeValue (Style::style,Style::background::url (fotoUrl)+
                                        Style::background::size::contain+
                                        Style::background::repeat::norepeat+
                                        Style::background::position::center_center);
    }else{
        photoWidget->setAttributeValue (Style::style,Style::background::url ("img/person.jpg")+
                                        Style::background::size::contain+
                                        Style::background::repeat::norepeat+
                                        Style::background::position::center_center);
    }


    photoWidget->setHeight (100);

    {
        auto infoContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        infoContainer->addStyleClass(Bootstrap::Grid::col_full_12);
        auto container = infoContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mTCUser.AdSoyad ().toStdString ())),0,AlignmentFlag::Center);
        layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mTCUser.Mahalle ().toStdString ())),0,AlignmentFlag::Center);
        layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mTCUser.CepTelefonu ().toStdString ())),0,AlignmentFlag::Center);
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

    auto titleRemoved = row->addWidget(cpp14::make_unique<WText>("<h5><b>Dikkat Bu Sistem Yılbaşından(01.02.2020 (Şubat) ) Sonra Kapatılacaktır.</b></h5>"));
    titleRemoved->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    titleRemoved->setAttributeValue (Style::style,Style::color::color (Style::color::Red::Crimson));

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
        layout->addWidget(cpp14::make_unique<WText>(tarih));
    }
    {
        layout->addWidget(cpp14::make_unique<WText>(saat));
    }
    {
        layout->addWidget(cpp14::make_unique<WText>(mahalle));
    }

    {
        layout->addWidget(cpp14::make_unique<WText>(durum));
    }
    {
        layout->addWidget(cpp14::make_unique<WText>(birim));
    }

    {
        layout->addWidget(cpp14::make_unique<WText>(adres));
    }
    {
        layout->addWidget(cpp14::make_unique<WText>("<b>Çarğı Merkezi Personeli: "+cagriMerkeziPersoneli+"</b>"));
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

Giris::Taleplerim::AciklamaEkle::AciklamaEkle(mongocxx::database *_db, std::string _oid, WContainerWidget *widget, const bsoncxx::document::value &user, bool isVatandas)
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


        auto pushDoc = document{};

        try {
            pushDoc.append(kvp("$push",make_document(kvp(SBLDKeys::SikayetKey::mainKey::asama,doc))));
        } catch (bsoncxx::exception &e) {
            this->showMessage("UYARI",WString("$push Error: {1}").arg(e.what()).toUTF8());
            return;
        }


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

    auto titleRemoved = row->addWidget(cpp14::make_unique<WText>("<h5><b>Dikkat Bu Sistem Yılbaşından(01.02.2020 (Şubat) ) Sonra Kapatılacaktır.</b></h5>"));
    titleRemoved->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    titleRemoved->setAttributeValue (Style::style,Style::color::color (Style::color::Red::Crimson));



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

                        container->addWidget(cpp14::make_unique<WText>(link,TextFormat::UnsafeXHTML));





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






Giris::Personel::PersonelWidget::PersonelWidget(mongocxx::database *_db, bsoncxx::document::value* _user)
    :ContainerWidget ("",ContainerWidget::Horizontal),mUser(new SerikBLDCore::User(_db,*_user))
{

    auto containerFluid = this->Header ()->addWidget(cpp14::make_unique<WContainerWidget>());
    containerFluid->addStyleClass(Bootstrap::Grid::container_fluid);
    auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
    _row->addStyleClass(Bootstrap::Grid::row);
    this->initHeader(_row);
    mMenuContainer = _row->addWidget(cpp14::make_unique<WContainerWidget>());
    mMenuContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    mMenuContainer->addStyleClass (Bootstrap::Grid::Hidden::hidden_xs);
    mMenuContainer->setPadding(0,AllSides);


    mMobilMenuContainer = _row->addWidget(cpp14::make_unique<WContainerWidget>());
    mMobilMenuContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    mMobilMenuContainer->addStyleClass (Bootstrap::Grid::Hidden::hidden_lg+
                                        Bootstrap::Grid::Hidden::hidden_md+
                                        Bootstrap::Grid::Hidden::hidden_sm);
    mMobilMenuContainer->setPadding(0,AllSides);



    this->initMenu();

    this->initMobilMenu ();


}

void Giris::Personel::PersonelWidget::initMenu()
{


    auto _mContainer = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _mContainer->setPadding(35,Side::Bottom|Side::Top);

    Wt::WMenu *menu = _mContainer->addWidget(Wt::cpp14::make_unique<Wt::WMenu>());
    menu->setStyleClass("nav nav-pills nav-stacked");

    this->Content ()->clear ();
    this->Content ()->addWidget (Wt::cpp14::make_unique<Giris::Personel::Yenilikler>());

    menu->addItem("Kod Repository")->clicked ().connect ([&](){
        this->Content ()->clear ();
        this->Content ()->addWidget (Wt::cpp14::make_unique<Giris::Personel::Yenilikler>());
    });


    menu->addItem("Bilgilerim")->clicked ().connect([&](){
        this->Content ()->clear ();
        auto personel = SerikBLDCore::IK::Personel();
        personel.setDocumentView (mUser->view ());
        this->Content ()->addWidget (Wt::cpp14::make_unique<PersonelPage>( std::move(personel) , new SerikBLDCore::DB(mUser->db ()) , mUser )  );
    });

    menu->addItem(WString::fromUTF8("Dilekçeler"))->clicked ().connect ([&]{
        this->Content ()->clear ();
        this->Content ()->addWidget (Wt::cpp14::make_unique<DilekceYonetim>(mUser->db (),mUser->Value ()));
    });



    menu->addItem(WString::fromUTF8("TaleplerimV2"))->clicked ().connect ([&](){
        this->Content ()->clear();
        this->Content ()->addWidget (Wt::cpp14::make_unique<TalepYonetim>(mUser->db (),mUser->Value ()));
    });


    menu->addItem(WString::fromUTF8("BaşvurularımV2"))->clicked ().connect ([&](){
        this->Content ()->clear ();
        this->Content ()->addWidget (Wt::cpp14::make_unique<V2::GelenBasvurular>(new SerikBLDCore::DB(mUser->db ()) , mUser->Value ()));
    });


    if( this->mUser->Birimi () == "Yazı İşleri Müdürlüğü" || this->mUser->Statu () == SerikBLDCore::User::Baskan || this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi )
    {
        menu->addItem(WString::fromUTF8("Meclis"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<v2::MeclisPageManager>(new SerikBLDCore::DB(mUser->db ())));
        });
    }


    if(this->mUser->Statu () == SerikBLDCore::User::Baskan ||
            this->mUser->Birimi ()  == "Bilgi İşlem Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("Anons Sistemi"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<AnonsWidget>(mUser->db (),mUser->Value ()));
        });
    }

    if(this->mUser->Statu () == SerikBLDCore::User::Baskan ||
            this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi ||
            this->mUser->Birimi ()  == "Kadın ve Aile Hizmetleri Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("Stok Yardım"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<KadinAileStock>(mUser->db (),mUser->Value ()));
        });
    }




    if(this->mUser->Statu () == SerikBLDCore::User::Baskan ||
            this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi ||
            this->mUser->Birimi () == "İnsan Kaynakları ve Eğitim Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("IK"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<IKManagerPage>(this->mUser->getDB ()));
        });
    }




    if( this->mUser->Statu () == SerikBLDCore::User::Baskan || this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi )
    {
        menu->addItem(WString::fromUTF8("Giriş Çıkış"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<GirisCikisWidget>(mUser->db (),mUser->Value ()));
        });

    }else{

        menu->addItem(WString::fromUTF8("Taleplerim"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<Taleplerim>(mUser->db (),mUser->Value ()));
        });


        menu->addItem(WString::fromUTF8("Evrak Arşivi"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<EvrakArsiv>(mUser->db (),mUser->Value ()));
        });


        menu->addItem(WString::fromUTF8("Başvurularım"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<BilgiEdinmeClient>(mUser->db (),mUser->Value ()));
        });


        menu->addItem(WString::fromUTF8("Giriş Çıkışlarım"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<GirisCikisWidget>(mUser->db (),mUser->Value ()));
        });
    }
}

void Giris::Personel::PersonelWidget::initMobilMenu()
{
    Wt::WNavigationBar *navigation = mMobilMenuContainer->addNew<Wt::WNavigationBar>();
    navigation->setResponsive(true);



//    auto _mContainer = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    _mContainer->setPadding(35,Side::Bottom|Side::Top);

    auto _menu = Wt::cpp14::make_unique<Wt::WMenu>();
    _menu->setStyleClass("nav nav-pills nav-stacked");
    auto menu = navigation->addMenu (std::move(_menu), Wt::AlignmentFlag::Right);

    this->Content ()->clear ();
    this->Content ()->addWidget (Wt::cpp14::make_unique<Giris::Personel::Yenilikler>());

    menu->addItem("Kod Repository")->clicked ().connect ([&](){
        this->Content ()->clear ();
        this->Content ()->addWidget (Wt::cpp14::make_unique<Giris::Personel::Yenilikler>());
    });


    menu->addItem("Bilgilerim")->clicked ().connect([&](){
        this->Content ()->clear ();
        auto personel = SerikBLDCore::IK::Personel();
        personel.setDocumentView (mUser->view ());
        this->Content ()->addWidget (Wt::cpp14::make_unique<PersonelPage>( std::move(personel) , new SerikBLDCore::DB(mUser->db ()) , mUser )  );
    });

    menu->addItem(WString::fromUTF8("Dilekçeler"))->clicked ().connect ([&]{
        this->Content ()->clear ();
        this->Content ()->addWidget (Wt::cpp14::make_unique<DilekceYonetim>(mUser->db (),mUser->Value ()));
    });



    menu->addItem(WString::fromUTF8("TaleplerimV2"))->clicked ().connect ([&](){
        this->Content ()->clear();
        this->Content ()->addWidget (Wt::cpp14::make_unique<TalepYonetim>(mUser->db (),mUser->Value ()));
    });


    menu->addItem(WString::fromUTF8("BaşvurularımV2"))->clicked ().connect ([&](){
        this->Content ()->clear ();
        this->Content ()->addWidget (Wt::cpp14::make_unique<V2::GelenBasvurular>(new SerikBLDCore::DB(mUser->db ()) , mUser->Value ()));
    });


    if( this->mUser->Birimi () == "Yazı İşleri Müdürlüğü" || this->mUser->Statu () == SerikBLDCore::User::Baskan || this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi )
    {
        menu->addItem(WString::fromUTF8("Meclis"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<v2::MeclisPageManager>(new SerikBLDCore::DB(mUser->db ())));
        });
    }


    if(this->mUser->Statu () == SerikBLDCore::User::Baskan ||
            this->mUser->Birimi ()  == "Bilgi İşlem Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("Anons Sistemi"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<AnonsWidget>(mUser->db (),mUser->Value ()));
        });
    }

    if(this->mUser->Statu () == SerikBLDCore::User::Baskan ||
            this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi ||
            this->mUser->Birimi ()  == "Kadın ve Aile Hizmetleri Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("Stok Yardım"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<KadinAileStock>(mUser->db (),mUser->Value ()));
        });
    }




    if(this->mUser->Statu () == SerikBLDCore::User::Baskan ||
            this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi ||
            this->mUser->Birimi () == "İnsan Kaynakları ve Eğitim Müdürlüğü" )
    {
        menu->addItem(WString::fromUTF8("IK"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<IKManagerPage>(this->mUser->getDB ()));
        });
    }




    if( this->mUser->Statu () == SerikBLDCore::User::Baskan || this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi )
    {
        menu->addItem(WString::fromUTF8("Giriş Çıkış"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<GirisCikisWidget>(mUser->db (),mUser->Value ()));
        });

    }else{

        menu->addItem(WString::fromUTF8("Taleplerim"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<Taleplerim>(mUser->db (),mUser->Value ()));
        });


        menu->addItem(WString::fromUTF8("Evrak Arşivi"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<EvrakArsiv>(mUser->db (),mUser->Value ()));
        });


        menu->addItem(WString::fromUTF8("Başvurularım"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<BilgiEdinmeClient>(mUser->db (),mUser->Value ()));
        });


        menu->addItem(WString::fromUTF8("Giriş Çıkışlarım"))->clicked ().connect ([&](){
            this->Content ()->clear ();
            this->Content ()->addWidget (Wt::cpp14::make_unique<GirisCikisWidget>(mUser->db (),mUser->Value ()));
        });
    }
}

void Giris::Personel::PersonelWidget::initHeader(WContainerWidget* _row)
{


    auto tempContainer = _row->addWidget(cpp14::make_unique<WContainerWidget>());
    tempContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+
                                 Bootstrap::Grid::Medium::col_md_12+
                                 Bootstrap::Grid::Small::col_sm_12+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
    tempContainer->setAttributeValue(Style::style,Style::background::color::rgba(120,155,175));
    tempContainer->addStyleClass ("boxShadow");


    auto photoWidget = tempContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    photoWidget->setMargin (5,Side::Top);
    photoWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    photoWidget->setAttributeValue (Style::style,Style::background::url (this->mUser->PhotoFilePathWeb ())+
                                    Style::background::repeat::norepeat+
                                    Style::background::position::center_center+
                                    Style::background::size::cover);
    photoWidget->setHeight (145);
    photoWidget->setWidth (120);

    {
        auto infoContainer = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        infoContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+
                                     Bootstrap::Grid::Medium::col_md_12+
                                     Bootstrap::Grid::Small::col_sm_12+
                                     Bootstrap::Grid::ExtraSmall::col_xs_6);
        infoContainer->setHeight (155);
        infoContainer->setAttributeValue(Style::style,Style::background::color::rgba(100,125,155));
        infoContainer->addStyleClass ("boxShadow");


        auto container = infoContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        layout->addStretch(1);

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->mUser->AdSoyad ())),0,AlignmentFlag::Center);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->mUser->Birimi ())),0,AlignmentFlag::Center);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }

        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->mUser->Telefon ())),0,AlignmentFlag::Center);
        } catch (bsoncxx::exception &e) {
            layout->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        }
        try {
            layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->mUser->Statu ())),0,AlignmentFlag::Center);
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

int64_t Giris::Personel::BaseWidget::count(std::string collection, const bsoncxx::document::view &filter)
{
    std::int64_t count = 0;
    try {
        count = this->db()->collection(collection).count(filter);
    } catch (mongocxx::exception &e) {

    }
    return count;
}



//int64_t Giris::Personel::BaseWidget::count(std::string collection, const bsoncxx::builder::basic::document &filter)
//{
//    std::int64_t count = 0;
//    try {
//        count = this->db()->collection(collection).count(filter.view());
//    } catch (mongocxx::exception &e) {

//    }
//    return count;
//}

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
                            textLayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::GorevliType::adsoyad].get_utf8().value.to_string()));
                        } catch (bsoncxx::exception &e) {
                            textLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
                        }

                        try {
                            textLayout->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::SikayetKey::GorevliType::tel].get_utf8().value.to_string()));
                        } catch (bsoncxx::exception &e) {
                            textLayout->addWidget(cpp14::make_unique<WText>(WString("Telefon Yok: {1}").arg(e.what())));
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














Giris::LoginWidgetV2::LoginWidgetV2(SerikBLDCore::DB *_db)
    :smsManager( new SMSManager(_db))
{
    this->initLoginScreen ();
}

void Giris::LoginWidgetV2::initLoginScreen()
{

    Content ()->setContentAlignment (AlignmentFlag::Center);
    {
        auto container = Content ()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setContentAlignment(AlignmentFlag::Center);

        container->setMargin(100,Side::Top);

        auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        sContainer->setMaximumSize(500,WLength::Auto);
        sContainer->setAttributeValue(Style::style,Style::Border::border("2px solid gray")+Style::Border::borderRardius("20","20","20","20"));
        sContainer->setPadding(25,AllSides);

        auto gLayout = sContainer->setLayout(cpp14::make_unique<WGridLayout>());

        {
            gLayout->addWidget(cpp14::make_unique<WText>("Telefon Numarası"),0,0,AlignmentFlag::Center|AlignmentFlag::Middle);
            mTelefonNumarasi = gLayout->addWidget(cpp14::make_unique<WLineEdit>(),0,1,AlignmentFlag::Center);
            mTelefonNumarasi->setPlaceholderText("Telefon Numarasını Giriniz");
            mTelefonNumarasi->setWidth(150);
            mTelefonNumarasi->setInputMask("99999999999");
        }
        {
            gLayout->addWidget(cpp14::make_unique<WText>("Şifreniz"),1,0,AlignmentFlag::Center|AlignmentFlag::Middle);
            mSifre = gLayout->addWidget(cpp14::make_unique<WLineEdit>(),1,1,AlignmentFlag::Center);
            mSifre->setPlaceholderText("Şifrenizi Giriniz");
            mSifre->setEchoMode(EchoMode::Password);
            mSifre->setWidth(150);

        }
    }


    {
        auto btnWidget = Content ()->addWidget(cpp14::make_unique<WContainerWidget>());
        btnWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto layout = btnWidget->setLayout(cpp14::make_unique<WVBoxLayout>());

        mGirisButton = layout->addWidget(cpp14::make_unique<WPushButton>("Giriş"),0,AlignmentFlag::Center);
        mGirisButton->addStyleClass(Bootstrap::Button::Primary);
//        mGirisButton->clicked().connect(this,&Giris::LoginWidget::ConfirmLogin);

        mUnuttumButton = layout->addWidget(cpp14::make_unique<WPushButton>("Şifremi Unuttum"),0,AlignmentFlag::Center);
        mUnuttumButton->addStyleClass(Bootstrap::Button::Warning);
//        mUnuttumButton->clicked().connect(this,&Giris::LoginWidget::sendtempPasswordSMS);

        mkayitOlButton = layout->addWidget(cpp14::make_unique<WPushButton>("Kayıt Ol"),0,AlignmentFlag::Center);
        mkayitOlButton->addStyleClass(Bootstrap::Button::info);
//        mkayitOlButton->clicked().connect(this,&Giris::LoginWidget::kayitOld);
    }

}

void Giris::LoginWidgetV2::sendNewPassword()
{




}
