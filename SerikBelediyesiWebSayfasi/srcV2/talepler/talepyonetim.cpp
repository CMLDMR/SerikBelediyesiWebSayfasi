#include "talepyonetim.h"

TalepYonetim::TalepYonetim(mongocxx::database *_db, bsoncxx::document::value _user)
    :ContainerWidget ("Talep/Şikayet Yönetimi"),
      TalepManager (_db),
      mUser(new User (_db,_user))
{

    this->initControlPanel ();


}

void TalepYonetim::initControlPanel()
{
    this->Header ()->clear ();

    auto kResult = this->kaynakPipeLine (this->mUser->Birimi ().c_str ());
    {

        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
//        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (15,Side::Top|Side::Bottom);
        auto toolbarWidget = container->addWidget (cpp14::make_unique<WToolBar>());

        auto SmsBtn = createButton("btn-primary", TalepKey::KaynakKey::Sms+" "+Bootstrap::Badges::badget(std::to_string(kResult.Sms)));
        SmsBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(SmsBtn));

        auto WebBtn = createButton("btn-success", TalepKey::KaynakKey::Web+" "+Bootstrap::Badges::badget(std::to_string(kResult.Web)));
        WebBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(WebBtn));

        auto MobilBtn = createButton("btn-danger", TalepKey::KaynakKey::Mobil+" "+Bootstrap::Badges::badget(std::to_string(kResult.Mobil)));
        MobilBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(MobilBtn));

        auto TelefonBtn = createButton("btn-info", TalepKey::KaynakKey::Telefon+" "+Bootstrap::Badges::badget(std::to_string(kResult.Telefon)));
        TelefonBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(TelefonBtn));

        auto BeyazmasaBtn = createButton("btn-default", TalepKey::KaynakKey::Beyazmasa+" "+Bootstrap::Badges::badget(std::to_string(kResult.Beyazmasa)));
        BeyazmasaBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(BeyazmasaBtn));


        auto SosyalMedyaBtn = createButton("btn-warning", TalepKey::KaynakKey::SosyalMedya+" "+Bootstrap::Badges::badget(std::to_string(kResult.SosyalMedya)));
        SosyalMedyaBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(SosyalMedyaBtn));
    }


    auto result = this->durumPipeLine (this->mUser->Birimi ().c_str ());


    {

        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
//        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (15,Side::Top|Side::Bottom);
        auto toolbarWidget = container->addWidget (cpp14::make_unique<WToolBar>());


        auto devamBtn = createButton("btn-primary", TalepKey::DurumKey::DevamEdiyor+" "+Bootstrap::Badges::badget(std::to_string(result.DevamEdiyor)));
        devamBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(devamBtn));

        auto tamamlandiBtn = createButton("btn-success", TalepKey::DurumKey::Tamamlandi+" "+Bootstrap::Badges::badget(std::to_string(result.Tamamlandi)));
        tamamlandiBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(tamamlandiBtn));

        auto redEdildiBtn = createButton("btn-danger", TalepKey::DurumKey::RedEdildi+" "+Bootstrap::Badges::badget(std::to_string(result.RedEdildi)));
        redEdildiBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(redEdildiBtn));

        auto teyitEdilmemisBtn = createButton("btn-info", TalepKey::DurumKey::TeyitEdilmemis+" "+Bootstrap::Badges::badget(std::to_string(result.TeyitEdilmemis)));
        teyitEdilmemisBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(teyitEdilmemisBtn));

        auto beklemedeBtn = createButton("btn-warning", TalepKey::DurumKey::Beklemede+" "+Bootstrap::Badges::badget(std::to_string(result.Beklemede)));
        beklemedeBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(beklemedeBtn));
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
