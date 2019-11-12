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

    {

        auto toolbarWidget = this->Header ()->addWidget (cpp14::make_unique<WToolBar>());

        auto devamBtn = createButton("btn-primary", TalepKey::DurumKey::DevamEdiyor+" "+Bootstrap::Badges::badget(std::to_string(55)));
        devamBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(devamBtn));

        auto tamamlandiBtn = createButton("btn-success", TalepKey::DurumKey::Tamamlandi+" "+Bootstrap::Badges::badget(std::to_string(44)));
        tamamlandiBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(tamamlandiBtn));

        auto redEdildiBtn = createButton("btn-danger", TalepKey::DurumKey::RedEdildi+" "+Bootstrap::Badges::badget(std::to_string(22)));
        redEdildiBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(redEdildiBtn));

        auto teyitEdilmemisBtn = createButton("btn-info", TalepKey::DurumKey::TeyitEdilmemis+" "+Bootstrap::Badges::badget(std::to_string(33)));
        teyitEdilmemisBtn->clicked().connect([=](){
//            this->initTalepler(SBLDKeys::SikayetKey::durumType::devamediyor);
        });
        toolbarWidget->addButton(std::move(teyitEdilmemisBtn));

        auto beklemedeBtn = createButton("btn-warning", TalepKey::DurumKey::Beklemede+" "+Bootstrap::Badges::badget(std::to_string(11)));
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
