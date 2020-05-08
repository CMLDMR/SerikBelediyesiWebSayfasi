#include "talepyonetim.h"
#include "talepview.h"
#include "taleplistwidget.h"

using namespace SerikBLDCore;

TalepYonetim::TalepYonetim(mongocxx::database *_db, const bsoncxx::document::value _user)
    :ContainerWidget ("Talep/Şikayet Yönetimi V2"),
      TalepManager (_db),
      mUser(new SerikBLDCore::User (_db,_user)),
      mTalepKategoriManager( new SerikBLDCore::TalepKategoriManager(new SerikBLDCore::DB(_db)))
{

    mTCManagerV2 = new SerikBLDCore::TCManagerV2(this->getDB ());

    mTalepKategoriManager->setLimit (1000);
    mTalepKategoriManager->UpdateList ();

    if( this->mUser->Statu () == SerikBLDCore::User::Mudur ){
        this->initControlPanel ();
    }else if (this->mUser->Statu () == SerikBLDCore::User::Personel ) {
        this->initPersonelPanel ();
    }else if ( this->mUser->Statu () == SerikBLDCore::User::Baskan || this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi ){

    }

    this->Content ()->setMargin (15,Side::Top|Side::Bottom);
    this->Content ()->setWidth (WLength("100%"));
    this->Content ()->decorationStyle ().setBorder (WBorder(BorderStyle::Solid,BorderWidth::Thin),Side::Top);
    this->clickOid ().connect (this,&TalepYonetim::initTalep);

    this->initKategoriDagilimi();
}



