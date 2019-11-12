#include "talepyonetim.h"

TalepYonetim::TalepYonetim(mongocxx::database *_db, bsoncxx::document::value _user)
    :ContainerWidget ("Talep/Şikayet Yönetimi V2"),
      TalepManager (_db),
      mUser(new User (_db,_user))
{

    if( this->mUser->Statu () == User::Mudur ){
        this->initControlPanel ();
    }


//    this->Content ()->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    this->Content ()->setMargin (15,Side::Top|Side::Bottom);
    this->Content ()->setWidth (WLength("100%"));
    this->Content ()->decorationStyle ().setBorder (WBorder(BorderStyle::Solid,BorderWidth::Thin),Side::Top);


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



        auto devamEdiyorWidget = cpp14::make_unique<WContainerWidget>();
        devamEdiyorWidget->addWidget (cpp14::make_unique<WText>(TalepKey::DurumKey::DevamEdiyor +" "+Bootstrap::Badges::badget(std::to_string(result.DevamEdiyor))));
        devamEdiyorWidget->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::LightSeaGreen)+
                                              Style::color::color (Style::color::White::Snow)+
                                              Style::font::weight::bold);
        devamEdiyorWidget->setWidth (WLength(std::to_string (100.0/6.0)+"%"));
        devamEdiyorWidget->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        devamEdiyorWidget->decorationStyle ().setCursor (Cursor::PointingHand);
        devamEdiyorWidget->clicked ().connect ([=](){
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::DevamEdiyor.c_str ()) );
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
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::Tamamlandi.c_str ()) );
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
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::RedEdildi.c_str ()) );
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
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::TeyitEdilmemis.c_str ()) );
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
            this->listTalepler (Talep()
                                .setBirim (this->mUser->Birimi ().c_str ())
                                .setDurum (TalepKey::DurumKey::Beklemede.c_str ()) );
        });
        toolbarWidget->addWidget (std::move(beklemedeWidget));

    }
}

void TalepYonetim::listTalepler(const Talep &filter)
{
    if( filter.view ().empty () ) return;


    auto cursor = this->findTalep (filter);
    this->Content ()->clear ();
    for( auto item : cursor )
    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                  +Bootstrap::Grid::Medium::col_md_4
                                  +Bootstrap::Grid::Small::col_sm_6
                                  +Bootstrap::Grid::ExtraSmall::col_xs_12);

        {
            auto _container = container->addWidget (cpp14::make_unique<WContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>(item.tarih ().toStdString ()));
            _container->setWidth (WLength(std::to_string (100.0)+"%"));

        }

        {
            auto _container = container->addWidget (cpp14::make_unique<WContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>(item.mahalle ().toStdString ()));
            _container->setWidth (WLength(std::to_string (100.0)+"%"));

        }

        {
            auto _container = container->addWidget (cpp14::make_unique<WContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>(item.durum ().toStdString ()));
            _container->setWidth (WLength(std::to_string (100.0)+"%"));
            _container->setAttributeValue (Style::style,Style::background::color::color (item.durumColor ().toStdString ()+";")+
                                           Style::color::color (Style::color::White::Snow)+
                                           Style::font::weight::bold);
            _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        }

        container->decorationStyle ().setCursor (Cursor::PointingHand);


        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (5,Side::Top|Side::Bottom);
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
