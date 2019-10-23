#include "bilgiedinmeclient.h"

BilgiEdinmeClient::BilgiEdinmeClient(mongocxx::database *_db, bsoncxx::document::value &_user)
    :DBClass (_db),UserClass (_user),ContainerWidget ("Bilgi Edinme Başvuruları")
{

    setPadding(15,Side::Top|Side::Left|Side::Right);

    auto btn = Header()->addWidget(cpp14::make_unique<WPushButton>("Gelen Başvurular"));
    btn->addStyleClass(Bootstrap::Button::Primary);

    btn->clicked().connect([=](){
        this->Content()->clear();
        this->Content()->addWidget(cpp14::make_unique<BilgiEdinmeYonetim>(this->db(),this->UserValue()));
    });

    Footer()->addWidget(cpp14::make_unique<WText>("<u>Sayfa Sonu</u>"))->setMargin(15,Side::Top);
}