void TalepYonetim::initControlPanel()
{
    this->Header ()->clear ();

    auto kResult = this->kaynakPipeLine (this->mUser->Birimi ().c_str ());
    {

        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (15,Side::Top|Side::Bottom);
        auto toolbarWidget = container->addWidget (cpp14::make_unique<WToolBar>());

        auto SmsBtn = createButton("btn-primary", TalepKey::KaynakKey::Sms+" "+Bootstrap::Badges::badget(std::to_string(kResult.Sms)));
        SmsBtn->clicked().connect([=](){
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setKaynak (TalepKey::KaynakKey::Sms.c_str ()));
        });
        toolbarWidget->addButton(std::move(SmsBtn));

        auto WebBtn = createButton("btn-success", TalepKey::KaynakKey::Web+" "+Bootstrap::Badges::badget(std::to_string(kResult.Web)));
        WebBtn->clicked().connect([=](){
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setKaynak (TalepKey::KaynakKey::Web.c_str ()));
        });
        toolbarWidget->addButton(std::move(WebBtn));

        auto MobilBtn = createButton("btn-danger", TalepKey::KaynakKey::Mobil+" "+Bootstrap::Badges::badget(std::to_string(kResult.Mobil)));
        MobilBtn->clicked().connect([=](){
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setKaynak (TalepKey::KaynakKey::Mobil.c_str ()));
        });
        toolbarWidget->addButton(std::move(MobilBtn));

        auto TelefonBtn = createButton("btn-info", TalepKey::KaynakKey::Telefon+" "+Bootstrap::Badges::badget(std::to_string(kResult.Telefon)));
        TelefonBtn->clicked().connect([=](){
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setKaynak (TalepKey::KaynakKey::Telefon.c_str ()));
        });
        toolbarWidget->addButton(std::move(TelefonBtn));

        auto BeyazmasaBtn = createButton("btn-default", TalepKey::KaynakKey::Beyazmasa+" "+Bootstrap::Badges::badget(std::to_string(kResult.Beyazmasa)));
        BeyazmasaBtn->clicked().connect([=](){
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setKaynak (TalepKey::KaynakKey::Beyazmasa.c_str ()));
        });
        toolbarWidget->addButton(std::move(BeyazmasaBtn));


        auto SosyalMedyaBtn = createButton("btn-warning", TalepKey::KaynakKey::SosyalMedya+" "+Bootstrap::Badges::badget(std::to_string(kResult.SosyalMedya)));
        SosyalMedyaBtn->clicked().connect([=](){
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setKaynak (TalepKey::KaynakKey::SosyalMedya.c_str ()));
        });
        toolbarWidget->addButton(std::move(SosyalMedyaBtn));
    }

    auto result = this->durumPipeLine (this->mUser->Birimi ().c_str ());
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (15,Side::Top|Side::Bottom);
        auto toolbarWidget = container->addWidget (cpp14::make_unique<WToolBar>());


        auto siralamaContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        siralamaContainer->addStyleClass (Bootstrap::Grid::col_full_12);

        auto typeContainer = siralamaContainer->addWidget (cpp14::make_unique<WComboBox>());
        typeContainer->addItem ("Tarihe Göre Sırala(+)");
        typeContainer->addItem ("Tarihe Göre Sırala(-)");
        typeContainer->addItem ("Mahalleye Göre Sırala(+)");
        typeContainer->addItem ("Mahalleye Göre Sırala(-)");

        auto GoruntulemetypeContainer = siralamaContainer->addWidget (cpp14::make_unique<WComboBox>());
        GoruntulemetypeContainer->addItem ("Normal Görünüm");
        GoruntulemetypeContainer->addItem ("Detaylı Görünüm");


        auto telefonileSorgulaContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        telefonileSorgulaContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        auto telefonIleLineEdit = telefonileSorgulaContainer->addWidget (cpp14::make_unique<WLineEdit>());
        telefonIleLineEdit->setPlaceholderText ("Telefon Numarası ile Listele (05321234567)");

        telefonIleLineEdit->changed ().connect ([=](){
            if( telefonIleLineEdit->text ().toUTF8 ().size () == 11 )
            {
                auto tc = mTCManagerV2->Load_byTEL (telefonIleLineEdit->text ().toUTF8 ());
                if( tc )
                {

                    SerikBLDCore::FindOptions options;
                    options.setLimit (1000);

                    if( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(+)" ){
                        options.setSort (SerikBLDCore::Item("").append("_id",-1));
                    }else if ( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(-)" ) {
                        options.setSort (SerikBLDCore::Item("").append("_id",1));
                    } else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(+)" ) {
                        options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,-1));
                    }else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(-)" ) {
                        options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,1));
                    }


                    SiralamaType sType;
                    if( GoruntulemetypeContainer->currentIndex () == 0 )
                    {
                        sType = SiralamaType::NormalSiralama;
                    }else{
                        sType = SiralamaType::Raporlama;
                    }

                    this->listTalepler (Talep()
                                        .setBirim (this->mUser->Birimi ().c_str ())
                                        .setTCOID (tc.value ()->oid ().value ().to_string ().c_str ()) , options , sType );

                }
            }
        });




        auto devamEdiyorWidget = cpp14::make_unique<WContainerWidget>();
        devamEdiyorWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::DevamEdiyor +" "+Bootstrap::Badges::badget(std::to_string(result.DevamEdiyor))));
        devamEdiyorWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::LightSeaGreen)+
                                              Style::color::color (Style::color::White::Snow)+
                                              Style::font::weight::bold);
        devamEdiyorWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        devamEdiyorWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        devamEdiyorWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        devamEdiyorWidget->clicked ().connect ([=](){

            SerikBLDCore::FindOptions options;
            options.setLimit (1000);

            if( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(+)" ){
                options.setSort (SerikBLDCore::Item("").append("_id",-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append("_id",1));
            } else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(+)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,1));
            }


            SiralamaType sType;
            if( GoruntulemetypeContainer->currentIndex () == 0 )
            {
                sType = SiralamaType::NormalSiralama;
            }else{
                sType = SiralamaType::Raporlama;
            }

            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::DevamEdiyor.c_str ()) , options, sType );
        });
        toolbarWidget->addWidget (std::move(devamEdiyorWidget));




        auto tamamlandiWidget = cpp14::make_unique<WContainerWidget>();
        tamamlandiWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::Tamamlandi +" "+Bootstrap::Badges::badget(std::to_string(result.Tamamlandi))));
        tamamlandiWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::DodgerBlue)+
                                             Style::color::color (Style::color::White::Snow)+
                                             Style::font::weight::bold);
        tamamlandiWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        tamamlandiWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        tamamlandiWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        tamamlandiWidget->clicked ().connect ([=](){
            SerikBLDCore::FindOptions options;
            options.setLimit (1000);

            if( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(+)" ){
                options.setSort (SerikBLDCore::Item("").append("_id",-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append("_id",1));
            } else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(+)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,1));
            }

            SiralamaType sType;
            if( GoruntulemetypeContainer->currentIndex () == 0 )
            {
                sType = SiralamaType::NormalSiralama;
            }else{
                sType = SiralamaType::Raporlama;
            }

            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::Tamamlandi.c_str ()) , options , sType);
        });
        toolbarWidget->addWidget (std::move(tamamlandiWidget));




        auto redEdildiWidget = cpp14::make_unique<WContainerWidget>();
        redEdildiWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::RedEdildi +" "+Bootstrap::Badges::badget(std::to_string(result.RedEdildi))));
        redEdildiWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Crimson)+
                                            Style::color::color (Style::color::White::Snow)+
                                            Style::font::weight::bold);
        redEdildiWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        redEdildiWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        redEdildiWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        redEdildiWidget->clicked ().connect ([=](){

            SerikBLDCore::FindOptions options;
            options.setLimit (1000);

            if( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(+)" ){
                options.setSort (SerikBLDCore::Item("").append("_id",-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append("_id",1));
            } else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(+)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,1));
            }

            SiralamaType sType;
            if( GoruntulemetypeContainer->currentIndex () == 0 )
            {
                sType = SiralamaType::NormalSiralama;
            }else{
                sType = SiralamaType::Raporlama;
            }

            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::RedEdildi.c_str ()),options , sType );
        });
        toolbarWidget->addWidget (std::move(redEdildiWidget));



        auto teyitEdilmemisWidget = cpp14::make_unique<WContainerWidget>();
        teyitEdilmemisWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::TeyitEdilmemis +" "+Bootstrap::Badges::badget(std::to_string(result.TeyitEdilmemis))));
        teyitEdilmemisWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::PaleVioletRed)+
                                                 Style::color::color (Style::color::White::Snow)+
                                                 Style::font::weight::bold);
        teyitEdilmemisWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        teyitEdilmemisWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        teyitEdilmemisWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        teyitEdilmemisWidget->clicked ().connect ([=](){

            SerikBLDCore::FindOptions options;
            options.setLimit (1000);

            if( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(+)" ){
                options.setSort (SerikBLDCore::Item("").append("_id",-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append("_id",1));
            } else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(+)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,1));
            }

            SiralamaType sType;
            if( GoruntulemetypeContainer->currentIndex () == 0 )
            {
                sType = SiralamaType::NormalSiralama;
            }else{
                sType = SiralamaType::Raporlama;
            }

            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::TeyitEdilmemis.c_str ()),options , sType );
        });
        toolbarWidget->addWidget (std::move(teyitEdilmemisWidget));



        auto beklemedeWidget = cpp14::make_unique<WContainerWidget>();
        beklemedeWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::Beklemede +" "+Bootstrap::Badges::badget(std::to_string(result.Beklemede))));
        beklemedeWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Orange::GoldenRod)+
                                            Style::color::color (Style::color::White::Snow)+
                                            Style::font::weight::bold);
        beklemedeWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        beklemedeWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        beklemedeWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        beklemedeWidget->clicked ().connect ([=](){

            SerikBLDCore::FindOptions options;
            options.setLimit (1000);

            if( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(+)" ){
                options.setSort (SerikBLDCore::Item("").append("_id",-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Tarihe Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append("_id",1));
            } else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(+)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,-1));
            }else if ( typeContainer->currentText ().toUTF8 () == "Mahalleye Göre Sırala(-)" ) {
                options.setSort (SerikBLDCore::Item("").append(SerikBLDCore::TalepKey::Mahalle,1));
            }

            SiralamaType sType;
            if( GoruntulemetypeContainer->currentIndex () == 0 )
            {
                sType = SiralamaType::NormalSiralama;
            }else{
                sType = SiralamaType::Raporlama;
            }

            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::Beklemede.c_str ()),options , sType);
        });
        toolbarWidget->addWidget (std::move(beklemedeWidget));

    }


}

void TalepYonetim::initPersonelPanel()
{
    this->Header ()->clear ();

    Talep filterPipeResult;
    filterPipeResult.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
    filterPipeResult.setBirim (this->mUser->Birimi ().c_str ());

    auto kResult = this->kaynakPipeLine (filterPipeResult);


    {

        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (15,Side::Top|Side::Bottom);
        auto toolbarWidget = container->addWidget (cpp14::make_unique<WToolBar>());

        auto SmsBtn = createButton("btn-primary", TalepKey::KaynakKey::Sms+" "+Bootstrap::Badges::badget(std::to_string(kResult.Sms)));
        SmsBtn->clicked().connect([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setKaynak (TalepKey::KaynakKey::Sms.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addButton(std::move(SmsBtn));

        auto WebBtn = createButton("btn-success", TalepKey::KaynakKey::Web+" "+Bootstrap::Badges::badget(std::to_string(kResult.Web)));
        WebBtn->clicked().connect([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setKaynak (TalepKey::KaynakKey::Web.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addButton(std::move(WebBtn));

        auto MobilBtn = createButton("btn-danger", TalepKey::KaynakKey::Mobil+" "+Bootstrap::Badges::badget(std::to_string(kResult.Mobil)));
        MobilBtn->clicked().connect([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setKaynak (TalepKey::KaynakKey::Mobil.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addButton(std::move(MobilBtn));

        auto TelefonBtn = createButton("btn-info", TalepKey::KaynakKey::Telefon+" "+Bootstrap::Badges::badget(std::to_string(kResult.Telefon)));
        TelefonBtn->clicked().connect([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setKaynak (TalepKey::KaynakKey::Telefon.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addButton(std::move(TelefonBtn));

        auto BeyazmasaBtn = createButton("btn-default", TalepKey::KaynakKey::Beyazmasa+" "+Bootstrap::Badges::badget(std::to_string(kResult.Beyazmasa)));
        BeyazmasaBtn->clicked().connect([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setKaynak (TalepKey::KaynakKey::Beyazmasa.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addButton(std::move(BeyazmasaBtn));


        auto SosyalMedyaBtn = createButton("btn-warning", TalepKey::KaynakKey::SosyalMedya+" "+Bootstrap::Badges::badget(std::to_string(kResult.SosyalMedya)));
        SosyalMedyaBtn->clicked().connect([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setKaynak (TalepKey::KaynakKey::SosyalMedya.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addButton(std::move(SosyalMedyaBtn));
    }

    auto result = this->durumPipeLine (filterPipeResult);

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (15,Side::Top|Side::Bottom);
        auto toolbarWidget = container->addWidget (cpp14::make_unique<WToolBar>());



        auto devamEdiyorWidget = cpp14::make_unique<WContainerWidget>();
        devamEdiyorWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::DevamEdiyor +" "+Bootstrap::Badges::badget(std::to_string(result.DevamEdiyor))));
        devamEdiyorWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::LightSeaGreen)+
                                              Style::color::color (Style::color::White::Snow)+
                                              Style::font::weight::bold);
        devamEdiyorWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        devamEdiyorWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        devamEdiyorWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        devamEdiyorWidget->clicked ().connect ([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setDurum (TalepKey::DurumKey::DevamEdiyor.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addWidget (std::move(devamEdiyorWidget));




        auto tamamlandiWidget = cpp14::make_unique<WContainerWidget>();
        tamamlandiWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::Tamamlandi +" "+Bootstrap::Badges::badget(std::to_string(result.Tamamlandi))));
        tamamlandiWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::DodgerBlue)+
                                             Style::color::color (Style::color::White::Snow)+
                                             Style::font::weight::bold);
        tamamlandiWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        tamamlandiWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        tamamlandiWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        tamamlandiWidget->clicked ().connect ([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setDurum (TalepKey::DurumKey::Tamamlandi.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addWidget (std::move(tamamlandiWidget));




        auto redEdildiWidget = cpp14::make_unique<WContainerWidget>();
        redEdildiWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::RedEdildi +" "+Bootstrap::Badges::badget(std::to_string(result.RedEdildi))));
        redEdildiWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Crimson)+
                                            Style::color::color (Style::color::White::Snow)+
                                            Style::font::weight::bold);
        redEdildiWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        redEdildiWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        redEdildiWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        redEdildiWidget->clicked ().connect ([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setDurum (TalepKey::DurumKey::RedEdildi.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addWidget (std::move(redEdildiWidget));



        auto teyitEdilmemisWidget = cpp14::make_unique<WContainerWidget>();
        teyitEdilmemisWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::TeyitEdilmemis +" "+Bootstrap::Badges::badget(std::to_string(result.TeyitEdilmemis))));
        teyitEdilmemisWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::PaleVioletRed)+
                                                 Style::color::color (Style::color::White::Snow)+
                                                 Style::font::weight::bold);
        teyitEdilmemisWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        teyitEdilmemisWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        teyitEdilmemisWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        teyitEdilmemisWidget->clicked ().connect ([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setDurum (TalepKey::DurumKey::TeyitEdilmemis.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addWidget (std::move(teyitEdilmemisWidget));



        auto beklemedeWidget = cpp14::make_unique<WContainerWidget>();
        beklemedeWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::Beklemede +" "+Bootstrap::Badges::badget(std::to_string(result.Beklemede))));
        beklemedeWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Orange::GoldenRod)+
                                            Style::color::color (Style::color::White::Snow)+
                                            Style::font::weight::bold);
        beklemedeWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        beklemedeWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        beklemedeWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        beklemedeWidget->clicked ().connect ([=](){
            Talep filter;
            filter.append(TalepKey::GorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ())))));
            filter.setBirim (this->mUser->Birimi ().c_str ());
            filter.setDurum (TalepKey::DurumKey::Beklemede.c_str ());
            this->listTalepler (filter);
        });
        toolbarWidget->addWidget (std::move(beklemedeWidget));

    }

}

void TalepYonetim::listTalepler(const Talep &filter, const FindOptions &options, TalepYonetim::SiralamaType siralamaType)
{






    if( filter.view ().empty () ) return;



    auto cursor = this->findTalep (filter,options);
    this->Content ()->clear ();

    SerikBLDCore::TCManagerV2 *mTCManager = new SerikBLDCore::TCManagerV2(this->getDB ());


    int index = 1;
    for( auto item : cursor )
    {
        auto kategoriName = mTalepKategoriManager->KategoriName(item.kategoriOid ());


        if( siralamaType == SiralamaType::NormalSiralama )
        {
            this->Content ()->addWidget (cpp14::make_unique<TalepListWidget>(item.oid ().toStdString (),
                                                                             item.mahalle ().toStdString (),
                                                                             item.tarih ().toStdString (),
                                                                             item.durum ().toStdString (),
                                                                             item.durumColor ().toStdString (),
                                                                             kategoriName.toStdString ()))->ClickItem ().connect ([=](std::string oid){
                _clickOid.emit (oid);
            });
        }else{

            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
            container->setMargin (5,Side::Top|Side::Bottom);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->clicked ().connect ([=](){
                _clickOid.emit (item.oid ().toStdString ());
            });
            auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());


            vLayout->addWidget (cpp14::make_unique<WText>(WString("{1}. {2}  <b>{3}</b> {4}<br>").arg (index++)
                                                          .arg (item.tarih ().toStdString ())
                                                          .arg(kategoriName.toStdString ())
                                                          .arg(item.durum ().toStdString ()),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);

            vLayout->addWidget (cpp14::make_unique<WText>(WString("<b>Mahalle: {1}</b><br>").arg (item.mahalle ().toStdString ()),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            if( item.kisiGizli () )
            {
                vLayout->addWidget (cpp14::make_unique<WText>(WString("AdSoyad: Kişi Bilgilerini Gizlemiş<br>"),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
                vLayout->addWidget (cpp14::make_unique<WText>(WString("Telefon: Kişi Bilgilerini Gizlemiş<br>"),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            }else{
                auto tc = mTCManager->Load_byOID (item.tcOid ().toStdString ());
                vLayout->addWidget (cpp14::make_unique<WText>(WString("AdSoyad: {1}<br>").arg (tc.value ()->AdSoyad ().toStdString ()),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
                vLayout->addWidget (cpp14::make_unique<WText>(WString("Telefon: {1}<br>").arg (tc.value ()->CepTelefonu ().toStdString ()),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            }

            vLayout->addWidget (cpp14::make_unique<WText>(WString("Adres  : {1}<br>").arg (item.adres ().toStdString ()),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            vLayout->addWidget (cpp14::make_unique<WText>(WString("Konu   : {1}<br>").arg (item.konu ().toStdString ()),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            vLayout->addWidget (cpp14::make_unique<WText>(WString(".<br>"),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            vLayout->addWidget (cpp14::make_unique<WText>(WString(".<br>"),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            vLayout->addWidget (cpp14::make_unique<WText>(WString(".<br>"),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
            vLayout->addWidget (cpp14::make_unique<WText>(WString("________________________________________________________"),TextFormat::UnsafeXHTML),0,AlignmentFlag::Left);
        }



    }

}

void TalepYonetim::initKategoriDagilimi()
{

    auto list = this->kategoriPipeLine (Talep().setBirim (this->mUser->Birimi ().c_str ()));
    double totalSum = 0;
    for( auto item : list )
    {
        totalSum += static_cast<double>(item.mSayi);
    }

    Footer ()->clear ();
    Footer ()->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    Footer ()->setMargin (25,Side::Top|Side::Bottom);
    Footer ()->setWidth (WLength("100%"));

    auto textContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    textContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    textContainer->setContentAlignment (AlignmentFlag::Center);
    auto text_ = textContainer->addWidget (cpp14::make_unique<WText>(WString("<h4>Kategori Dağılımı {1}</h4>").arg (totalSum),TextFormat::UnsafeXHTML));

    for( auto item : list )
    {
        auto relativeContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        relativeContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        relativeContainer->setPositionScheme (PositionScheme::Relative);
        relativeContainer->setHeight (25);
        relativeContainer->setMargin (10,Side::Bottom);
        auto absoluteContainer = relativeContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        absoluteContainer->setOffsets (WLength("25%"),Side::Left|Side::Top);
        absoluteContainer->setHeight (25);
        absoluteContainer->setPositionScheme (PositionScheme::Absolute);

        auto _width = static_cast<double>(item.mSayi)/totalSum*100.0;

        absoluteContainer->setWidth (WLength(WString("{1}%").arg ( _width/4*3 ).toUTF8 ()));
        absoluteContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (),this->getRandom (),this->getRandom ()));
        absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        auto textLabel = relativeContainer->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (item.mKategori)));
        textLabel->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::DimGray));
        textLabel->setPositionScheme (PositionScheme::Absolute);
        textLabel->setOffsets (8,Side::Left|Side::Top);
        textLabel->decorationStyle ().setCursor (Cursor::PointingHand);

        textLabel->clicked ().connect ([=](){
            this->listTalepler (Talep()
                                .setKategoriOid (item.mKategoriOid.c_str ())
                                .setBirim (this->mUser->Birimi ().c_str ()));
        });


        auto text = relativeContainer->addWidget (cpp14::make_unique<WText>(WString("%{1}  /{2} Adet").arg ((int)_width).arg (item.mSayi)));
        text->addStyleClass ("textShadow");
        text->setPositionScheme (PositionScheme::Absolute);
        text->setOffsets (8,Side::Top);
        if( _width < 12.5 )
        {
            text->setOffsets (WLength(WString("{1}%").arg (25+_width/4.0*3.0).toUTF8 ()),Side::Left);
            text->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black));

        }else{
            text->setOffsets (WLength("26%"),Side::Left);
            text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        }

    }

}

void TalepYonetim::initTalep(const std::string &oid)
{


    Talep filter;
    filter.setOid (oid);

    auto mTalep = this->findOneTalep (filter);

    if( !mTalep )
    {
        this->showMessage ("Uyarı","Bu Talep Yüklenemedi. Lütfen Daha Sonra Tekrar Deneyiniz");
    }else{
        this->Content ()->clear ();

        auto talepView = this->Content ()->addWidget (cpp14::make_unique<TalepView>(std::move(mTalep),
                                                                                    this->db (),
                                                                                    this->mUser,
                                                                                    false));

        if( this->mUser->Statu () == SerikBLDCore::User::Mudur )
        {
            talepView->DurumChanged ().connect (this,&TalepYonetim::initControlPanel );
        }else if( this->mUser->Statu () == SerikBLDCore::User::Personel )
        {
            talepView->DurumChanged ().connect (this,&TalepYonetim::initPersonelPanel );
        }else if( this->mUser->Statu () == SerikBLDCore::User::Baskan || this->mUser->Statu () == SerikBLDCore::User::BaskanYardimcisi )
        {

        }
    }

}

std::unique_ptr<WPushButton> TalepYonetim::createButton(const char *classname, const WString &name)
{
    auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
    button->setTextFormat(Wt::TextFormat::XHTML);
    button->setText(name);
    button->addStyleClass(classname);
    return button;
}

Signal<std::string> &TalepYonetim::clickOid()
{
    return _clickOid;
}
